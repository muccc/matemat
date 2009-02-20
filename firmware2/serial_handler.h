#ifndef __SERIAL_HANDLER_
#define __SERIAL_HANDLER_

#include <stdint.h>
#include "config.h"

#define SERIAL_BUFFERLEN        50
extern uint8_t serial_buffer[SERIAL_BUFFERLEN];

#define SERIAL_ESCAPE   '\\'
#define SERIAL_START    '0'
#define SERIAL_END    '1'

void serial_putcenc(uint8_t c);
void serial_putsenc(char * s);
inline void serial_putStart(void);
inline void serial_putStop(void);
void serial_sendFrames(char * s);
void serial_sendFramec(uint8_t s);
void serial_putenc(uint8_t * d, uint16_t n);
unsigned int readline( void );
uint16_t serial_readline(void);

#endif
