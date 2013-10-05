#include "cooler.h"
#include "temperature.h"
#include "pinutils.h"
#include "config.h"

struct coolerdata cooler;

void cooler_init(void)
{
    cooler.state = COOLER_IDLE;
    cooler.updated = 1;

    DDR_CONFIG_OUT(MATEMAT_COOLER);
    PIN_CLEAR(MATEMAT_COOLER);
}

void cooler_tick(void)     //alle 10ms
{
    cooler.updated = 0;
    if(temperature.updated == 1 && temperature.temp[TEMP_BOTTOM] < 1000){
        cooler.updated = 1;
        if(cooler.state == COOLER_IDLE){
            if(temperature.temp[TEMP_BOTTOM] > temperature.temp[TEMP_START]){
                cooler.state = COOLER_COOLING;
                PIN_SET(MATEMAT_COOLER);
                temperature_dead_time(300);
            }
        }else if(cooler.state == COOLER_COOLING){
            if(temperature.temp[TEMP_BOTTOM] < temperature.temp[TEMP_STOP]){
                cooler.state = COOLER_IDLE;
                PIN_CLEAR(MATEMAT_COOLER);
                temperature_dead_time(300);
            }
        }

    }
}
