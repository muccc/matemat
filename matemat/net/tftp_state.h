/*
 * Copyright (c) 2007,2008 by Stefan Siegl <stesie@brokenpipe.de>
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
 */

#ifndef TFTP_STATE_H
#define TFTP_STATE_H

#include "tftp_net.h"
#include "../dataflash/fs.h"

/* state */
struct tftp_connection_state_t {
#ifdef DATAFLASH_SUPPORT
    fs_inode_t     fs_inode;
    unsigned       df_access   :1;
#endif
    unsigned       download    :1;
    unsigned       finished    :1;

#ifdef BOOTLOADER_SUPPORT
    unsigned       bootp_image :1;
    unsigned       fire_req    :1;		/* this connection is for just
						 * starting a tftp request */

    unsigned char  filename[TFTP_FILENAME_MAXLEN];
#endif

    uint16_t       transfered;			/* also retry countdown */
};

#endif /* TFTP_STATE_H */
