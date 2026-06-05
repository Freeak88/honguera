---
title: MQTT Topics
description: Topic topology between the ESP32 node and the Aletheia stack.
---

**Base:** `honguera/` · **QoS:** 1 (at-least-once) for critical telemetry · **Retain:** true for actuator state

## Telemetry (ESP32 → Mosquitto)

### Sensors (publish every 5 s)

| Topic | Payload (JSON) | Frequency |
|-------|----------------|-----------|
| `honguera/honguera-01/sensors/sht40` | `{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}` | 5 s |
| `honguera/honguera-01/sensors/co2` | `{"co2_ppm": 520, "ts": 1713738900}` | 5 s |
| `honguera/honguera-01/sensors/ds18b20` | `{"temp_c": 28.1, "ts": 1713738900}` | 10 s (optional) |

### Node status

| Topic | Payload | Retain |
|-------|---------|--------|
| `honguera/honguera-01/status/uptime` | `{"uptime_s": 3600, "heap_free": 180000}` | true |
| `honguera/honguera-01/status/rssi` | `{"rssi_dbm": -65, "quality": "good"}` | true |
| `honguera/honguera-01/status/will` | `{"status": "offline", "last_seen": 1713738900}` | true (LWT) |

## Control (Aletheia → ESP32)

### Actuators (subscribe)

| Topic | Payload | Action |
|-------|---------|--------|
| `honguera/honguera-01/control/relay/heater` | `{"state": "ON", "duty_cycle_pct": 80}` | 700 W cable (SSR) |
| `honguera/honguera-01/control/relay/mister` | `{"state": "ON", "duration_s": 10}` | Ultrasonic transducers |
| `honguera/honguera-01/control/fan/injection` | `{"state": "ON", "speed_pct": 100}` | 12 V mist fan |
| `honguera/honguera-01/control/fan/exhaust` | `{"state": "ON", "speed_pct": 50}` | Top exhaust |

### Operation modes

| Topic | Payload | Phase |
|-------|---------|-------|
| `honguera/honguera-01/mode` | `{"phase": "incubation", "target_temp_c": 25.0, "target_humidity_pct": 85}` | Incubation |
| `honguera/honguera-01/mode` | `{"phase": "induction", "target_temp_c": 15.0, "target_humidity_pct": 90}` | Induction |
| `honguera/honguera-01/mode` | `{"phase": "fruiting", "target_temp_c": 18.0, "target_humidity_pct": 95, "light_on": true}` | Fruiting |

## Full telemetry payload example

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
