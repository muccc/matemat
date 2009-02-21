#!/usr/bin/python

# $Id$
# ----------------------------------------------------------------------------
# "THE MATE-WARE LICENSE"
# codec <codec@muc.ccc.de> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a mate in return.
# ----------------------------------------------------------------------------

import logging

class Logger:
    def __init__(self, logname):
        self.logger = logging.getLogger(logname)
        self.logger.setLevel(logging.DEBUG)

        lh = logging.FileHandler("/tmp/consumr.log")
        lh.setLevel(logging.DEBUG)

        ch = logging.StreamHandler()
        ch.setLevel(logging.DEBUG)

        formatter = logging.Formatter("%(asctime)s [%(name)s]: %(levelname)s: %(message)s")
        lh.setFormatter(formatter)
        ch.setFormatter(formatter)

        self.logger.addHandler(lh)
        self.logger.addHandler(ch)

        self.logger.debug("Logger::__init__ %s" % logname)

    def log(self, record):
        self.logger.info(record)

    def debug(self, record):
        self.logger.debug(record)

    def error(self, record):
        self.logger.error(record)


if __name__ == '__main__':
    logger = Logger('selftest')
    logger.log("log()")
    logger.debug("debug()")
    logger.error("error()")
