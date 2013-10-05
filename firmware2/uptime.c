#include "uptime.h"
#include "debug.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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
        char buffer[64];
        sprintf(buffer, "uptime=%lu", uptime.uptime);
        debug(2,buffer);
    }
}


