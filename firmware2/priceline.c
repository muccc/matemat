#include <stdint.h>
#include "priceline.h"
#include "config.h"
#include "pinutils.h"
#include "debug.h"
#include "matemat.h"

volatile struct pricelinedata priceline;
volatile uint8_t plfromint = 0;

void priceline_init(void)
{
    priceline.pl = 0;
    DDR_CONFIG_IN(PRICELINE1);
    DDR_CONFIG_IN(PRICELINE3);

    DDR_CONFIG_OUT(SERVELINE1);
    DDR_CONFIG_OUT(SERVELINE3);

    PIN_CLEAR(SERVELINE1);
    PIN_CLEAR(SERVELINE3);

    priceline.updated = 1;
}

inline void priceline_interrupt(void)
{
    static uint8_t count1 = 0;
    static uint8_t count3 = 0;
    if(PIN_HIGH(PRICELINE1)){
        if(priceline.pl == 0 && count1++ == 20){
            priceline.pl = 1;
            plfromint = 1;
        }
    }else{
        count1=0;
    }

    if(PIN_HIGH(PRICELINE3)){
        if(priceline.pl == 0 && count3++==20){
            priceline.pl = 3;
            plfromint = 1;
        }
    }else{
        count3=0;
    }

    if(!priceline.grant && count3==0 && count1==0 && priceline.pl != 0){
        PIN_CLEAR(SERVELINE1);
        PIN_CLEAR(SERVELINE3);
        priceline.pl = 0;
        plfromint = 1;
    }

}

uint8_t priceline_get(void)
{
    return priceline.pl;
}

uint8_t priceline_serve(uint8_t pl)
{
    if(pl != priceline.pl)
        return 0;
    switch(pl){
        case 1:
            PIN_SET(SERVELINE1);
            priceline.grant = 500;
        break;
        case 3:
            PIN_SET(SERVELINE3);
            priceline.grant = 500;
        break;
        default:
            return 0;

    }
    return 1;
}

void priceline_tick(void)
{
    static uint16_t count = 0;
    priceline.updated = 0;
    if(count-- == 0){
        if(plfromint){
            priceline.updated = 1;
            plfromint = 0;
        }
        if(priceline.grant){
            priceline.grant-=50;
            if(priceline.grant == 0){
                matemat_served();
            }
        }
        count = 5;
        /*if(PIN_HIGH(PRICELINE1)){
            debug('I',"PL1 DOWN");
        }else{
            debug('I',"PL1 UP");
        }
        debug('D', "pl=%d", priceline.pl);*/
    }
}


