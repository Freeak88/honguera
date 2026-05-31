# 🌿 HONGUERA — Contrato de Telemetría e Interfaz de Datos MVP

Este documento define el contrato de datos oficial para el sistema **Honguera v0.1**. Establece el formato de la telemetría publicada por los nodos embebidos, la normalización que realiza la capa de integración (Node-RED) y la persistencia en series temporales en InfluxDB v2, junto con las consultas Flux analíticas listas para el panel de Grafana.

---

## 📡 1. Especificación del Transporte MQTT

* **Topic de Publicación:** `honguera/+/telemetry` (donde `+` es el ID dinámico del nodo, ej: `01`)
* **Intervalo de Publicación:** Cada 15 segundos (configurable en firmware).
* **Calidad de Servicio (QoS):** `1` (Entrega asegurada al menos una vez).

### Payload MQTT Esperado (JSON Emitido por el Nodo)
El firmware del ESP32 emite el siguiente formato JSON en el topic indicado, incluyendo el bloque centralizado de seguridad (`safety`):

```json
{
  "version": "0.1.0",
  "millis": 45120,
  "sensors": {
    "temperature": 18.2,
    "humidity": 89.5,
    "co2": 720,
    "water_temp": 16.5,
    "sustrate_moisture": 64,
    "enzymatic_orp": -12.5,
    "substrate_orp_mv": -12.5
  },
  "actuators": {
    "heater": false,
    "humidifier": true,
    "extractor": false,
    "extractor_aire": false,
    "valvula_riego": false,
    "light": false
  },
  "targets": {
    "temp": 18.0,
    "humidity": 90.0,
    "co2_max": 800
  },
  "safety": {
    "dry_run_actuators": true,
    "actuator_active_high": true,
    "orp_anoxia_alert": false,
    "anoxia_reading_count": 0,
    "recovery_reading_count": 0
  },
  "rssi": -65
}
```

---

## 🔄 2. Normalización y Mapeo en Node-RED

La capa Node-RED suscribe al broker, intercepta la telemetría del nodo y realiza un **mapeo defensivo** para estandarizar los nombres de variables del backend sin alterar la compatibilidad del firmware legado.

### Tabla de Normalización General:
| Campo en Firmware (ESP32) | Campo en InfluxDB (Normalizado) | Tipo de Dato | Función / Tratamiento |
| :--- | :--- | :--- | :--- |
| `sensors.temperature` | `air_temperature` | Float | Temperatura del aire (°C). |
| `sensors.humidity` | `air_humidity` | Float | Humedad relativa del aire (%). |
| `sensors.co2` | `co2_ppm` | Integer | Concentración de CO₂ (ppm). |
| `sensors.water_temp` | `water_temp` | Float | Temperatura del agua de humidificación (°C). |
| `sensors.sustrate_moisture` | `substrate_moisture_pct` | Integer | Humedad del sustrato mapeada a porcentaje (0-100%). |
| `sensors.enzymatic_orp` o `substrate_orp_mv` | `substrate_orp_mv` | Float | Potencial de oxidación-reducción estimado (mV). |
| `actuators.heater` | `heater` | Integer (0/1) | Estado del calefactor SSR. Mapeado de bool a 0/1. |
| `actuators.humidifier` | `humidifier` | Integer (0/1) | Estado del humidificador. Mapeado de bool a 0/1. |
| `actuators.extractor_aire` | `extractor_aire` | Integer (0/1) | Estado del extractor. Mapeado de bool (o campo legado `extractor`) a 0/1. |
| `actuators.valvula_riego` | `valvula_riego` | Integer (0/1) | Estado de la solenoide de irrigación. Mapeado de bool a 0/1. |
| `actuators.light` | `light` | Integer (0/1) | Estado de luces LED de crecimiento. Mapeado de bool a 0/1. |
| `rssi` | `rssi` | Integer | Calidad de señal Wi-Fi (dBm). Conservado como entero. |

### Campos del Bloque de Seguridad (`safety`):
| Campo en Payload | Campo en InfluxDB | Tipo de Dato | Significado / Mapeo |
| :--- | :--- | :--- | :--- |
| `safety.dry_run_actuators` | `dry_run_actuators` | Integer (0/1) | Estado del modo simulación (1 = relés aislados, 0 = activo). |
| `safety.actuator_active_high` | `actuator_active_high` | Integer (0/1) | Nivel lógico de relés (1 = active HIGH, 0 = active LOW). |
| `safety.orp_anoxia_alert` | `orp_anoxia_alert` | Integer (0/1) | Alerta física activa por anoxia sostenida en sustrato (1 = Alerta, 0 = OK). |
| `safety.anoxia_reading_count` | `anoxia_reading_count` | Integer | Contador de lecturas consecutivas con ORP por debajo del umbral (< 0 mV). |
| `safety.recovery_reading_count`| `recovery_reading_count`| Integer | Contador de lecturas consecutivas con ORP en recuperación (> +50 mV). |

### 🛡️ Políticas de Programación Defensiva (Middleware Node-RED)
1. **Prevención de Caídas:** Si una métrica de sensor viene corrupta (`NaN`), vacía (`null`) o inexistente (`undefined`), Node-RED omite únicamente ese campo específico. La estructura de InfluxDB permite escrituras parciales, garantizando que el resto de las métricas válidas persistan de manera segura.
2. **Filtrado de Valores Especiales:** Si un microcontrolador se reinicia o reporta fallos en el bus I2C entregando strings en lugar de números, Node-RED intenta parsearlos. Si el resultado es inválido, descarta la métrica individual de manera silenciosa para no contaminar la base de datos temporal.

---

## 🗄️ 3. Estructura de Persistencia InfluxDB v2

Los datos son estructurados en el bucket `telemetry` de InfluxDB con los siguientes elementos:

* **Medición (Measurement):** `telemetry`
* **Etiquetas (Tags):**
  * `node`: ID dinámico del nodo (obtenido del topic, ej: `01`). Permite separar la telemetría de múltiples hongueras en una misma red distribuida.
  * `version`: Versión del firmware para control de despliegue.
* **Campos (Fields):** Listado normalizado de variables de sensores, actuadores y seguridad.

---

## 🔬 4. Significado Biológico de las Métricas

### 💨 Dióxido de Carbono (`co2_ppm`)
Los hongos respiran consumiendo oxígeno ($O_2$) y exhalando dióxido de carbono ($CO_2$).
* **Fase de Incubación (Colonización del Micelio):** Se toleran niveles elevados de $CO_2$ (hasta 5,000 ppm o más) ya que esto estimula el crecimiento vegetativo de las hifas en el sustrato.
* **Fase de Fructificación (Aparición de Setas):** El micelio necesita ventilación constante. Niveles por encima de 800 - 1000 ppm provocan setas deformes (tallos largos y sombreros diminutos, buscando oxígeno) o abortos.

### ⚡ Potencial Redox del Sustrato (`substrate_orp_mv`)
Mide la actividad de transferencia de electrones en el microentorno del sustrato.
* **Oxigenación y Aerobiosis (ORP > 0 mV):** Indica un sustrato saludable con actividad aeróbica adecuada. Las enzimas fúngicas extracelulares (como las lacasas y peroxidasas) requieren un potencial oxidativo óptimo para degradar lignina y celulosa.
* **Anoxia y Asfixia (ORP < 0 mV):** Indica saturación de agua o falta de intercambio gaseoso en la bolsa de cultivo. Se detiene el metabolismo fúngico secundario y se favorece la fermentación bacteriana indeseada (contaminación).

> [!WARNING]
> **Advertencia de Interpretación Química-Biológica:** El potencial Redox (ORP) obtenido mediante una sonda de platino es un **indicador proxy indirecto** de la oxigenación y el estado electroquímico del sustrato. No representa una medición directa de la concentración o actividad molecular de las enzimas fúngicas (ej. lacasa), sino de las condiciones químicas del medio que posibilitan o restringen dicha actividad metabólica.

### 💧 Humedad de Sustrato (`substrate_moisture_pct`)
El micelio requiere un sustrato húmedo ($60-70\%$), pero no encharcado. Un sustrato demasiado seco impide el transporte de nutrientes solubles. Un sustrato empapado ($>75\%$) elimina los microporos de aire provocando anoxia inmediata (caída abrupta del ORP) y pudrición de las hifas.

---

## 📊 5. Consultas Flux Listas para Grafana MVP

Configura estas consultas en tus paneles de Grafana utilizando el motor de datos de InfluxDB v2:

### A. Panel de Seguridad: Diagnóstico y Alerta de Anoxia ORP (¡NUEVO!)
Permite observar la evolución temporal del ORP, los contadores de ventana de tiempo y la consecuente activación del extractor para mitigar la anoxia.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "substrate_orp_mv" or r["_field"] == "orp_anoxia_alert" or r["_field"] == "anoxia_reading_count" or r["_field"] == "extractor_aire")
  |> aggregateWindow(every: v.windowPeriod, fn: last, createEmpty: false)
  |> yield(name: "safety_monitoring")
```
*(Tip: Configura `orp_anoxia_alert` y `extractor_aire` como líneas de color brillante de tipo escala binaria/llenado en segundo plano, y `substrate_orp_mv` en un eje Y a la izquierda en mV).*

### B. Correlación de Actividad Biológica (CO₂ vs ORP)
Permite verificar la salud respiratoria del micelio y su respuesta metabólica en el sustrato.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "co2_ppm" or r["_field"] == "substrate_orp_mv")
  |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
  |> yield(name: "biocontrol")
```
*(Tip: Configura dos ejes Y. Eje izquierdo para ppm de CO₂ y eje derecho para ORP en mV).*

### C. Dinámica de Humedad del Sustrato e Irrigación
Permite comprobar la respuesta física del sustrato al activarse la solenoide de riego.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "substrate_moisture_pct" or r["_field"] == "valvula_riego")
  |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
  |> yield(name: "irrigacion")
```
*(Tip: Grafica `valvula_riego` como una serie binaria de fondo o un panel "State Timeline").*

### D. Clima Aéreo de la Honguera (Temperatura vs Humedad del Aire)
Verifica que las condiciones ambientales aéreas estén en el rango de fructificación óptima.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "air_temperature" or r["_field"] == "air_humidity")
  |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
  |> yield(name: "clima_aereo")
```

### E. Historial de Actuadores (Timeline de Relés)
Representación visual del estado de todos los actuadores físicos en el tiempo.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "heater" or r["_field"] == "humidifier" or r["_field"] == "extractor_aire" or r["_field"] == "valvula_riego" or r["_field"] == "light")
  |> aggregateWindow(every: v.windowPeriod, fn: last, createEmpty: false)
  |> yield(name: "actuadores")
```
*(Tip: Utiliza el panel nativo **State Timeline** de Grafana para ver las activaciones del sistema).*

### F. Diagnóstico de Conexión (RSSI)
Supervisa la intensidad de señal Wi-Fi recibida en la honguera para prever desconexiones del nodo.
```flux
from(bucket: "telemetry")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "telemetry")
  |> filter(fn: (r) => r["_field"] == "rssi")
  |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
  |> yield(name: "diagnostico_wifi")
```
