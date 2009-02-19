#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------


import serial
import socket
import time

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket.bind(('127.0.0.1', 4444))

serial = serial.Serial('/dev/ttyS0', 115200)

def msg2lcd(val):
	lcdmsg = "D%s\n" % val
	if debug: print "msg2lcd: %s" % val
	serial.write(lcdmsg)
	
	ret = ''
	while ret != 'D':
		ret = serial.read(1)
		if debug: print "msg2lcd return: %s" % ret

def readPurse():
	purse = open("/mnt/token/purse", "r")
	pursedata = purse.readlines()
	
	tokencount = 0
	for token in pursedata:
		tokenvalue = verifyToken(token)
		tokencount = tokencount+tokenvalue

	return tokencount

# TODO: Add verification of token
def verifyToken(val):
	return 1

def getPriceline():
	serial.write('V')
	reply = serial.read(1)
	if reply == '-':
		return False
	return int(reply)

# TODO: Add verification of credit (check against priceline) 
def verifyCredit(x, y):
	return True

def talkSerial(send, expect):
	serial.write(send)
	reply = serial.read(1)
	if debug: print "talkSerial reply: %s" % reply
	if reply == expect:
		return True
	else:
		return False

while 1:
	data = s.recv(23)
	processPacket(data)

	if data == 'START':
		msg2lcd("Starting")
		if debug: print "START"
		break
	elif data == 'UNSUPPORTED':
		msg2lcd("Unsupported device")
		if debug: print "UNSUPPORTED"
		break
	elif data == 'MOUNT':
		token = readPurse()
		priceline = getPriceline()
		if verifyCredit(token, priceline):
			ret = talkSerial('O', 'D')
			if ret:
				msg2lcd("Enjoy it")
				os.system("/usr/local/bin/udev-handler.sh stop")
				if debug: print "Enjoy it"
		else
			msg2lcd("Not enough credits")
			os.system("/usr/local/bin/udev-handler.sh stop")
			if debug: print "Not enough credits"
		break
	elif data == 'DONE':
		msg2lcd("Goodbye")
		time.sleep(1.5)
		msg2lcd("Standby")
		break
