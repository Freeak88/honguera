---
title: Species Profiles
description: Cultivation parameters per species — each one a distinct biological firmware.
---

Each species requires different conditions in each phase. These profiles define the targets that Honguera loads via MQTT.

## Oyster (Pleurotus ostreatus) — default

```json
{
  "species": "pleurotus_ostreatus",
  "phases": {
    "incubacion":      { "temp": 25.0, "humidity": 80.0, "co2_max": 5000, "light": false, "duration_days": "14-21" },
    "induccion":       { "temp": 15.0, "humidity": 95.0, "co2_max": 1000, "light": true,  "duration_days": "1-2" },
    "fructificacion":  { "temp": 18.0, "humidity": 90.0, "co2_max": 800,  "light": true,  "duration_days": "5-14" }
  }
}
```

## Shiitake (Lentinula edodes)

```json
{
  "species": "lentinula_edodes",
  "phases": {
    "incubacion":      { "temp": 22.0, "humidity": 75.0, "co2_max": 5000, "light": false, "duration_days": "30-60" },
    "induccion":       { "temp": 16.0, "humidity": 95.0, "co2_max": 800,  "light": true,  "duration_days": "3-5" },
    "fructificacion":  { "temp": 18.0, "humidity": 85.0, "co2_max": 1000, "light": true,  "duration_days": "7-14" }
  }
}
```

## Lion's Mane (Hericium erinaceus)

```json
{
  "species": "hericium_erinaceus",
  "phases": {
    "incubacion":      { "temp": 22.0, "humidity": 80.0, "co2_max": 5000, "light": false, "duration_days": "14-21" },
    "induccion":       { "temp": 18.0, "humidity": 95.0, "co2_max": 800,  "light": true,  "duration_days": "2-3" },
    "fructificacion":  { "temp": 18.0, "humidity": 90.0, "co2_max": 600,  "light": true,  "duration_days": "10-21" }
  }
}
```

## Reishi (Ganoderma lucidum)

```json
{
  "species": "ganoderma_lucidum",
  "phases": {
    "incubacion":      { "temp": 28.0, "humidity": 80.0, "co2_max": 5000, "light": false, "duration_days": "21-30" },
    "induccion":       { "temp": 25.0, "humidity": 95.0, "co2_max": 1000, "light": true,  "duration_days": "3-5" },
    "fructificacion":  { "temp": 25.0, "humidity": 90.0, "co2_max": 800,  "light": true,  "duration_days": "30-60" }
  }
}
```

## Adding a species

1. Create a JSON file in `docs/species/` named after the species
2. Include all 3 phases with targets
3. Document the source of the parameters (paper, own experience, reference)
4. Open a PR

## Sources

- Stamets, P. *Growing Gourmet and Medicinal Mushrooms* (2000)
- Oei, P. *Mushroom Cultivation* (2003)
- Own experimental data (quincho, Buenos Aires, 2026)
