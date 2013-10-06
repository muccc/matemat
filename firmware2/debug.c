#include "serial_handler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char buffer[128];

void debug(uint8_t level, char *fmt, ...)
{
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(buffer, fmt, myargs);
    serial_putStart(SERIAL_START_DEBUG);
    serial_putcenc(level);
    serial_putsenc(buffer);
    serial_putStop();
    va_end(myargs);
}
