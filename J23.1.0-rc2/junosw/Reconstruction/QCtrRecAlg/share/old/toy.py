#!/usr/bin/env python
# -*- coding:utf-8 -*-
#
#  Toy simulation data genertor
#  Author: Zhengyun You  2013-12-26
#

import sys
sys.setdlopenflags( 0x100 | 0x2 ) # RTLD_GLOBAL | RTLD_NOW

import libSniperMgr
import libSniperPython as sp

mgr = libSniperMgr.SniperMgr()
sp.setProperty("Sniper", "EvtMax", 10)
sp.setProperty("Sniper", "InputSvc", "NONE")
sp.setProperty("Sniper", "LogLevel", 2)
sp.setProperty("Sniper", "Dlls", ["QCtrRecAlg", "SimEvent", "Geometry", "TopAlg"])
mgr.configure()

svcMgr = sp.SvcMgr.instance()
algMgr = sp.AlgMgr.instance()

#service
#DataBuffer service
bufsvc=sp.SvcMgr.get("DataBufSvc", True)
svcMgr.add(bufsvc.name())
vp=["/Event/Sim/SimHeader", "/Topology/TopHeader"]
bufsvc.setProp("ValidPaths", vp)
op=["/Event/Sim/SimHeader"]
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

# begin to run
if mgr.initialize():
    mgr.run()
mgr.finalize()
