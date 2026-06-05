// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

// GitHub Pages: https://freeak88.github.io/honguera/
export default defineConfig({
  site: 'https://freeak88.github.io',
  base: '/honguera',
  integrations: [
    starlight({
      title: 'Honguera',
      description:
        'Open hardware for precision fungal cultivation. A mycelium of sensors — ESP32, MQTT, and a distributed intelligence network.',
      logo: { src: './src/assets/logo.svg' },
      favicon: '/favicon.svg',
      social: [
        { icon: 'github', label: 'GitHub', href: 'https://github.com/Freeak88/honguera' },
      ],
      editLink: {
        baseUrl: 'https://github.com/Freeak88/honguera/edit/main/web/',
      },
      defaultLocale: 'root',
      locales: {
        root: { label: 'English', lang: 'en' },
        es: { label: 'Español', lang: 'es' },
        'pt-br': { label: 'Português (BR)', lang: 'pt-BR' },
      },
      sidebar: [
        {
          label: 'Start here',
          translations: { es: 'Empezar', 'pt-BR': 'Comece aqui' },
          items: ['getting-started', 'architecture'],
        },
        {
          label: 'Hardware',
          items: ['hardware/spec'],
        },
        {
          label: 'Firmware & Software',
          translations: { es: 'Firmware y Software', 'pt-BR': 'Firmware e Software' },
          items: ['firmware', 'software/deploy'],
        },
        {
          label: 'Reference',
          translations: { es: 'Referencia', 'pt-BR': 'Referência' },
          items: ['reference/mqtt-topics', 'species/profiles'],
        },
        {
          label: 'Community',
          translations: { es: 'Comunidad', 'pt-BR': 'Comunidade' },
          items: ['community/contributing'],
        },
      ],
      customCss: ['./src/styles/custom.css'],
    }),
  ],
});
