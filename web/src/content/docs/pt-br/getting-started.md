---
title: Comece aqui
description: Clone o micélio, suba o stack, grave o nó.
sidebar:
  order: 1
---

Honguera é um sistema de controle climático de precisão para cultivo fúngico indoor. Gerencia temperatura, umidade e CO₂ usando um ESP32, sensores padrão e uma rede de inteligência distribuída.

**Projetado para se replicar. Como um esporo.**

:::note
A documentação completa em português está em andamento — as páginas ainda não traduzidas aparecem em inglês. [Contribuições de tradução são bem-vindas!](https://github.com/Freeak88/honguera/blob/main/CONTRIBUTING.md)
:::

## Pré-requisitos

- ESP32 DevKit (qualquer variante)
- Sensores: SHT40 + MH-Z19B (+ DS18B20 opcional)
- [PlatformIO](https://platformio.org/) instalado
- Docker + Docker Compose
- Rede WiFi local

Custo total do protótipo: **~110–170 USD** → [BOM completa](https://github.com/Freeak88/honguera/blob/main/hardware/BOM/BOM_v0.1.md)

## Início rápido

```bash
# Clone o micélio
git clone https://github.com/Freeak88/honguera.git
cd honguera

# Suba a rede (MQTT + InfluxDB + Node-RED + Grafana)
cd software && docker compose up -d

# Grave o nó (PlatformIO)
cd firmware && pio run --target upload

# Monitore a telemetria
pio device monitor
```

## O que acabou de acontecer?

| Serviço | Porta | Papel |
|---------|-------|-------|
| Mosquitto | 1883 | Broker MQTT — o sistema nervoso |
| InfluxDB | 8086 | Séries temporais — a memória do micélio |
| Node-RED | 1880 | Orquestração + automação |
| Grafana | 3000 | Dashboards em tempo real |

O nó ESP32 ("hifa") publica telemetria JSON a cada 15 segundos via MQTT e escuta comandos de controle e mudança de fase.

## Próximos passos

- Entenda [a arquitetura](/honguera/pt-br/architecture/)
- Revise a [especificação da câmara](/honguera/pt-br/hardware/spec/)
- Implante o [stack de software](/honguera/pt-br/software/deploy/)
- Escolha uma espécie nos [perfis](/honguera/pt-br/species/profiles/)
