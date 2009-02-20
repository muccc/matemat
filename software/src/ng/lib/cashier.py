#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

import socket

class Cashier:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.connect(('127.0.0.1', 4444))

    def send(self, msg):
        sent = self.socket.send(msg)
        if sent == 0:
            return False
        else:
            return True

    def recv(self, expect):
         data = self.socket.recv(64)
         data = data.strip()
         print "Received: %s" % data
         if data == expect:
             return True
         else:
             return False

    def checkToken(self, token):
        sent = self.send("Tc%s" % token)

        if sent: rcvd = self.recv("OK")
        else: return False
        
        if rcvd: return True
        else: return False

    def checkCredit(self):
        sent = self.send("Td")

        if sent: rcvd = self.recv("OK")
        else: return False

        if rcvd: return True
        else: return False

# "Testing"

if __name__ == '__main__':
    cashier = Cashier()
    ret = cashier.checkToken("12345678")
    print "ret=%s" % ret
