#!/usr/bin/env python
#-*- coding: utf-8 -*-

# Description:
#   Pack all data into a string.
# Support format:
#   - base64

# The supported data structs:
#   - dict: key and value
# The original formats example (before encoding):
# 
# # comment line
# ----- key -----
# CONENT HERE, MAYBE MULTIPLE LINES
# ..... key .....
#
# ----- light_yield -----
# # unit: /MeV
# 10000
# ..... light_yield .....
#
# ----- refractive -----
# # energy (eV), index 
# 
# ..... refractive .....
#

def packit_begin(k):
    return "\n----- %s -----\n"%k

def packit_content(v):
    return v

def packit_end(k):
    return "\n..... %s .....\n"%k

def packit(k, v):
    s = ""
    s += packit_begin(k)
    s += packit_content(v)
    s += packit_end(k)

    return s

import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--encode", choices=["plain", "base64"],
                    help="Encode type")
parser.add_argument("--kv", action="append",
                    help="add a key value pair: light_yield:10000")
parser.add_argument("--kf", action="append",
                    help="add a key value(from file): refractive:/my/file")

args = parser.parse_args()
# print(args)

kv_list = args.kv if args.kv else []
kf_list = args.kf if args.kf else []

s = ""

for kv in kv_list:
    k,v = kv.split(":",1)
    # print k,v
    s += packit(k,v)

for kf in kf_list:
    k,f = kf.split(":",1)
    # print k,f
    s += packit(k, open(f).read())

if args.encode == "base64":
    import base64
    print base64.b64encode(s)
else:
    print s
