---
title: Firmware
description: ESP32 control loop — sensors, hysteresis control, MQTT telemetry.
sidebar:
  order: 3
---

The node firmware runs on ESP32 with PlatformIO (Arduino framework).

```
firmware/
├── src/
│   └── main.cpp       ← Control loop + sensors + MQTT
├── lib/               ← Custom libraries
└── platformio.ini     ← Dependencies and config
```

## Features (v0.1)

- ✅ SHT40 (T/H), MH-Z19B (CO₂), DS18B20 (water) reading
- ✅ Hysteresis control: heater, humidifier, exhaust
- ✅ MQTT pub/sub: JSON telemetry every 15 s
- ✅ Remote phase control (incubation → induction → fruiting)
- ✅ Manual override via MQTT
- ✅ Last Will + online/offline status

## Up next

- [ ] OTA updates
- [ ] Auto PID tuning
- [ ] Power-saving mode (solar-ready)
- [ ] WiFi Manager (AP captive portal)
- [ ] Multi-node (multiple chambers, one broker)

## Build & flash

```bash
cd firmware
pio run --target upload
pio device monitor
```

See the [MQTT topic reference](/honguera/reference/mqtt-topics/) for the full telemetry and control surface.
