/* 
 * File:   protolink_oled_example.c
 * Author: artga
 *
 * Created on 21 de marzo de 2026, 12:07 PM
 */



#define PROTOLINK_DEFAULT     true
#include <protolink/v2.h> //<-- https://github.com/galio-electronics/protolink
#include <bootloader.h>
#include <stdio.h>
#include <stdlib.h>
#define FW_VERSION "1.0.3"

// Configuración I2C del micro
#use i2c(Master,I2C1,FAST=400000,STREAM=OLED_I2C)
#define OLED_I2C_STREAM OLED_I2C
#include <oled_i2c/oled_ssd1306.h>
#include <oled_i2c/images/galio_logo_128X32.h>

void oled_show_boot_screen(void)
{
   oled_clear();
   oled_draw_bitmap_const(0, 0, 128, 4, galio_logo_128x32);
   oled_print_xy_const(18, 5, "PROTOLINK");
   oled_print_xy_const(28, 6, "FW 1.0.3");
   oled_print_xy_const(18, 7, "BOOTING...");
}
void main(void) {
    
    delay_ms(100);
    protolink_io_init();
    protolink_timer0_init();
    //protolink_interrupts_init();
    delay_ms(500);
    int16 contador = 0;
    char datos[21];

    oled_init();
    oled_show_boot_screen();
//    oled_print_xy_const(0, 0, "PROTOLINK");
//    oled_print_xy_const(0, 1, "OLED SSD1306 OK");
//    oled_print_xy_const(0, 2, "RS485: INIT");
//    oled_print_xy_const(0, 3, "ID: 03");
//    oled_print_xy_const(0, 4, "FW: 1.0.0");
    output_low(LED1);
    output_low(LED2);
    while(TRUE)
    {
        contador++;
        sprintf(datos,"Contador: %ld",contador);
        //oled_print_xy(0,7,datos);      
        protolink_debug_msg("hello world\r\n");
        delay_ms(1000);
    }

    
}