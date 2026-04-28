# Perfiles de Especie — Parámetros de Cultivo

Cada especie requiere condiciones distintas en cada fase. Estos perfiles definen los targets que Hoguera carga vía MQTT.

## Gírgola (Pleurotus ostreatus) — Default

```json
{
  "species": "pleurotus_ostreatus",
  "phases": {
    "incubacion": {
      "temp": 25.0,
      "humidity": 80.0,
      "co2_max": 5000,
      "light": false,
      "duration_days": "14-21"
    },
    "induccion": {
      "temp": 15.0,
      "humidity": 95.0,
      "co2_max": 1000,
      "light": true,
      "duration_days": "1-2"
    },
    "fructificacion": {
      "temp": 18.0,
      "humidity": 90.0,
      "co2_max": 800,
      "light": true,
      "duration_days": "5-14"
    }
  }
}
```

## Shiitake (Lentinula edodes)

```json
{
  "species": "lentinula_edodes",
  "phases": {
    "incubacion": {
      "temp": 22.0,
      "humidity": 75.0,
      "co2_max": 5000,
      "light": false,
      "duration_days": "30-60"
    },
    "induccion": {
      "temp": 16.0,
      "humidity": 95.0,
      "co2_max": 800,
      "light": true,
      "duration_days": "3-5"
    },
    "fructificacion": {
      "temp": 18.0,
      "humidity": 85.0,
      "co2_max": 1000,
      "light": true,
      "duration_days": "7-14"
    }
  }
}
```

## Lion's Mane (Hericium erinaceus)

```json
{
  "species": "hericium_erinaceus",
  "phases": {
    "incubacion": {
      "temp": 22.0,
      "humidity": 80.0,
      "co2_max": 5000,
      "light": false,
      "duration_days": "14-21"
    },
    "induccion": {
      "temp": 18.0,
      "humidity": 95.0,
      "co2_max": 800,
      "light": true,
      "duration_days": "2-3"
    },
    "fructificacion": {
      "temp": 18.0,
      "humidity": 90.0,
      "co2_max": 600,
      "light": true,
      "duration_days": "10-21"
    }
  }
}
```

## Reishi (Ganoderma lucidum)

```json
{
  "species": "ganoderma_lucidum",
  "phases": {
    "incubacion": {
      "temp": 28.0,
      "humidity": 80.0,
      "co2_max": 5000,
      "light": false,
      "duration_days": "21-30"
    },
    "induccion": {
      "temp": 25.0,
      "humidity": 95.0,
      "co2_max": 1000,
      "light": true,
      "duration_days": "3-5"
    },
    "fructificacion": {
      "temp": 25.0,
      "humidity": 90.0,
      "co2_max": 800,
      "light": true,
      "duration_days": "30-60"
    }
  }
}
```

## Agregar una especie

1. Crear archivo JSON en `docs/species/` con nombre de especie
2. Incluir las 3 fases con targets
3. Documentar fuente de los parámetros (paper, experiencia propia, referencia)
4. PR al repo

## Fuentes

- Stamets, P. *Growing Gourmet and Medicinal Mushrooms* (2000)
- Oei, P. *Mushroom Cultivation* (2003)
- Datos experimentales propios (quincho Buenos Aires, 2026)
