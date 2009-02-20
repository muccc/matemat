#ifndef __DISPLAY_H_
#define __DISPLAY_H_
#include <stdint.h>

void display_tick(uint8_t force);
void display_init(void);
void display_updatetemp(void);
void display_putcoolerstate(void);
void display_uptime(void);
void display_priceline(void);
void display_msg(char * msg);
#endif
