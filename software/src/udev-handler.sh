#!/bin/bash

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------
#

UTYPE=$1
UPATH=$2

if [ ! $UTYPE == "disk" ];
then
	echo "UNSUPPORTED" | nc -u localhost 4444
	exit 1
fi

/usr/local/bin/lcdreport.py -t "Starting"

mount $UPATH /mnt/token/ && echo $UPATH | nc -u localhost 4444
umount $UPATH && echo "DONE" | nc -u localhost 4444


exit &?
