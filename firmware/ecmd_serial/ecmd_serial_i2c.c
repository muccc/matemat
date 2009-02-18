/*
 * Copyright (c) 2008 by Christian Dietrich <stettberger@dokucode.de>
 * Copyright (c) 2008 by Jochen Roessner <jochen@lugrot.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../config.h"
#include "ecmd_serial_i2c.h"
#include "../ecmd_parser/ecmd.h"


#define ECMD_SERIAL_I2C_ADDR 8

#ifdef ECMD_SERIAL_I2C_SUPPORT

static char buffer[ECMD_SERIAL_I2C_BUFFER_LEN];
static uint8_t len, sent, parse;

static void
init_twi(void){
  
  TWCR = 0; //fuer das Initialisieren bei einem status fehler

  /* INIT fuer den TWI i2c
  * hier wird die Addresse des µC festgelegt
  * (in den oberen 7 Bit, das LSB(niederwertigstes Bit)
  * steht dafür ob der µC auf einen general callreagiert
  */ 
  TWAR = ECMD_SERIAL_I2C_ADDR << 1;
  
  /* TWI Control Register, hier wird der TWI aktiviert, 
   * der Interrupt aktiviert und solche Sachen
   */
  TWCR = (1<<TWIE) | (1<<TWEN) | (1<<TWEA); 
  
  /* TWI Status Register init */
  TWSR &= 0xFC; 
}

void ecmd_serial_i2c_init(void) {
  len = 0;
  sent = 0;
  parse = 0;
  init_twi();
}

void 
ecmd_serial_i2c_periodic(void)
{
  /* error detection on i2c bus */
  if((TWSR & 0xF8) == 0x00)
    init_twi();
  if (parse) {
    len = ecmd_parse_command(buffer, buffer, sizeof(buffer));
    parse = 0;
  }
}

/* twi interrupt */
ISR (TWI_vect)
{
  switch (TWSR & 0xF8){
  case 0x80: /* databyte was received */
    if (len < (sizeof(buffer) - 1)) 
        buffer[len++] = TWDR;
    if (buffer[len-1] == 0) {
      /* EOF message */
      parse = 1;
    }
    break;
  case 0x60: /* Start condition + write was received */
    /* Der Avr wurde mit seiner Adresse angesprochen  */
    len = 0;
  break;
  case 0xA8: /* Start condition + read was received */
    sent = 0; /* fall through */
  case 0xB8: /* the answer is sent */
    if (sent < len)
      TWDR = buffer[sent++];
    else 
      TWDR = 0;
  break;
  }
  TWCR |= (1<<TWINT); //TWI wieder aktivieren
}





#endif
