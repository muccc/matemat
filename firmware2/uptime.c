#include "uptime.h"
#include <stdint.h>

struct uptimedata uptime;

void uptime_init(void)
{
    uptime.uptime = 0;
    uptime.updated = 1;
}

void uptime_tick(void)
{
    static uint16_t count = 100;
    uptime.updated = 0;
    if(--count == 0){
        uptime.updated = 1;
        count = 100;
        uptime.uptime++;
    }
}


