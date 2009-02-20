#include <stdint.h>

#include "serial_handler.h"
#include "config.h"

void debug(uint8_t level, char * s)
{
    if(DEBUGLEVEL >= level){
        serial_putStart();
        serial_putcenc('D');
        serial_putcenc('0'+level);
        serial_putsenc(s);
        serial_putStop();
    }
}
