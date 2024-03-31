#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import sys

# From PyCintex in ROOT
sys.setdlopenflags( 0x100 | 0x2 )    # RTLD_GLOBAL | RTLD_NOW
import os
import libSniperMgr
import libSniperPython as sp

from FlattenAlg import FlattenAlg

class Sim2CalibApp(object):

    def __init__(self, jobname):
        self.jobname = jobname
        self.evtmax = 1
        self.loglevel = 0

        self.input_file = "/publicfs/dyb/data/userdata/chengyp/dyb2_v2/ep/less/water/close_positron_10031.root"

        self.pmt_data_file = os.path.join(os.environ["ELECSIMROOT"],
                                            "share", "PmtData.root")
        self.out_file = os.path.join(os.environ["WAVEFITALGROOT"],
                                            "share", "test_output.root")

        self.dlls = ["DetSimResultCnv", "ElecSim", "WaveFit", "BufWithIO"]

    def run(self):

        mgr = libSniperMgr.SniperMgr()
        mgr.setProp("EvtMax", self.evtmax)
        mgr.setProp("InputSvc", "NONE")
        mgr.setProp("LogLevel", self.loglevel)
        mgr.setProp("Dlls", self.dlls)
        mgr.configure()
        # init svcs
        self.init_svcs()
        # init algs
        self.init_algs()

        if mgr.initialize():
            mgr.run()
        mgr.finalize()

    def init_svcs(self):
        self.svcMgr = sp.SvcMgr.instance()
        self.init_svc_buffer()
        self.init_svc_io()

    def init_svc_buffer(self):
        # Create Data Buffer Svc
        bufsvc=sp.SvcMgr.get("DataBufSvcV2", True)
        self.svcMgr.add(bufsvc.name())

        vp = ["/Topology/TopHeader",
              "/Event/Sim/SimHeader",
              "/Event/Sim/ElecFeeCrate",
              "/Event/Calib/CalibHeader"]
        bufsvc.setProp("ValidPaths", vp)

    def init_svc_io(self):
        # Create IO Svc
        sios=sp.SvcMgr.get("SniperIOSvc", True)
        self.svcMgr.add(sios.name())

    def init_algs(self):
        self.algMgr = sp.AlgMgr.instance()
        self.init_alg_topalg()
        self.init_alg_flatten_t2s()
        self.init_alg_flatten_s2e()
        self.init_alg_flatten()

        self.init_alg_e2c()

    def init_alg_topalg(self):
        topalg = sp.AlgMgr.get("TopAlgV2",True)
        self.algMgr.add(topalg.name())

    def init_alg_flatten_t2s(self):
        self.t2s = sp.AlgMgr.get("TupleToSimHeaderV2",True)
        self.t2s.setProp("InputRootFile", self.input_file)
        self.t2s.setProp("InputTreeName", "evt")

    def init_alg_flatten_s2e(self):
        self.s2e = sp.AlgMgr.get("ElecSimAlg", True)
        self.s2e.setProp("PmtDataFile", self.pmt_data_file)

    def init_alg_flatten(self):
        self.flattenalg = FlattenAlg("FlattenAlg")
        self.algMgr.reg(self.flattenalg)
        self.algMgr.add(self.flattenalg.name())

        self.flattenalg.algs = [self.t2s, self.s2e]

    def init_alg_e2c(self):
        e2c = sp.AlgMgr.get("WaveFitAlg", True)
        e2c.setProp("OutFile", self.out_file)
        self.algMgr.add(e2c.name())


if __name__ == "__main__":

    sim2calib = Sim2CalibApp("test-app")
    sim2calib.loglevel = 3
    sim2calib.run()
