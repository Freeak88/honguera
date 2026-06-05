// Landing page copy for the three locales.
const species = [
  { emoji: '🟤', name: 'Oyster', latin: 'Pleurotus ostreatus', temp: '18°C', humidity: '90%', co2: '≤800 ppm', difficulty: '⭐' },
  { emoji: '🟡', name: 'Shiitake', latin: 'Lentinula edodes', temp: '18°C', humidity: '85%', co2: '≤1000 ppm', difficulty: '⭐⭐' },
  { emoji: '🟠', name: "Lion's Mane", latin: 'Hericium erinaceus', temp: '18°C', humidity: '90%', co2: '≤600 ppm', difficulty: '⭐⭐' },
  { emoji: '🔴', name: 'Reishi', latin: 'Ganoderma lucidum', temp: '25°C', humidity: '90%', co2: '≤800 ppm', difficulty: '⭐⭐⭐' },
];

export const en = {
  tagline: 'Open Hardware for Precision Fungal Cultivation',
  description:
    'Open hardware climate control for indoor mushroom growing: ESP32, MQTT, InfluxDB, Grafana and a predictive ML layer. Designed to replicate, like a spore.',
  poem: 'A mycelium of sensors. A network that thinks. A body that grows.',
  nav: { docs: 'Docs', species: 'Species', contribute: 'Contribute' },
  cta: { start: 'Get started', source: 'View source' },
  pitch: {
    intro:
      'Honguera is a precision climate control system for indoor fungal cultivation. It manages temperature, humidity and CO₂ with an ESP32, standard sensors and a distributed intelligence network.',
    punch: "It's not a thermostat. It's a digital mycelium.",
    body:
      'A real mycelium has no central brain. Each hypha senses, decides and responds locally — yet shares information with the entire network. Honguera works the same way: distributed sensors, local control at the node, and an ML layer that learns the thermal inertia of the space.',
  },
  organs: {
    title: 'The Body of the Mycelium',
    sub: 'Five organs, one organism. Each layer maps a biological function to open technology.',
    items: [
      { icon: '🌱', name: 'Hyphae', tech: 'ESP32 + sensors', role: 'Sense the environment, decide locally.' },
      { icon: '🕸️', name: 'Mantle', tech: 'Mosquitto · MQTT', role: 'Signaling network between hyphae.' },
      { icon: '🪨', name: 'Soil', tech: 'InfluxDB', role: 'Memory. Stores what was learned.' },
      { icon: '🍃', name: 'Frond', tech: 'Grafana · Node-RED', role: 'Visualization. The visible surface.' },
      { icon: '🧠', name: 'Mycelium', tech: 'ML layer', role: 'Distributed intelligence. Predicts, adapts.' },
    ],
  },
  species: {
    title: 'Supported Species',
    sub: 'Each species is a distinct "biological firmware", loaded as a JSON profile via MQTT.',
    items: species,
    link: 'Full profiles with all 3 growth phases',
  },
  quickstart: {
    title: 'Replicate. Like a spore.',
    c1: 'Clone the mycelium',
    c2: 'Spin up the network (MQTT + InfluxDB + Node-RED + Grafana)',
    c3: 'Flash the node (PlatformIO)',
    link: 'Full getting started guide',
  },
  roadmap: {
    title: 'Roadmap',
    items: [
      { version: 'v0.1', name: 'SPORE', when: 'Now — prototype, Buenos Aires' },
      { version: 'v0.2', name: 'HYPHAE', when: 'Q3 2026 — PCB + WiFi Manager' },
      { version: 'v0.3', name: 'MYCELIUM', when: 'Q4 2026 — ML + multi-node' },
      { version: 'v1.0', name: 'FRUIT', when: '2027 — public release' },
    ],
  },
  community: {
    title: 'The mycelium grows through connection',
    sub: 'Every contribution is a new hypha joining the network. Hardware, firmware, mycology or docs — all welcome.',
    issues: 'Open an issue',
    guide: 'Contributing guide',
  },
  footer: {
    made: 'Made with 🍄 by the open hardware community',
    by: 'A project by',
    quote: 'Where the sensor network behaves like a hyphal network.',
  },
};

export const es = {
  tagline: 'Hardware Abierto para Cultivo Fúngico de Precisión',
  description:
    'Control climático de hardware abierto para cultivo de hongos: ESP32, MQTT, InfluxDB, Grafana y una capa de ML predictivo. Diseñado para replicarse, como una espora.',
  poem: 'Un micelio de sensores. Una red que piensa. Un cuerpo que crece.',
  nav: { docs: 'Docs', species: 'Especies', contribute: 'Contribuir' },
  cta: { start: 'Empezar', source: 'Ver código' },
  pitch: {
    intro:
      'Honguera es un sistema de control climático de precisión para cultivo fúngico indoor. Gestiona temperatura, humedad y CO₂ con un ESP32, sensores estándar y una red de inteligencia distribuida.',
    punch: 'No es un termostato. Es un micelio digital.',
    body:
      'Un micelio real no tiene cerebro central. Cada hifa sensa, decide y responde localmente — pero comparte información con toda la red. Honguera funciona igual: sensores distribuidos, control local en el nodo y una capa de ML que aprende la inercia térmica del espacio.',
  },
  organs: {
    title: 'El Cuerpo del Micelio',
    sub: 'Cinco órganos, un organismo. Cada capa mapea una función biológica a tecnología abierta.',
    items: [
      { icon: '🌱', name: 'Hifas', tech: 'ESP32 + sensores', role: 'Sensan el ambiente, deciden localmente.' },
      { icon: '🕸️', name: 'Manto', tech: 'Mosquitto · MQTT', role: 'Red de señalización entre hifas.' },
      { icon: '🪨', name: 'Suelo', tech: 'InfluxDB', role: 'Memoria. Guarda lo aprendido.' },
      { icon: '🍃', name: 'Fronda', tech: 'Grafana · Node-RED', role: 'Visualización. La superficie visible.' },
      { icon: '🧠', name: 'Micelio', tech: 'Capa ML', role: 'Inteligencia distribuida. Predice, se adapta.' },
    ],
  },
  species: {
    title: 'Especies Soportadas',
    sub: 'Cada especie es un "firmware biológico" distinto, cargado como perfil JSON vía MQTT.',
    items: [
      { ...species[0], name: 'Gírgola' },
      species[1],
      { ...species[2], name: 'Melena de León' },
      species[3],
    ],
    link: 'Perfiles completos con las 3 fases de crecimiento',
  },
  quickstart: {
    title: 'Replicate. Como una espora.',
    c1: 'Cloná el micelio',
    c2: 'Levantá la red (MQTT + InfluxDB + Node-RED + Grafana)',
    c3: 'Flasheá el nodo (PlatformIO)',
    link: 'Guía completa para empezar',
  },
  roadmap: {
    title: 'Hoja de ruta',
    items: [
      { version: 'v0.1', name: 'ESPORA', when: 'Ahora — prototipo, Buenos Aires' },
      { version: 'v0.2', name: 'HIFAS', when: 'Q3 2026 — PCB + WiFi Manager' },
      { version: 'v0.3', name: 'MICELIO', when: 'Q4 2026 — ML + multinodo' },
      { version: 'v1.0', name: 'FRUTO', when: '2027 — release público' },
    ],
  },
  community: {
    title: 'El micelio crece por conexión',
    sub: 'Cada contribución es una nueva hifa que se une a la red. Hardware, firmware, micología o docs — todo suma.',
    issues: 'Abrir un issue',
    guide: 'Guía de contribución',
  },
  footer: {
    made: 'Hecho con 🍄 por la comunidad de hardware abierto',
    by: 'Un proyecto de',
    quote: 'Donde la red de sensores se comporta como una red de hifas.',
  },
};

export const ptBR = {
  tagline: 'Hardware Aberto para Cultivo Fúngico de Precisão',
  description:
    'Controle climático em hardware aberto para cultivo de cogumelos: ESP32, MQTT, InfluxDB, Grafana e uma camada de ML preditivo. Projetado para se replicar, como um esporo.',
  poem: 'Um micélio de sensores. Uma rede que pensa. Um corpo que cresce.',
  nav: { docs: 'Docs', species: 'Espécies', contribute: 'Contribuir' },
  cta: { start: 'Começar', source: 'Ver código' },
  pitch: {
    intro:
      'Honguera é um sistema de controle climático de precisão para cultivo fúngico indoor. Gerencia temperatura, umidade e CO₂ com um ESP32, sensores padrão e uma rede de inteligência distribuída.',
    punch: 'Não é um termostato. É um micélio digital.',
    body:
      'Um micélio real não tem cérebro central. Cada hifa sente, decide e responde localmente — mas compartilha informação com toda a rede. Honguera funciona do mesmo jeito: sensores distribuídos, controle local no nó e uma camada de ML que aprende a inércia térmica do espaço.',
  },
  organs: {
    title: 'O Corpo do Micélio',
    sub: 'Cinco órgãos, um organismo. Cada camada mapeia uma função biológica para tecnologia aberta.',
    items: [
      { icon: '🌱', name: 'Hifas', tech: 'ESP32 + sensores', role: 'Sentem o ambiente, decidem localmente.' },
      { icon: '🕸️', name: 'Manto', tech: 'Mosquitto · MQTT', role: 'Rede de sinalização entre hifas.' },
      { icon: '🪨', name: 'Solo', tech: 'InfluxDB', role: 'Memória. Guarda o que foi aprendido.' },
      { icon: '🍃', name: 'Fronde', tech: 'Grafana · Node-RED', role: 'Visualização. A superfície visível.' },
      { icon: '🧠', name: 'Micélio', tech: 'Camada ML', role: 'Inteligência distribuída. Prevê, se adapta.' },
    ],
  },
  species: {
    title: 'Espécies Suportadas',
    sub: 'Cada espécie é um "firmware biológico" distinto, carregado como perfil JSON via MQTT.',
    items: [
      { ...species[0], name: 'Shimeji / Ostra' },
      species[1],
      { ...species[2], name: 'Juba de Leão' },
      species[3],
    ],
    link: 'Perfis completos com as 3 fases de crescimento',
  },
  quickstart: {
    title: 'Replique. Como um esporo.',
    c1: 'Clone o micélio',
    c2: 'Suba a rede (MQTT + InfluxDB + Node-RED + Grafana)',
    c3: 'Grave o nó (PlatformIO)',
    link: 'Guia completo para começar',
  },
  roadmap: {
    title: 'Roadmap',
    items: [
      { version: 'v0.1', name: 'ESPORO', when: 'Agora — protótipo, Buenos Aires' },
      { version: 'v0.2', name: 'HIFAS', when: 'Q3 2026 — PCB + WiFi Manager' },
      { version: 'v0.3', name: 'MICÉLIO', when: 'Q4 2026 — ML + multinó' },
      { version: 'v1.0', name: 'FRUTO', when: '2027 — release público' },
    ],
  },
  community: {
    title: 'O micélio cresce por conexão',
    sub: 'Cada contribuição é uma nova hifa se unindo à rede. Hardware, firmware, micologia ou docs — tudo é bem-vindo.',
    issues: 'Abrir uma issue',
    guide: 'Guia de contribuição',
  },
  footer: {
    made: 'Feito com 🍄 pela comunidade de hardware aberto',
    by: 'Um projeto de',
    quote: 'Onde a rede de sensores se comporta como uma rede de hifas.',
  },
};
