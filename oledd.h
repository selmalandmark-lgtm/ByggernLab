#ifndef OLED_H
#define OLED_H

#define LARGE_FONT 8
#define MEDIUM_FONT 5
#define SMALL_FONT 4

#include <avr/io.h>

// OLED pins
#define OLED_DC PB1     // Data/Command
#define OLED_CS PB2     // Chip Select display

// Control macros

// DC LOW = Command, DC HIGH = Da
#define OLED_DC_HIGH()    (PORTB |= (1 << OLED_DC))
#define OLED_DC_LOW()     (PORTB &= ~(1 << OLED_DC))

#define OLED_CS_HIGH()    (PORTB |= (1 << OLED_CS))
#define OLED_CS_LOW()     (PORTB &= ~(1 << OLED_CS))

void OLED_init(void);
void OLED_write_command(uint8_t cmd);
void OLED_write_data(uint8_t dat#ifndef OLED_H
#define OLED_H

#define LARGE_FONT 8
#define MEDIUM_FONT 5
#define SMALL_FONT 4

#include <avr/io.h>

// OLED pins
#define OLED_DC PB1     // Data/Command
#define OLED_CS PB2     // Chip Select display

// Control macros

// DC LOW = Command, DC HIGH = Da
#define OLED_DC_HIGH()    (PORTB |= (1 << OLED_DC))
#define OLED_DC_LOW()     (PORTB &= ~(1 << OLED_DC))

#define OLED_CS_HIGH()    (PORTB |= (1 << OLED_CS))
#define OLED_CS_LOW()     (PORTB &= ~(1 << OLED_CS))

void OLED_init(void);
void OLED_write_command(uint8_t cmd);
void OLED_write_data(uint8_t data); // char data
void OLED_set_brightness(uint8_t brightness);
void OLED_print_arrow(uint8_t row, uint8_t col);
void OLED_pos(uint8_t row, uint8_t col);
void OLED_clear_line(uint8_t line);
void OLED_clear_screen();
void OLED_goto_line(uint8_t line);
void OLED_print(char* data, uint8_t font);
//void OLED_print(char c);
//void OLED_string8(const char* data);



#endifa); // char data
void OLED_set_brightness(uint8_t brightness);
void OLED_print_arrow(uint8_t row, uint8_t col);
void OLED_pos(uint8_t row, uint8_t col);
void OLED_clear_line(uint8_t line);
void OLED_clear_screen();
void OLED_goto_line(uint8_t line);
void OLED_print(char* data, uint8_t font);
//void OLED_print(char c);
//void OLED_string8(const char* data);



#endif