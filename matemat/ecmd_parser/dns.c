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
#include "../bit-macros.h"
#include "../dns/resolv.h"
#include "../eeprom.h"
#include "ecmd.h"

#ifdef DNS_SUPPORT

#ifndef TEENSY_SUPPORT
extern int16_t print_ipaddr (uip_ipaddr_t *addr, char *output, uint16_t len);
extern int8_t parse_ip(char *cmd, uip_ipaddr_t *ptr);
#endif


int16_t parse_cmd_show_dns(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    return print_ipaddr (resolv_getserver (), output, len);
} /* }}} */

#ifndef BOOTP_SUPPORT
int16_t parse_cmd_dns(char *cmd, char *output, uint16_t len)
/* {{{ */ {
    uip_ipaddr_t dnsaddr;

    while (*cmd == ' ')
	cmd++;

    /* try to parse ip */
    if (parse_ip (cmd, &dnsaddr))
	return -1;

    resolv_conf (&dnsaddr);
    struct eeprom_config_ext_t new_cfg;
    memset(&new_cfg, 0, sizeof(new_cfg));
    memcpy (new_cfg.dns_server, &dnsaddr, sizeof(uip_ipaddr_t));
    return eeprom_save_config_ext (&new_cfg);
} /* }}} */
#endif /* not BOOTP_SUPPORT */

int16_t parse_cmd_nslookup (char *cmd, char *output, uint16_t len)
/* {{{ */ {
  while (*cmd == 32) cmd ++;
  uip_ipaddr_t *addr = resolv_lookup (cmd);

  if (addr) {
    return print_ipaddr (addr, output, len);
  }
  else {
    resolv_query (cmd, NULL);
    return snprintf_P (output, len, PSTR ("nslookup triggered, try again for result."));
  }
} /* }}} */

#endif /* DNS_SUPPORT */
