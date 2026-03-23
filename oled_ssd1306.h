/* 
 * File:   oled_ssd1306.h
 * Author: artga
 *
 * Created on 21 de marzo de 2026, 12:08 PM
 */

#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include <stdint.h>

// Dirección 7-bit típica del SSD1306: 0x3C
// En CCS normalmente write = 0x78
#define OLED_I2C_ADDR_WRITE   0x78

#define OLED_WIDTH            128
#define OLED_HEIGHT           64
#define OLED_PAGES            8

void oled_init(void);
void oled_clear(void);
void oled_set_pos(uint8_t col, uint8_t page);
void oled_putc(char c);
void oled_print(char *text);
void oled_print_xy(uint8_t col, uint8_t page, char *text);
void oled_print_const(rom char *text);
void oled_print_xy_const(uint8_t col, uint8_t page, rom char *text);
void oled_fill(uint8_t pattern);
void oled_clear_line(int8 page);
void oled_draw_bitmap_const(int8 x, int8 page, int8 width, int8 pages, rom int8 *bitmap);
#include "oled_ssd1306.c"
#endif
