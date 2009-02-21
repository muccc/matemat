#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

import cashier
import sys

cashier = cashier.Cashier()

purse = open('/mnt/token/purse', 'r')
purselines = purse.readlines()

for token in purselines:
    cashier.checkToken(token)

cashier.checkCredit()

sys.exit(0)
