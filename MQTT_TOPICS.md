# MQTT — Topología de Tópicos (Hoguera-01 → Aletheia)

**Base:** `hoguera/`
**QoS:** 1 (al menos una vez) para telemetría crítica
**Retain:** true para estado de actuadores

---

## Telemetría (ESP32 → Mosquitto)

### Sensores (Publish cada 5s)
| Tópico | Payload (JSON) | Frecuencia |
|--------|----------------|------------|
| `hoguera/hoguera-01/sensors/sht40` | `{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}` | 5s |
| `hoguera/hoguera-01/sensors/co2` | `{"co2_ppm": 520, "ts": 1713738900}` | 5s |
| `hoguera/hoguera-01/sensors/ds18b20` | `{"temp_c": 28.1, "ts": 1713738900}` | 10s (opcional) |

### Estado del Nodo
| Tópico | Payload | Retain |
|--------|---------|--------|
| `hoguera/hoguera-01/status/uptime` | `{"uptime_s": 3600, "heap_free": 180000}` | true |
| `hoguera/hoguera-01/status/rssi` | `{"rssi_dbm": -65, "quality": "good"}` | true |
| `hoguera/hoguera-01/status/will` | `{"status": "offline", "last_seen": 1713738900}` | true (LWT) |

---

## Control (Aletheia → ESP32)

### Actuadores (Subscribe)
| Tópico | Payload | Acción |
|--------|---------|--------|
| `hoguera/hoguera-01/control/relay/heater` | `{"state": "ON", "duty_cycle_pct": 80}` | Cable 700W (SSR) |
| `hoguera/hoguera-01/control/relay/mister` | `{"state": "ON", "duration_s": 10}` | Transductores ultrasónicos |
| `hoguera/hoguera-01/control/fan/injection` | `{"state": "ON", "speed_pct": 100}` | Cooler 12V niebla |
| `hoguera/hoguera-01/control/fan/exhaust` | `{"state": "ON", "speed_pct": 50}` | Extractor superior |

### Modos de Operación
| Tópico | Payload | Fase |
|--------|---------|------|
| `hoguera/hoguera-01/mode` | `{"phase": "incubation", "target_temp_c": 25.0, "target_humidity_pct": 85}` | Incubación |
| `hoguera/hoguera-01/mode` | `{"phase": "induction", "target_temp_c": 15.0, "target_humidity_pct": 90}` | Inducción |
| `hoguera/hoguera-01/mode` | `{"phase": "fruiting", "target_temp_c": 18.0, "target_humidity_pct": 95, "light_on": true}` | Fructificación |

---

## Ejemplo Payload Completo (Telemetría)

```json
{
  "node_id": "hoguera-01",
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

---

## Node-RED Flow (Primeras conexiones)

**In:**
- `mqtt in` → `hoguera/hoguera-01/sensors/#`
- Parse JSON → Timestamp → Function (InfluxDB Line Protocol)

**Out:**
- `influxdb out` → bucket `sensors`
- `debug` → log
- `mqtt out` → `hoguera/hoguera-01/control/relay/heater` (cuando ML decide)
