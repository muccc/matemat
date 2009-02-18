#!/usr/bin/python

#
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------
#

import serial
import time
import sys

def lcdmsg(val):
        lcdmsg = "D                   \n"
        serial.write(lcdmsg)
        serial.read(1)
        lcdmsg = "D%s\n" % val
        print lcdmsg
        serial.write(lcdmsg)
        serial.read(1)

serdev = "/dev/ttyS0"
serial = serial.Serial(serdev, 115200)

lcdmsg("foo")

purse = open("/mnt/token/purse", "r")
pursedata = purse.readlines()

tokencount = 0
for line in pursedata:
        tokencount = tokencount+1

lcdmsg("Token Count: %s" % tokencount)

while 1:
        serial.write("V")
        mmstate = serial.read(1)
        print mmstate
        if mmstate != "-":
                serial.write("O")
                mmstate = serial.read(1)
                print mmstate
                if mmstate == "D":
                        lcdmsg("Okay")
                        sys.exit(0)
# EOF
