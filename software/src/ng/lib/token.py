#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

# TODO: Add SQL (;

import sqlite3

class Token:
    def __init__(self):
        self.tokencount = 0
        self.cost = 0
        self.tokenreset = False
        self.db = sqlite3.connect('token.db')
        self.db_cur = self.db.cursor()
        self.tokenlist = []

    def add(self, token):
        return True 
    
    def check(self, token):
        print token
        if self.tokenreset:
            self.tokenreset = False
            self.tokencount = 0
            self.tokenlist = []
        
        self.db_cur.execute('SELECT hash FROM tokens WHERE used=0 AND hash=? LIMIT 1', (token,))
        if self.db_cur.fetchone():
            print "FOUND UNUSED TOKEN: %s" % token
            self.tokencount = self.tokencount+1
            self.tokenlist.append(token)
            return True
        else:
            return False

    def eot(self):
        return self.tokencount
            
    def assets(self, priceline):
        self.cost = 0
        print "translating priceline"
        self.db_cur.execute('SELECT price FROM pricelines WHERE priceline=?', (priceline,))
        print "executed query"
        price = self.db_cur.fetchone()
        print "price=",price
        #while price[0] != priceline:
        #price = self.db_cur.fetchone()
        #    print "new price=",price
        
        #self.cost = int(price[0])
        self.cost = int(price[0])
        print "COST: %s" % self.cost
        if self.tokencount >= self.cost:
            return True
        else:
            return False

    def finish(self, priceline):
        rejected = 0
        if self.cost != 2342:
            for token in self.tokenlist:
                if rejected < self.cost:
                    print "REJECT TOKEN: %s (%s)" % (token, rejected)
                    self.db.execute('UPDATE tokens SET used=1 WHERE hash=?', (token,))
                    rejected = rejected+1
         
        self.db.commit()
        self.tokenreset = True
        return True

if __name__ == '__main__':
    token = Token()
    print "-- Check --"
    token.check("dtkzoxcfxf")
    token.check("pqvszoxmwe")
    token.check("bwlfszyxqw")
    print "-- EOT --"
    crd = token.eot()
    print crd
    print "-- Assets --"
    ret = token.assets('1')
    print ret
    print "-- Finish --"
    token.finish(1)
