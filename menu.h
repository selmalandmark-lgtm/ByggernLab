#include "oled.h"
#include <util/delay.h>
#include <avr/io.h>



void menu_init();

uint8_t navigation(uint8_t x, uint8_t y);
static void menu_draw_cursor(uint8_t active_line);