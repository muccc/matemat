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
        self.log.level('INFO')
        self.log.info('__init__(): invoked')
        
    def send(self, msg):
        self.log.info('send(%s): invoked' % msg)

        sent = self.socket.send(msg)
        if sent != 0:
            self.log.debug('send(%s): Send OK' % msg)
            return True
        else:
            self.log.debug('send(%s): Send failed' % msg)
            return False

    def recv(self, expect):
        self.log.info('recv(%s): invoked' % expect)
        try:
            data = self.socket.recv(64)
        except:
            return False
        
        data = data.strip()
        if data == expect:
            self.log.debug('recv(%s): Receive OK' % expect)
            return True
        else:
            self.log.debug('recv(%s): Receive failed' % expect)
            return False

    def isReady(self):
        self.log.info('isReady(): invoked')
        sent = self.send("Rd")
        if sent:
            rcvd = self.recv("READY")
        else:
            return False
                        
        if rcvd:
            return True
        else:
            return False

    def reportBadPurse(self): 
        self.log.info('reportBadPurse(): invoked')
        sent = self.send("Bp")

    def abort(self):
        self.log.info('abort(): invoked')
        sent = self.send("Ab")
        if sent: rcvd = self.recv("OK")
        else: return False
                        
        if rcvd: return True
        else: return False

    def checkToken(self, token):
        self.log.info('checkToken(%s): invoked' % token)
        sent = self.send("Tc%s" % token)

        if sent:
            rcvd = self.recv("OK")
        else:
            return False
        
        if rcvd:
            self.log.debug('checkToken(%s): True' % token)
            return True
        else:
            self.log.debug('checkToken(%s): False' % token)
            return False

    def checkCredit(self):
        self.log.info('checkCredit(): invoked')
        sent = self.send("Td")

# "Testing"

if __name__ == '__main__':
    cashier = Cashier()
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkToken("12345678")
    ret = cashier.checkCredit()
