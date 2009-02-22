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

        self.lh = logging.FileHandler("/tmp/consumr.log")
        self.lh.setLevel(logging.DEBUG)

        self.ch = logging.StreamHandler()
        self.ch.setLevel(logging.DEBUG)

        formatter = logging.Formatter("%(asctime)s [%(name)s]: %(levelname)s: %(message)s")
        self.lh.setFormatter(formatter)
        self.ch.setFormatter(formatter)

        self.logger.addHandler(self.lh)
        self.logger.addHandler(self.ch)

        self.logger.debug("Logger::__init__ %s" % logname)

    def info(self, record):
        self.logger.info(record)

    def debug(self, record):
        self.logger.debug(record)

    def error(self, record):
        self.logger.error(record)

    def level(self, level):
        self.logger.debug("Logger::level() => %s" % level)
        if level == 'DEBUG': level = logging.DEBUG
        elif level == 'INFO': level = logging.INFO
        elif level == 'ERROR': level = logging.ERROR
        elif level == 'CRITICAL': level = logging.CRITICAL
        self.lh.setLevel(level)
        self.ch.setLevel(level)

if __name__ == '__main__':
    logger = Logger('selftest')
    logger.level('CRITICAL')
    logger.log("log()")
    logger.debug("debug()")
    logger.error("error()")
