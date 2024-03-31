#!/usr/bin/env python
#-*- coding: utf-8 -*-

from __future__ import print_function

import argparse

##############################################################################
# --help-more
##############################################################################

def MoreHelp():
    helpmore = False
    import argparse
    import sys
    if '--help-more' in sys.argv:
        helpmore = True

    def mh(helpstr):
        if helpmore:
            return helpstr
        return argparse.SUPPRESS
    return mh

mh = MoreHelp()

##############################################################################
# ThreeVector
##############################################################################

class MakeTVAction(argparse.Action):
    def __init__(self, option_strings, dest, nargs=None, **kwargs):
        #print "__init__ begin"
        #print option_strings 
        #print dest 
        #print nargs
        #print kwargs
        #print "__init__ end"
        super(MakeTVAction, self).__init__(option_strings, dest, nargs, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        #print "__call__ begin"
        #print parser
        #print namespace
        #print values
        # == convert a list into 3-tuple ==
        if len(values) % 3:
            print("please set %s like x1 y1 z1 x2 y2 z2 ..." %option_string)
            sys.exit(-1)
        it = iter(values)
        values = list(zip(*([it]*3)))
        setattr(namespace, self.dest, values)
        #print option_string
        #print "__call__ end"

