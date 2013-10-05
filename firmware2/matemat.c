#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "config.h"
#include "lib/uart.h"
#include "lib/lcd.h"
#include "debug.h"
#include "serial_handler.h"
#include "temperature.h"
#include "display.h"
#include "cooler.h"
#include "uptime.h"
#include "priceline.h"


#define PRESCALER1  1
#define F_TIMER1    1000
#define PRE_1S      1000
#define PRE_10MS    10

volatile uint8_t timebase = 0;
volatile uint8_t timebase1s = 0;
volatile uint8_t timebase10ms = 0;

ISR(TIMER1_COMPA_vect )
{
    static uint16_t count1s = PRE_1S;
    static uint16_t count10ms = PRE_10MS;
    timebase++;
    if(--count1s == 0){
        timebase1s=1;
        count1s = PRE_1S;
    }
    if(--count10ms == 0){
        timebase10ms=1;
        count10ms = PRE_10MS;
   }
   priceline_interrupt();
}

void cmd(uint8_t * cmd, uint8_t len)
{
    uint8_t ret;
    char buffer[20];
//    debug(2,"cmd");
    switch(cmd[0]){
        case 'r':
            debug(1,"jumping to bootloader");
            while(1);
        break;
        case 'd':
            cmd[len] = 0;
            display_msg((char*)cmd+1);
            serial_sendFrames("dD");
        break;
        case 'p':
            ret = priceline_get();
            sprintf(buffer,"p=%d",ret);
            serial_sendFrames(buffer);
        break;
        case 's':
            ret = priceline_serve(cmd[1]-'0');
            if(ret){
                serial_sendFrames("sO");
            }else{
                serial_sendFrames("sN");
            }
        break;

    }
}

void matemat_served(void)
{
    serial_sendFrames("sD");
}

int main(void)
{
    uint8_t gotcmd = 0;
    //char buffer[20];

    uart_init( UART_BAUD_SELECT(UART_BAUDRATE,F_CPU));
    sei();

    debug(1,"reset!");
    wdt_enable(WDTO_2S);
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_puts_P("booting");
    OCR1A = (F_CPU/PRESCALER1/F_TIMER1)-1UL;
    TCCR1B = 1<<WGM12 | 1<<CS10;           //ctc, div by 1
    TIMSK |= 1<<OCIE1A;         //enable timer interrupt
    cooler_init();
    temperature_init();
    uptime_init();
    priceline_init();
    display_init();
    display_tick(1);
    debug(1,"init done"); 
    while(1){
        wdt_reset();
        gotcmd = serial_readline();
        if(timebase1s){
            timebase1s = 0;
        }
        if(timebase10ms){
            timebase10ms = 0;
            temperature_tick();
            cooler_tick();
            uptime_tick();
            priceline_tick();
            display_tick(0);
        }
        if(gotcmd){
           cmd(serial_buffer, gotcmd);
           gotcmd = 0;
        }
    }
    return 0;
}
