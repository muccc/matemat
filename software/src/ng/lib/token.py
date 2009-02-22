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
import hashlib
import logger
import sys

class Token:
    def __init__(self):
        self.tokencount = 0
        self.cost = 0
        self.tokenreset = False
        self.db = sqlite3.connect('token.db')
        self.db_cur = self.db.cursor()
        self.tokenlist = []
        self.log = logger.Logger('Token')
        self.log.debug('__init__(): invoked')

    def add(self, token):
        return True 
    
    def hash(self, token):
        self.log.debug('hash(): invoked')
        self.log.log('hash(): token=%s' % token)
        self.hashlib = hashlib.sha512()
        self.hashlib.update(token)
        self.log.log('hash(): return=%s' % self.hashlib.hexdigest())
        return self.hashlib.hexdigest()

    def check(self, token):
        self.log.debug('check(): invoked')
        if self.tokenreset:
            self.log.log('check(): Resetting in-memory token data')

            self.tokenreset = False
            self.tokencount = 0
            self.tokenlist = []
        
        hashtoken = self.hash(token)

        self.db_cur.execute('SELECT hash FROM tokens WHERE used=0 AND hash=? LIMIT 1', (hashtoken,))
        if self.db_cur.fetchone() == None:
            self.log.log('check(): %s is unused' % token)
            # print "FOUND UNUSED TOKEN: %s" % token
            self.tokencount = self.tokencount+1
            self.tokenlist.append(token)
            return True
        else:
            self.log.log('check(): %s is used' % token)
            return False

    def eot(self):
        self.log.debug('eot(): invoked')
        self.log.log('eot(): Number of valid tokens: %s' % self.tokencount)
        return self.tokencount
            
    def assets(self, priceline):
        self.log.debug('assets(): invoked')
        self.cost = 0
        #print "translating priceline"
        self.db_cur.execute('SELECT price FROM pricelines WHERE priceline=?', (priceline,))
        #print "executed query"
        price = self.db_cur.fetchone()
        #print "price=",price
        self.log.log('assets(): price=%s' % price)
        #while price[0] != priceline:
        #price = self.db_cur.fetchone()
        #    print "new price=",price
        
        #self.cost = int(price[0])
        self.cost = int(price[0])
        self.log.log('assets(): cost=%s' % self.cost)
        #print "COST: %s" % self.cost
        if self.tokencount >= self.cost:
            self.log.log('assets(): Liquidity is given')
            return True
        else:
            self.log.log('assets(): Liquidity is not given')
            return False

    def finish(self, priceline):
        self.log.debug('finish(): invoked')
        rejected = 0
        if self.cost != 2342:
            for token in self.tokenlist:
                if rejected < self.cost:
                    self.log.log('finish(): Rejecting %s' % token)
                    hashtoken = self.hash(token)
                    # print "REJECT TOKEN: %s (%s)" % (token, rejected)
                    self.db.execute('UPDATE tokens SET used=1 WHERE hash=?', (hashtoken,))
                    rejected = rejected+1
         
        self.db.commit()
        self.tokenreset = True
        return True

if __name__ == '__main__':
    token = Token()
    token.hash("ppsqmxjdgu")
    token.hash("ppsqmxjdgu")
    token.hash("ppsqmxjdgu")
    token.hash("ppsqmxjdgu")
    sys.exit(0)
    token.check("frnzjbcns")
    crd = token.eot()
    ret = token.assets('1')
    token.finish(1)
