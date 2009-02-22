#!/usr/bin/python

import serial
import serialinterface
import time
#import logger
import logging
class Matemat:
    def __init__(self):
        self.interface = serialinterface.SerialInterface("/dev/ttyS0",115200,5);
        #self.log = logger.Logger('Matemat')
        self.log = logging.getLogger("Matemat")
        ch = logging.StreamHandler()
        formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
        ch.setFormatter(formatter)
        self.log.addHandler(ch)
        self.log.setLevel(logging.DEBUG)
        self.log.debug('__init__(): invoked')

    def waitForReply(self,reply):
        self.log.debug('waitForReply(): invoked')
        self.log.info('waitForReply(): reply=%s' % reply)
        while True:
            msg = self.interface.readMessage()
            if msg == False:
                return False
            if msg in reply:
                self.log.info('waitForReply(): msg=%s' % msg)
                return msg


    def writeLCD(self, msg):
        self.log.debug('writeLCD(): invoked')
        self.log.info('writeLCD(): msg=%s' % msg)
        msg = "d"+msg
        self.interface.writeMessage(msg);
        return self.waitForReply(["dD"])
    
    def getPriceline(self):
        self.log.debug('getPriceline(): invoked')
        self.interface.writeMessage("p")
        while True:
            msg = self.interface.readMessage()
            if msg == False:
                return -1
            if msg[0] == 'p':
                self.log.info('getPriceline(): priceline=%s' % msg[2])
                return int(msg[2])

    def serve(self,priceline):
        self.log.debug('serve(): invoked')
        self.log.info('serve(): priceline=%s' % priceline)
        self.interface.writeMessage("s"+str(priceline))
        ret = self.waitForReply(["sO","sN"])
        if ret == False:
            return False
        if ret == "sN":
            return False
        return True

    def completeserve(self):
        self.log.debug('completeserve(): invoked')
        return self.waitForReply(["sD"])

# "Testing"

if __name__ == '__main__':
    m = Matemat()
    m.writeLCD("luvv")
    while m.getPriceline() != 3:
        time.sleep(0.2)
    m.serve(3)
    m.completeserve()
