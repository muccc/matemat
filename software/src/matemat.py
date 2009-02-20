import serial
import serialinterface
import time

class Matemat:
    def __init__(self):
        self.interface = serialinterface.SerialInterface("/dev/ttyS0",115200,5);

    def waitForReply(self,reply):
        while True:
            msg = self.interface.readMessage()
            if msg == False:
                return False
            if msg in reply:
                return msg


    def writeLCD(self, msg):
        msg = "d"+msg
        self.interface.writeMessage(msg);
        return self.waitForReply(["dD"])
    
    def getPriceline(self):
        self.interface.writeMessage("p")
        while True:
            msg = self.interface.readMessage()
            if msg == False:
                return -1
            if msg[0] == 'p':
                return int(msg[2])
    def serve(self,priceline):
        self.interface.writeMessage("s"+str(priceline))
        ret = self.waitForReply(["sO","sN"])
        if ret == False:
            return False
        if ret == "sN":
            return False
        return True
    def completeserve(self):
        return self.waitForReply(["sD"])

#m = Matemat()
#m.writeLCD("luvv")
#while m.getPriceline() != 3:
#    time.sleep(0.2)
#m.serve(3)
#m.completeserve()



