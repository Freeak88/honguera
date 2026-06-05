---
title: Architecture
description: The body of the mycelium — hyphae, mantle, soil, frond, and the ML layer.
sidebar:
  order: 2
---

A real mycelium has no central brain. Each hypha senses, decides, and responds locally — yet shares information with the entire network. Honguera works the same way: distributed sensors, local control at the node, and an ML layer that learns the thermal inertia of the space.

```
                         ┌─────────────┐
                         │   Mycelium   │
                         │  (ML Layer)  │
                         │  Predictive  │
                         └──────┬───────┘
                                │ learns thermal inertia
                                ▼
┌──────────┐    MQTT     ┌──────────┐    writes    ┌──────────┐
│ Hyphae×N │◄───────────►│  Mantle  │─────────────►│  Soil    │
│ (ESP32)  │   pub/sub   │(Mosquitto)│              │(InfluxDB)│
└────┬─────┘             └──────────┘              └──────────┘
     │                                                     │
     │ senses         ┌──────────┐                         │
     ├────────────────►│  Frond   │◄────────────────────────┘
     │                 │(Grafana) │        reads
     │                 └──────────┘
     │
     │ actuates
     ▼
┌──────────┐  ┌───────────┐  ┌──────────┐
│  Heater   │  │ Humidifier │  │ Exhaust  │
│ SSR 700W  │  │ Ultrasonic │  │  CO₂     │
└──────────┘  └───────────┘  └──────────┘
```

## The organs

| Organ | Component | Biological function |
|-------|-----------|---------------------|
| **Hyphae** | ESP32 + sensors | Sense the environment, decide locally |
| **Mantle** | Mosquitto (MQTT) | Signaling network between hyphae |
| **Soil** | InfluxDB | Memory. Stores what was learned |
| **Frond** | Grafana / Node-RED | Visualization. The visible surface |
| **Mycelium** | ML Layer | Distributed intelligence. Predicts, adapts |

## Predictive ML (roadmap)

The mycelium doesn't react. **It anticipates.**

| Phase | Model | Target |
|-------|-------|--------|
| v0.1 | Simple hysteresis | Functional ✅ |
| v0.2 | Linear regression | Learn thermal inertia |
| v0.3 | LSTM | 15-min prediction, ±0.3°C |
| v0.4 | Federated (multi-node) | Learn from other mycelia |

The ML layer takes indoor/outdoor temperature, actuator state and timestamps as inputs, targeting a constant 18°C with deviation under 0.5°C — using preventive pulses of the 700W heating cable timed to the thermal inertia of the room.
