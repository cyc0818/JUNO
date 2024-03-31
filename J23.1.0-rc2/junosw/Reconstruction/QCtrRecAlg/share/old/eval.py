#!/usr/bin/env python
# -*- coding:utf-8 -*-
#
#  Config file to run rec evaluation
#  Author: Zhengyun You  2013-12-20
#

import sys
sys.setdlopenflags( 0x100 | 0x2 ) # RTLD_GLOBAL | RTLD_NOW

import libSniperMgr
import libSniperPython as sp

mgr = libSniperMgr.SniperMgr()
mgr.setProp("EvtMax", 10)
mgr.setProp("InputSvc", "NONE")
mgr.setProp("LogLevel", 2)  # Debug:2  Info:3  Warn:4  Error:5  Fatal:6
sp.setProperty("Sniper", "Dlls", ["QCtrRecAlg", "RecEvent", "SimEvent", "TopAlg"])

mgr.configure()

svcMgr = sp.SvcMgr.instance()
algMgr = sp.AlgMgr.instance()

#get the DataBuffer service
bufsvc=sp.SvcMgr.get("DataBufSvc", True)
svcMgr.add(bufsvc.name())

vp=["/Event/Sim/Test", "/Event/Rec/Test", "/Event/Ana/Test",
        "/Event/Rec/RecHeader", "/Event/Calib/CalibHeader",
        "/Event/Sim/SimHeader",
        "/Topology/TopHeader"]
bufsvc.setProp("ValidPaths", vp)
ip=["/Event/Rec/RecHeader", "/Event/Sim/SimHeader"]
bufsvc.setProp("InputItems", ip)

# IO service
sios=sp.SvcMgr.get("SniperIOSvc", True)
svcMgr.add(sios.name())
sios.setProp("InputFile",["recEvtOut.root"])

#RootWriter service
rw = sp.SvcMgr.get("RootWriter", True)
svcMgr.add(rw.name())
d = {"FILE1": "recEvtEval.root"}
rw.setProp("Output", d)

#algorithm
topalg = sp.AlgMgr.get("TopAlg", True)
algMgr.add(topalg.name())

evalalg = sp.AlgMgr.get("QCtrEvalAlg", True)
algMgr.add(evalalg.name())

# begin to run
if mgr.initialize():
    mgr.run()
mgr.finalize()
