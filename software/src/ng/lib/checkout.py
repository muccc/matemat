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
import token

class Checkout:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind(('127.0.0.1', 4444))
        self.token = token.Token()

    def listen(self):
        while True:
            data, self.raddr = self.socket.recvfrom(64)
            data = data.strip()
            print "Received: %s" % data
            self.interpret(data)

    def send(self, msg):
        sent = self.socket.sendto(msg, self.raddr)
        if sent == 0:
            return False
        else:
            return True

    def interpret(self, data):
        cmd = "%s%s" % (data[0], data[1])
        tokendata = data.rstrip("Tacd")
        if cmd == "Ta":
            if self.token.add(tokendata):
                self.send("OK")
                return True
            else:
                self.send("FAIL")
                return False
        elif cmd == "Tc":
            if self.token.check(tokendata):
                self.send("OK")
                return True
            else:
                self.send("FAIL")
                return False
        elif cmd == "Td":
            if self.token.eot(tokendata):
                self.send("OK")
                return True
            else:
                self.send("FAIL")
                return False
        else:
            return False

# "Testing"
if __name__ == '__main__':
    co = Checkout()
    co.listen()
