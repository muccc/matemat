/* vim:fdm=marker ts=4 et ai
 * {{{
 *
 * Copyright (c) 2007,2008 by Christian Dietrich <stettberger@dokucode.de>
 * (c) by Alexander Neumann <alexander@bumpern.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
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

#ifndef _CLOCK_H
#define _CLOCK_H

#include "../config.h"

struct clock_datetime_t {
    uint8_t sec;
    union {
        uint8_t cron_fields[5];
        struct {
            uint8_t min;
            uint8_t hour;
            uint8_t day;
            uint8_t month;
            uint8_t dow;
        };
    };
    uint8_t year;
};

/* current_time is the amount of seconds since 1.1.1900, 00:00:00 UTC */
#define EPOCH_YEAR 1970
/* 1.1.1970 was a thursday */
#define EPOCH_DOW 4

void clock_init(void);
void clock_tick(void);

/* the actual time as unix time stamp */
uint32_t clock_get_time(void);

/* when was the clock synced the last time (unix timestamp) */
uint32_t clock_last_sync(void);

/* when was the device booted (unix timestamp) */
uint32_t clock_get_startup(void);

/* the actual time */
void clock_set_time(uint32_t new_sync_timestamp);

/** convert time in timestamp to a datetime struct */
void clock_datetime(struct clock_datetime_t *d, uint32_t timestamp);

/** convert current time to a datetime struct */
#define clock_current_datetime(d) clock_datetime(d, clock_get_time())

/** test if given year is a leap year */
#define is_leap_year(y)     (((y % 4) == 0) && ( (y % 100 != 0) || (y % 400 == 0) ))

#endif
