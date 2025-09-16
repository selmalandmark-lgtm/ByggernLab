
#define F_CPU 4915200
#define BAUD_RATE 9600
#define MYUBRR (F_CPU/16/BAUD_RATE-1)

#include "uart.h"
#include "xmem.h"
#include "sram.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>


int main(void) {
    UART_init(MYUBRR);
    fdevopen(UART_putchar, UART_getchar);
    //test_latch();
    SRAM_init();
    SRAM_test();

}
//yellow = data
//purple = adress