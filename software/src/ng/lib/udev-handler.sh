#!/bin/bash

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------
#

start(){
	mount /mnt/token/ && /usr/local/bin/udev.py 
    umount /mnt/token
	return $?
}

#stop(){
#	umount /mnt/token &&
#	return $?
#}

$1

exit $?
