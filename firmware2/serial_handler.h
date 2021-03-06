#ifndef __SERIAL_HANDLER_
#define __SERIAL_HANDLER_

#include <stdint.h>
#include "config.h"

#define SERIAL_BUFFERLEN        50
extern uint8_t serial_buffer[SERIAL_BUFFERLEN];

#define SERIAL_ESCAPE           '\\'
#define SERIAL_START_CONTROL    'C'
#define SERIAL_START_DEBUG      'D'
#define SERIAL_START_TEMP       'T'
#define SERIAL_END              '9'

void serial_putcenc(uint8_t c);
void serial_putsenc(char * s);
void serial_putStart(uint8_t command);
void serial_putStartDebug(void);
void serial_putStop(void);
void serial_sendFrames(uint8_t command, char * s);
void serial_sendFramec(uint8_t command, uint8_t s);
void serial_putenc(uint8_t * d, uint16_t n);
unsigned int readline( void );
uint16_t serial_readline(void);

#endif
