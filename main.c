#define F_CPU 4915200UL
#define BAUD_RATE 9600
#define MYUBRR (F_CPU/16/BAUD_RATE-1)

#include "uart.h"
#include "xmem.h"
#include "sram.h"
#include "address_decoder.h"
#include "adc.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include "spi.h"
#include "oled.h"

//picocom -b 9600 /dev/ttyS0


int main(void) {
    UART_init(MYUBRR);
    fdevopen(UART_transmit, UART_receive);
    //UART_transmit('s');
    //test_latch();
    SRAM_init();
    //while(1){printf("hei ");_delay_ms(1000);}

    SRAM_test();
    dec_test();
    
    adc_init();
    spi_init();
    _delay_ms(400);

    //printf("Starter SPI-test...\n");
    spi_selectSlave(0);
    spi_write(0x05);
    _delay_ms(40);
    spi_write(1);
    _delay_ms(40);
    spi_write(3);
    _delay_ms(40);

    spi_write(0x01);
    _delay_ms(40);
    spi_write(1);
    _delay_ms(40);
    spi_write(3);
    _delay_ms(40);

    spi_write(0x02);
    _delay_ms(40);
    spi_write(1);
    _delay_ms(40);
    spi_write(3);
    _delay_ms(40);

    //uint8_t answer = spi_read();
    //printf("Svar fra slave 1: 0x%02X\n", answer);

    //spi_selectSlave(1);
    //spi_write(0xAC);
    //answer = spi_read();
    //printf("Svar fra slave 2: 0x%02X\n", answer);

    OLED_init();
    OLED_home();
    OLED_clear_line(0);
    OLED_print("HELLO");

    while(1){};

}
//yellow = data
//purple = adress

/*    UART_init(MYUBRR);
    fdevopen(UART_putchar, UART_getchar);
    //UART_transmit('s');
    //test_latch();
    SRAM_init();
    SRAM_test();
    //dec_test();
    adc_init();
    adc_values_t data;
    //pos_calibrate();

   int16_t center_x=162;
    int16_t center_y=157;
    while(1){
        adc_values_t pos = position(center_x, center_y);
        printf("Joystick X: %d%%\tJoystick Y: %d%%\n", pos.joystick_x, pos.joystick_y);
        printf("Slider X:   %d\tSlider Y:   %d\n\n", pos.slider_x, pos.slider_y);    
        _delay_ms(500);
   }*/