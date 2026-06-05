---
title: Especificación de la Nave
description: Estructura, aislamiento y hardware de climatización de precisión de la unidad de cultivo.
---

Especificación de la unidad de cultivo v1.0 ("la nave"). Prototipo ubicado en un quincho de Buenos Aires.

## Estructura y aislamiento

- **Dimensiones:** 2 m (ancho) × 1.5 m (largo) × 2 m (altura máxima)
- **Esqueleto:** madera curada (tirantes 2×2") — soporte ~150 kg de sustrato
- **Cerramiento:** PVC externo
- **Recubrimiento:** paneles aluminio + fibra de vidrio (aislante térmico de alta eficiencia)
  - Bloqueo total de luz (incubación)
  - Retención calórica para el invierno
- **Sistema de carga:** suspensión vertical tipo macramé, hilo de polipropileno
  - FAE 360°, evita acumulación de CO₂ en la base

## Hardware de climatización de precisión

### Calefacción primaria (calor seco)
- Cable calefactor de losa radiante, **700 W**, sobre base aislante en piso
- Control: ESP32 → relé de estado sólido (SSR)

### Humidificación térmica (calor húmedo)
- **Contenedor:** bidón de 10 L de agua templada
- **Calefacción del agua:** calentador de pecera de 50 W → set 28°C
- **Atomización:** 2–3 transductores piezoeléctricos ultrasónicos de 25 mm (1.66 MHz), drivers USB/5V
- **Inyección:** cooler de 12 V fuerza la salida de niebla templada hacia la nave

### Gestión gaseosa
- Extractor superior controlado por niveles de CO₂

## Capa de control e IoT

- **MCU:** ESP32 ("Honguera-01")
- **Sensores:**
  - SHT40 — humedad + temperatura (interior de la nave)
  - MH-Z19B — CO₂ (ppm)
  - DS18B20 — opcional, sumergible, monitoreo del agua del bidón
- **Comunicación:** MQTT sobre WiFi local

## Lista de materiales

| Componente | Spec | Cantidad |
|------------|------|----------|
| Cable calefactor losa radiante | 700 W | 1 |
| SSR | Compatible ESP32 | 1 |
| Calentador de pecera | 50 W | 1 |
| Transductores ultrasónicos | 25 mm, 1.66 MHz | 2–3 |
| Cooler de inyección | 12 V | 1 |
| ESP32 | — | 1 |
| SHT40 | Temp + humedad | 1 |
| MH-Z19B | CO₂ ppm | 1 |
| DS18B20 | Sumergible (opcional) | 1 |
| Bidón | 10 L | 1 |
| Tirantes de madera | 2×2" curada | Según diseño |
| Paneles aluminio + fibra de vidrio | Aislante | Según diseño |
| Hilo de polipropileno | Macramé | Según diseño |
| Tira LED | 6500 K luz fría | 1 |

→ [BOM completo con precios](https://github.com/Freeak88/honguera/blob/main/hardware/BOM/BOM_v0.1.md)
