/**
 * Hoguera v0.1 — Main Controller
 * ESP32-based fungal cultivation precision system
 * 
 * Sensors: SHT40 (T/H), MH-Z19B (CO2), DS18B20 (water temp)
 * Actuators: SSR (heater), ultrasonic humidifier, fans
 * Comms: MQTT over WiFi
 * 
 * License: GPLv3
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_SHT4x.h>
#include <MHZ19.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ─── Configuration ──────────────────────────────────────────

struct Config {
    // WiFi
    const char* wifi_ssid;
    const char* wifi_pass;
    
    // MQTT
    const char* mqtt_host;
    int mqtt_port;
    const char* mqtt_user;
    const char* mqtt_pass;
    const char* mqtt_topic_base;
    
    // Pins
    int pin_ssr_heater;       // Cable calefactor 700W
    int pin_humidifier;       // Transductores ultrasónicos
    int pin_fan_inject;       // Cooler inyección niebla
    int pin_fan_extract;      // Extractor superior
    int pin_led;              // LED 6500K
    int pin_mhz19_rx;
    int pin_mhz19_tx;
    int pin_ds18b20;
    
    // Phase targets (loaded from MQTT/config)
    float target_temp;
    float target_humidity;
    int target_co2_max;
    bool light_on;
};

Config cfg = {
    .wifi_ssid       = "HOGUERA_NET",
    .wifi_pass       = "changeme",
    .mqtt_host       = "192.168.1.100",
    .mqtt_port       = 1883,
    .mqtt_user       = "hoguera",
    .mqtt_pass       = "changeme",
    .mqtt_topic_base = "hoguera/01",
    
    .pin_ssr_heater   = 25,
    .pin_humidifier   = 26,
    .pin_fan_inject   = 27,
    .pin_fan_extract  = 14,
    .pin_led          = 12,
    .pin_mhz19_rx     = 16,
    .pin_mhz19_tx     = 17,
    .pin_ds18b20      = 4,
    
    .target_temp      = 18.0,
    .target_humidity  = 90.0,
    .target_co2_max   = 800,
    .light_on         = false,
};

// ─── Globals ────────────────────────────────────────────────

WiFiClient espClient;
PubSubClient mqtt(espClient);
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
MHZ19 mhz19;
OneWire oneWire(cfg.pin_ds18b20);
DallasTemperature ds18b20(&oneWire);

// Sensor readings
struct SensorData {
    float temperature;
    float humidity;
    int co2;
    float water_temp;
    unsigned long timestamp;
};

SensorData sensors = {0};
bool sht4_ok = false;
bool mhz19_ok = false;

// Control state
bool heater_on = false;
bool humidifier_on = false;
bool extractor_on = false;

// Timing
unsigned long last_read = 0;
unsigned long last_publish = 0;
const unsigned long READ_INTERVAL = 5000;    // 5s
const unsigned long PUBLISH_INTERVAL = 15000; // 15s

// ─── WiFi ───────────────────────────────────────────────────

void setup_wifi() {
    Serial.printf("[WIFI] Connecting to %s", cfg.wifi_ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.wifi_ssid, cfg.wifi_pass);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf(" OK (%s)\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println(" FAILED");
    }
}

// ─── MQTT ───────────────────────────────────────────────────

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char buf[256];
    if (length >= sizeof(buf)) length = sizeof(buf) - 1;
    memcpy(buf, payload, length);
    buf[length] = '\0';
    
    String t = String(topic);
    
    // Phase control: hoguera/01/phase/set
    if (t.endsWith("/phase/set")) {
        JsonDocument doc;
        deserializeJson(doc, buf);
        
        if (doc.containsKey("temp"))     cfg.target_temp = doc["temp"];
        if (doc.containsKey("humidity")) cfg.target_humidity = doc["humidity"];
        if (doc.containsKey("co2_max"))  cfg.target_co2_max = doc["co2_max"];
        if (doc.containsKey("light"))    cfg.light_on = doc["light"];
        
        Serial.printf("[MQTT] Phase updated: T=%.1f H=%.1f CO2<%d Light=%d\n",
            cfg.target_temp, cfg.target_humidity, cfg.target_co2_max, cfg.light_on);
    }
    
    // Manual override: hoguera/01/override
    if (t.endsWith("/override")) {
        JsonDocument doc;
        deserializeJson(doc, buf);
        
        if (doc.containsKey("heater"))     digitalWrite(cfg.pin_ssr_heater, doc["heater"] ? HIGH : LOW);
        if (doc.containsKey("humidifier")) digitalWrite(cfg.pin_humidifier, doc["humidifier"] ? HIGH : LOW);
        if (doc.containsKey("extractor"))  digitalWrite(cfg.pin_fan_extract, doc["extractor"] ? HIGH : LOW);
        
        Serial.println("[MQTT] Manual override applied");
    }
}

void mqtt_reconnect() {
    String client_id = "hoguera-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    String will_topic = String(cfg.mqtt_topic_base) + "/status";
    
    if (mqtt.connect(client_id.c_str(), cfg.mqtt_user, cfg.mqtt_pass,
                     will_topic.c_str(), 1, true, "{\"status\":\"offline\"}")) {
        
        Serial.println("[MQTT] Connected");
        
        // Publish online status
        mqtt.publish(will_topic.c_str(), "{\"status\":\"online\"}", true);
        
        // Subscribe
        String base = String(cfg.mqtt_topic_base);
        mqtt.subscribe((base + "/phase/set").c_str());
        mqtt.subscribe((base + "/override").c_str());
    }
}

// ─── Sensors ────────────────────────────────────────────────

void read_sensors() {
    sensors.timestamp = millis();
    
    // SHT40
    if (sht4_ok) {
        sensors_event_t h, t;
        sht4.getEvent(&h, &t);
        sensors.temperature = t.temperature;
        sensors.humidity = h.relative_humidity;
    }
    
    // MH-Z19B
    if (mhz19_ok) {
        sensors.co2 = mhz19.getCO2();
        if (mhz19.errorCode != RESULT_OK) {
            Serial.printf("[CO2] Error: %d\n", mhz19.errorCode);
        }
    }
    
    // DS18B20
    ds18b20.requestTemperatures();
    sensors.water_temp = ds18b20.getTempCByIndex(0);
}

// ─── Control Logic ──────────────────────────────────────────

void control_loop() {
    float temp = sensors.temperature;
    float hum = sensors.humidity;
    int co2 = sensors.co2;
    
    // ── Temperature control (hysteresis ±0.5°C) ──
    if (temp < cfg.target_temp - 0.5) {
        heater_on = true;
    } else if (temp > cfg.target_temp + 0.5) {
        heater_on = false;
    }
    digitalWrite(cfg.pin_ssr_heater, heater_on ? HIGH : LOW);
    
    // ── Humidity control (hysteresis ±3%) ──
    if (hum < cfg.target_humidity - 3.0) {
        humidifier_on = true;
        digitalWrite(cfg.pin_fan_inject, HIGH); // Fan on to push mist
    } else if (hum > cfg.target_humidity + 3.0) {
        humidifier_on = false;
        digitalWrite(cfg.pin_fan_inject, LOW);
    }
    digitalWrite(cfg.pin_humidifier, humidifier_on ? HIGH : LOW);
    
    // ── CO2 control ──
    if (co2 > cfg.target_co2_max) {
        extractor_on = true;
    } else if (co2 < cfg.target_co2_max * 0.7) {
        extractor_on = false;
    }
    digitalWrite(cfg.pin_fan_extract, extractor_on ? HIGH : LOW);
    
    // ── Light ──
    digitalWrite(cfg.pin_led, cfg.light_on ? HIGH : LOW);
}

// ─── Publish ────────────────────────────────────────────────

void publish_telemetry() {
    JsonDocument doc;
    doc["version"] = HOGUERA_VERSION;
    doc["millis"] = millis();
    
    // Sensors
    JsonObject s = doc["sensors"].to<JsonObject>();
    s["temperature"] = round(sensors.temperature * 10) / 10.0;
    s["humidity"] = round(sensors.humidity * 10) / 10.0;
    s["co2"] = sensors.co2;
    s["water_temp"] = round(sensors.water_temp * 10) / 10.0;
    
    // Actuators
    JsonObject a = doc["actuators"].to<JsonObject>();
    a["heater"] = heater_on;
    a["humidifier"] = humidifier_on;
    a["extractor"] = extractor_on;
    a["light"] = cfg.light_on;
    
    // Targets
    JsonObject tgt = doc["targets"].to<JsonObject>();
    tgt["temp"] = cfg.target_temp;
    tgt["humidity"] = cfg.target_humidity;
    tgt["co2_max"] = cfg.target_co2_max;
    
    // WiFi RSSI
    doc["rssi"] = WiFi.RSSI();
    
    char buf[512];
    serializeJson(doc, buf);
    
    String topic = String(cfg.mqtt_topic_base) + "/telemetry";
    mqtt.publish(topic.c_str(), buf);
}

// ─── Setup ──────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.printf("\n[HOGUERA] v%s booting...\n", HOGUERA_VERSION);
    
    // Pin modes
    pinMode(cfg.pin_ssr_heater, OUTPUT);
    pinMode(cfg.pin_humidifier, OUTPUT);
    pinMode(cfg.pin_fan_inject, OUTPUT);
    pinMode(cfg.pin_fan_extract, OUTPUT);
    pinMode(cfg.pin_led, OUTPUT);
    
    // Init SHT40
    if (sht4.begin()) {
        sht4.setPrecision(SHT4X_HIGH_PRECISION);
        sht4.setHeater(SHT4X_NO_HEATER);
        sht4_ok = true;
        Serial.println("[SHT40] OK");
    } else {
        Serial.println("[SHT40] NOT FOUND");
    }
    
    // Init MH-Z19B
    Serial2.begin(9600, SERIAL_8N1, cfg.pin_mhz19_rx, cfg.pin_mhz19_tx);
    mhz19.begin(Serial2);
    mhz19.autoCalibration(false);
    mhz19_ok = true;
    Serial.println("[MHZ19B] OK");
    
    // Init DS18B20
    ds18b20.begin();
    Serial.println("[DS18B20] OK");
    
    // WiFi + MQTT
    setup_wifi();
    mqtt.setServer(cfg.mqtt_host, cfg.mqtt_port);
    mqtt.setCallback(mqtt_callback);
    mqtt_reconnect();
    
    Serial.println("[HOGUERA] Ready");
}

// ─── Main Loop ──────────────────────────────────────────────

void loop() {
    // MQTT keepalive
    if (!mqtt.connected()) {
        mqtt_reconnect();
    }
    mqtt.loop();
    
    unsigned long now = millis();
    
    // Read sensors
    if (now - last_read >= READ_INTERVAL) {
        last_read = now;
        read_sensors();
        control_loop();
    }
    
    // Publish telemetry
    if (now - last_publish >= PUBLISH_INTERVAL) {
        last_publish = now;
        publish_telemetry();
    }
}
