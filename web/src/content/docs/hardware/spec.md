---
title: Chamber Specification
description: Structure, insulation, and the precision climate hardware of the cultivation unit.
---

Specification of the v1.0 cultivation unit ("the vessel"). Prototype located in a quincho in Buenos Aires.

## Structure & insulation

- **Dimensions:** 2 m (width) × 1.5 m (length) × 2 m (max height)
- **Skeleton:** cured wood (2×2" beams) — supports ~150 kg of substrate
- **Enclosure:** external PVC
- **Lining:** aluminum + fiberglass panels (high-efficiency thermal insulation)
  - Total light blocking (incubation)
  - Heat retention for winter
- **Loading system:** vertical macramé-style suspension, polypropylene cord
  - 360° air exchange, prevents CO₂ pooling at the base

## Precision climate hardware

### Primary heating (dry heat)
- Radiant floor heating cable, **700 W**, over an insulating base
- Control: ESP32 → solid state relay (SSR)

### Thermal humidification (wet heat)
- **Container:** 10 L drum of tempered water
- **Water heating:** 50 W aquarium heater → set to 28°C
- **Atomization:** 2–3 ultrasonic piezoelectric transducers, 25 mm (1.66 MHz), USB/5V drivers
- **Injection:** 12 V fan pushes warm mist into the chamber

### Gas management
- Top exhaust fan controlled by CO₂ levels

## Control & IoT layer

- **MCU:** ESP32 ("Honguera-01")
- **Sensors:**
  - SHT40 — humidity + temperature (chamber interior)
  - MH-Z19B — CO₂ (ppm)
  - DS18B20 — optional, submersible, water drum monitoring
- **Communication:** MQTT over local WiFi

## Bill of materials

| Component | Spec | Qty |
|-----------|------|-----|
| Radiant floor heating cable | 700 W | 1 |
| SSR | ESP32-compatible | 1 |
| Aquarium heater | 50 W | 1 |
| Ultrasonic transducers | 25 mm, 1.66 MHz | 2–3 |
| Injection fan | 12 V | 1 |
| ESP32 | — | 1 |
| SHT40 | Temp + humidity | 1 |
| MH-Z19B | CO₂ ppm | 1 |
| DS18B20 | Submersible (optional) | 1 |
| Water drum | 10 L | 1 |
| Wood beams | 2×2" cured | per design |
| Aluminum + fiberglass panels | Insulation | per design |
| Polypropylene cord | Macramé | per design |
| LED strip | 6500 K cool light | 1 |

→ [Full BOM with prices](https://github.com/Freeak88/honguera/blob/main/hardware/BOM/BOM_v0.1.md)
