---
title: Firmware
description: Loop de control en ESP32 — sensores, control por histéresis, telemetría MQTT.
sidebar:
  order: 3
---

El firmware del nodo corre en ESP32 con PlatformIO (framework Arduino).

```
firmware/
├── src/
│   └── main.cpp       ← Loop de control + sensores + MQTT
├── lib/               ← Librerías propias
└── platformio.ini     ← Dependencias y configuración
```

## Funcionalidades (v0.1)

- ✅ Lectura de SHT40 (T/H), MH-Z19B (CO₂), DS18B20 (agua)
- ✅ Control por histéresis: calefactor, humidificador, extractor
- ✅ Pub/sub MQTT: telemetría JSON cada 15 s
- ✅ Control de fase remoto (incubación → inducción → fructificación)
- ✅ Override manual vía MQTT
- ✅ Last Will + estado online/offline

## Próximamente

- [ ] Actualizaciones OTA
- [ ] Auto-tuning PID
- [ ] Modo bajo consumo (solar-ready)
- [ ] WiFi Manager (portal cautivo AP)
- [ ] Multinodo (varias naves, un broker)

## Compilar y flashear

```bash
cd firmware
pio run --target upload
pio device monitor
```

Mirá la [referencia de tópicos MQTT](/honguera/es/reference/mqtt-topics/) para la superficie completa de telemetría y control.
