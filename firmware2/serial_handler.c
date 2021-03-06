#include <stdint.h>
#include <string.h>
#include "lib/uart.h"
#include "serial_handler.h"

char buffer[SERIAL_BUFFERLEN];
uint8_t serial_buffer[SERIAL_BUFFERLEN];

void serial_putcenc(uint8_t c)
{
    if(c == SERIAL_ESCAPE)
        uart_putc(SERIAL_ESCAPE);
    uart_putc(c);
}

void serial_putsenc(char * s)
{
    while(*s){
        if(*s == SERIAL_ESCAPE)
            uart_putc(SERIAL_ESCAPE);
        uart_putc(*s++);
    }
}

void serial_putenc(uint8_t * d, uint16_t n)
{
    uint16_t i;
    for(i=0;i<n;i++){
        if(*d == SERIAL_ESCAPE)
            uart_putc(SERIAL_ESCAPE);
        uart_putc(*d++);
    }
}

inline void serial_putStart(uint8_t command)
{
    uart_putc(SERIAL_ESCAPE);
    uart_putc(command);
}

inline void serial_putStartDebug(void)
{
    uart_putc(SERIAL_ESCAPE);
    uart_putc(SERIAL_START_DEBUG);
}

inline void serial_putStop(void)
{
    uart_putc(SERIAL_ESCAPE);
    uart_putc(SERIAL_END);
}

void serial_sendFrames(uint8_t command, char * s)
{
    serial_putStart(command);
    serial_putsenc(s);
    serial_putStop();
}

void serial_sendFramec(uint8_t command, uint8_t s)
{
    serial_putStart(command);
    serial_putcenc(s);
    serial_putStop();
}

uint16_t serial_readline(void)
{
    uint16_t l;
    l = readline();
    if(l){
        memcpy(serial_buffer,buffer,l);
    }
    return l;
}

unsigned int readline( void )
{
    static int fill = 0;
    static uint8_t escaped = 0;
    int  i = uart_getc();
    uint8_t data;

    if ( i & UART_NO_DATA ){
        return 0;
    }
    data = i&0xFF;
    if(data == SERIAL_ESCAPE){
        if(!escaped){
            escaped = 1;
            return 0;
        }
        escaped = 0;
    }else if(escaped){
        escaped = 0;
        if(data == SERIAL_START_CONTROL){
            fill = 0;
            return 0;
        }else if( data == SERIAL_END){
            return fill;
        }
    }
    //if(fill != -1){
        buffer[fill++] = data;
        if(fill >= SERIAL_BUFFERLEN)
            fill = SERIAL_BUFFERLEN -1;
    //}
    return 0;
}

