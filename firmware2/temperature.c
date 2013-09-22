#include <stdint.h>

#include "temperature.h"
#include "lib/lm75.h"
#include "display.h"

struct temperaturedata temperature;
static uint16_t count;

void temperature_init(void)
{
    temperature.temp[TEMP_START] = temp(TEMP_START_VAL);
    temperature.temp[TEMP_STOP] = temp(TEMP_STOP_VAL);
    count = 0;
}

void temperature_tick(void)     //alle 10ms
{
    temperature.updated = 0;
    if(count-- == 0){
        temperature.updated = 1;
        count = 50;
        temperature.temp[TEMP_BOTTOM] = GET_BOTTOM;
        temperature.temp[TEMP_MIDDLE] = GET_MIDDLE;
    }
}

void temperature_dead_time(uint16_t deadtime)
{
    if(count < deadtime){
        count = deadtime;
    }
}
