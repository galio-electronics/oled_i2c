#include "fonts/font5x7.h"

// Ajusta esto si quieres usar otro stream I2C de CCS
// Ejemplo si declaraste:
// #use i2c(MASTER, FAST=400000, SDA=PIN_C4, SCL=PIN_C3, STREAM=OLED_I2C)
#ifndef OLED_I2C_STREAM
   #define OLED_I2C_STREAM
#endif

static void oled_cmd(uint8_t cmd);
static void oled_data(uint8_t data);

static void oled_cmd(uint8_t cmd)
{
   i2c_start(OLED_I2C_STREAM);
   i2c_write(OLED_I2C_STREAM, OLED_I2C_ADDR_WRITE);
   i2c_write(OLED_I2C_STREAM, 0x00);   // control byte = command
   i2c_write(OLED_I2C_STREAM, cmd);
   i2c_stop(OLED_I2C_STREAM);
}

static void oled_data(uint8_t data)
{
   i2c_start(OLED_I2C_STREAM);
   i2c_write(OLED_I2C_STREAM, OLED_I2C_ADDR_WRITE);
   i2c_write(OLED_I2C_STREAM, 0x40);   // control byte = data
   i2c_write(OLED_I2C_STREAM, data);
   i2c_stop(OLED_I2C_STREAM);
}

void oled_set_pos(uint8_t col, uint8_t page)
{
   oled_cmd(0xB0 | (page & 0x07));                 // Page start address
   oled_cmd(0x00 | (col & 0x0F));                  // Lower nibble
   oled_cmd(0x10 | ((col >> 4) & 0x0F));           // Upper nibble
}

void oled_fill(uint8_t pattern)
{
   uint8_t page;
   uint8_t col;

   for(page = 0; page < OLED_PAGES; page++)
   {
      oled_set_pos(0, page);
      for(col = 0; col < OLED_WIDTH; col++)
      {
         oled_data(pattern);
      }
   }
}

void oled_clear(void)
{
   oled_fill(0x00);
   oled_set_pos(0, 0);
}

void oled_putc(char c)
{
   uint8_t i;
   uint8_t index;

   if(c < 0x20 || c > 0x7E)
      c = '?';

   index = c - 0x20;

   for(i = 0; i < 5; i++)
   {
      oled_data(font5x7[index][i]);
   }

   oled_data(0x00);   // espacio entre caracteres
}

void oled_print( char *text)
{
   while(*text != '\0')
   {
      oled_putc(*text);
      text++;
   }
}

void oled_print_xy(uint8_t col, uint8_t page,char *text)
{
   oled_set_pos(col, page);
   oled_print(text);
}

void oled_print_const( rom char *text)
{
   while(*text != '\0')
   {
      oled_putc(*text);
      text++;
   }
}

void oled_print_xy_const(uint8_t col, uint8_t page,rom char *text)
{
   oled_set_pos(col, page);
   oled_print_const(text);
}

void oled_init(void)
{
   delay_ms(100);

   oled_cmd(0xAE); // Display OFF

   oled_cmd(0xD5); // Set display clock divide ratio/oscillator frequency
   oled_cmd(0x80);

   oled_cmd(0xA8); // Set multiplex ratio
   oled_cmd(0x3F); // 1/64 duty

   oled_cmd(0xD3); // Set display offset
   oled_cmd(0x00);

   oled_cmd(0x40); // Set start line = 0

   oled_cmd(0x8D); // Charge pump
   oled_cmd(0x14); // Enable

   oled_cmd(0x20); // Memory addressing mode
   oled_cmd(0x02); // Page addressing mode

   oled_cmd(0xA1); // Segment remap
   oled_cmd(0xC8); // COM output scan direction remapped

   oled_cmd(0xDA); // COM pins hardware config
   oled_cmd(0x12);

   oled_cmd(0x81); // Contrast
   oled_cmd(0xCF);

   oled_cmd(0xD9); // Pre-charge period
   oled_cmd(0xF1);

   oled_cmd(0xDB); // VCOMH deselect level
   oled_cmd(0x40);

   oled_cmd(0xA4); // Entire display ON from RAM
   oled_cmd(0xA6); // Normal display

   oled_cmd(0x2E); // Deactivate scroll

   oled_cmd(0xAF); // Display ON

   oled_clear();
}
void oled_clear_line(int8 page)
{
   int8 i;

   oled_set_pos(0, page);

   for(i = 0; i < 21; i++)
   {
      oled_putc(' ');
   }

   oled_set_pos(0, page);
}
void oled_draw_bitmap_const(int8 x, int8 page, int8 width, int8 pages, rom int8 *bitmap)
{
   int8 p;
   int8 col;
   int16 index = 0;

   for(p = 0; p < pages; p++)
   {
      oled_set_pos(x, page + p);

      for(col = 0; col < width; col++)
      {
         oled_data(bitmap[index]);
         index++;
      }
   }
}