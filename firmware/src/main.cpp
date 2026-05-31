/**
 * Honguera v0.1 — Main Controller
 * ESP32-based fungal cultivation precision system
 * 
 * Sensors: SHT40 (T/H), MH-Z19B (CO2), DS18B20 (water temp), HW-390 (moisture), PH-4502C (ORP/Redox)
 * Actuators: SSR (heater), ultrasonic humidifier, injection fan, exhaust fan, solenoide (water valve), led
 * Comms: MQTT over WiFi
 * 
 * License: GPLv3
 */
#ifdef ESP8266_SIM_ONLY
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <Adafruit_SHT4x.h>
#include <MHZ19.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ─── Configuration ──────────────────────────────────────────

// Simulation Engine Toggle (Set to true to test state machine logic in laboratory without physical sensors)
#ifndef SIMULATE_SENSOR_INPUTS
#define SIMULATE_SENSOR_INPUTS 0
#endif

// Physical & calibration constants for analog sensors
// 1. Substrate Moisture Sensor (HW-390 / v2.0 Capacitive)
// 12-bit ADC values range from 0 to 4095. Capacitive sensors are inversely proportional to moisture.
// High raw voltage = Dry substrate. Low raw voltage = Wet substrate.
const int MOISTURE_DRY_RAW = 2800; // Calibration value in air (0% humidity)
const int MOISTURE_WET_RAW = 1300; // Calibration value in water (100% humidity)

// 2. ORP (Oxidation-Reduction Potential) / Redox sensor (PH-4502C + Platinum BNC probe)
// PH-4502C is powered by 5V, giving output up to ~3.7V (high ORP).
// A physical voltage divider is required to bring it down to ESP32 range (0 - 3.3V).
// Using R1 = 10kOhm and R2 = 20kOhm resistor divider: V_esp32 = V_sensor * (20 / (10 + 20)) = V_sensor * 2/3.
const float ORP_DIVIDER_RATIO = 0.6667;  // 2/3 scale down factor
const float ORP_OFFSET_VOLTAGE = 2.50;   // Sensor output at 0 mV ORP (before divider, adjusted by offset potentiometer)
const float ADC_VREF = 3.30;             // ESP32 ADC reference voltage (V)
const float ADC_MAX_VAL = 4095.0;        // ESP32 12-bit ADC maximum raw value

// Umbrales de Control Automatizado (Hysteresis & Biological targets)
const int HUMEDAD_SUSTRATO_MIN = 60;  // Encender riego si baja del 60%
const int HUMEDAD_SUSTRATO_MAX = 70;  // Apagar riego al llegar al 70%
const float ORP_ANONXIA_UMBRAL = 0.0; // Alerta si el ORP cae de 0 mV (Anoxia)
const float ORP_RECOVERY_UMBRAL = 50.0; // Umbral de salida de alerta de anoxia (histéresis en mV)
const int REQUIRED_CONSECUTIVE_READINGS = 12; // 12 lecturas consecutivas (1 min sostenido a 5s por lectura)

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
    int pin_ssr_heater;       // Cable calefactor 700W (Reassigned to GPIO 23)
    int pin_humidifier;       // Transductores ultrasónicos (Reassigned to GPIO 19)
    int pin_fan_inject;       // Cooler inyección niebla (GPIO 27)
    int pin_extractor_aire;   // Extractor superior para oxigenar/CO2 (GPIO 26) - Consolidado
    int pin_led;              // LED 6500K (GPIO 12)
    int pin_mhz19_rx;
    int pin_mhz19_tx;
    int pin_ds18b20;
    int pin_moisture;         // ADC1 Pin for HW-390 (GPIO 34)
    int pin_orp;              // ADC1 Pin for ORP/Redox (GPIO 35)
    int pin_valvula_riego;    // Solenoide para hidratar el sustrato (GPIO 25)
    
    // Phase targets (loaded from MQTT/config)
    float target_temp;
    float target_humidity;
    int target_co2_max;
    bool light_on;

    // Safety configurations
    bool dry_run_actuators;     // Si es true, no activa físicamente los GPIOs de los actuadores
    bool actuator_active_high;   // Define si los relés se activan con nivel HIGH (true) o LOW (false)
};

Config cfg = {
    .wifi_ssid       = "HONGUERA_NET",
    .wifi_pass       = "changeme",
    .mqtt_host       = "192.168.1.100",
    .mqtt_port       = 1883,
    .mqtt_user       = "honguera",
    .mqtt_pass       = "changeme",
    .mqtt_topic_base = "honguera/01",
    
    .pin_ssr_heater   = 23,  // Moved from 25 to free pin 23 to allow pin_valvula_riego to use 25
    .pin_humidifier   = 19,  // Moved from 26 to free pin 19 to allow pin_extractor_aire to use 26
    .pin_fan_inject   = 27,
    .pin_extractor_aire = 26, // Replaced pin_fan_extract (was 14) and set to 26
    .pin_led          = 12,
    .pin_mhz19_rx     = 16,
    .pin_mhz19_tx     = 17,
    .pin_ds18b20      = 4,
    .pin_moisture     = 34,  // GPIO 34 (ADC1 Channel 6)
    .pin_orp          = 35,  // GPIO 35 (ADC1 Channel 7)
    .pin_valvula_riego = 25, // GPIO 25 (relé solenoide de riego)
    
    .target_temp      = 18.0,
    .target_humidity  = 90.0,
    .target_co2_max   = 800,
    .light_on         = false,

    .dry_run_actuators = true,       // Default: true for maximum safety in MVP testing
    .actuator_active_high = true,   // Default: true (Active HIGH relés/SSRs)
};

// ─── Globals ────────────────────────────────────────────────

WiFiClient espClient;
PubSubClient mqtt(espClient);
#ifndef ESP8266_SIM_ONLY
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
MHZ19 mhz19;
OneWire oneWire(cfg.pin_ds18b20);
DallasTemperature ds18b20(&oneWire);
#endif

// Sensor readings
struct SensorData {
    float temperature;
    float humidity;
    int co2;
    float water_temp;
    int substrate_moisture;  // % (0 to 100)
    float substrate_orp_estimated_mv; // ORP estimado (proxy del estado redox, no actividad enzimática directa)
    unsigned long timestamp;
};

SensorData sensors = {0};
bool sht4_ok = false;
bool mhz19_ok = false;

// Control state (Logical States)
bool heater_on = false;
bool humidifier_on = false;
bool fan_inject_on = false;
bool extractor_aire_on = false; // Consolidated exhaust actuator state
bool valvula_riego_on = false;

// Safety tracking variables for ORP / Anoxia
bool orp_anoxia_alert = false;
int anoxia_reading_count = 0;
int recovery_reading_count = 0;

// Simulation variables
#if SIMULATE_SENSOR_INPUTS
unsigned long sim_cycles = 0;
#endif

// Timing
unsigned long last_read = 0;
unsigned long last_publish = 0;
const unsigned long READ_INTERVAL = 5000;    // 5s

// Simulation vs Production Publish Interval abstraction
#if SIMULATE_SENSOR_INPUTS
const unsigned long PUBLISH_INTERVAL = 5000;  // 5s in simulation for step-by-step state matching
#else
const unsigned long PUBLISH_INTERVAL = 15000; // 15s in production
#endif

// ─── Safe Digital Write Function ─────────────────────────────

/**
 * Centralized driver function to write physical states to GPIOs.
 * Implements the DRY_RUN_ACTUATORS check and active level translation.
 */
void safeDigitalWrite(int pin, bool logicalState) {
#ifdef ESP8266_SIM_ONLY
    // No-op for physical writes on ESP8266 simulation to avoid crashes or conflicts with non-existent ESP32 GPIOs
    return;
#else
    if (cfg.dry_run_actuators) {
        // In dry-run mode, force the physical GPIO to remain in its OFF state.
        // OFF state depends on ACTUATOR_ACTIVE_HIGH.
        bool physicalOff = cfg.actuator_active_high ? LOW : HIGH;
        digitalWrite(pin, physicalOff);
        return;
    }
    
    // Standard active-high / active-low translation
    bool physicalState;
    if (cfg.actuator_active_high) {
        physicalState = logicalState ? HIGH : LOW;
    } else {
        physicalState = logicalState ? LOW : HIGH;
    }
    digitalWrite(pin, physicalState);
#endif
}

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
    
    // Phase control: honguera/01/phase/set
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
    
    // Manual override: honguera/01/override
    if (t.endsWith("/override")) {
        JsonDocument doc;
        deserializeJson(doc, buf);
        
        if (doc.containsKey("heater")) {
            heater_on = doc["heater"];
            safeDigitalWrite(cfg.pin_ssr_heater, heater_on);
        }
        if (doc.containsKey("humidifier")) {
            humidifier_on = doc["humidifier"];
            safeDigitalWrite(cfg.pin_humidifier, humidifier_on);
            fan_inject_on = humidifier_on;
            safeDigitalWrite(cfg.pin_fan_inject, fan_inject_on);
        }
        if (doc.containsKey("extractor") || doc.containsKey("extractor_aire")) {
            bool state = doc.containsKey("extractor") ? doc["extractor"] : doc["extractor_aire"];
            extractor_aire_on = state;
            safeDigitalWrite(cfg.pin_extractor_aire, extractor_aire_on);
        }
        if (doc.containsKey("riego")) {
            valvula_riego_on = doc["riego"];
            safeDigitalWrite(cfg.pin_valvula_riego, valvula_riego_on);
        }
        
        Serial.println("[MQTT] Manual override applied with safety checks");
    }
}

void mqtt_reconnect() {
#ifdef ESP8266_SIM_ONLY
    String client_id = "honguera-" + String(ESP.getChipId(), HEX);
#else
    String client_id = "honguera-" + String((uint32_t)ESP.getEfuseMac(), HEX);
#endif
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

#if SIMULATE_SENSOR_INPUTS
const char* get_simulation_phase_name(unsigned long step) {
    if (step < 5)   return "A_NORMAL";
    if (step < 20)  return "B_ORP_LOW_NO_TRIGGER";
    if (step < 35)  return "C_ORP_LOW_WITH_TRIGGER";
    return "D_RECOVERY";
}
#endif

void read_sensors() {
    sensors.timestamp = millis();
    
#if SIMULATE_SENSOR_INPUTS
    // Simulation Engine: Cycle through 4 distinct phases (A, B, C, D) to validate state machine strictly
    // Step is 0-indexed and calculated first, then sim_cycles is incremented for the next sample
    unsigned long step = sim_cycles % 50;
    sim_cycles++;
    
    if (step < 5) {
        // A) Estado Normal (samples 0 - 4)
        sensors.temperature = 18.5;
        sensors.humidity = 72.0;
        sensors.co2 = 550;
        sensors.water_temp = 16.0;
        sensors.substrate_moisture = 65;
        sensors.substrate_orp_estimated_mv = 120.0;
    } 
    else if (step < 20) {
        // B) ORP bajo sin trigger biológico (samples 5 - 19, 15 samples)
        // ORP is -10 mV, but CO2 (450) and humidity (70%) are low, so NO alarm triggers (anoxia_reading_count stays 0)
        sensors.temperature = 18.5;
        sensors.humidity = 70.0;
        sensors.co2 = 450;
        sensors.water_temp = 16.0;
        sensors.substrate_moisture = 64;
        sensors.substrate_orp_estimated_mv = -10.0;
    } 
    else if (step < 35) {
        // C) ORP bajo con trigger biológico (samples 20 - 34, 15 samples)
        // ORP is -15 mV, and CO2 (780) / Hum (88%) triggers alarm at step 31 (after 12 consecutive samples of strict anoxia)
        sensors.temperature = 18.2;
        sensors.humidity = 88.0;
        sensors.co2 = 780;
        sensors.water_temp = 15.8;
        sensors.substrate_moisture = 63;
        sensors.substrate_orp_estimated_mv = -15.0;
    } 
    else {
        // D) Recuperación de ORP (samples 35 - 49, 15 samples)
        // ORP is +62 mV. Recovery counter increments, alarm resolves at step 46 (after 12 consecutive samples)
        sensors.temperature = 18.4;
        sensors.humidity = 75.0;
        sensors.co2 = 580;
        sensors.water_temp = 16.0;
        sensors.substrate_moisture = 66;
        sensors.substrate_orp_estimated_mv = 62.0;
    }
    Serial.printf("[SIMULATION] Step %lu/50 (%s) | T=%.1f H=%.1f CO2=%d ORP=%.1f mV | Alr=%d Cnt=%d\n", 
                  step, get_simulation_phase_name(step), sensors.temperature, sensors.humidity, sensors.co2, sensors.substrate_orp_estimated_mv,
                  orp_anoxia_alert, orp_anoxia_alert ? recovery_reading_count : anoxia_reading_count);
#else
#ifndef ESP8266_SIM_ONLY
    // Read actual physical sensors
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

    // ─── Sensor de Humedad Capacitivo (HW-390 / v2.0) ───
    int raw_moisture = analogRead(cfg.pin_moisture);
    int mapped_moisture = map(raw_moisture, MOISTURE_DRY_RAW, MOISTURE_WET_RAW, 0, 100);
    sensors.substrate_moisture = constrain(mapped_moisture, 0, 100);

    // ─── Sensor Redox / ORP (PH-4502C + Platinum BNC probe) ───
    unsigned long orp_sum = 0;
    const int ORP_SAMPLE_COUNT = 20;
    for (int i = 0; i < ORP_SAMPLE_COUNT; i++) {
        orp_sum += analogRead(cfg.pin_orp);
        delayMicroseconds(500); // 500 us oversampling pause
    }
    float raw_orp_avg = (float)orp_sum / ORP_SAMPLE_COUNT;

    float voltage_esp32 = (raw_orp_avg / ADC_MAX_VAL) * ADC_VREF;
    float voltage_sensor = voltage_esp32 / ORP_DIVIDER_RATIO;
    
    // ORP estimado (proxy del estado redox, no actividad enzimática directa)
    sensors.substrate_orp_estimated_mv = (voltage_sensor - ORP_OFFSET_VOLTAGE) * 1000.0;
#else
    // Fallback sensors defaults for ESP8266 if simulation inputs is disabled
    sensors.temperature = 18.0;
    sensors.humidity = 70.0;
    sensors.co2 = 500;
    sensors.water_temp = 16.0;
    sensors.substrate_moisture = 60;
    sensors.substrate_orp_estimated_mv = 100.0;
#endif
#endif
}

// ─── Biological Redox Engine ───────────────────────────────

/**
 * Calculates substrate ORP state machines strictly, preventing leaks between phases.
 */
void updateOrpAnoxiaState(float orp, int co2, float hum) {
    bool orp_low = orp < ORP_ANONXIA_UMBRAL;
    bool biological_risk = co2 > 600 || hum > 85.0;
    bool anoxia_condition = orp_low && biological_risk;

    // 1. Accumulate anoxia counter strictly when BOTH ORP is low AND biological risk context is present
    if (anoxia_condition) {
        anoxia_reading_count++;
        recovery_reading_count = 0; // Reset recovery count
    } else {
        anoxia_reading_count = 0; // Drop instantly to 0 to prevent leakage/preloading from Phase B
    }

    if (orp_anoxia_alert) {
        // 2. Accumulate recovery counter strictly when alarm is active and ORP has returned above +50 mV
        if (orp > ORP_RECOVERY_UMBRAL) {
            recovery_reading_count++;
        } else {
            recovery_reading_count = 0; // Reset recovery instantly if it drops below threshold
        }
        
        // Check for sustained recovery (12 consecutive samples)
        if (recovery_reading_count >= REQUIRED_CONSECUTIVE_READINGS) {
            orp_anoxia_alert = false;
            recovery_reading_count = 0;
            Serial.println("[SAFETY] Alert resolved: Substrate ORP recovered to healthy levels.");
        }
    } else {
        // Force recovery count to 0 if alert is not active
        recovery_reading_count = 0;

        // Check for sustained anoxia trigger (12 consecutive samples of full combined condition)
        if (anoxia_reading_count >= REQUIRED_CONSECUTIVE_READINGS) {
            orp_anoxia_alert = true;
            anoxia_reading_count = 0; // Reset upon trigger
            Serial.println("[SAFETY] !!! ALERT: Substrate Anoxia detected (Sustained low ORP + Bio Risk) !!!");
        }
    }
}

// ─── Control Logic ──────────────────────────────────────────

void controlar_actuadores() {
    float temp = sensors.temperature;
    float hum = sensors.humidity;
    int co2 = sensors.co2;
    
    // ─── CONTROL DE TEMPERATURA POR HISTÉRESIS (Hysteresis ±0.5°C) ───
    if (temp < cfg.target_temp - 0.5) {
        heater_on = true;
    } else if (temp > cfg.target_temp + 0.5) {
        heater_on = false;
    }
    safeDigitalWrite(cfg.pin_ssr_heater, heater_on);
    
    // ─── CONTROL DE HUMEDAD AMBIENTAL POR HISTÉRESIS (Hysteresis ±3%) ───
    if (hum < cfg.target_humidity - 3.0) {
        humidifier_on = true;
        fan_inject_on = true; // Fan on to push mist
    } else if (hum > cfg.target_humidity + 3.0) {
        humidifier_on = false;
        fan_inject_on = false;
    }
    safeDigitalWrite(cfg.pin_humidifier, humidifier_on);
    safeDigitalWrite(cfg.pin_fan_inject, fan_inject_on);
    
    // ─── CONTROL DE RIEGO POR HISTÉRESIS (Humedad de Sustrato) ───
    if (sensors.substrate_moisture < HUMEDAD_SUSTRATO_MIN) {
        // Sustrato seco: Activamos riego seguro
        valvula_riego_on = true;
    } 
    else if (sensors.substrate_moisture >= HUMEDAD_SUSTRATO_MAX) {
        // Sustrato óptimo: Cortamos irrigación para evitar asfixiar raíces/hifas
        valvula_riego_on = false;
    }
    safeDigitalWrite(cfg.pin_valvula_riego, valvula_riego_on);

    // ─── CONTROL ENZIMÁTICO / INYECCIÓN DE OXÍGENO (ORP & CO2 con Histéresis Temporal) ───
    updateOrpAnoxiaState(sensors.substrate_orp_estimated_mv, co2, hum);

    // Decisión de Extracción basada en Alerta ORP o CO2 estándar
    if (orp_anoxia_alert) {
        extractor_aire_on = true; // Forzado por anoxia
    } 
    else {
        // Lógica estándar de CO2 con histéresis simple
        if (co2 > cfg.target_co2_max) {
            extractor_aire_on = true;
        } else if (co2 < cfg.target_co2_max * 0.7) {
            extractor_aire_on = false;
        }
    }
    safeDigitalWrite(cfg.pin_extractor_aire, extractor_aire_on);
    
    // ─── CONTROL DE ILUMINACIÓN ───
    safeDigitalWrite(cfg.pin_led, cfg.light_on);
}

// ─── Publish ────────────────────────────────────────────────

void publish_telemetry() {
    JsonDocument doc;
    doc["version"] = HONGUERA_VERSION;
    doc["millis"] = millis();
    
    // Sensors
    JsonObject s = doc["sensors"].to<JsonObject>();
    s["temperature"] = round(sensors.temperature * 10) / 10.0;
    s["humidity"] = round(sensors.humidity * 10) / 10.0;
    s["co2"] = sensors.co2;
    s["water_temp"] = round(sensors.water_temp * 10) / 10.0;
    s["sustrate_moisture"] = sensors.substrate_moisture;
    s["enzymatic_orp"] = round(sensors.substrate_orp_estimated_mv * 10) / 10.0;     // Legacy payload key
    s["substrate_orp_mv"] = round(sensors.substrate_orp_estimated_mv * 10) / 10.0;  // Normalized contract key
    
    // Actuators
    JsonObject a = doc["actuators"].to<JsonObject>();
    a["heater"] = heater_on;
    a["humidifier"] = humidifier_on;
    a["extractor"] = extractor_aire_on;      // Legacy key
    a["extractor_aire"] = extractor_aire_on; // Normalized contract key
    a["valvula_riego"] = valvula_riego_on;
    a["light"] = cfg.light_on;
    
    // Targets
    JsonObject tgt = doc["targets"].to<JsonObject>();
    tgt["temp"] = cfg.target_temp;
    tgt["humidity"] = cfg.target_humidity;
    tgt["co2_max"] = cfg.target_co2_max;
    
    // Safety block
    JsonObject safety = doc["safety"].to<JsonObject>();
    safety["dry_run_actuators"] = cfg.dry_run_actuators;
    safety["actuator_active_high"] = cfg.actuator_active_high;
    safety["orp_anoxia_alert"] = orp_anoxia_alert;
    safety["anoxia_reading_count"] = anoxia_reading_count;
    safety["recovery_reading_count"] = recovery_reading_count;

#if SIMULATE_SENSOR_INPUTS
    safety["simulation_enabled"] = true;
    // Calculate current step evaluated in the last loop
    unsigned long current_step = (sim_cycles > 0) ? ((sim_cycles - 1) % 50) : 0;
    safety["simulation_step"] = current_step;
    safety["simulation_phase"] = get_simulation_phase_name(current_step);
#else
    safety["simulation_enabled"] = false;
    safety["simulation_step"] = 0;
    safety["simulation_phase"] = "PHYSICAL";
#endif

    // WiFi RSSI
    doc["rssi"] = WiFi.RSSI();
    
    // Measure dynamic serialized size
    size_t jsonSize = measureJson(doc);
    Serial.printf("[MQTT] JSON size: %u bytes\n", jsonSize);
    
    char buf[1024]; // Safe buffer scaled up to 1024 bytes
    serializeJson(doc, buf);
    
    String topic = String(cfg.mqtt_topic_base) + "/telemetry";
    mqtt.publish(topic.c_str(), buf);
}

// ─── Setup ──────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.printf("\n[HONGUERA] v%s booting...\n", HONGUERA_VERSION);
    
#ifndef ESP8266_SIM_ONLY
    // Pin modes
    pinMode(cfg.pin_ssr_heater, OUTPUT);
    pinMode(cfg.pin_humidifier, OUTPUT);
    pinMode(cfg.pin_fan_inject, OUTPUT);
    pinMode(cfg.pin_extractor_aire, OUTPUT);
    pinMode(cfg.pin_led, OUTPUT);
    pinMode(cfg.pin_valvula_riego, OUTPUT);
    
    // Initialize actuators to safe OFF state at boot
    safeDigitalWrite(cfg.pin_ssr_heater, false);
    safeDigitalWrite(cfg.pin_humidifier, false);
    safeDigitalWrite(cfg.pin_fan_inject, false);
    safeDigitalWrite(cfg.pin_extractor_aire, false);
    safeDigitalWrite(cfg.pin_led, false);
    safeDigitalWrite(cfg.pin_valvula_riego, false);

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
    
    // Init Analog Sensors
    analogReadResolution(12); // Set ESP32 ADC resolution to 12-bit (0-4095) explicitly
    Serial.println("[ANALOG SENSORES] Configured 12-bit ADC resolution");
#else
    Serial.println("[ESP8266 SIM ONLY] Skipping hardware pin setup and physical sensors");
#endif
    
    // WiFi + MQTT
    setup_wifi();
    mqtt.setServer(cfg.mqtt_host, cfg.mqtt_port);
    mqtt.setCallback(mqtt_callback);
    mqtt_reconnect();
    
    Serial.println("[HONGUERA] Ready");
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
        controlar_actuadores();
    }
    
    // Publish telemetry
    if (now - last_publish >= PUBLISH_INTERVAL) {
        last_publish = now;
        publish_telemetry();
    }
}
