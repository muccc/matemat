#include "lm75.h"
#include "i2cmaster.h"
#ifdef MATEMAT_SUPPORT

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

#endif
