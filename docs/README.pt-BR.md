<div align="center">

🌐 [English](../README.md) · [Español](README.es.md) · [Português BR](#)

<img src="../assets/logo.svg" width="100" alt="Honguera">

# 🌿 HONGUERA

### **Hardware Aberto para Cultivo de Precisão Fúngica**

*Um micélio de sensores. Uma rede que pensa. Um corpo que cresce.*

<img src="../assets/mycelium-banner.svg" width="560" alt="Rede de micélio">

[![License: CERN-OHL-S](https://img.shields.io/badge/Hardware-CERN--OHL--S_2.0-8b5cf6?style=flat-square)](../../LICENSE)
[![License: GPLv3](https://img.shields.io/badge/Firmware-GPLv3-6d28d9?style=flat-square)](../../firmware/LICENSE)
[![License: CC BY-SA](https://img.shields.io/badge/Docs-CC_BY--SA_4.0-7c3aed?style=flat-square)](../LICENSE)
[![Platform: ESP32](https://img.shields.io/badge/MCU-ESP32-1a1a2e?style=flat-square&logo=espressif&logoColor=white)](../../firmware/)
[![Protocol: MQTT](https://img.shields.io/badge/Protocolo-MQTT-3b0764?style=flat-square&logo=eclipsemosquitto&logoColor=white)](../../software/docker-compose.yml)
[![Visits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFreeak88%2Fhonguera&count_bg=%237c3aed&title_bg=%231e1b4b&title=visitas&edge_bg=%231e1b4b&count_bg=%238b5cf6)](https://github.com/Freeak88/honguera)

[_specs_](../../SPEC.md) · [ _guia de montagem_ ](../build-guide/) · [ _espécies_ ](../species/profiles.md) · [ _BOM_ ](../../hardware/BOM/BOM_v0.1.md) · [ _contribuir_ ](../../CONTRIBUTING.md)

</div>

---

> **Honguera** é um sistema de climatização de precisão para cultivo fúngico indoor.
> Controla temperatura, umidade e CO₂ com um ESP32, sensores padrão e uma rede de inteligência distribuída.
>
> **Não é um termostato. É um micélio digital.**

Um micélio real não tem cérebro central. Cada hifa percebe, decide e responde localmente — mas compartilha informação com toda a rede. Honguera funciona igual: sensores distribuídos, controle local no nó, e uma camada de ML que aprende a inércia térmica do espaço como o micélio aprende a geometria do substrato.

**Projetado para se replicar. Como um esporo.**

---

## ⚡ Início Rápido

```bash
# Clonar o micélio
git clone https://github.com/Freeak88/honguera.git
cd honguera

# Subir a rede (MQTT + InfluxDB + Node-RED + Grafana)
cd software && docker compose up -d

# Flash do nó (PlatformIO)
cd firmware && pio run --target upload

# Monitorar telemetria
pio device monitor
```

<details>
<summary>🔧 Pré-requisitos</summary>

- ESP32 DevKit (qualquer variante)
- Sensores: SHT40 + MH-Z19B (+ DS18B20 opcional)
- PlatformIO instalado
- Docker + Docker Compose
- Rede WiFi local

Custo total do protótipo: **~$110-170 USD** → [BOM completo](../../hardware/BOM/BOM_v0.1.md)
</details>

---

## 🧬 Arquitetura — O Corpo do Micélio

```
                         ┌─────────────┐
                         │   Micélio   │
                         │ (Camada ML) │
                         │  Preditivo  │
                         └──────┬───────┘
                                │ aprende inércia térmica
                                ▼
┌──────────┐    MQTT     ┌──────────┐    escreve    ┌──────────┐
│ Hifas ×N │◄───────────►│  Manto   │─────────────►│ Solo     │
│ (ESP32)  │   pub/sub   │(Mosquitto)│              │(InfluxDB)│
└────┬─────┘             └──────────┘              └──────────┘
     │                                                     │
     │ percebe        ┌──────────┐                         │
     ├────────────────►│ Fronde   │◄────────────────────────┘
     │                 │(Grafana) │        lê
     │                 └──────────┘
     │
     │ atua
     ▼
┌──────────┐  ┌───────────┐  ┌──────────┐
│ Aquecedor │  │ Umidificador│  │ Extrator  │
│  SSR 700W │  │ Ultrassônico│  │   CO₂     │
└──────────┘  └───────────┘  └──────────┘
```

| Órgão | Componente | Função biológica |
|-------|-----------|------------------|
| **Hifas** | ESP32 + sensores | Percebem o ambiente, decidem localmente |
| **Manto** | Mosquitto (MQTT) | Rede de sinalização entre hifas |
| **Solo** | InfluxDB | Memória. Armazena o aprendizado |
| **Fronde** | Grafana / Node-RED | Visualização. A superfície visível |
| **Micélio** | Camada ML | Inteligência distribuída. Prediz, adapta |

---

## 🍄 Espécies Suportadas

Cada espécie é um "firmware biológico" diferente. Carrega-se como perfil JSON via MQTT:

| Espécie | Temp frutificação | Umidade | CO₂ máx | Dificuldade |
|---------|-------------------:|--------:|--------:|:-----------:|
| 🟤 **Shimeji** _(P. ostreatus)_ | 18°C | 90% | 800ppm | ⭐ |
| 🟡 **Shiitake** _(L. edodes)_ | 18°C | 85% | 1000ppm | ⭐⭐ |
| 🟠 **Juba de Leão** _(H. erinaceus)_ | 18°C | 90% | 600ppm | ⭐⭐ |
| 🔴 **Reishi** _(G. lucidum)_ | 25°C | 90% | 800ppm | ⭐⭐⭐ |

→ [Perfis completos com as 3 fases](../species/profiles.md)

---

## 📐 Hardware

### Especificação da Câmara

| Parâmetro | Valor |
|-----------|-------|
| Dimensões | 2m × 1.5m × 2m |
| Estrutura | Madeira curada + isolamento alumínio/fibra de vidro |
| Capacidade | ~150kg substrato |
| Aquecimento | Cabo piso radiante 700W + SSR |
| Umidificação | 3× transdutores piezoelétricos 1.66MHz |
| Gestão gasosa | Extrator controlado por CO₂ |

→ [SPEC.md](../../SPEC.md) · [Plantas PDF](../../blueprints/) · [BOM](../../hardware/BOM/BOM_v0.1.md)

---

## 💻 Firmware

### Features v0.1

- ✅ Leitura SHT40 (T/H), MH-Z19B (CO₂), DS18B20 (água)
- ✅ Controle com histerese: aquecedor, umidificador, extrator
- ✅ MQTT pub/sub: telemetria JSON a cada 15s
- ✅ Controle de fase remoto (incubação → indução → frutificação)
- ✅ Override manual via MQTT
- ✅ Last Will + status online/offline

---

## 🧠 Stack de Software

```bash
docker compose up -d    # Um comando para subir todo o ecossistema
```

| Serviço | Porta | Papel |
|---------|-------|-------|
| Mosquitto | 1883 | Broker MQTT — o sistema nervoso |
| InfluxDB | 8086 | Série temporal — a memória do micélio |
| Node-RED | 1880 | Orquestração + automação |
| Grafana | 3000 | Dashboards em tempo real |

---

## 🔬 ML Preditivo (Roadmap)

O micélio não reage. **Antecipa.**

| Fase | Modelo | Target |
|------|--------|--------|
| v0.1 | Histerese simples | Funcional ✅ |
| v0.2 | Regressão linear | Aprender inércia térmica |
| v0.3 | LSTM | Predição a 15min, ±0.3°C |
| v0.4 | Federado (multi-nó) | Aprender de outros micélios |

---

## 🗺️ Roadmap

```
┌──────────────┐    ┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│  v0.1 ESPORO │───►│  v0.2 HIFAS  │───►│  v0.3 MICÉLIO│───►│  v1.0 FRUTO │
│  Protótipo   │    │  PCB + WiFi  │    │  ML + Multi  │    │  Release     │
│  quincho BA  │    │  Manager     │    │  nó          │    │  público     │
└──────────────┘    └──────────────┘    └──────────────┘    └──────────────┘
     ✅ AGORA            Q3 2026             Q4 2026            2027
```

---

## 🤝 Comunidade

O micélio cresce em conexão. Não funciona isolado.

### Contribuir

Cada contribuição é uma hifa nova que se junta à rede.

1. Fork → Branch → PR
2. Uma ideia = um PR. Simples.
3. Convenções em [CONTRIBUTING.md](../../CONTRIBUTING.md)

[![PRs Welcome](https://img.shields.io/badge/PRs-bem--vindos-8b5cf6?style=flat-square)](../../CONTRIBUTING.md)

---

## 📜 Licenças

Projeto multi-núcleo. Cada camada respira com sua própria licença:

| Camada | Licença | Por quê |
|--------|---------|---------|
| 🔩 Hardware (PCB, mecânica) | **CERN-OHL-S 2.0** | Derivativos devem permanecer abertos |
| ⚡ Firmware | **GPLv3** | Copyleft forte. Protege a rede |
| 📄 Documentação | **CC BY-SA 4.0** | Atribuição + compartilhar igual |

---

<div align="center">

**Feito com 🍄 pela comunidade hardware aberto**

*Onde a rede de sensores se comporta como uma rede de micélio.*

</div>
