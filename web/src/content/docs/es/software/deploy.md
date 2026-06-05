---
title: Stack de Software y Deploy
description: Mosquitto, InfluxDB, Node-RED y Grafana — un solo comando de docker compose.
---

Todo el stack del servidor ("Aletheia") se levanta con un solo comando:

```bash
cd software && docker compose up -d
```

| Servicio | Puerto | Rol |
|----------|--------|-----|
| Mosquitto | 1883 | Broker MQTT — el sistema nervioso |
| InfluxDB | 8086 | Series temporales — la memoria del micelio |
| Node-RED | 1880 | Orquestación + automatización |
| Grafana | 3000 | Dashboards en tiempo real |

## Verificar

```bash
docker compose ps
docker compose logs -f mosquitto
```

## Primer test

```bash
# Suscribirse a todos los tópicos del nodo
mosquitto_sub -h localhost -t "honguera/#" -v

# Publicar mensaje de prueba
mosquitto_pub -h localhost -t "honguera/honguera-01/sensors/sht40" \
  -m '{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}'
```

## InfluxDB

Crear token para Node-RED (si no se usó el de init):

```bash
docker exec -it honguera-influxdb influx
influx auth create --org honguera --write-bucket sensors --read-bucket sensors --all-access
```

## Node-RED — primer flow

Instalar paquetes:

- `node-red-contrib-influxdb`
- `node-red-dashboard` (opcional, UI rápida)

Flow:

1. `mqtt in` → broker `mosquitto`, tópico `honguera/honguera-01/sensors/#`
2. `json` parse
3. `function` → transformar a InfluxDB Line Protocol
4. `influxdb out` → bucket `sensors`

## Grafana — dashboard base

Datasource:

- Name: `Honguera InfluxDB`
- URL: `http://influxdb:8086`
- Org: `honguera`
- Token: (copiar de InfluxDB)

Paneles:

1. Temperatura vs tiempo (SHT40)
2. Humedad vs tiempo (SHT40)
3. CO₂ vs tiempo (MH-Z19B)
4. Estado de actuadores (gauge)

## Comandos útiles

```bash
docker compose logs -f          # logs en tiempo real
docker compose restart mosquitto
docker compose down             # parar el stack
docker compose down -v          # parar + borrar volúmenes (cuidado)
```
