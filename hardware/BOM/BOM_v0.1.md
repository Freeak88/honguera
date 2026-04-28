# Hoguera — BOM (Bill of Materials)

## v0.1 — Prototipo Quincho

### Control y Sensado

| # | Componente | Spec | Cantidad | Precio estimado (USD) | Notas |
|---|-----------|------|----------|----------------------|-------|
| 1 | ESP32 DevKit | 38 pins, WiFi+BT | 1 | $4-6 | Cualquier variante sirve |
| 2 | SHT40 | Temp + Humedad, I2C | 1 | $3-5 | Adafruit breakout recomendado |
| 3 | MH-Z19B | CO₂ NDIR, UART | 1 | $15-20 | Cuidado con falsificaciones |
| 4 | DS18B20 | Temp sumergible, 1-Wire | 1 | $3-5 | Opcional (agua bidón) |
| 5 | SSR | 5V input, 25A output | 1 | $5-8 | Para cable calefactor |

### Actuadores

| # | Componente | Spec | Cantidad | Precio estimado (USD) | Notas |
|---|-----------|------|----------|----------------------|-------|
| 6 | Cable calefactor | Losa radiante 700W | 1 | $10-15 | Potencia según tamaño nave |
| 7 | Transductor ultrasónico | 25mm, 1.66MHz | 3 | $2-3 c/u | Humidificación |
| 8 | Driver ultrasónico | USB/5V | 3 | $1-2 c/u | Para transductores |
| 9 | Cooler inyección | 12V, 80-120mm | 1 | $3-5 | Empuja niebla a nave |
| 10 | Extractor | 12V, 80-120mm | 1 | $3-5 | Extracción CO₂ techo |

### Estructura

| # | Componente | Spec | Cantidad | Precio estimado (USD) | Notas |
|---|-----------|------|----------|----------------------|-------|
| 11 | Tirantes madera | 2×2" curada | 6-8 | $15-25 | Estructura nave |
| 12 | Paneles aluminio + fibra vidrio | Aislante térmico | Según diseño | $20-30 | Cerramiento |
| 13 | PVC | Planchas externas | Según diseño | $10-15 | Protección externa |
| 14 | Hilo polipropileno | Macramé | 1 rollo | $3-5 | Suspensión sustrato |
| 15 | LED tira | 6500K luz fría | 1 metro | $3-5 | Fructificación |
| 16 | Bidón | 10L plástico | 1 | $3-5 | Reserva agua humidificador |
| 17 | Calentador pecera | 50W sumergible | 1 | $5-8 | Agua templada bidón |

### Electrónica auxiliar

| # | Componente | Cantidad | Precio estimado | Notas |
|---|-----------|----------|----------------|-------|
| 18 | Fuente 12V 5A | 1 | $5-8 | Alimentación general |
| 19 | Step-down LM2596 | 1 | $1 | 12V → 5V (ESP32) |
| 20 | Cables + conectores | — | $5 | Jumper, terminales |
| 21 | Protoboard/PCB | 1 | $2-5 | Montaje |

### Total estimado: **$110-170 USD** (según proveedor y ubicación)

## Notas

- Precios basados en AliExpress / MercadoLibre Argentina, 2026
- Alternativas de sensores: DHT22 (barato pero menos preciso), BME280 (buen reemplazo SHT40)
- El MH-Z19B es el componente más caro; verificar autenticidad (versiones B genuinas tienen calibración ABC)
- Para producción: considerar PCB dedicada (JLCPCB ~$5 por 5 unidades)
