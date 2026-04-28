#!/usr/bin/env python3
"""
Hoguera - Generador de Planos Técnicos
Genera PDF con:
1. Plano de la nave (estructura física)
2. Esquema eléctrico
3. Lista de componentes
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import Rectangle, FancyBboxPatch, Circle
import numpy as np
from reportlab.lib.pagesizes import A3, landscape
from reportlab.pdfgen import canvas
from reportlab.lib.units import cm, mm
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Image, Table, TableStyle
from reportlab.lib import colors
import tempfile
import os
import datetime

# Configuración
OUTPUT_PDF = "/root/.openclaw/workspace/HOGUERA/blueprints/HOGUERA_v1.0_PLANOS.pdf"
TEMP_DIR = "/root/.openclaw/workspace/HOGUERA/blueprints/temp"

os.makedirs(TEMP_DIR, exist_ok=True)
os.makedirs(os.path.dirname(OUTPUT_PDF), exist_ok=True)

def draw_nave_structure():
    """Genera plano de la estructura de la nave"""
    fig, ax = plt.subplots(figsize=(18, 12), dpi=150)

    # Configuración del plano
    ax.set_xlim(-0.5, 2.5)
    ax.set_ylim(-0.5, 2.5)
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.3)
    ax.set_xlabel('Metros (m)', fontsize=10, fontweight='bold')
    ax.set_ylabel('Metros (m)', fontsize=10, fontweight='bold')
    ax.set_title('HOGUERA v1.0 - Estructura de la Nave\nDimensiones: 2m × 1.5m × 2m (alto)',
                 fontsize=16, fontweight='bold', pad=20)

    # Nave principal (vista superior)
    nave = Rectangle((0, 0), 2, 1.5,
                     linewidth=3,
                     edgecolor='#2E7D32',
                     facecolor='#E8F5E9',
                     label='Nave Principal')
    ax.add_patch(nave)

    # Estructura interna de carga (esqueleto madera)
    # Tirantes longitudinales
    for x in [0.25, 0.75, 1.25, 1.75]:
        tirante = Rectangle((x, 0), 0.05, 1.5,
                           linewidth=1,
                           edgecolor='#8B4513',
                           facecolor='#D2B48C',
                           alpha=0.7)
        ax.add_patch(tirante)

    # Tirantes transversales
    for y in [0.25, 0.75, 1.25]:
        tirante = Rectangle((0, y), 2, 0.05,
                           linewidth=1,
                           edgecolor='#8B4513',
                           facecolor='#D2B48C',
                           alpha=0.7)
        ax.add_patch(tirante)

    # Sistema de suspensión tipo macramé (puntos de anclaje)
    anchor_points = [(0.5, 0.375), (1.0, 0.375), (1.5, 0.375),
                     (0.5, 0.75), (1.0, 0.75), (1.5, 0.75),
                     (0.5, 1.125), (1.0, 1.125), (1.5, 1.125)]

    for i, (x, y) in enumerate(anchor_points):
        circle = Circle((x, y), 0.03, facecolor='#FF6F00', edgecolor='#E65100', linewidth=2)
        ax.add_patch(circle)
        ax.text(x, y + 0.08, f'B{i+1}', ha='center', fontsize=7, fontweight='bold')

    # Componentes clave (ubicación aproximada)
    components = [
        ('Cable Calefactor (700W)', 1.0, 0.1, '#E53935'),
        ('Humidificador', 0.2, 1.3, '#1E88E5'),
        ('Extractor CO₂', 1.8, 1.3, '#43A047'),
        ('LED 6500K', 1.0, 1.4, '#FFD600'),
        ('Sensor SHT40', 0.1, 0.1, '#9C27B0'),
        ('Sensor CO₂', 1.9, 1.4, '#9C27B0'),
    ]

    for label, x, y, color in components:
        rect = FancyBboxPatch((x - 0.15, y - 0.08), 0.3, 0.16,
                              boxstyle="round,pad=0.02",
                              linewidth=1.5,
                              edgecolor=color,
                              facecolor=color,
                              alpha=0.6)
        ax.add_patch(rect)
        ax.text(x, y, label, ha='center', va='center', fontsize=6,
                bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9, edgecolor='none'))

    # Escala gráfica
    ax.plot([0.1, 0.6], [-0.3, -0.3], 'k-', linewidth=2)
    ax.text(0.35, -0.4, '0.5 m', ha='center', fontsize=9, fontweight='bold')

    # Leyenda
    legend_elements = [
        patches.Patch(facecolor='#E8F5E9', edgecolor='#2E7D32', label='Nave (Aluminio+Fibra)'),
        patches.Patch(facecolor='#D2B48C', edgecolor='#8B4513', label='Estructura Madera (2x2")'),
        patches.Patch(facecolor='#FF6F00', edgecolor='#E65100', label='Puntos Suspensión'),
    ]
    ax.legend(handles=legend_elements, loc='upper right', fontsize=9,
              bbox_to_anchor=(2.35, 1), borderaxespad=0.)

    # Anotaciones técnicas
    ax.text(1.0, 0.75, 'Área de Cultivo\n~150kg sustrato',
            ha='center', va='center', fontsize=9, fontweight='bold',
            bbox=dict(boxstyle='round,pad=0.5', facecolor='yellow', alpha=0.3))

    # Vista lateral (insert)
    ax_inset = ax.inset_axes([0.65, 0.65, 0.3, 0.3])
    ax_inset.set_xlim(-0.2, 2.2)
    ax_inset.set_ylim(0, 2.2)
    ax_inset.set_aspect('equal')
    ax_inset.set_title('Vista Lateral', fontsize=8, fontweight='bold')

    # Perfil lateral
    ax_inset.add_patch(Rectangle((0, 0), 2, 2,
                               linewidth=2,
                               edgecolor='#2E7D32',
                               facecolor='#E8F5E9',
                               alpha=0.7))

    # Altura máxima
    ax_inset.arrow(2.1, 0, 0, 2, head_width=0.1, head_length=0.1, fc='red', ec='red', linewidth=2)
    ax_inset.text(2.15, 1, '2m\nALTURA', va='center', fontsize=7, color='red', fontweight='bold')

    # Ancho
    ax_inset.arrow(0, 2.1, 2, 0, head_width=0.1, head_length=0.1, fc='blue', ec='blue', linewidth=2)
    ax_inset.text(1, 2.15, '2m ANCHO', ha='center', fontsize=7, color='blue', fontweight='bold')

    # Cable calefactor en piso
    ax_inset.plot([0, 2], [0.05, 0.05], 'r-', linewidth=3, alpha=0.7)
    ax_inset.text(1, 0.15, 'Cable Calefactor 700W', ha='center', fontsize=6, color='red')

    plt.tight_layout(pad=3.0)
    temp_file = os.path.join(TEMP_DIR, '01_nave_estructura.png')
    plt.savefig(temp_file, dpi=150, bbox_inches='tight')
    plt.close()
    return temp_file

def draw_electrical_schematic():
    """Genera esquema eléctrico con layout expandido"""
    fig, ax = plt.subplots(figsize=(26, 20), dpi=150)

    # Expandir área de dibujo
    ax.set_xlim(-2, 18)
    ax.set_ylim(-1, 13)
    ax.set_aspect('equal')
    ax.grid(True, linestyle=':', alpha=0.3)
    ax.set_title('HOGUERA v1.0 - Esquema Eléctrico',
                 fontsize=20, fontweight='bold', pad=30)
    ax.axis('off')

    # Función para dibujar componente
    def draw_component(x, y, width, height, label, color='lightblue', linewidth=2):
        rect = FancyBboxPatch((x - width/2, y - height/2), width, height,
                              boxstyle="round,pad=0.1",
                              linewidth=linewidth,
                              edgecolor='black',
                              facecolor=color)
        ax.add_patch(rect)
        ax.text(x, y, label, ha='center', va='center',
                fontsize=9, fontweight='bold', wrap=True)

    # Layout horizontal expandido
    # Fila superior: 220V y Fuentes DC
    draw_component(3, 11.5, 2, 1.4, 'RED\n220V AC', '#FFEBEE', linewidth=3)
    draw_component(7, 11.5, 2, 1.4, 'Fuente DC\n5V/3A', '#E3F2FD', linewidth=2.5)
    draw_component(11, 11.5, 2, 1.4, 'Fuente DC\n12V/2A', '#E8F5E9', linewidth=2.5)

    # Fila media: ESP32
    draw_component(7, 8, 3, 3, 'ESP32\n(Hoguera-01)\nWiFi+MQTT', '#FFF9C4', linewidth=3)

    # Fila media-derecha: Actuadores principales
    draw_component(13, 9, 2, 1.4, 'SSR\n700W', '#FFCDD2', linewidth=2.5)
    draw_component(16, 9, 2, 1.4, 'Cable\nCalefactor\n700W', '#FFAB91', linewidth=2.5)

    draw_component(13, 7, 2, 1.4, 'Driver\nTransductores', '#C8E6C9', linewidth=2)
    draw_component(16, 7, 2, 1.4, 'Transductores\n2-3x', '#A5D6A7', linewidth=2)

    draw_component(13, 5, 2, 1.4, 'Cooler\nInyección', '#B3E5FC', linewidth=2)
    draw_component(16, 5, 2, 1.4, 'Calentador\nPecera', '#FFCC80', linewidth=2)

    draw_component(13, 3, 2, 1.4, 'Extractor\nCO₂', '#B3E5FC', linewidth=2)
    draw_component(16, 3, 2, 1.4, 'LED\n6500K', '#FFF59D', linewidth=2)

    # Fila inferior: Sensores
    draw_component(4, 1, 2, 1.2, 'SHT40\nT/H', '#E1BEE7', linewidth=2)
    draw_component(7, 1, 2, 1.2, 'MH-Z19B\nCO₂', '#E1BEE7', linewidth=2)
    draw_component(10, 1, 2, 1.2, 'DS18B20\nAgua', '#E1BEE7', linewidth=2)

    # ==================== LÍNEAS DE CONEXIÓN ====================

    # Líneas 220V AC (RED → Fuentes DC)
    ax.annotate('', xy=(7, 11), xytext=(4, 11.5),
                arrowprops=dict(arrowstyle='->', lw=3, color='red'))
    ax.text(5.5, 11.8, '220V', fontsize=9, color='red', fontweight='bold')

    ax.annotate('', xy=(11, 11), xytext=(4, 11.2),
                arrowprops=dict(arrowstyle='->', lw=3, color='red'))

    # Líneas DC 5V (Fuente 5V → ESP32)
    ax.plot([7, 7], [10.8, 9.5], 'b-', lw=3)
    ax.text(7.3, 10, '5V', fontsize=9, color='blue', fontweight='bold')

    # Líneas DC 12V (Fuente 12V → Coolers)
    ax.plot([11, 11], [10.8, 5], 'b-', lw=3)
    ax.plot([11, 13], [5, 5], 'b-', lw=2)
    ax.plot([11, 13], [5, 3], 'b-', lw=2)
    ax.text(11.3, 6, '12V', fontsize=9, color='blue', fontweight='bold')

    # Líneas de control (ESP32 → Actuadores)
    # ESP32 → SSR
    ax.plot([8.5, 13], [9, 9], 'purple', lw=2)
    ax.text(10.5, 9.3, 'GPIO', fontsize=8, color='purple')

    # ESP32 → Driver transductores
    ax.plot([8.5, 13], [7.5, 7], 'purple', lw=2)
    ax.text(10.5, 7.8, 'GPIO', fontsize=8, color='purple')

    # ESP32 → Cooler inyección
    ax.plot([8.5, 13], [7, 5], 'purple', lw=2)
    ax.text(10.5, 6.2, 'PWM', fontsize=8, color='purple')

    # ESP32 → Extractor
    ax.plot([8.5, 13], [6.5, 3], 'purple', lw=2)
    ax.text(10.5, 4.8, 'PWM', fontsize=8, color='purple')

    # Líneas de sensores (ESP32 → Sensores)
    ax.plot([7, 7], [6.5, 1.6], 'g-', lw=2)
    ax.plot([7, 4], [6.5, 1], 'g-', lw=2)
    ax.plot([7, 10], [6.5, 1], 'g-', lw=2)
    ax.text(5.5, 4, 'I2C', fontsize=8, color='green')
    ax.text(7.5, 4, 'UART', fontsize=8, color='green')
    ax.text(9.5, 4, '1-Wire', fontsize=8, color='green')

    # Línea de alta potencia (SSR → Cable calefactor)
    ax.plot([14, 16], [9, 9], 'red', lw=5)
    ax.text(15, 9.5, '220V', fontsize=9, color='red', fontweight='bold')

    # Línea de potencia 5V (Fuente 5V → Driver)
    ax.plot([7, 13], [10.5, 7], 'b-', lw=1.5)
    ax.text(9, 8.8, '5V', fontsize=8, color='blue')

    # Leyenda
    legend_elements = [
        patches.Patch(facecolor='red', label='220V AC (Potencia)'),
        patches.Patch(facecolor='blue', label='DC (Alimentación)'),
        patches.Patch(facecolor='green', label='I2C/UART/1-Wire (Sensores)'),
        patches.Patch(facecolor='purple', label='GPIO/PWM (Control)'),
    ]
    ax.legend(handles=legend_elements, loc='lower right', fontsize=11,
              bbox_to_anchor=(15, 0.5), borderaxespad=0.)

    plt.tight_layout(pad=3.0)
    temp_file = os.path.join(TEMP_DIR, '02_esquema_electrico.png')
    plt.savefig(temp_file, dpi=150, bbox_inches='tight')
    plt.close()
    return temp_file

def generate_pdf(nave_file, electrical_file):
    """Genera PDF final con todos los planos"""
    doc = SimpleDocTemplate(OUTPUT_PDF, pagesize=landscape(A3),
                           rightMargin=36, leftMargin=36,
                           topMargin=36, bottomMargin=18)

    story = []
    styles = getSampleStyleSheet()

    # Portada
    story.append(Paragraph("HOGUERA v1.0 - Planos Técnicos",
                          styles['Title']))
    story.append(Spacer(1, 0.5 * cm))

    story.append(Paragraph("Unidad de Cultivo de Precisión Fúngica",
                          styles['Heading2']))
    story.append(Paragraph("Gírgolas (Pleurotus ostreatus) - Quincho, Buenos Aires",
                          styles['Normal']))
    story.append(Spacer(1, 0.5 * cm))

    # Especificaciones técnicas
    specs_data = [
        ['Parámetro', 'Valor'],
        ['Dimensiones Nave', '2m × 1.5m × 2m (alto)'],
        ['Sustrato Total', '~150kg'],
        ['Calefacción Principal', 'Cable 700W (SSR controlado)'],
        ['Humidificación', 'Transductores 1.66MHz + Calentador 50W'],
        ['Control', 'ESP32 + MQTT'],
        ['Stack Backend', 'Mosquitto + InfluxDB + Node-RED'],
        ['Temperatura Objetivo', '18°C ±0.5°C'],
        ['Fases', 'Incubación → Inducción → Fructificación'],
    ]

    specs_table = Table(specs_data, colWidths=[6*cm, 15*cm])
    specs_table.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (0, -1), colors.grey),
        ('TEXTCOLOR', (0, 0), (0, -1), colors.whitesmoke),
        ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
        ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
        ('FONTSIZE', (0, 0), (-1, 0), 11),
        ('BOTTOMPADDING', (0, 0), (-1, 0), 12),
        ('BACKGROUND', (1, 0), (-1, -1), colors.beige),
        ('GRID', (0, 0), (-1, -1), 1, colors.black),
    ]))
    story.append(specs_table)
    story.append(Spacer(1, 1 * cm))

    # Plano 1: Estructura de la Nave
    story.append(Paragraph("1. Estructura de la Nave (Vista Superior + Lateral)",
                          styles['Heading3']))
    story.append(Spacer(1, 0.3 * cm))

    if os.path.exists(nave_file):
        img = Image(nave_file, width=40*cm, height=26*cm)
        story.append(img)
    else:
        story.append(Paragraph("[Imagen no disponible]", styles['Normal']))

    story.append(Spacer(1, 0.5 * cm))
    story.append(Paragraph("• Estructura de carga en madera curada (tirantes 2×2\")",
                          styles['Normal']))
    story.append(Paragraph("• Recubrimiento: Aluminio + Fibra de Vidrio (aislante térmico)",
                          styles['Normal']))
    story.append(Paragraph("• Sistema de suspensión tipo macramé (9 puntos de anclaje)",
                          styles['Normal']))
    story.append(Spacer(1, 1 * cm))

    # Plano 2: Esquema Eléctrico
    story.append(Paragraph("2. Esquema Eléctrico (Conexiones y Control)",
                          styles['Heading3']))
    story.append(Spacer(1, 0.3 * cm))

    if os.path.exists(electrical_file):
        img = Image(electrical_file, width=40*cm, height=26*cm)
        story.append(img)
    else:
        story.append(Paragraph("[Imagen no disponible]", styles['Normal']))

    story.append(Spacer(1, 0.5 * cm))
    story.append(Paragraph("• Alimentación: RED 220V AC → Fuentes DC (5V + 12V)",
                          styles['Normal']))
    story.append(Paragraph("• Control: ESP32 via GPIO/PWM → SSR, Drivers, Coolers",
                          styles['Normal']))
    story.append(Paragraph("• Sensores: I2C/UART/1-Wire → SHT40, MH-Z19B, DS18B20",
                          styles['Normal']))
    story.append(Spacer(1, 1 * cm))

    # Lista de componentes
    story.append(Paragraph("3. Lista de Componentes (BOM)",
                          styles['Heading3']))
    story.append(Spacer(1, 0.3 * cm))

    bom_data = [
        ['Componente', 'Especificación', 'Cantidad'],
        ['ESP32', 'MCU WiFi + BLE', 1],
        ['SSR', 'Relé Estado Sólido (40A)', 1],
        ['Cable Calefactor', 'Losa radiante 700W', 1],
        ['Fuente DC', '5V/3A', 1],
        ['Fuente DC', '12V/2A', 1],
        ['Transductor Ultrasónico', '25mm, 1.66MHz', '2-3'],
        ['Calentador Pecera', '50W', 1],
        ['Cooler', '12V (inyección)', 1],
        ['Extractor', '12V (CO₂)', 1],
        ['SHT40', 'Sensor T/H I2C', 1],
        ['MH-Z19B', 'Sensor CO₂ UART', 1],
        ['DS18B20', 'Sensor temperatura agua (opcional)', 1],
        ['Tirantes Madera', '2×2\" curada', 'Según diseño'],
        ['Paneles Aislantes', 'Aluminio + Fibra Vidrio', 'Según diseño'],
        ['LED', 'Tira 6500K', 1],
    ]

    bom_table = Table(bom_data, colWidths=[6*cm, 8*cm, 2*cm])
    bom_table.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.grey),
        ('TEXTCOLOR', (0, 0), (-1, 0), colors.whitesmoke),
        ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
        ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
        ('FONTSIZE', (0, 0), (-1, 0), 10),
        ('BOTTOMPADDING', (0, 0), (-1, 0), 12),
        ('BACKGROUND', (0, 1), (-1, -1), colors.beige),
        ('GRID', (0, 0), (-1, -1), 1, colors.black),
        ('FONTSIZE', (0, 1), (-1, -1), 9),
    ]))
    story.append(bom_table)
    story.append(Spacer(1, 0.5 * cm))

    # Notas
    story.append(Paragraph("Notas Importantes:",
                          styles['Heading4']))
    story.append(Paragraph("• El ESP32 debe instalarse fuera de la nave (protección contra humedad)",
                          styles['Normal']))
    story.append(Paragraph("• El SSR requiere disipador térmico para 700W continuos",
                          styles['Normal']))
    story.append(Paragraph("• Los transductores requieren agua limpia (calentador a 28°C)",
                          styles['Normal']))
    story.append(Paragraph("• El extractor debe activarse cuando CO₂ > 800 PPM",
                          styles['Normal']))
    story.append(Spacer(1, 0.5 * cm))

    # Footer
    story.append(Paragraph(f"Generado: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M')}",
                          styles['Normal']))
    story.append(Paragraph("Proyecto Hoguera v1.0 - Cultivo de Precisión Fúngica",
                          styles['Normal']))

    # Generar PDF
    doc.build(story)
    return OUTPUT_PDF

if __name__ == "__main__":
    print("🍄 Generando planos técnicos de Hoguera v1.0 (v3 - Layout expandido)...")

    # Generar imágenes
    print("  • Plano de la nave...")
    nave_file = draw_nave_structure()

    print("  • Esquema eléctrico...")
    electrical_file = draw_electrical_schematic()

    # Generar PDF
    print("  • Compilando PDF...")
    pdf_file = generate_pdf(nave_file, electrical_file)

    print(f"\n✅ Planos generados exitosamente:")
    print(f"   📄 PDF: {pdf_file}")
    print(f"   🖼️  Nave: {nave_file}")
    print(f"   ⚡ Eléctrico: {electrical_file}")

    # Limpiar temp
    import shutil
    try:
        shutil.rmtree(TEMP_DIR)
        print(f"\n🧹 Archivos temporales limpiados.")
    except:
        pass
