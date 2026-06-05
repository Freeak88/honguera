---
title: Software Stack & Deploy
description: Mosquitto, InfluxDB, Node-RED and Grafana — one docker compose command.
---

The whole server-side stack ("Aletheia") runs with a single command:

```bash
cd software && docker compose up -d
```

| Service | Port | Role |
|---------|------|------|
| Mosquitto | 1883 | MQTT broker — the nervous system |
| InfluxDB | 8086 | Time series — the mycelium's memory |
| Node-RED | 1880 | Orchestration + automation |
| Grafana | 3000 | Real-time dashboards |

## Verify

```bash
docker compose ps
docker compose logs -f mosquitto
```

## First test

```bash
# Subscribe to every node topic
mosquitto_sub -h localhost -t "honguera/#" -v

# Publish a test message
mosquitto_pub -h localhost -t "honguera/honguera-01/sensors/sht40" \
  -m '{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}'
```

## InfluxDB

Create a token for Node-RED (if the init token isn't used):

```bash
docker exec -it honguera-influxdb influx
influx auth create --org honguera --write-bucket sensors --read-bucket sensors --all-access
```

## Node-RED — first flow

Install packages:

- `node-red-contrib-influxdb`
- `node-red-dashboard` (optional, quick UI)

Flow:

1. `mqtt in` → broker `mosquitto`, topic `honguera/honguera-01/sensors/#`
2. `json` parse
3. `function` → transform to InfluxDB Line Protocol
4. `influxdb out` → bucket `sensors`

## Grafana — base dashboard

Datasource:

- Name: `Honguera InfluxDB`
- URL: `http://influxdb:8086`
- Org: `honguera`
- Token: (copy from InfluxDB)

Panels:

1. Temperature vs time (SHT40)
2. Humidity vs time (SHT40)
3. CO₂ vs time (MH-Z19B)
4. Actuator state (gauge)

## Useful commands

```bash
docker compose logs -f          # live logs
docker compose restart mosquitto
docker compose down             # stop stack
docker compose down -v          # stop + wipe volumes (careful)
```
