
# oled_ssd1306 — Driver para pantalla OLED SSD1306 en CCS C (PIC)

Driver liviano para pantallas OLED **128×64** con controlador **SSD1306** vía I²C, escrito en **CCS C** para microcontroladores PIC. Incluye fuente 5×7, soporte de bitmaps en memoria de programa (`rom`) y un ejemplo de integración con la plataforma **ProtoLink v2** de Galio Electronics.

---

## Características

- Comunicación I²C usando el sistema de STREAM de CCS (`#use i2c`)
- Modo de direccionamiento por páginas (Page Addressing Mode)
- Inicialización completa del SSD1306 (charge pump, contraste, remapeo de segmentos, etc.)
- Fuente de texto 5×7 píxeles, ASCII 0x20–0x7E
- Funciones de impresión en RAM y en memoria de programa (`rom`)
- Dibujo de bitmaps monocromáticos almacenados en `rom`
- Compatible con displays 128×64 y adaptable a 128×32 (ajustando `OLED_PAGES`)

---

## Archivos

| Archivo | Descripción |
|---|---|
| `oled_ssd1306.h` | Header principal — macros de configuración y prototipos |
| `oled_ssd1306.c` | Implementación del driver |
| `font5x7.h` | Tabla de fuente 5×7 en `const` (flash) |
| `galio_logo_128x32.h` | Bitmap de ejemplo: logo Galio 128×32 px |
| `protolink_oled_example.c` | Proyecto de ejemplo completo para ProtoLink v2 |

---

## Requisitos

- Compilador **CCS C** (PCWHD / PCM / PCH)
- Microcontrolador PIC con periférico I²C (probado con I2C1 a 400 kHz)
- Pantalla OLED SSD1306 128×64 px con dirección I²C `0x3C` (write byte `0x78`)
- Plataforma **ProtoLink v2** de Galio Electronics (solo para el ejemplo — no requerida por el driver)

---

## Uso rápido

### 1. Declarar el stream I²C antes de incluir el driver

```c
#use i2c(Master, I2C1, FAST=400000, STREAM=OLED_I2C)
#define OLED_I2C_STREAM OLED_I2C
#include "oled_ssd1306.h"
```

> **Importante:** `OLED_I2C_STREAM` debe estar definido **antes** de incluir `oled_ssd1306.h`.  
> Si se omite, el driver compila sin STREAM (modo default de CCS).

### 2. Inicializar y usar

```c
void main(void)
{
    oled_init();                              // Configura y enciende la pantalla

    oled_print_xy_const(0, 0, "HOLA MUNDO"); // Texto en col=0, página=0
    oled_print_xy_const(0, 1, "SSD1306 OK");

    // Mostrar un bitmap almacenado en flash
    oled_draw_bitmap_const(0, 4, 128, 4, galio_logo_128x32);

    while(TRUE);
}
```

---

## API de referencia

### Inicialización y borrado

| Función | Descripción |
|---|---|
| `oled_init()` | Secuencia de inicialización completa + limpia pantalla |
| `oled_clear()` | Limpia toda la pantalla (escribe 0x00) |
| `oled_fill(pattern)` | Rellena toda la pantalla con el byte `pattern` |
| `oled_clear_line(page)` | Borra una página (fila de 8 px) específica |

### Posicionamiento

| Función | Descripción |
|---|---|
| `oled_set_pos(col, page)` | Mueve el cursor a columna `col` (0–127), página `page` (0–7) |

### Texto en RAM

| Función | Descripción |
|---|---|
| `oled_putc(c)` | Imprime un carácter en la posición actual |
| `oled_print(text)` | Imprime un string desde RAM |
| `oled_print_xy(col, page, text)` | Posiciona e imprime string desde RAM |

### Texto en flash (`rom`)

| Función | Descripción |
|---|---|
| `oled_print_const(text)` | Imprime un string desde memoria de programa |
| `oled_print_xy_const(col, page, text)` | Posiciona e imprime string desde `rom` |

### Gráficos

| Función | Descripción |
|---|---|
| `oled_draw_bitmap_const(x, page, width, pages, bitmap)` | Dibuja bitmap `rom` de `width` columnas × `pages` páginas en posición (`x`, `page`) |

---

## Coordenadas y páginas

La pantalla se organiza en **8 páginas** de 8 píxeles de alto cada una:

```
Página 0  → filas  0–7
Página 1  → filas  8–15
  ...
Página 7  → filas 56–63
```

Un carácter 5×7 con espacio ocupa **6 columnas** y **1 página**.  
En 128 columnas caben hasta **21 caracteres** por línea (21 × 6 = 126 px).

---

## Generación de bitmaps

Para crear tus propios bitmaps compatibles:

1. Crea una imagen de 128×32 (o 128×64) en blanco y negro.
2. Conviértela al formato page-oriented SSD1306 (cada byte = 8 píxeles verticales, LSB arriba).
3. Declara el array resultante como `rom int8 nombre[N]` en un `.h`.
4. Llama a `oled_draw_bitmap_const(x, page, width, pages, nombre)`.

---

## Configuración para display 128×32

En `oled_ssd1306.h`, cambia:

```c
#define OLED_HEIGHT   32
#define OLED_PAGES    4
```

Y en `oled_init()` ajusta el comando de multiplex ratio:

```c
oled_cmd(0xA8);
oled_cmd(0x1F);  // 1/32 duty en lugar de 1/64
```

---

## Ejemplo incluido (`protolink_oled_example.c`)

Muestra una pantalla de arranque con el logo de Galio Electronics y el mensaje de versión de firmware, luego entra en un loop enviando un contador por el puerto debug de ProtoLink:

```
┌─────────────────────────────┐
│   [Logo Galio 128×32]       │
│                             │
│        PROTOLINK            │
│         FW 1.0.3            │
│        BOOTING...           │
└─────────────────────────────┘
```

---

## Notas

- El archivo `oled_ssd1306.h` incluye `oled_ssd1306.c` directamente al final (`#include "oled_ssd1306.c"`). Esto es intencional para el flujo de CCS C — **no agregar `oled_ssd1306.c` al proyecto como archivo fuente independiente** para evitar definiciones duplicadas.
- La fuente `font5x7` se declara como `const` (flash). En PICs con banco de programa separado, CCS C gestiona el acceso automáticamente.
- Caracteres fuera del rango ASCII imprimible (0x20–0x7E) se sustituyen por `?`.

---

## Licencia

MIT — libre para uso personal y comercial con atribución.

---

*Desarrollado por Galio Electronics · [galio-electronics.com](https://galio-electronics.com)*
