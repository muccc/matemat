#ifndef __LM75_H_
#define __LM75_H_
#include "config.h"
#include <stdint.h>

#define LM75_BASE    0x90
#define temp(x) (x * 2)

int16_t lm75_gettemp(uint8_t device);
void lm75_init(void);
void lm75_sprintf_temp(char * s, int16_t t, uint8_t pad);
#endif
