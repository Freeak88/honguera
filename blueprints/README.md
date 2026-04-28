# 📐 Planos Técnicos — Hoguera v1.0

## PDF Generado

**Archivo:** `HOGUERA_v1.0_PLANOS.pdf` (408KB)

**Contenido:**

1. **Portada**
   - Título del proyecto
   - Especificaciones técnicas resumidas
   - Tabla de parámetros clave

2. **Plano 1: Estructura de la Nave**
   - Vista superior (2m × 1.5m)
   - Vista lateral (altura 2m)
   - Sistema de suspensión tipo macramé (9 puntos de anclaje)
   - Ubicación de componentes clave
   - Escala gráfica y cotas

3. **Plano 2: Esquema Eléctrico**
   - Diagrama de conexiones completas
   - RED 220V AC → Fuentes DC (5V + 12V)
   - ESP32 (control) → SSR, Drivers, Coolers
   - Sensores: SHT40, MH-Z19B, DS18B20
   - Leyenda de colores para tipo de cableado

4. **Lista de Componentes (BOM)**
   - 16 componentes con especificaciones
   - Cantidades requeridas

## Generación

**Script:** `generate_blueprints.py`

**Dependencias:**
- `matplotlib` (gráficos técnicos)
- `reportlab` (generación PDF)
- `numpy` (cálculos)

**Ejecución:**
```bash
python3 generate_blueprints.py
```

## Notas Técnicas

### Estructura de la Nave
- **Dimensiones:** 2m (ancho) × 1.5m (largo) × 2m (alto)
- **Material:** Paneles aluminio + fibra de vidrio (aislante térmico)
- **Esqueleto:** Madera curada 2×2" (soporte ~150kg sustrato)
- **Sistema de carga:** Suspensión vertical tipo macramé, hilo polipropileno
- **Anclaje:** 9 puntos distribuidos en matriz 3×3

### Sistema Eléctrico
- **Potencia máxima:** ~750W continuos (cable calefactor 700W + auxiliares)
- **Fuentes DC:**
  - 5V/3A (ESP32 + sensores + transductores)
  - 12V/2A (Coolers + extractor)
- **Control:** SSR (40A) para cable calefactor 700W
- **Seguridad:** SSR requiere disipador térmico

### Ubicación de Componentes
- **ESP32:** Fuera de la nave (protección contra humedad)
- **Cable calefactor:** Piso de la nave (sobre base aislante)
- **Humidificador:** Bidón 10L + transductores + calentador 50W
- **Extractores:** Parte superior (gestión CO₂)
- **Sensores:** Distribuidos para muestreo representativo

## Próximos Pasos

1. ✅ Planos técnicos generados
2. ⏳ Deploy Docker Stack (Mosquitto + InfluxDB + Node-RED)
3. ⏳ Firmware ESP32 (telemetría MQTT)
4. ⏳ Modelo ML predictivo (LSTM/regresión)
5. ⏳ Calibración del sistema

## Referencias

- Especificación completa: `../SPEC.md`
- Topología MQTT: `../MQTT_TOPICS.md`
- Instrucciones deploy: `../DEPLOY.md`

---

**Generado:** 2026-04-21 19:12 UTC-3
**Versión:** 1.0
**Autor:** Aletheia
