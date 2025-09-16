#include "uart.h"

void UART_init(unsigned int ubrr){
  //set baudrate
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  // turn on reciving and sending and interrupt |(1<<RXCIE0)
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  //set frame format: 8 data bits two stop bit
  UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
  //(1<<URSEL0)|
  //fdevopen(UART_putchar, UART_getchar);
  
}

void UART_transmit(unsigned char data){
  while(!(UCSR0A & (1 << UDRE0))){ //when its ready, its empty
    
  }
  UDR0 = data;
  
}

/*
uint8_t uart_TryTransmit(unsigned chat data){
  if((UCSRA & (1 << UDRE))){ //when its ready, its emty
    UDR = data;
    return 1;
  }
  return 0;
}
*/

unsigned char UART_receive(void){
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

int UART_putchar(char c, FILE *stream)
{
  if (c == '\n'){
    UART_transmit('\r');
  }
  UART_transmit((uint8_t)c);
  return 0;
}

int UART_getchar(void){
  return UART_receive();
}

void UART_test_receive_transmit(){
    while(1){
        unsigned char data = UART_receive();
        UART_transmit(data);
    }
}
