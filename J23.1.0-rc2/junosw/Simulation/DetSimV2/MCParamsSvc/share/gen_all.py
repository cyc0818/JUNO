#!/usr/bin/env python
#-*- coding: utf-8 -*-
# author: Tao Lin

# In this script, we try to encode material properties using base64

import os.path
import base64

all_objects = [
    # object type, "streamer info"
    ("Material.LS.RINDEX", "vec_d2d"),
    ("Material.LS.ABSLENGTH", "vec_d2d"),
    ("Material.LS.ABSLENGTHwithUnits", "vec_d2d"),
    ("Material.LS.FASTCOMPONENT", "vec_d2d"),
    ("Material.LS.REEMISSIONPROB", "vec_d2d"),
    ("Material.LS.RAYLEIGH", "vec_d2d"),
    ("Material.LS.SCINTILLATIONYIELD", "vec_d2d"),
    ("Material.LS.RESOLUTIONSCALE", "vec_d2d"),
    ("Material.LS.GammaFASTTIMECONSTANT", "vec_d2d"),
    ("Material.LS.GammaSLOWTIMECONSTANT", "vec_d2d"),
    ("Material.LS.GammaYIELDRATIO", "vec_d2d"),
    ("Material.LS.AlphaFASTTIMECONSTANT", "vec_d2d"),
    ("Material.LS.AlphaSLOWTIMECONSTANT", "vec_d2d"),
    ("Material.LS.AlphaYIELDRATIO", "vec_d2d"),
    ("Material.LS.NeutronFASTTIMECONSTANT", "vec_d2d"),
    ("Material.LS.NeutronSLOWTIMECONSTANT", "vec_d2d"),
    ("Material.LS.NeutronYIELDRATIO", "vec_d2d"),
    ("Material.LS.scale", "vec_s2d"),
    ("Material.LS.scale", "map_s2d"),
]

TOPDIR = os.environ.get("JUNOTOP")
if not TOPDIR:
    print("Please specify JUNOTOP")

TOPDIR = os.path.join(TOPDIR, "data/Simulation/DetSim")

def load(name):
    path = os.path.join(TOPDIR, name.replace(".", "/"))
    content = ""
    with open(path) as f:
        content = base64.b64encode(f.read())
    return content

output_lists = []

for name, info in all_objects:
    strhash = "abcd"
    data = load(name)

    output = """{
    "hash": "%(strhash)s",
    "version": "trunk",
    "objectType": "%(name)s",
    "data": "%(data)s",
    "streamerInfo": "%(info)s",
    "insertionTime": ""
}"""%locals()
    output_lists.append(output)

print "["
print ",".join(output_lists)
print "]"
