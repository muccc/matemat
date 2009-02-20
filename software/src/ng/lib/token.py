#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

# TODO: Add SQL (;

class Token:
    def __init__(self):
        self.tokencount = 0
        self.tokenreset = False

    def add(self, token):
        return True 
    
    def check(self, token):
        if self.tokenreset:
            self.tokenreset = False
            self.tokencount = 0 

        self.tokencount = self.tokencount+1 
        return True

    def eot(self):
        if self.tokencount > 3:
            self.tokenreset = True
            return self.tokencount
        else:
            return False

    def assets(self, priceline):
        # TODO: Get price for PL and check credit (tokencount)

if __name__ == '__main__':
    token = Token()
    token.add("foo")
    token.check("foo")
    token.eot("foo")
