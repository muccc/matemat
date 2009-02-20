#include <string.h>
#include <stdio.h>

#include "lib/lm75.h"
#include "lib/i2cmaster.h"
int16_t lm75_gettemp(uint8_t device){
    device = LM75_BASE + (device << 1);
    if(i2c_start(device + 1)){
        return 100;
    }else{
        int16_t a = i2c_readAck() << 1;
        uint8_t b = i2c_readNak() >> 7;
        i2c_stop();
        if(a & 256)
            a |= 0xfe00;
        return a+b;
        //return 50*2+1;
    }
}

void lm75_init(void)
{
}

void lm75_sprintf_temp(char * s, int16_t t, uint8_t pad)
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
        sprintf(s+strlen(s),".%dC",b);
    }
}
