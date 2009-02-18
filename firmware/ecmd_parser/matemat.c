#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <string.h>

#include "../config.h"
#include "../debug.h"
#include "../uip/uip.h"
#include "../uip/uip_arp.h"
#include "../eeprom.h"
#include "../bit-macros.h"
#include "../portio.h"
#include "../lcd/hd44780.h"
#include "../named_pin/named_pin.h"
#include "../onewire/onewire.h"
#include "../rfm12/rfm12.h"
#include "../dns/resolv.h"
#include "../clock/clock.h"
#include "../yport/yport.h"
#include "../matemat/matemat.h"
#include "ecmd.h"


#ifdef MATEMAT_SUPPORT
int16_t parse_cmd_matemat_settemp(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    return 0;
} /* }}} */

int16_t parse_cmd_matemat_gettemp(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    uint8_t t;
    int ret = sscanf_P(cmd,
            PSTR("%x"),&t);
    if (ret == 1 && t < MAX_TEMPS){
        sprintf_temp(output,matemat_global.temps[t],0);
        return strlen(output);
    }else{
        return -1;
    }
} /* }}} */

int16_t parse_cmd_matemat_cooling(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    if(cmd[1] == '1')
        PIN_SET(MATEMAT_COOLER);
    if(cmd[1] == '0')
        PIN_CLEAR(MATEMAT_COOLER);
    return 0;
} /* }}} */
#endif /* RC5_SUPPORT */

