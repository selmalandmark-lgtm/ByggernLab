

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>


void UART_init(unsigned int ubrr);
void UART_transmit(unsigned char data);
unsigned char UART_receive(void);
int UART_putchar(char c, FILE* stream);
int UART_getchar(void);
void UART_test_receive_transmit();
