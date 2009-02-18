/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * Copyright (c) by Alexander Neumann <alexander@bumpern.de>
 * Copyright (c) 2007 by Stefan Siegl <stesie@brokenpipe.de>
 * Copyright (c) 2007 by Christian Dietrich <stettberger@dokucode.de>
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
 }}} */

#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "../config.h"
#include "../debug.h"
#include "../uip/uip.h"
#include "../uip/uip_arp.h"
#include "../eeprom.h"
#include "../bit-macros.h"
#include "../lcd/hd44780.h"
#include "ecmd.h"


#ifdef HD44780_SUPPORT
int16_t parse_cmd_lcd_clear(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    uint16_t line;

    int ret = sscanf_P(cmd,
            PSTR("%u"),
            &line);

    if (ret == 1) {
        if (line > 3)
            return -1;

        hd44780_goto(LO8(line), 0);
        for (uint8_t i = 0; i < 20; i++)
            fputc(' ', lcd);
        hd44780_goto(LO8(line), 0);

        return 0;
    } else {
        hd44780_clear();
        hd44780_goto(0, 0);
        return 0;
    }
} /* }}} */

int16_t parse_cmd_lcd_write(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    if (strlen(cmd) > 1) {
        fputs(cmd+1, lcd);
        return 0;
    } else
        return -1;
} /* }}} */

int16_t parse_cmd_lcd_goto(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    uint16_t line, pos = 0;

    int ret = sscanf_P(cmd,
            PSTR("%u %u"),
            &line, & pos);

    if (ret >= 1 && line < 4) {
        if (ret == 2 && pos >= 20) {
            pos = 20;
        } else if (ret == 1)
            pos = 0;

        debug_printf("going to line %u, pos %u\n", line, pos);

        hd44780_goto(LO8(line), LO8(pos));
        return 0;
    } else
        return -1;

} /* }}} */

int16_t parse_cmd_lcd_char(char *cmd, char *output, uint16_t len)
/* {{{ */ {
  if (strlen(cmd) < 26) 
    return -1;
  uint8_t n_char, data[8];
  int ret = sscanf_P(cmd, PSTR("%u %x %x %x %x %x %x %x %x"), &n_char,
                     &data[0], &data[1], &data[2], &data[3],
                     &data[4], &data[5], &data[6], &data[7]);

  if (ret == 9) {
    hd44780_define_char(n_char, data);
    return 0;
  } else
    return -1;
} /* }}} */

int16_t parse_cmd_lcd_init(char *cmd, char *output, uint16_t len)
/* {{{ */ {
  uint8_t cursor, blink;
  int ret = sscanf_P(cmd, PSTR("%u %u"), &cursor, &blink);
  if ( ret == 2 ) {
    hd44780_init(cursor, blink);
    return 0;
  } else
    return -1;
} /* }}} */

int16_t parse_cmd_lcd_shift(char *cmd, char *output, uint16_t len)
/* {{{ */ {
  if (strlen(cmd) < 1) 
    return -1;

  if (!strncmp_P(cmd + 1, PSTR("right"), 5))
    hd44780_shift(1);
  else if (!strncmp_P(cmd + 1, PSTR("left"), 4)) 
    hd44780_shift(0);
  else
    return -1;

  return 0;
} /* }}} */
#endif /* HD44780_SUPPORT */
