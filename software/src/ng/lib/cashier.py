#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

import socket
import logger

class Cashier:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.connect(('127.0.0.1', 4444))
        self.socket.settimeout(1)
        self.log = logger.Logger('Cashier')
        self.log.debug('__init__(): invoked')

    def send(self, msg):
        self.log.debug('send(): invoked')
        self.log.log('send(): msg=%s' % msg)

        sent = self.socket.send(msg)
        if sent == 0:
            return False
        else:
            return True

    def recv(self, expect):
        self.log.debug('recv(): invoked')
        self.log.log('recv(): expect=%s' % expect)
        try:
            data = self.socket.recv(64)
        except:
            return False
        data = data.strip()
        if data == expect:
            return True
        else:
            return False

    def isReady(self):
        self.log.debug('isReady(): invoked')
        sent = self.send("Rd")
        if sent: rcvd = self.recv("READY")
        else: return False
                        
        if rcvd: return True
        else: return False

    def abort(self):
        self.log.debug('abort(): invoked')
        sent = self.send("Ab")
        if sent: rcvd = self.recv("OK")
        else: return False
                        
        if rcvd: return True
        else: return False

    def checkToken(self, token):
        self.log.debug('checkToken(): invoked')
        self.log.log('checkToken(): token=%s' % token)
        sent = self.send("Tc%s" % token)

        if sent: rcvd = self.recv("OK")
        else: return False
        
        if rcvd: return True
        else: return False

    def checkCredit(self):
        self.log.debug('checkCredit(): invoked')
        sent = self.send("Td")

#        if sent: rcvd = self.recv("OK")
#        else: return False

#        if rcvd: return True
#        else: return False

# "Testing"

if __name__ == '__main__':
    cashier = Cashier()
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkCredit()
