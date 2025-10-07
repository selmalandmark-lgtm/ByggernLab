#include "../headers/oled.h"
#include "../headers/spi_driver.h"
#include <util/delay.h>
#include "../headers/fonts.h"

typedef struct {
  uint8_t row;
  uint8_t col;
} oled_position_t;

volatile oled_position_t position;

void OLED_init(void) {
    DDRB |= (1 << OLED_DC) | (1 << OLED_CS) | (1 << PB3);
    //PORTB &= ~(1 << OLED_CS);
    PORTB |= (1 << OLED_CS); //disattivo chip select oled dispaly lo attivo solo quando trasmetto
    PORTB |= (1<<PB3); //disattivo chip select avr board
    SPI_master_init();


    OLED_write_command(0xAE); // Display OFF
    OLED_write_command(0xA1);
    //OLED_write_command(0xDA); // Set Display Clock Divide Ratio
    //OLED_write_command(0x12); // Suggested ratio
    OLED_write_command(0xC8);
    
    OLED_write_command(0x20);  //addressing mode 
    OLED_write_command(0x02); 
    //OLED_write_command(0xA8); // 1/64 duty
    //OLED_write_command(0x3F); // Set Display Offset
    //OLED_write_command(0xD5); // No offset

    /*OLED_write_command(0x80);
    OLED_write_command(0x81);
    OLED_write_command(0x50); // 0x50
    OLED_write_command(0xD9);
    
    OLED_write_command(0x20);  //addressing mode 
    OLED_write_command(0x02);
    OLED_write_command(0xDB);

    OLED_write_command(0x30);
    OLED_write_command(0xAD);
    OLED_write_command(0x00);
    OLED_write_command(0xA4);
    OLED_write_command(0xA6);*/
    OLED_write_command(0xAF);
}

void OLED_write_command(uint8_t cmd) {
    OLED_DC_LOW(); // D = data, C = command
    OLED_CS_LOW(); 
    SPI_write(cmd);
    OLED_CS_HIGH();
}


void OLED_write_data(uint8_t data) {
    OLED_DC_HIGH();
    OLED_CS_LOW();
    SPI_write(data);
    OLED_CS_HIGH();
}

void OLED_set_brightness(uint8_t brightness) {
	OLED_write_command(0x81);
	if (brightness > 255)
		brightness = 255;
	OLED_write_command(brightness);
}

void OLED_print_arrow(uint8_t row, uint8_t col) {
    OLED_pos(row, col);
    OLED_write_data(0b00011000);
    OLED_write_data(0b00011000);
    OLED_write_data(0b01111110);
    OLED_write_data(0b00111100);
    OLED_write_data(0b00011000);
}


void OLED_clear_line(uint8_t line) {
    // Clear specified line 
    OLED_goto_line(line);

    OLED_CS_LOW();
    OLED_DC_HIGH(); //send data not command

    for (uint8_t col = 0; col < 128; col++) {
       SPI_write(0x00);
    }

    OLED_CS_HIGH();
    position.row = line;
    position.col = 0;
}

void OLED_clear_screen(){
    for(uint8_t i = 0; i < 8; i++){
        OLED_clear_line(i);
    }
}

void OLED_goto_line(uint8_t line) {
    if(line > 7 || line < 0) return;
    OLED_CS_LOW();
    OLED_DC_LOW(); //send a command
    SPI_write(0xB0 + line);  //set the page address
    SPI_write(0x00);  //set lower column address for column 0
    SPI_write(0x10);  //set higher column address for column 0

    OLED_CS_HIGH();
    position.row = line;
    //position.col = 0;
}
void OLED_goto_column(uint8_t column) {
    if(column> 127 || column < 0) return;
    OLED_CS_LOW();
    OLED_DC_LOW();
    uint8_t lower = column % 16;
    uint8_t higher = column / 16;
    SPI_write(0x00 + lower);
    SPI_write(0x10 + higher);
    OLED_CS_HIGH();
    position.col = column;
}

void OLED_pos(uint8_t row, uint8_t col) {
    // Set cursor to specified row and column 
	OLED_goto_line(row);
    OLED_goto_column(col);
}

/*void OLED_reset(void) {   
    for (int line = 0; line < 8; line++) {
      oled_clear_line(line);
    }
    oled_home();
}

void OLED_home() {
    // Set cursor to home position
    oled_pos(0,0);
}

void OLED_print(const char* str) {
    // Print a string to the OLED display (implementation depends on specific OLED model)
}
*/

/*void OLED_print(char c) {
    int index = c - 32;
    
    
    for (int i = 0; i < 8; i++) {
        uint8_t col = pgm_read_byte(&font8[index][i]);
        OLED_write_data(col);
    }
        
}

void OLED_string8(const char* data)  {
    int i = 0;
    while(data[i] != '\0')  {
        OLED_print(data[i]);
        ++i;
    }
}*/

void OLED_print(char* data, uint8_t font) {
    int i = 0;
    while (data[i] != '\0')  {
        
        for(uint8_t j = 0; j < font; j++) {
            switch(font)  {
                case LARGE_FONT:
                OLED_write_data(pgm_read_byte(&font8[data[i]-32][j]));
                break;

                case MEDIUM_FONT:
                OLED_write_data(pgm_read_byte(&font5[data[i]-32][j]));
                break;

                case SMALL_FONT:
                OLED_write_data(pgm_read_byte(&font4[data[i]-32][j]));
                break;
            } 
        } 
        ++i;
    } 
}    
