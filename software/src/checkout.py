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
import os

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket.bind(('127.0.0.1', 4444))
debug = True
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

	if debug: print "Tokencount: %s" % tokencount
	msg2lcd("Credits: %s " % tokencount)
	return tokencount

# TODO: Add verification of token
def verifyToken(val):
	return 1

def getPriceline():
	reply = '-'
	count = 0
	while reply == '-' and count <= 3000:
		serial.write('V')
		reply = serial.read(1)
		count = count+1
		if debug: print "getPriceline reply: %s" % reply

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
	data = socket.recv(23)
	data = data.strip()
	
	if data == 'START':
		msg2lcd("Starting")
		if debug: print "START"
	elif data == 'UNSUPPORTED':
		msg2lcd("Unsupported device")
		if debug: print "UNSUPPORTED"
	elif data == 'MOUNT':
		token = readPurse()
		priceline = getPriceline()
		if verifyCredit(token, priceline):
			if talkSerial('O', 'D'):
				msg2lcd("Enjoy it")
				os.system("/usr/local/bin/udev-handler.sh stop")
				if debug: print "Enjoy it"
		else:
			msg2lcd("Not enough credits")
			os.system("/usr/local/bin/udev-handler.sh stop")
			if debug: print "Not enough credits"
	elif data == 'DONE':
		msg2lcd("Goodbye")
		time.sleep(1.5)
		msg2lcd("Standby")
	else:
		print "FNORD"
