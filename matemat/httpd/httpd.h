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

#ifndef _HTTPD_H
#define _HTTPD_H

#include <stdint.h>
#include "../uip/uip.h"

/* constants */
#define HTTPD_PORT 80
#define HTTPD_ALTERNATE_PORT 8000
#define HTTPD_TIMEOUT 100 /* in 200ms-steps */

#define HTTPD_INDEX "idx.ht"
#define ECMD_INDEX "ecmd"

/* prototypes */
void httpd_init(void);
void httpd_main(void);

#endif
