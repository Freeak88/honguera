# 🍄 Proyecto "Hoguera" v1.0 — Unidad de Cultivo de Precisión Fúngica

**Creado:** 2026-04-21
**Estado:** Especificación definida
**Ubicación:** Quincho, Buenos Aires

---

## I. Estructura y Aislamiento (La Nave)

- **Dimensiones:** 2m (ancho) × 1.5m (largo) × 2m (altura máxima)
- **Esqueleto:** Madera curada (tirantes 2×2") — soporte ~150kg sustrato
- **Cerramiento:** PVC externo
- **Recubrimiento:** Paneles aluminio + fibra de vidrio (aislante térmico alta eficiencia)
  - Bloqueo total de luz (incubación)
  - Retención calórica para invierno quincho
- **Sistema de carga:** Suspensión vertical tipo macramé, hilo polipropileno
  - FAE 360°, evita acumulación CO₂ en base

## II. Sistema de Climatización de Precisión (Hardware)

### Calefacción Primaria (Calor Seco)
- Cable calefactor losa radiante **700W** sobre base aislante en piso
- Control: ESP32 → Relé Estado Sólido (SSR)

### Humidificación Térmica (Calor Húmedo)
- **Contenedor:** Bidón 10L agua templada
- **Calefacción agua:** Calentador pecera 50W → set 28°C
- **Atomización:** 2-3 transductores piezoeléctricos ultrasónicos 25mm (1.66MHz), drivers USB/5V
- **Inyección:** Cooler 12V fuerza salida niebla templada hacia nave

### Gestión Gaseosa
- Extractor superior controlado por niveles CO₂

## III. Capa de Control e IoT (Nodo Periférico)

- **MCU:** ESP32 ("Hoguera-01")
- **Sensores:**
  - SHT40: Humedad + Temperatura (interior nave)
  - MH-Z19B: CO₂ (PPM)
  - DS18B20: (opcional) sumergible, monitoreo agua bidón
- **Comunicación:** MQTT sobre WiFi local

## IV. Orquestación en Aletheia (Cerebro Agéntico)

- **Stack:** Docker (Mosquitto + InfluxDB + Node-RED)
- **ML Predictivo:**
  - Arquitectura: Red Neuronal regresión simple o LSTM
  - Inputs: T_in, T_ext, Estado actuadores, Timestamp
  - Target: 18°C constantes, desviación <0.5°C
  - Optimización: pulso preventivo del cable 700W según inercia térmica del quincho
  - Aprende inercia del aluminio/fibra de vidrio

## V. Roadmap Biológico (Gírgolas)

| Fase | Condiciones | Duración estimada |
|------|-------------|-------------------|
| **1. Incubación** | Oscuridad total, 24-26°C (calor base 700W) | ~2-3 semanas |
| **2. Inducción** | Choque térmico → 15°C (ventilación nocturna quincho) | 24-48h |
| **3. Fructificación** | Niebla templada cíclica, 18°C constantes, LED 6500K | Hasta cosecha |

---

## VI. Componentes Hardware (BOM)

| Componente | Spec | Cantidad |
|------------|------|----------|
| Cable calefactor losa radiante | 700W | 1 |
| SSR | Compatible ESP32 | 1 |
| Calentador pecera | 50W | 1 |
| Transductores ultrasónicos | 25mm, 1.66MHz | 2-3 |
| Cooler inyección | 12V | 1 |
| ESP32 | — | 1 |
| SHT40 | Temp + Humedad | 1 |
| MH-Z19B | CO₂ PPM | 1 |
| DS18B20 | Sumergible (opcional) | 1 |
| Bidón | 10L | 1 |
| Tirantes madera | 2×2" curada | Según diseño |
| Paneles aluminio+fibra vidrio | Aislante | Según diseño |
| Hilo polipropileno | Macramé | Según diseño |
| LED | 6500K luz fría | 1 tira |

---

## Siguiente paso

Diseñar docker-compose (Mosquitto + InfluxDB + Node-RED) y firmware ESP32 para primer telemetría.
