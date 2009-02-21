#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

import socket
import sys
import time
import token
import matemat

class Checkout:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind(('127.0.0.1', 4444))
        self.token = token.Token()
        self.matemat = matemat.Matemat()

    def listen(self):
        self.matemat.writeLCD("OBEY AND CONSUME")
        while True:
            data, self.raddr = self.socket.recvfrom(64)
            data = data.strip()
            self.interpret(data)

    def send(self, msg):
        print "sending",msg
        sent = self.socket.sendto(msg, self.raddr)
        if sent == 0:
            return False
        else:
            return True

    def interpret(self, data):
        print "interpret:", data
        cmd = "%s%s" % (data[0], data[1])
        print "cmd:",cmd
        tokendata = data.lstrip("Tacd")
        print "tokendata:",tokendata

        if cmd == "Ta":
            if self.token.add(tokendata):
                self.send("OK")
                return True
            else:
                self.send("FAIL")
                return False
        elif cmd == "Rd":
            self.send("READY")
            return True
        elif cmd == "Tc":
            if self.token.check(tokendata):
                self.send("OK")
                return True
            else:
                self.send("FAIL")
                return False
        elif cmd == "Td":
            credit = self.token.eot()
            self.matemat.writeLCD("Credit: %s" % credit)
            
            priceline = 0
            while priceline == 0:
                priceline = self.matemat.getPriceline()
                #time.sleep(1)
            print "checking liquidity"

            liquidity = self.token.assets(priceline)

            if liquidity == False:
                self.matemat.writeLCD("Not enough credits")
                #time.sleep(1.0)
                self.send("FAIL")
                return False

            if self.matemat.serve(priceline):
                self.matemat.writeLCD("Enjoy it")
                self.matemat.completeserve()
                self.token.finish(priceline)
                self.send("OK")
                return True
            else:
                self.matemat.writeLCD("Failed to serve")
                self.send("FAIL")
                return False

# "Testing"
if __name__ == '__main__':
    co = Checkout()
    co.listen()
