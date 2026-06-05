---
title: Tópicos MQTT
description: Topología de tópicos entre el nodo ESP32 y el stack Aletheia.
---

**Base:** `honguera/` · **QoS:** 1 (al menos una vez) para telemetría crítica · **Retain:** true para estado de actuadores

## Telemetría (ESP32 → Mosquitto)

### Sensores (publish cada 5 s)

| Tópico | Payload (JSON) | Frecuencia |
|--------|----------------|------------|
| `honguera/honguera-01/sensors/sht40` | `{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}` | 5 s |
| `honguera/honguera-01/sensors/co2` | `{"co2_ppm": 520, "ts": 1713738900}` | 5 s |
| `honguera/honguera-01/sensors/ds18b20` | `{"temp_c": 28.1, "ts": 1713738900}` | 10 s (opcional) |

### Estado del nodo

| Tópico | Payload | Retain |
|--------|---------|--------|
| `honguera/honguera-01/status/uptime` | `{"uptime_s": 3600, "heap_free": 180000}` | true |
| `honguera/honguera-01/status/rssi` | `{"rssi_dbm": -65, "quality": "good"}` | true |
| `honguera/honguera-01/status/will` | `{"status": "offline", "last_seen": 1713738900}` | true (LWT) |

## Control (Aletheia → ESP32)

### Actuadores (subscribe)

| Tópico | Payload | Acción |
|--------|---------|--------|
| `honguera/honguera-01/control/relay/heater` | `{"state": "ON", "duty_cycle_pct": 80}` | Cable 700 W (SSR) |
| `honguera/honguera-01/control/relay/mister` | `{"state": "ON", "duration_s": 10}` | Transductores ultrasónicos |
| `honguera/honguera-01/control/fan/injection` | `{"state": "ON", "speed_pct": 100}` | Cooler 12 V niebla |
| `honguera/honguera-01/control/fan/exhaust` | `{"state": "ON", "speed_pct": 50}` | Extractor superior |

### Modos de operación

| Tópico | Payload | Fase |
|--------|---------|------|
| `honguera/honguera-01/mode` | `{"phase": "incubation", "target_temp_c": 25.0, "target_humidity_pct": 85}` | Incubación |
| `honguera/honguera-01/mode` | `{"phase": "induction", "target_temp_c": 15.0, "target_humidity_pct": 90}` | Inducción |
| `honguera/honguera-01/mode` | `{"phase": "fruiting", "target_temp_c": 18.0, "target_humidity_pct": 95, "light_on": true}` | Fructificación |

## Ejemplo de payload completo (telemetría)

```json
{
  "node_id": "honguera-01",
  "timestamp": 1713738900,
  "sensors": {
    "temp_c": 23.4,
    "humidity_pct": 68.2,
    "co2_ppm": 520,
    "water_temp_c": 28.1
  },
  "actuators": {
    "heater_state": "OFF",
    "mister_state": "OFF",
    "injection_fan": "OFF",
    "exhaust_fan": "OFF"
  },
  "status": {
    "uptime_s": 3600,
    "rssi_dbm": -65
  }
}
```
