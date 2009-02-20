#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------


#import serial
import socket
import time
import os
import matemat

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket.bind(('127.0.0.1', 4444))
debug = True
#serial = serial.Serial('/dev/ttyS0', 115200)
mate = matemat.Matemat()

def readPurse():
	purse = open("/mnt/token/purse", "r")
	pursedata = purse.readlines()
	
	tokencount = 0
	for token in pursedata:
		tokenvalue = verifyToken(token)
		tokencount = tokencount+tokenvalue

	if debug: print "Tokencount: %s" % tokencount
	mate.writeLCD("Credits: %s " % tokencount)
	return tokencount

# TODO: Add verification of token
def verifyToken(val):
	return 1

def getPriceline():
	reply = 0
	count = 0
	while reply == 0 and count <= 3000:
		reply = mate.getPriceline()
		count = count+1
		if debug: print "getPriceline reply: %s" % reply
		if reply == -1:
			return -1
	return reply

# TODO: Add verification of credit (check against priceline) 
def verifyCredit(x, y):
	return True

while 1:
	data = socket.recv(23)
	data = data.strip()
	
	if data == 'START':
		mate.writeLCD("Starting")
		if debug: print "START"
	elif data == 'UNSUPPORTED':
		mate.writeLCD("Unsupported device")
		if debug: print "UNSUPPORTED"
	elif data == 'MOUNT':
		token = readPurse()
		priceline = getPriceline()
		if verifyCredit(token, priceline):
			if mate.serve(priceline):
				if mate.completeserve():
					mate.writeLCD("Enjoy it")
	    				if debug: print "Enjoy it"
				else:
					mate.writeLCD("error in complete")
					if debug: print "error while completing the serve"
			else:
				mate.writeLCD("cannot serve you")
				if debug: print "could not start serving"
			os.system("/usr/local/bin/udev-handler.sh stop")
		else:
			mate.writeLCD("Not enough credits")
			os.system("/usr/local/bin/udev-handler.sh stop")
			if debug: print "Not enough credits"
	elif data == 'DONE':
		mate.writeLCD("Goodbye")
		time.sleep(1.5)
		mate.writeLCD("Standby")
	else:
		print "FNORD"
