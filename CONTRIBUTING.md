# Contribuir a Hoguera

Gracias por tu interés en contribuir.

## Cómo contribuir

### Reportar issues
- Bugs de firmware → label `firmware`
- Errores en esquemas/PCB → label `hardware`
- Mejoras de documentación → label `docs`

### Pull requests
1. Fork del repo
2. Branch descriptivo (`feature/pcb-v2`, `fix/sensor-drift`)
3. Un commit lógico por cambio
4. Describir qué cambia y por qué

### Convenciones

**Firmware (C++):**
- PlatformIO project
- Comentar funciones públicas
- Usar `camelCase` para variables, `PascalCase` para clases

**Hardware (KiCad):**
- Nombrar componentes según estándar (R1, C1, U1...)
- Incluir footprint 3D cuando exista
- Anotar voltajes y potencias en el esquemático

**Documentación:**
- Markdown
- Fotos reales > renders cuando sea posible
- Incluir medidas en mm

### Scope del proyecto

Hoguera se enfoca en:
- Cultivo fúngico indoor de precisión
- Hardware replicable con componentes accesibles
- Control autónomo con feedback de sensores

Fuera de scope:
- Cultivo exterior / invernadero sin control
- Procesamiento post-cosecha
- Análisis genético o de sustratos

## Código de conducta

Ser respetuoso. Construir para la comunidad. Compartir conocimiento.

## Licencia

Al contribuir, aceptas que tu trabajo se distribuya bajo las licencias del proyecto:
- Hardware: CERN-OHL-S 2.0
- Firmware: GPLv3
- Documentación: CC BY-SA 4.0
