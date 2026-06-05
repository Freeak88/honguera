---
title: Arquitectura
description: El cuerpo del micelio — hifas, manto, suelo, fronda y la capa de ML.
sidebar:
  order: 2
---

Un micelio real no tiene cerebro central. Cada hifa sensa, decide y responde localmente — pero comparte información con toda la red. Honguera funciona igual: sensores distribuidos, control local en el nodo y una capa de ML que aprende la inercia térmica del espacio.

```
                         ┌─────────────┐
                         │   Micelio    │
                         │  (Capa ML)   │
                         │  Predictivo  │
                         └──────┬───────┘
                                │ aprende la inercia térmica
                                ▼
┌──────────┐    MQTT     ┌──────────┐   escribe    ┌──────────┐
│ Hifas ×N │◄───────────►│  Manto   │─────────────►│  Suelo   │
│ (ESP32)  │   pub/sub   │(Mosquitto)│              │(InfluxDB)│
└────┬─────┘             └──────────┘              └──────────┘
     │                                                     │
     │ sensa          ┌──────────┐                         │
     ├────────────────►│  Fronda  │◄────────────────────────┘
     │                 │(Grafana) │         lee
     │                 └──────────┘
     │
     │ actúa
     ▼
┌──────────┐  ┌───────────┐  ┌──────────┐
│ Calefactor│  │Humidificad.│  │ Extractor│
│ SSR 700W  │  │ Ultrasónico│  │   CO₂    │
└──────────┘  └───────────┘  └──────────┘
```

## Los órganos

| Órgano | Componente | Función biológica |
|--------|------------|-------------------|
| **Hifas** | ESP32 + sensores | Sensan el ambiente, deciden localmente |
| **Manto** | Mosquitto (MQTT) | Red de señalización entre hifas |
| **Suelo** | InfluxDB | Memoria. Guarda lo aprendido |
| **Fronda** | Grafana / Node-RED | Visualización. La superficie visible |
| **Micelio** | Capa ML | Inteligencia distribuida. Predice, se adapta |

## ML predictivo (hoja de ruta)

El micelio no reacciona. **Anticipa.**

| Fase | Modelo | Objetivo |
|------|--------|----------|
| v0.1 | Histéresis simple | Funcional ✅ |
| v0.2 | Regresión lineal | Aprender inercia térmica |
| v0.3 | LSTM | Predicción a 15 min, ±0.3°C |
| v0.4 | Federado (multinodo) | Aprender de otros micelios |

La capa de ML toma temperatura interior/exterior, estado de actuadores y timestamps como inputs, con objetivo de 18°C constantes y desviación menor a 0.5°C — usando pulsos preventivos del cable de 700 W sincronizados con la inercia térmica del quincho.
