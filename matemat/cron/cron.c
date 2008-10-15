/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 * Copyright (c) 2008 by Christian Dietrich <stettberger@dokucode.de>
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

#include <avr/pgmspace.h>
#include <string.h>
#include "../syslog/syslog.h"
#include "cron.h"
#include "../config.h"

#ifdef CRON_SUPPORT

uint32_t last_check;

void test(void);

void test(void)
{
#ifdef SYSLOG_SUPPORT
    syslog_send_P(PSTR("cron event matched!"));
#endif
}

/* Cron configuration:
 * Fields: Min Hour Day Month Dow
 * Values: 
 * * -1    for every value in this field ( like * in a normal cron )
 * * >0    for exactly this value in this field
 * * < -1  step value, e.g. -2: every second minute
 */

struct cron_event_t events[] PROGMEM = 
{ { { {-1, -2, -1, -1, -1} }, test}, /* when hour % 2 == 0 */
  { { {51, -1, -1, -1, -1} }, test}, /* when minute is 51 */
  { { {-2, -1, -1, -1, -1} }, test}, /* when minute % 2 == 0 */
  { { {-1, -1, -1, -1, -1} }, NULL},
};

void 
cron_periodic(void)
/* {{{ */ {
    /* convert time to something useful */
    struct clock_datetime_t d;
    uint32_t timestamp = clock_get_time();

    /* Only check the tasks every minute */
    if ((timestamp - last_check) < 60) return;

    clock_datetime(&d, timestamp);

    struct cron_event_t event;

    /* check every event for a match */
    for (uint8_t i = 0; ; i++) {
        memcpy_P(&event, &events[i], sizeof(struct cron_event_t));

        /* end of task list reached */
        if (event.handler == NULL) break;

        uint8_t r = cron_check_event(&event, &d);

        /* if it matches, execute the handler function */
        if (r > 0) {
            event.handler();
        }

    }

    /* save the actual timestamp */
    last_check = timestamp - d.sec;

} /* }}} */

uint8_t 
cron_check_event(struct cron_event_t *event, struct clock_datetime_t *d)
/* {{{ */ {

    for (uint8_t f = 0; f < 5; f++) {

        /* if this field has a wildcard, just go on checking */
        if (event->fields[f] == -1)
            continue;

        /* IF THis field has an absolute value, check this value, if it does
         * not match, this event does not match */
        if (event->fields[f] >= 0 && event->fields[f] != d->cron_fields[f])
            return 0;

        /* if this field has a step value, extract value and check */
        if (event->fields[f] < 0) {
            uint8_t step = -(event->fields[f]);

            /* if this is not within the steps, this event does not match */
            if ((d->cron_fields[f] % step) != 0)
                return 0;

        }

    }

    /* if all fields match, this event matches */
    return 1;

} /* }}} */

#endif
