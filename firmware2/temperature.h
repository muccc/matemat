#ifndef __TEMPERATURE_H_
#define __TEMPERATURE_H_
#include <stdint.h>

#define MAX_TEMPS       4

#define TEMP_BOTTOM     0
#define TEMP_MIDDLE     1
#define TEMP_START        2
#define TEMP_STOP        3

struct temperaturedata{
    int16_t temp[MAX_TEMPS];
    uint8_t updated:1;
};

extern struct temperaturedata temperature;
void temperature_init(void);
void temperature_tick(void);
void temperature_dead_time(uint16_t deadtime);
void temperature_format(uint8_t index, char *buffer);

#endif
