/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * Copyright (c) 2007 by Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <stdint.h>
#include "../config.h"
#include "stella.h"


void
stella_process (unsigned char *buf, uint8_t len)
{
  int re_sort = 0;

  for (; len >= 2; buf += 2, len -= 2)
    if(*buf >= STELLA_SET_COLOR_0 && *buf < (STELLA_SET_COLOR_0 + STELLA_PINS))
    {
      stella_fade[*buf] = buf[1];
      stella_color[*buf] = buf[1];
      re_sort = 1;
    }
    else if(*buf >= STELLA_FADE_COLOR_0 && *buf < (STELLA_FADE_COLOR_0 + STELLA_PINS))
    {
      stella_fade[*buf & 0x07] = buf[1];
    }
    else if(*buf >= STELLA_FLASH_COLOR_0 && *buf < (STELLA_FLASH_COLOR_0 + STELLA_PINS))
    {
      stella_fade[*buf & 0x07] = 0;
      stella_color[*buf & 0x07] = buf[1];
      re_sort = 1;
    }
    else if(*buf == STELLA_SELECT_FADE_FUNC)
    {
      stella_fade_func = buf[1];
    }
  if (re_sort)
    stella_sort (stella_color);
}
