dnl
dnl atmega32.m4
dnl
dnl   Copyright (c) 2008 by Georg von Zengen <oni@chaosplatz.com>
dnl  
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License as published by 
dnl   the Free Software Foundation; either version 2 of the License, or
dnl   (at your option) any later version.
dnl  
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl  
dnl   You should have received a copy of the GNU General Public License
dnl   along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl 

/* spi defines */
pin(SPI_MOSI, PB5)
pin(SPI_MISO, PB6)
pin(SPI_SCK, PB7)

ifdef(`conf_RFM12', `dnl
dnl
dnl Ancient zerties.org RFM12 pinout:
dnl   chip select: PC3
dnl   tx-led: 	   PD4
dnl   rx-led:	   PD5
dnl   interrupt:   INT0
dnl
dnl Configuration suggest for Etherrape hardware
dnl (all pins available at SPI connector)
dnl 
  /* port the rfm12 module CS is attached to */
  pin(SPI_CS_RFM12, PB0)

  /* port the LEDS for rfm12 txrx attached to */
  pin(RFM12_TX_PIN, PB3)
  pin(RFM12_RX_PIN, PB1)

  RFM12_USE_INT(2)
')

ifdef(`conf_ZBUS', `dnl
  /* port config for zbus */
  pin(ZBUS_RXTX_PIN, PC2)
  pin(ZBUS_TX_PIN, PD4)
  pin(ZBUS_RX_PIN, PD5)
')

ifdef(`conf_ECMD_SERIAL_USART_RS485', `dnl
  pin(ECMD_SERIAL_USART_TX, PC2)
')

ifdef(`conf_STELLA', `dnl
  STELLA_PORT_RANGE(PD5,PD7)
')

#define _ATMEGA32

/* ATmega32 specific adjustments */
#define _TIMSK_TIMER1 TIMSK
#define _UDRIE_UART0 UDRIE
#define _TXEN_UART0 TXEN
#define _RXEN_UART0 RXEN
#define _RXCIE_UART0 RXCIE
#define _TXCIE_UART0 TXCIE
#define _UBRRH_UART0 UBRRH
#define _UBRRL_UART0 UBRRL
#define _UCSRA_UART0 UCSRA
#define _UCSRB_UART0 UCSRB
#define _UCSRC_UART0 UCSRC
#define _UCSZ0_UART0 UCSZ0
#define _UCSZ1_UART0 UCSZ1
#define _SIG_UART_RECV_UART0 SIG_USART_RECV
#define _SIG_UART_DATA_UART0 SIG_USART_DATA
#define _UDR_UART0 UDR
#define _UDRE_UART0 UDRE
#define _RXC_UART0 RXC
#define _TXC_UART0 TXC
#define USART0_UDRE_vect USART_UDRE_vect
#define USART0_RX_vect USART_RXC_vect
#define USART0_TX_vect USART_TXC_vect
#define _IVREG MCUCR
#define _EIMSK GICR

#define _TCCR2_PRESCALE TCCR2
#define _OUTPUT_COMPARE_IE2 OCIE2
#define _OUTPUT_COMPARE_REG2 OCR2
#define _SIG_OUTPUT_COMPARE2 SIG_OUTPUT_COMPARE2B
#define _SIG_OVERFLOW2 SIG_OVERFLOW2
#define _TIMSK_TIMER2 TIMSK

/* workaround for avr-libc devs not being able to decide how these registers
 * should be named... */
#ifdef SPCR0
    #define _SPCR0 SPCR0
#else
    #define _SPCR0 SPCR
#endif

#ifdef SPE0
    #define _SPE0 SPE0
#else
    #define _SPE0 SPE
#endif

#ifdef MSTR0
    #define _MSTR0 MSTR0
#else
    #define _MSTR0 MSTR
#endif

#ifdef SPSR0
    #define _SPSR0 SPSR0
#else
    #define _SPSR0 SPSR
#endif

#ifdef SPIF0
    #define _SPIF0 SPIF0
#else
    #define _SPIF0 SPIF
#endif

#ifdef SPDR0
    #define _SPDR0 SPDR0
#else
    #define _SPDR0 SPDR
#endif

#ifdef SPI2X0
    #define _SPI2X0 SPI2X0
#else
    #define _SPI2X0 SPI2X
#endif

#define _TIFR_TIMER1 TIFR

dnl don't know if it is the right value
#define BOOTLOADER_SECTION 0xe000 /* atmega32 with 4096 words bootloader */


#ifdef HD44780_USE_PORTC
    #define HD44780_CTRL_PORT C
    #define HD44780_DATA_PORT C
    #define HD44780_RS PC0
    #define HD44780_RW PC1
    #define HD44780_EN PC2
    #define HD44780_D4 PC3
    #define HD44780_D5 PC4
    #define HD44780_D6 PC5
    #define HD44780_D7 PC6
    #define HD44780_DATA_SHIFT 3
#else
    #define HD44780_CTRL_PORT A
    #define HD44780_DATA_PORT A
    #define HD44780_RS PA0
    #define HD44780_RW PA1
    #define HD44780_EN PA2
    #define HD44780_D4 PA3
    #define HD44780_D5 PA4
    #define HD44780_D6 PA5
    #define HD44780_D7 PA6
    #define HD44780_DATA_SHIFT 3
#endif


#define NET_MAX_FRAME_LENGTH 1500
#define ADC_CHANNELS 8
