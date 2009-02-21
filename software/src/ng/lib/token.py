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
        self.tokenreset = False
        self.db = sqlite3.connect('token.db')
        self.db_cur = self.db.cursor()
        self.tokenlist = []

    def add(self, token):
        return True 
    
    def check(self, token):
        if self.tokenreset:
            self.tokenreset = False
            self.tokencount = 0 

        if self.db_cur.execute('SELECT hash FROM tokens WHERE used=0 AND hash=%s' % token).fetchone():
            self.tokencount = self.tokencount+1
            self.tokenlist.append(token)
            return True
        else:
            return False

    def eot(self):
        self.tokenreset = True
        return self.tokencount
            
    def assets(self, priceline):
        cost = 0
        self.db_cur.execute('SELECT * FROM pricelines')
        price = self.db_cur.fetchone()
        while price:
            if price[1] == priceline:
                cost = price[0]
                break
            else:
                price = self.db_cur.fetchone()
                cost = 2342

        if (self.tokencount*0.5) >= cost:
            return True
        else:
            return False

    def finish(self):
         for token in self.tokenlist:
             print token
             self.db.execute('UPDATE tokens SET used=1 WHERE hash=%s' % token)
         
         self.db.commit()
         return True

if __name__ == '__main__':
    token = Token()
    token.check("123")
    crd = token.eot()
    print crd
    ret = token.assets('1')
    print ret
    token.finish()
