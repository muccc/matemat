/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
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

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdio.h>

#include "portio.h"
#include "uip/uip.h"

/* global configuration struct */
typedef union {
    uint8_t link:1;
} global_status_t;

typedef struct {
    global_status_t status;

    uint8_t request_bootloader :1;
    uint8_t request_reset      :1;
    uint8_t request_wdreset    :1;
} global_config_t;

extern global_config_t cfg;

#endif /* _GLOBAL_H */
