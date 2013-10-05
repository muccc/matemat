#include <stdint.h>

#include "temperature.h"
//#include "lib/lm75.h"
#include "display.h"
#include "ds18x20/ds18x20.h"
#include "ds18x20/onewire.h"
#include "debug.h"
#include "config.h"
#include "serial_handler.h"

#include <string.h>
#include <stdio.h>

struct sensor {
    uint8_t id[OW_ROMCODE_SIZE];
    uint8_t index;
};

struct sensor sensors[] = { {.id = {0x28, 0x50, 0xee, 0xa6, 0x04, 0x00, 0x00, 0x76}, TEMP_BOTTOM},
                            {.id = {0x28, 0xfa, 0x2e, 0xd8, 0x04, 0x00, 0x00, 0x7a}, TEMP_MIDDLE}};

struct temperaturedata temperature;
static uint16_t count;
void temperature_init(void)
{
    temperature.temp[TEMP_START] = TEMP_START_VAL;
    temperature.temp[TEMP_STOP] = TEMP_STOP_VAL;
    count = 0;
	ow_reset();
    uint8_t i;
    
    uint8_t diff = OW_SEARCH_FIRST;
    uint8_t id[OW_ROMCODE_SIZE];
    
    while(diff != OW_LAST_DEVICE) {
        DS18X20_find_sensor(&diff, id);
        
        if( diff == OW_PRESENCE_ERR ) {
            serial_putStart();
            serial_putsenc("D1nosensor");
            serial_putStop();
            break;
        }

        if( diff == OW_DATA_ERR ) {
            serial_putStart();
            serial_putsenc("D1buserror");
            serial_putStop();
            break;
        }

        serial_putStart();
        serial_putcenc('S');
        for(i = 0; i < 8; i++) {
            serial_putcenc(id[i]);
        }
        serial_putStop();
    }

    count = 200;
    DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
}

void temperature_tick(void)     //alle 10ms
{
    temperature.updated = 0;
    if(count-- == 0){
        count = 200;
        //while( DS18X20_conversion_in_progress() );
        int16_t temp;
        uint8_t i;
        for(i = 0; i < sizeof(sensors) / sizeof(sensors[0]); i++) {
            if(DS18X20_read_decicelsius(sensors[i].id, &temp) == DS18X20_OK) {
                temperature.temp[sensors[i].index] = temp;
                temperature.updated = 1;
            }
        }
        DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
    }
}

void temperature_dead_time(uint16_t deadtime)
{
    if(count < deadtime){
        count = deadtime;
    }
}

void temperature_format(uint8_t index, char *buffer)
{
    if(DS18X20_format_from_decicelsius(temperature.temp[index], buffer, 16) != DS18X20_OK) {
        strcpy(buffer, "####.#");
    }
}

