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
import getopt
import os

debug = True

###
## msg2lcd(message): Send a message to the matemat LCD.
## TODO: Handle 'D' returned from controller.
#

serial = serial.Serial("/dev/ttyS0",115200)
def msg2lcd(val):
    lcdmsg = "D%s\n" % val
    if debug: print "LCD-Message: %s" % val
    serial.write(lcdmsg)
    ret = ''
    while ret != 'D':
        ret = serial.read(1)
        if debug: print "Lcd return="+ret

def readPurse():
	purse = open("/tmp/usb/purse", "r")
	pursedata = purse.readlines()

	tokencount = 0
	for token in pursedata:
		tokencount = tokencount+1

	if debug: print "Token Count: %s" % tokencount
	return tokencount

def talkSerial(send, expect):
	serial.write(send)
	reply = serial.read(1)
	if debug: print "talkSerial() reply: %s" % reply
	if reply == expect:
		return True
	else:
		return False
def getPriceline():
    serial.write('V')
    reply = serial.read(1)
    if reply == '-':
        return -1
    return int(reply)


type = ""
path = ""

log = open("/tmp/matemat.log","a")
log.write(time.ctime()+" trying to parse arguments\n")

try:
    opts, args = getopt.getopt(sys.argv[1:], "t:p:", ["type=", "path="])
except getopt.GetoptError:
    usage()
    sys.exit(2)
for opt, arg in opts:
    if opt in ("-t", "--type"):
        type = arg
    if opt in ("-p", "--path"):
        path = arg

if type != "disk":
    log.write("method not yet supported\n")
    log.write("aborting\n")
    log.close()
    sys.exit(1)

log.write("trying to mount...")
ret = os.system("mount "+path+" /tmp/usb")
log.write("mount returned with "+str(ret)+"\n")

if ret != 0:
    log.write("aborting\n")
    log.close()
    sys.exit(2)
log.write("mount successful\n")

try:
    t = readPurse()
    msg2lcd("tokens: "+str(t))
    pl = -1
    while pl == -1:
        pl = getPriceline()
    if pl == 1:
        price = 3
    else:
        price = 2

    ret = talkSerial('O','D')
    msg2lcd("serving "+str(price)+" tokens")
    time.sleep(5)
    msg2lcd("goodbye")
except:
    log.write("unexpected error\n")

log.write("trying to unmount..")
ret = os.system("umount /tmp/usb")
log.write("umount returned with "+str(ret)+"\n")

if ret == 0:
    log.write("umount successful\n")
else:
    log.write("umount failed\n")

log.close()
