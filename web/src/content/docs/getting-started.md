---
title: Getting Started
description: Clone the mycelium, spin up the stack, flash the node.
sidebar:
  order: 1
---

Honguera is a precision climate control system for indoor fungal cultivation. It manages temperature, humidity, and CO₂ using an ESP32, standard sensors, and a distributed intelligence network.

**Designed to replicate. Like a spore.**

## Prerequisites

- ESP32 DevKit (any variant)
- Sensors: SHT40 + MH-Z19B (+ optional DS18B20)
- [PlatformIO](https://platformio.org/) installed
- Docker + Docker Compose
- Local WiFi network

Total prototype cost: **~$110–170 USD** → [Full BOM](https://github.com/Freeak88/honguera/blob/main/hardware/BOM/BOM_v0.1.md)

## Quick Start

```bash
# Clone the mycelium
git clone https://github.com/Freeak88/honguera.git
cd honguera

# Spin up the network (MQTT + InfluxDB + Node-RED + Grafana)
cd software && docker compose up -d

# Flash the node (PlatformIO)
cd firmware && pio run --target upload

# Monitor telemetry
pio device monitor
```

## What just happened?

| Service | Port | Role |
|---------|------|------|
| Mosquitto | 1883 | MQTT broker — the nervous system |
| InfluxDB | 8086 | Time series — the mycelium's memory |
| Node-RED | 1880 | Orchestration + automation |
| Grafana | 3000 | Real-time dashboards |

The ESP32 node ("hypha") publishes JSON telemetry every 15 seconds over MQTT and listens for control and phase-change commands.

## Next steps

- Understand [the architecture](/honguera/architecture/)
- Review the [chamber spec](/honguera/hardware/spec/)
- Deploy the full [software stack](/honguera/software/deploy/)
- Pick a species from the [profiles](/honguera/species/profiles/)
