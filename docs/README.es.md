<div align="center">

🌐 [English](../README.md) · [Español](#) · [Português BR](README.pt-BR.md)

<img src="../assets/logo.svg" width="100" alt="Honguera">

# 🌿 HONGUERA

### **Hardware Abierto para Cultivo de Precisión Fúngica**

*Un micelio de sensores. Una red que piensa. Un cuerpo que crece.*

<img src="../assets/mycelium-banner.svg" width="560" alt="Red de micelio">

[![License: CERN-OHL-S](https://img.shields.io/badge/Hardware-CERN--OHL--S_2.0-8b5cf6?style=flat-square)](../../LICENSE)
[![License: GPLv3](https://img.shields.io/badge/Firmware-GPLv3-6d28d9?style=flat-square)](../../firmware/LICENSE)
[![License: CC BY-SA](https://img.shields.io/badge/Docs-CC_BY--SA_4.0-7c3aed?style=flat-square)](../LICENSE)
[![Platform: ESP32](https://img.shields.io/badge/MCU-ESP32-1a1a2e?style=flat-square&logo=espressif&logoColor=white)](../../firmware/)
[![Protocol: MQTT](https://img.shields.io/badge/Protocolo-MQTT-3b0764?style=flat-square&logo=eclipsemosquitto&logoColor=white)](../../software/docker-compose.yml)
[![Visits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFreeak88%2Fhonguera&count_bg=%237c3aed&title_bg=%231e1b4b&title=visitas&edge_bg=%231e1b4b&count_bg=%238b5cf6)](https://github.com/Freeak88/honguera)

[_specs_](../../SPEC.md) · [ _guía de armado_ ](../build-guide/) · [ _especies_ ](../species/profiles.md) · [ _BOM_ ](../../hardware/BOM/BOM_v0.1.md) · [ _contribuir_ ](../../CONTRIBUTING.md)

</div>

---

> **Honguera** es un sistema de climatización de precisión para cultivo fúngico indoor.
> Controla temperatura, humedad y CO₂ con un ESP32, sensores estándar y una red de inteligencia distribuida.
>
> **No es un termostato. Es un micelio digital.**

Un micelio real no tiene cerebro central. Cada hifa percibe, decide y responde localmente — pero comparte información con toda la red. Honguera funciona igual: sensores distribuidos, control local en el nodo, y una capa de ML que aprende la inercia térmica del espacio como el micelio aprende la geometría del sustrato.

**Diseñado para replicarse. Como una espora.**

---

## ⚡ Inicio Rápido

```bash
# Clonar el micelio
git clone https://github.com/Freeak88/honguera.git
cd honguera

# Levantar la red (MQTT + InfluxDB + Node-RED + Grafana)
cd software && docker compose up -d

# Flashear el nodo (PlatformIO)
cd firmware && pio run --target upload

# Monitorear telemetría
pio device monitor
```

<details>
<summary>🔧 Requisitos previos</summary>

- ESP32 DevKit (cualquier variante)
- Sensores: SHT40 + MH-Z19B (+ DS18B20 opcional)
- PlatformIO instalado
- Docker + Docker Compose
- Red WiFi local

Costo total del prototipo: **~$110-170 USD** → [BOM completo](../../hardware/BOM/BOM_v0.1.md)
</details>

---

## 🧬 Arquitectura — El Cuerpo del Micelio

```
                         ┌─────────────┐
                         │   Micelio    │
                         │ (Capa ML)    │
                         │  Predictivo  │
                         └──────┬───────┘
                                │ aprende inercia térmica
                                ▼
┌──────────┐    MQTT     ┌──────────┐    escribe    ┌──────────┐
│ Hifas ×N │◄───────────►│  Manto   │─────────────►│ Suelo    │
│ (ESP32)  │   pub/sub   │(Mosquitto)│              │(InfluxDB)│
└────┬─────┘             └──────────┘              └──────────┘
     │                                                     │
     │ percibe        ┌──────────┐                         │
     ├────────────────►│ Fronda   │◄────────────────────────┘
     │                 │(Grafana) │        lee
     │                 └──────────┘
     │
     │ actúa
     ▼
┌──────────┐  ┌───────────┐  ┌──────────┐
│ Calefactor│  │ Humidificador│  │ Extractor │
│  SSR 700W │  │ Ultrasónico │  │   CO₂     │
└──────────┘  └───────────┘  └──────────┘
```

| Órgano | Componente | Función biológica |
|--------|-----------|-------------------|
| **Hifas** | ESP32 + sensores | Perciben el entorno, deciden localmente |
| **Manto** | Mosquitto (MQTT) | Red de señalización entre hifas |
| **Suelo** | InfluxDB | Memoria. Almacena lo aprendido |
| **Fronda** | Grafana / Node-RED | Visualización. La superficie visible |
| **Micelio** | Capa ML | Inteligencia distribuida. Predice, adapta |

---

## 🍄 Especies Soportadas

Cada especie es un "firmware biológico" distinto. Se carga como perfil JSON vía MQTT:

| Especie | Temp fructificación | Humedad | CO₂ máx | Dificultad |
|---------|--------------------:|--------:|--------:|:----------:|
| 🟤 **Gírgola** _(P. ostreatus)_ | 18°C | 90% | 800ppm | ⭐ |
| 🟡 **Shiitake** _(L. edodes)_ | 18°C | 85% | 1000ppm | ⭐⭐ |
| 🟠 **Lion's Mane** _(H. erinaceus)_ | 18°C | 90% | 600ppm | ⭐⭐ |
| 🔴 **Reishi** _(G. lucidum)_ | 25°C | 90% | 800ppm | ⭐⭐⭐ |

→ [Perfiles completos con las 3 fases](../species/profiles.md)

---

## 📐 Hardware

### Especificación de la Nave

| Parámetro | Valor |
|-----------|-------|
| Dimensiones | 2m × 1.5m × 2m |
| Estructura | Madera curada + aislamiento aluminio/fibra vidrio |
| Capacidad | ~150kg sustrato |
| Calefacción | Cable losa radiante 700W + SSR |
| Humidificación | 3× transductores piezoeléctricos 1.66MHz |
| Gestión gaseosa | Extractor controlado por CO₂ |

→ [SPEC.md](../../SPEC.md) · [Planos PDF](../../blueprints/) · [BOM](../../hardware/BOM/BOM_v0.1.md)

[![KiCad](https://img.shields.io/badge/PCB-KiCad-1a1a2e?style=flat-square&logo=kicad&logoColor=white)](../../hardware/kicad/)
[![JLCPCB](https://img.shields.io/badge/Producción-JLCPCB-ff6600?style=flat-square)](https://jlcpcb.com/)

---

## 💻 Firmware

```
firmware/
├── src/
│   └── main.cpp       ← Loop de control + sensores + MQTT
├── lib/               ← Librerías propias
└── platformio.ini     ← Dependencias y config
```

### Features v0.1

- ✅ Lectura SHT40 (T/H), MH-Z19B (CO₂), DS18B20 (agua)
- ✅ Control con histéresis: calefactor, humidificador, extractor
- ✅ MQTT pub/sub: telemetría JSON cada 15s
- ✅ Control de fase remoto (incubación → inducción → fructificación)
- ✅ Override manual vía MQTT
- ✅ Last Will + status online/offline

### Próximo

- [ ] OTA updates
- [ ] PID tuning automático
- [ ] Modo ahorro energético (solar-ready)
- [ ] WiFi Manager (AP captive portal)
- [ ] Multi-nodo (varias naves, un broker)

[![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange?style=flat-square&logo=platformio&logoColor=white)](../../firmware/)
[![Arduino](https://img.shields.io/badge/Framework-Arduino-00979D?style=flat-square&logo=arduino&logoColor=white)](../../firmware/)

---

## 🧠 Stack de Software

```bash
docker compose up -d    # Un comando para levantar todo el ecosistema
```

| Servicio | Puerto | Rol |
|----------|--------|-----|
| Mosquitto | 1883 | Broker MQTT — el sistema nervioso |
| InfluxDB | 8086 | Serie temporal — la memoria del micelio |
| Node-RED | 1880 | Orquestación + automatización |
| Grafana | 3000 | Dashboards en tiempo real |

[![Docker](https://img.shields.io/badge/Docker-Compose-2496ED?style=flat-square&logo=docker&logoColor=white)](../../software/docker-compose.yml)
[![InfluxDB](https://img.shields.io/badge/InfluxDB-2.7-22ADF6?style=flat-square&logo=influxdb&logoColor=white)](../../software/)
[![Grafana](https://img.shields.io/badge/Grafana-Latest-F46800?style=flat-square&logo=grafana&logoColor=white)](../../software/)
[![Node-RED](https://img.shields.io/badge/Node--RED-Latest-8F0000?style=flat-square&logo=nodered&logoColor=white)](../../software/)

---

## 🔬 ML Predictivo (Roadmap)

El micelio no reacciona. **Anticipa.**

| Fase | Modelo | Target |
|------|--------|--------|
| v0.1 | Histéresis simple | Funcional ✅ |
| v0.2 | Regresión lineal | Aprender inercia térmica |
| v0.3 | LSTM | Predicción a 15min, ±0.3°C |
| v0.4 | Federado (multi-nodo) | Aprender de otros micelios |

---

## 🗺️ Roadmap

```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│  v0.1 ESPORA │───►│  v0.2 HIFAS  │───►│  v0.3 MICELIO│───►│  v1.0 FRUTO │
│  Prototipo   │    │  PCB + WiFi  │    │  ML + Multi  │    │  Release     │
│  quincho BA  │    │  Manager     │    │  nodo        │    │  público     │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
     ✅ AHORA            Q3 2026             Q4 2026            2027
```

---

## 🤝 Comunidad

El micelio crece en conexión. No funciona aislado.

| Canal | Link |
|-------|------|
| 💬 Discord | _próximamente_ |
| 📰 Hackaday.io | _próximamente_ |
| 📘 Instructables | _próximamente_ |
| 🐛 Issues | [GitHub Issues](../../issues) |
| 📖 Wiki | _próximamente_ |

### Contribuir

Cada contribución es una hifa nueva que se une a la red.

1. Fork → Branch → PR
2. Una idea = un PR. Simple.
3. Convenciones en [CONTRIBUTING.md](../../CONTRIBUTING.md)

[![PRs Welcome](https://img.shields.io/badge/PRs-bienvenidos-8b5cf6?style=flat-square)](../../CONTRIBUTING.md)

---

## 📜 Licencias

Proyecto multinúcleo. Cada capa respira con su propia licencia:

| Capa | Licencia | Por qué |
|------|----------|---------|
| 🔩 Hardware (PCB, mecánica) | **CERN-OHL-S 2.0** | Derivados deben permanecer abierto |
| ⚡ Firmware | **GPLv3** | Copyleft fuerte. Protege la red |
| 📄 Documentación | **CC BY-SA 4.0** | Atribución + compartir igual |

---

<div align="center">

**Hecho con 🍄 por la comunidad hardware abierto**

*Donde la red de sensores se comporta como una red de micelio.*

</div>
