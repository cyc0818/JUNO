#!/usr/bin/env python
# -*- coding:utf-8 -*-
#
#  Config file to run reconstruction
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
sp.setProperty("Sniper", "Dlls", ["QCtrRecAlg", "RecEvent", "SimEvent", "Geometry", "TopAlg", "DetSimResultCnv", "PmtRec"])

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
#sios.setProp("InputFile",["input.root"])
sios.setProp("OutputFile","recEvtOut.root")

#Geometry service
geom=sp.SvcMgr.get("RecGeomSvc", True)
svcMgr.add(geom.name())
geom.setProp("GeomFile", "sim_old.root");
#To use default geometry in CdGeom/share/
#geom.setProp("GeomFile", "default");

#algorithm
topalg = sp.AlgMgr.get("TopAlg", True)
algMgr.add(topalg.name())

x = sp.AlgMgr.get("TupleToSimHeader/x",True)
x.setProp("InputRootFile", "sim_old.root")
x.setProp("InputTreeName", "evt")
algMgr.add(x.name())

pullSimAlg = sp.AlgMgr.get("PullSimHeaderAlg", True)
algMgr.add(pullSimAlg.name())

qctralg = sp.AlgMgr.get("QCtrRecAlg/qctralg", True)
algMgr.add(qctralg.name())

# begin to run
if mgr.initialize():
    mgr.run()
mgr.finalize()
