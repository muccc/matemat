#ifndef __TEMPERATURE_H_
#define __TEMPERATURE_H_
#include "lib/lm75.h"

#define MAX_TEMPS       4

#define TEMP_BOTTOM     0
#define TEMP_MIDDLE     1
#define TEMP_START        2
#define TEMP_STOP        3

#define GET_BOTTOM      (lm75_gettemp(BOTTOM_ADR))
#define GET_MIDDLE      (lm75_gettemp(MIDDLE_ADR))

struct temperaturedata{
    int16_t temp[MAX_TEMPS];
    uint8_t updated:1;
};

extern struct temperaturedata temperature;
void temperature_init(void);
void temperature_tick(void);
void temperature_dead_time(uint16_t deadtime);

#endif
