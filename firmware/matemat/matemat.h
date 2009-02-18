#ifndef __MATEMAT_H_
#define __MATEMAT_H_

#include "../config.h"
#include <stdint.h>

#ifdef MATEMAT_SUPPORT

#define MAX_TEMPS       4

#define TEMP_BOTTOM     0
#define TEMP_MIDDLE     1
#define TEMP_START        2
#define TEMP_STOP        3

#define BOTTOM_ADR      3
#define MIDDLE_ADR      7

#define GET_BOTTOM      (lm75_gettemp(BOTTOM_ADR))
#define GET_MIDDLE      (lm75_gettemp(MIDDLE_ADR))

#define TEMP_START_VAL    15
#define TEMP_STOP_VAL    10

#define temp(x) (x * 2)

#define MODE_COOLING    0
#define MODE_IDLE       1

extern struct matemat_global matemat_global;

struct matemat_global {
    uint16_t matemat_packetcount;
//    int16_t temp1;
//    int16_t temp2;
    uint8_t push;
    int16_t temps[MAX_TEMPS];
    uint8_t mode;
};

void sprintf_temp(char * s, int16_t t, uint8_t pad);
void matemat_periodic(void);
void matemat_init(void);
void matemat_process(void);
#endif
#endif
