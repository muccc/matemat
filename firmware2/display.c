#include <stdio.h>
#include <string.h>

#include "lib/lcd.h"
#include "display.h"
#include "config.h"
#include "temperature.h"
#include "cooler.h"
#include "uptime.h"
#include "priceline.h"

void printtemp(uint8_t index, uint8_t x, uint8_t y)
{
    char buf[20];
    lcd_gotoxy(x,y);
    temperature_format(index, buf);
    lcd_puts(buf);
    lcd_putc(' ');
    lcd_putc(0xDF);
    lcd_putc('C');
}

void display_init(void)
{
    lcd_clrscr();
    lcd_puts_P("   Matemat blubb");
    lcd_gotoxy(0,2);
    lcd_puts_P("Middle: ");

    lcd_gotoxy(0,3);
    lcd_puts_P("Bottom: ");

    lcd_gotoxy(17,1);
    lcd_puts_P("PL");
   
}

void display_msg(char * msg)
{
    uint8_t len = strlen(msg);
    uint8_t i;
    lcd_gotoxy(0,0);
    for(i=len;i<20;i++)
        msg[i] = ' ';
    msg[20] = 0;
    lcd_puts(msg);
}

void display_tick(uint8_t force)
{
    static uint16_t count = 0;
    static uint8_t state = 0;
    if(count-- == 0){
        count = 50;
        lcd_gotoxy(19,3);
        if(state)
            lcd_putc('.');
        else
            lcd_putc(' ');
        state = !state;
    }
    if(temperature.updated || force){
        display_updatetemp();
    }
    if(cooler.updated || force){
        display_putcoolerstate();
    }
    if(uptime.updated || force){
        display_uptime();
    }
    if(priceline.updated || force){
        display_priceline();
    }
}

void display_priceline(void)
{
    lcd_gotoxy(19,1);
    if(priceline.pl == 0)
        lcd_putc('-');
    else
        lcd_putc('0'+priceline.pl);
}

void display_uptime(void)
{
    char buffer[20];
    lcd_gotoxy(0,1);
    sprintf(buffer,"%lu",uptime.uptime);
    lcd_puts(buffer);
}

void display_putcoolerstate(void)
{
    lcd_gotoxy(10,1);
    switch(cooler.state){
        case COOLER_IDLE:
            lcd_puts_P("idle   ");
        break;
        case COOLER_COOLING:
            lcd_puts_P("cooling");
        break;
        default:
            lcd_puts_P("?mode? ");
        break;
    }
}

void display_updatetemp(void)
{
    printtemp(TEMP_BOTTOM,8,3);
    printtemp(TEMP_MIDDLE,8,2);   
}

