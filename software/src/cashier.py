#!/usr/bin/python

# $Id$
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


###
## msg2lcd(message): Send a message to the matemat LCD.
## TODO: Handle 'D' returned from controller.
#
def msg2lcd(val):
	lcdmsg = "D%s\n" % val
	serial.write(lcdmsg)
	debug && print "LCD-Message: %s" % val

def readPurse():
	purse = open("/mnt/token/purse", "r")
	pursedata = purse.readlines()

	tokencount = 0
	for token in pursedata:
		tokencount = tokencount+1

	debug && print "Token Count: %s" % tokencount
	return tokencount

def talkSerial(send, expect):
	serial.write(send)
	reply = serial.read(1)
	debug && print "talkSerial() reply: %s" % reply
	if reply == expect:
		return TRUE
	else
		return FALSE
