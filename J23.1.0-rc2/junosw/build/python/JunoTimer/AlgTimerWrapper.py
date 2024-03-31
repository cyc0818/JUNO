#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

"""
This is a wrapper Alg to measure 'execute' time in a specific Algorithm
"""
import sys
# From PyCintex in ROOT
sys.setdlopenflags( 0x100 | 0x2 )    # RTLD_GLOBAL | RTLD_NOW

import libSniperPython as SP
import libJunoTimer

import ROOT
from array import array

class AlgTimerWrapper(SP.AlgBase):

    def __init__(self, name):
        SP.AlgBase.__init__(self, name)

        self.junoTimerSvc = None
        self.rootWriterSvc = None
        self.alg = None

        self.output = None
        self.treename = None
        self.rootfile = None
        self.tree = {}
        self.time_elapsed = array('f', [0.])

    def initialize(self):
        if self.alg is None:
            return False
        self.junoTimerSvc = SP.SvcMgr.get("JunoTimerSvc", False)
        if self.junoTimerSvc is None:
            return False
        self.t = self.junoTimerSvc.get(self.alg.name())

        # open file
        self.rootWriterSvc = SP.SvcMgr.get("RootWriter", False)

        # create histogram
        if not self.treename:
            self.treename = self.name() + "_timer"
        self.tree["time_vs_evt"] = ROOT.TTree(self.treename, "timer") 
        self.tree["time_vs_evt"].Branch("elapsed", self.time_elapsed, 'timer/F')

        if self.output:
            self.rootWriterSvc.attach(self.output, self.tree["time_vs_evt"])


        return self.alg.initialize()

    def execute(self):
        if self.alg is None:
            return False
        self.t.start()
        rc=self.alg.execute()
        self.t.stop()

        self.time_elapsed[0] = self.t.elapsed()
        self.tree["time_vs_evt"].Fill()
        return rc

    def finalize(self):
        if self.alg is None:
            return False
        rc = self.alg.finalize()
        return rc

if __name__ == "__main__":
    import libSniperMgr as SM
    mgr = SM.SniperMgr()
    mgr.setProp("EvtMax", 1000)
    mgr.setProp("InputSvc", "NONE")
    mgr.setProp("LogLevel", 0)
    mgr.setProp("Dlls", ["RootWriter", "JunoTimer"])

    svcMgr = SP.SvcMgr.instance()
    algMgr = SP.AlgMgr.instance()

    rw = SP.SvcMgr.get("RootWriter", True)
    rw.setProp("Output", {"FILE1":"hello.root"})
    svcMgr.add(rw.name())

    junoTimerSvc = SP.SvcMgr.get("JunoTimerSvc", True)
    print junoTimerSvc
    svcMgr.add(junoTimerSvc.name())

    dummyAlg = AlgTimerWrapper("AlgTimerWrapper")
    dummyAlg.output = "FILE1/some/path"
    print dummyAlg.name()
    algMgr.add(dummyAlg.name())

    import MemoryCheck
    cm = MemoryCheck.checkMemory("CheckMemory")
    cm.interval = 50
    algMgr.add(cm.name())
    dummyAlg.alg = cm

    if mgr.initialize():
        mgr.run()
    mgr.finalize()

