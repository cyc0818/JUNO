#!/usr/bin/env python3
# -*- coding:utf-8 -*-

# parse arg
import sys
print(sys.argv)

def get_parser():
    import argparse
    parser = argparse.ArgumentParser(description='Run Solar Generator.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--type", default="B8", help="neutrino type", choices = ["pp", "Be7", "B8", "N13", "O15", "F17", "pep", "hep"])
    return parser


parser = get_parser()
args = parser.parse_args()

import Sniper
import NuSolGen

task = Sniper.Task("task")
task.setEvtMax(args.evtmax)
task.setLogLevel(1)

# = Data Buffer Mgr =
import RootIOSvc
import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
readout=task.createSvc("RootOutputSvc/OutputSvc")
outputdata={"/Event/Gen": "out.root"};
readout.property("OutputStreams").set(outputdata)

# = gen tools =
import GenTools
gt = task.createAlg("GenTools")

# create alg and tool
gun = gt.createTool("GtNuSolTool/nusol")
gun.property("inputMode").set("generateRealTime")
gun.property("neutrinoType").set(args.type)
gt.property("GenToolNames").set([gun.objName()])

task.show()
task.run()
