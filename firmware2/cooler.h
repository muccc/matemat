#ifndef __COOLER_H_
#define __COOLER_H_
#include "config.h"
#include "temperature.h"

#define COOLER_COOLING    0
#define COOLER_IDLE       1

struct coolerdata{
    uint8_t state;
    uint8_t updated:1;
};

extern struct coolerdata cooler;

void cooler_init(void);
void cooler_tick(void);

#endif
