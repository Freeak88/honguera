# Registro de Validación de Simulación Embebida (ESP8266)

- **Fecha:** 2026-05-30
- **Entorno de Compilación:** `esp8266-sim`
- **Puerto de Carga:** `COM15` (USB-SERIAL CH340)
- **Resultado del Upload:** `SUCCESS` (Flasheo exitoso en 30.59 segundos)
- **Resultado del Serial Monitor:** `PASS`
- **Comportamiento WiFi:** `FAILED` esperado, no bloqueante (Expiración del timeout tras 20 segundos sin congelar el bucle principal)
- **Aclaración:** El envío y flujo MQTT end-to-end no ha sido validado en esta prueba debido a la falta de conexión WiFi en el laboratorio local.

---

## Fases de Simulación Observadas y Verificadas

### 1. Fase A (A_NORMAL)
- **Pasos:** 0 a 4
- **Comportamiento:** Lecturas estables y saludables. Alerta inactiva (`orp_anoxia_alert = false`, `Alr=0`) y contadores en cero (`Cnt=0`).

### 2. Fase B (B_ORP_LOW_NO_TRIGGER)
- **Pasos:** 5 a 19
- **Comportamiento:** El ORP desciende a `-10.0 mV`. Sin embargo, al no haber riesgo biológico concomitante (CO2 y Humedad por debajo de los umbrales), el sistema no pre-carga los contadores. La alerta sigue inactiva y `anoxia_reading_count` en `0`.

### 3. Fase C (C_ORP_LOW_WITH_TRIGGER)
- **Pasos:** 20 a 34
- **Comportamiento:** Se combina ORP bajo (`-15.0 mV`) con condiciones biológicas críticas (CO2 = `780 ppm` y Humedad = `88%`).
- **Alerta activada en Fase C:** El acumulador `anoxia_reading_count` se incrementa paso a paso de forma consecutiva. Al alcanzar la lectura 12 consecutiva (final del Paso 31), se activa la alarma:
  `[SAFETY] !!! ALERT: Substrate Anoxia detected (Sustained low ORP + Bio Risk) !!!`
  La bandera de alerta pasa a `true` (`Alr=1`) y el contador se reinicia. Los actuadores lógicos (como el extractor superior) se configuran en ON, manteniéndose los GPIOs físicos apagados por el modo seguro `dry_run_actuators = true`.

### 4. Fase D (D_RECOVERY)
- **Pasos:** 35 a 49
- **Comportamiento:** El ORP se eleva a `+62.0 mV`. El acumulador `recovery_reading_count` escala de forma consecutiva durante 12 lecturas.
- **Alerta resuelta en Fase D:** Al cabo de la lectura 12 consecutiva de recuperación (final del Paso 46), la alerta se desactiva limpiamente:
  `[SAFETY] Alert resolved: Substrate ORP recovered to healthy levels.`
  El estado regresa a `Alr=0` e inicia el siguiente ciclo normal.

---

## Observaciones de Telemetría y Límites MQTT

- **Tamaño de JSON Observado:** **571–591 bytes** serializados (ejemplo: `[MQTT] JSON size: 590 bytes`).
- **Límite de Paquete:** Validado con éxito tras definir la directiva `-DMQTT_MAX_PACKET_SIZE=1024` en `platformio.ini`, previniendo descartes silenciosos en el cliente MQTT.
