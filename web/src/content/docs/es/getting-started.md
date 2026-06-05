---
title: Empezar
description: Cloná el micelio, levantá el stack, flasheá el nodo.
sidebar:
  order: 1
---

Honguera es un sistema de control climático de precisión para cultivo fúngico indoor. Gestiona temperatura, humedad y CO₂ usando un ESP32, sensores estándar y una red de inteligencia distribuida.

**Diseñado para replicarse. Como una espora.**

## Prerrequisitos

- ESP32 DevKit (cualquier variante)
- Sensores: SHT40 + MH-Z19B (+ DS18B20 opcional)
- [PlatformIO](https://platformio.org/) instalado
- Docker + Docker Compose
- Red WiFi local

Costo total del prototipo: **~110–170 USD** → [BOM completo](https://github.com/Freeak88/honguera/blob/main/hardware/BOM/BOM_v0.1.md)

## Inicio rápido

```bash
# Cloná el micelio
git clone https://github.com/Freeak88/honguera.git
cd honguera

# Levantá la red (MQTT + InfluxDB + Node-RED + Grafana)
cd software && docker compose up -d

# Flasheá el nodo (PlatformIO)
cd firmware && pio run --target upload

# Monitoreá la telemetría
pio device monitor
```

## ¿Qué acaba de pasar?

| Servicio | Puerto | Rol |
|----------|--------|-----|
| Mosquitto | 1883 | Broker MQTT — el sistema nervioso |
| InfluxDB | 8086 | Series temporales — la memoria del micelio |
| Node-RED | 1880 | Orquestación + automatización |
| Grafana | 3000 | Dashboards en tiempo real |

El nodo ESP32 ("hifa") publica telemetría JSON cada 15 segundos por MQTT y escucha comandos de control y cambio de fase.

## Próximos pasos

- Entendé [la arquitectura](/honguera/es/architecture/)
- Revisá la [especificación de la nave](/honguera/es/hardware/spec/)
- Desplegá el [stack de software](/honguera/es/software/deploy/)
- Elegí una especie en los [perfiles](/honguera/es/species/profiles/)
