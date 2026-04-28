# Deploy — Hoguera Stack (Aletheia)

## 1. Crear estructura de volúmenes

```bash
cd /root/.openclaw/workspace/HOGUERA

mkdir -p mosquitto/{config,data,logs}
mkdir -p influxdb/{data,config}
mkdir -p nodered/data
mkdir -p grafana/{data,provisioning/{datasources,dashboards}}
```

## 2. Arrancar stack

```bash
docker-compose up -d
```

## 3. Verificar estado

```bash
docker-compose ps
docker-compose logs -f mosquitto
```

## 4. Acceso a servicios

- **Mosquitto:** `mqtt://<IP-Aletheia>:1883`
- **InfluxDB:** http://<IP-Aletheia>:8086
  - User: `admin`
  - Password: `hoguera_secure_2026`
  - Org: `hoguera`
  - Bucket: `sensors`
- **Node-RED:** http://<IP-Aletheia>:1880
- **Grafana:** http://<IP-Aletheia>:3000
  - User: `admin`
  - Password: `hoguera_vis_2026`

## 5. Primer test (desde terminal)

```bash
# Suscribirse a todos los tópicos del nodo
mosquitto_sub -h localhost -t "hoguera/#" -v

# Publicar mensaje de prueba
mosquitto_pub -h localhost -t "hoguera/hoguera-01/sensors/sht40" -m '{"temp_c": 23.4, "humidity_pct": 68.2, "ts": 1713738900}'
```

## 6. Configurar InfluxDB (via UI o CLI)

```bash
# Influx CLI dentro del contenedor
docker exec -it hoguera-influxdb influx

# Crear token para Node-RED (si no se usó el de init)
influx auth create --org hoguera --write-bucket sensors --read-bucket sensors --all-access
```

## 7. Node-RED — Primer flow

Instalar paquetes:
- `node-red-contrib-influxdb`
- `node-red-dashboard` (opcional para UI rápida)

Configurar:
1. `mqtt in` → broker `mosquitto`, tópico `hoguera/hoguera-01/sensors/#`
2. `json` parse
3. `function` → transformar a InfluxDB Line Protocol
4. `influxdb out` → bucket `sensors`

## 8. Grafana — Dashboard base

Datasource:
- Name: `Hoguera InfluxDB`
- URL: `http://influxdb:8086`
- Org: `hoguera`
- Token: (copiar de InfluxDB)

Panel simple:
- Panel 1: Temperatura vs Tiempo (SHT40)
- Panel 2: Humedad vs Tiempo (SHT40)
- Panel 3: CO₂ vs Tiempo (MH-Z19B)
- Panel 4: Estado actuadores (gauge)

---

## Comandos útiles

```bash
# Logs en tiempo real
docker-compose logs -f

# Reiniciar servicio
docker-compose restart mosquitto

# Stop stack
docker-compose down

# Limpiar datos (cuidado)
docker-compose down -v
rm -rf mosquitto/data influxdb/data nodered/data grafana/data
```
