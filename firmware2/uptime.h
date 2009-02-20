#ifndef _UPTIME_H_
#define _UPTIME_H_
#include "stdint.h"

void uptime_init(void);
void uptime_tick(void);

struct uptimedata{
    uint32_t    uptime;
    uint8_t     updated:1;
};
extern struct uptimedata uptime;
#endif
