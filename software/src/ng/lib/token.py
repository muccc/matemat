#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

class Token:
    def __init__(self):
        return None

    def add(self, token):
        return True 
    
    def check(self, token):
        return True

    def eot(self, token):
        return True

if __name__ == '__main__':
    token = Token()
    token.add("foo")
    token.check("foo")
    token.eot("foo")
