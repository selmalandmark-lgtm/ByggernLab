#define F_CPU 4915200UL
#define BAUD_RATE 9600
#define MYUBRR (F_CPU/16/BAUD_RATE-1)


#include "uart.h"
#include "xmem.h"
#include "sram.h"
#include "address_decoder.h"
#include "adc.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include "spi.h"
#include "oled.h"
#include "menu.h"
#include "user_io_board.h"

//picocom -b 9600 /dev/ttyS0

int main(void) {
    UART_init(MYUBRR);
    fdevopen(UART_transmit, UART_receive);
    SRAM_init();
    adc_init();
    user_io_init();
    spi_init();
    _delay_ms(400);

    spi_selectSlave(DISP_SS2);
    OLED_init();
    //menu();

    adc_values_t cal_data = pos_calibrate();
    adc_values_t pos;
    uint8_t x_center = cal_data.joystick_x;
    uint8_t y_center = cal_data.joystick_y;
    
    menu_init();
    uint8_t menu_pos=navigation(x_center, y_center);
    printf("menu_pos: %d ", menu_pos);
    printf("ferdig");
    _delay_ms(100);

    Buttons btn;

    while(1){
        if(user_io_read_buttons(&btn) == 0){
            //printf("%d\r\n", btn.L7);
            printf("L:%d%d%d%d%d%d%d  R:%d%d%d%d%d%d  Nav U:%d D:%d L:%d R:%d B:%d\r\n",
                btn.L7,btn.L6,btn.L5,btn.L4,btn.L3,btn.L2,btn.L1,
                btn.R6,btn.R5,btn.R4,btn.R3,btn.R2,btn.R1,
                btn.NU,btn.ND,btn.NL,btn.NR,btn.NB);
        }
        _delay_ms(1000);
    }
}

    


//yellow = data
//purple = adress


/* joystick-test 
  adc_values_t cal_data = pos_calibrate();
    adc_values_t pos;

    while (1){
        pos=position(165 , 160);
        printf("x: %d\n",pos.joystick_x);
        _delay_ms(500);
    }
*/