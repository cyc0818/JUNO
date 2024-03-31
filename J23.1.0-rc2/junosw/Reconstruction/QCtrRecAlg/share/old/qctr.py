#!/usr/bin/env python
# -*- coding:utf-8 -*-
#
#  Config file to run reconstruction with ToySim generator
#  Author: Zhengyun You  2013-11-20
#

import sys
sys.setdlopenflags( 0x100 | 0x2 ) # RTLD_GLOBAL | RTLD_NOW

import libSniperMgr
import libSniperPython as sp

mgr = libSniperMgr.SniperMgr()
sp.setProperty("Sniper", "EvtMax", 10)
sp.setProperty("Sniper", "InputSvc", "NONE")
sp.setProperty("Sniper", "LogLevel", 2)
sp.setProperty("Sniper", "Dlls", ["QCtrRecAlg", "RecEvent", "SimEvent", "Geometry", "TopAlg", "PmtRec"])
mgr.configure()

svcMgr = sp.SvcMgr.instance()
algMgr = sp.AlgMgr.instance()

#service
#DataBuffer service
bufsvc=sp.SvcMgr.get("DataBufSvc", True)
svcMgr.add(bufsvc.name())
vp=["/Event/Sim/Test", "/Event/Rec/Test", "/Event/Ana/Test",
        "/Event/Rec/RecHeader", "/Event/Calib/CalibHeader",
        "/Event/Sim/SimHeader",
        "/Topology/TopHeader"]
bufsvc.setProp("ValidPaths", vp)
op=["/Event/Rec/RecHeader",
        "/Event/Calib/CalibHeader",
        "/Event/Sim/SimHeader", ]
bufsvc.setProp("OutputItems", op)
ip=[]
bufsvc.setProp("InputItems", ip)

# IO service
sios=sp.SvcMgr.get("SniperIOSvc", True)
svcMgr.add(sios.name())
sios.setProp("OutputFile","recEvtOut.root")

#Geometry service
geom=sp.SvcMgr.get("RecGeomSvc", True)
svcMgr.add(geom.name())

#RootWriter service
rw = sp.SvcMgr.get("RootWriter", True)
svcMgr.add(rw.name())
d = {"FILE1": "toySim.root"}
rw.setProp("Output", d)

#algorithm
topalg = sp.AlgMgr.get("TopAlg", True)
algMgr.add(topalg.name())

toyalg = sp.AlgMgr.get("QCtrToySimAlg/toyalg", True)
algMgr.add(toyalg.name())

pullSimAlg = sp.AlgMgr.get("PullSimHeaderAlg", True)
algMgr.add(pullSimAlg.name())

qctralg = sp.AlgMgr.get("QCtrRecAlg/qctralg", True)
algMgr.add(qctralg.name())
qctralg.setProp("UsePerfectVtx", True)

# begin to run
if mgr.initialize():
    mgr.run()
mgr.finalize()
