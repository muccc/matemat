#include <stdint.h>

#include "temperature.h"
#include "lib/lm75.h"
#include "display.h"

struct temperaturedata temperature;

void temperature_init(void)
{
    temperature.temp[TEMP_START] = temp(TEMP_START_VAL);
    temperature.temp[TEMP_STOP] = temp(TEMP_STOP_VAL);
}

void temperature_tick(void)     //alle 10ms
{
    static uint16_t count = 0;
    temperature.updated = 0;
    if(count-- == 0){
        temperature.updated = 1;
        count = 50;
        temperature.temp[TEMP_BOTTOM] = GET_BOTTOM;
        temperature.temp[TEMP_MIDDLE] = GET_MIDDLE;
    }
}
