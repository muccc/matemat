#include "matemat.h"
#include "i2cmaster.h"
#include "lm75.h"
#include "../lcd/hd44780.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#ifdef MATEMAT_SUPPORT

#define USE_USART 0
#include "../usart.h"


volatile uint8_t kick = 0;
struct matemat_global matemat_global;

generate_usart_init(MATEMAT_SERIAL_USART_UBRR)

volatile unsigned int grant=0;
uint8_t msg[30];
uint8_t msgp = 0;

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
    //    kick = 1;
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

    hd44780_goto(1,17);
    fprintf_P(lcd, PSTR("PL"));
}

void matemat_putmsg(void)
{
    cli();
    hd44780_goto(0,0);
    fprintf(lcd,msg);
    sei(); 
}

void matemat_putmode(void)
{
    cli();
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
    sei();
}


volatile unsigned char pl = '-';

void matemat_process(void)
{
    static uint8_t state = 0;
    static uint8_t timer = 0;

    if(kick){
        cli();
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
        sei();
        
        //hd44780_goto(1,19);
        //hd44780_put(pl,NULL);

        //if( (PINC & (1<<PC4)) )
        //    hd44780_put('2',NULL);
        //else if( (PINC & (1<<PC3)) )
        //    hd44780_put('3',NULL);
        //else
        //    hd44780_put('-',NULL);
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
        if(timer++ == 10){
            matemat_global.push = 1;
            timer = 0;
        }
    }
   
}

uint8_t setPL(uint8_t p)
{
    switch(p){
        case '1':
            PORTC |= (1<<PC5);
            grant = 2000;
            return 1;
        break;
        case '3':
            PORTC |= (1<<PC6);
            grant = 2000;
            return 1;
        break;
    };
    return 0;
}

ISR(TIMER0_OVF_vect , ISR_BLOCK)
{
    static unsigned int i=0;
    static unsigned char count1=0;
    static unsigned char count3=0;
    if(i-- == 0){
        i = 3588;
        kick = 1;
    }

    if(grant)
        grant--;

    if(PINC & (1<<PC4)){
        if(pl == '-' && count1++ == 70){
            pl = '1';
            hd44780_goto(1,19);
            hd44780_put(pl,NULL);
        }
    }else{
        count1=0;
    }

    if(PINC & (1<<PC3)){
        if(pl == '-' && count3++==70){
            pl = '3';
            hd44780_goto(1,19);
            hd44780_put(pl,NULL);
        }
    }else{
        count3=0;
    }

    if(!grant && count3==0 && count1==0 && pl != '-'){
        PORTC &= ~(1<<PC5);
        PORTC &= ~(1<<PC6);
        pl = '-';
        hd44780_goto(1,19);
        hd44780_put(pl,NULL);
    }
}

SIGNAL(usart(USART,_RX_vect))
{
  static uint8_t reported = '-';
  static uint8_t print = 0;
  /* Ignore errors */
  if ((usart(UCSR,A) & _BV(usart(DOR))) || (usart(UCSR,A) & _BV(usart(FE)))) {
    uint8_t v = usart(UDR);
    (void) v;
    return; 
  }
  uint8_t data = usart(UDR);
  if(print){
    if(data == '\r' || data == '\n' || data == 0){
        print = 0;
        msg[20] = 0;
        matemat_putmsg();
        usart(UDR) = 'D';
    }else{
        msg[msgp++]=data;
        if(msgp == 20)
            msgp--;
    }
    return;
  }
  if(data == 'V'){
      usart(UDR) = pl;
      reported = pl;
  }else if(data == 'O'){
      if(setPL(reported))
        usart(UDR) = 'D';
      else
        usart(UDR) = 'N';
  }else if(data == 'D'){
    for(print=0;print<20;print++)
        msg[print] = ' ';
    print=1;
    msgp = 0;
  }

}

void matemat_init()
{
    DDR_CONFIG_OUT(MATEMAT_COOLER);
    PIN_CLEAR(MATEMAT_COOLER);
    DDRC &= ~((1<<PC4)|(1<<PC3));
    DDRC |= ((1<<PC5)|(1<<PC6));
    PORTC |= (1<<PC4)|(1<<PC3);
    PORTC &= ~((1<<PC5)|(1<<PC6));

    i2c_init();
    lm75_init();
    matemat_setupdisp();
    usart_init();
  //  usart(UCSR,B) &= ~_BV(usart(RXCIE));

    matemat_global.mode = MODE_IDLE;
    matemat_global.temps[TEMP_START] = temp(TEMP_START_VAL);
    matemat_global.temps[TEMP_STOP] = temp(TEMP_STOP_VAL);

    TCCR0B = 1<<CS01;       //div by 8
    TIMSK0 |= 1<<TOIE0;
}

#endif
