#ifndef _PRICELINE_H_
#define _PRICELINE_H_
#include "stdint.h"


struct pricelinedata{
    uint8_t     pl:3;
    uint8_t     updated:1;
    uint16_t    grant;
};
extern volatile struct pricelinedata priceline;

void priceline_interrupt(void);
uint8_t priceline_get(void);
uint8_t priceline_serve(uint8_t pl);
void priceline_init(void);
void priceline_tick(void);

#endif
