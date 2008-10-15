#ifndef __LM75_H_
#define __LM75_H_
#include "../config.h"
#include <stdint.h>

#define LM75_BASE    0x90

int16_t lm75_gettemp(uint8_t device);
void lm75_init(void);
#endif
