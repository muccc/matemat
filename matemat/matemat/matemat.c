#include "matemat.h"
#include "i2cmaster.h"
#include "lm75.h"
#include "../lcd/hd44780.h"
#include <avr/pgmspace.h>
#include <string.h>

#ifdef MATEMAT_SUPPORT

volatile uint8_t kick = 0;
struct matemat_global matemat_global;

void sprintf_temp(char * s, int16_t t, uint8_t pad)
{
    uint8_t b;
    if(t == 100){
        sprintf(s,"# C");
    }else{
        if(t==-1){
            if(pad)
                sprintf(s," -0");
            else
                sprintf(s,"-0");
        }else{
            if(pad)
                sprintf(s,"%2d",t/2);
            else
                sprintf(s,"%d",t/2);
        }
        if(t & 1)
            b = 5;
        else
            b = 0;
        sprintf(s+strlen(s),".%d C",b);
    }
}

void printtemp(uint16_t t, uint8_t x, uint8_t y){
    char buf[20];
    hd44780_goto(x,y);
    sprintf_temp(buf, t, 1);
    fprintf(lcd,buf);
}

void matemat_periodic(void)
{
    static uint8_t t = 0;
    if(++t == 50){
        kick = 1;
        t = 0;
    }
}

void matemat_setupdisp(void)
{
    hd44780_clear();
    hd44780_goto(0,0);
    fprintf_P(lcd, PSTR("   Matemat ready"));
    
    hd44780_goto(2,0);
    fprintf_P(lcd, PSTR("Middle: \n"));

    hd44780_goto(3,0);
    fprintf_P(lcd, PSTR("Bottom: \n"));
}

void matemat_putmode(void)
{
    hd44780_goto(1,10);
    switch(matemat_global.mode){
        case MODE_IDLE:
            fprintf_P(lcd, PSTR("idle   "));
        break;
        case MODE_COOLING:
            fprintf_P(lcd, PSTR("cooling"));
        break;
        default:
            fprintf_P(lcd, PSTR("?mode? "));
        break;
    }
}

void matemat_process(void)
{
    static uint8_t state = 0;
    static uint8_t timer = 0;
    if(kick){
        matemat_global.temps[TEMP_BOTTOM] = GET_BOTTOM;
        printtemp(matemat_global.temps[TEMP_BOTTOM],3,8);
        matemat_global.temps[TEMP_MIDDLE] = GET_MIDDLE;
        printtemp(matemat_global.temps[TEMP_MIDDLE],2,8);
        hd44780_goto(1,0);
        fprintf(lcd,"%d",matemat_global.matemat_packetcount);
        hd44780_goto(3,19);
        if(state)
            hd44780_put('.',NULL);
        else
            hd44780_put(' ',NULL);
        state = !state;

        if(matemat_global.mode == MODE_IDLE){
            if(matemat_global.temps[TEMP_BOTTOM] > matemat_global.temps[TEMP_START]){
                matemat_global.mode = MODE_COOLING;
                matemat_putmode();
                PIN_SET(MATEMAT_COOLER);
            }
        }else if(matemat_global.mode == MODE_COOLING){
            if(matemat_global.temps[TEMP_BOTTOM] < matemat_global.temps[TEMP_STOP]){
                matemat_global.mode = MODE_IDLE;
                matemat_putmode();
                PIN_CLEAR(MATEMAT_COOLER);
            }
        }

        kick = 0;
        if(timer++ == 1){
            matemat_global.push = 1;
            timer = 0;
        }
    }
   
}

void matemat_init()
{
    i2c_init();
    lm75_init();
    matemat_setupdisp();
    DDR_CONFIG_OUT(MATEMAT_COOLER);
    PIN_SET(MATEMAT_COOLER);
    matemat_global.mode = MODE_COOLING;
    matemat_global.temps[TEMP_START] = temp(TEMP_START_VAL);
    matemat_global.temps[TEMP_STOP] = temp(TEMP_STOP_VAL);
}

#endif
