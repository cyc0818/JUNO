#!/usr/bin/env python
# -*- coding:utf-8 -*_

import argparse

parser = argparse.ArgumentParser(description="Run GenDecay.")
parser.add_argument("--nuclear", default="Th-232", help="mother nuclide name")
parser.add_argument("--stop-nuclear", default="", help="stop nuclide name")
parser.add_argument("--parent-abundance", default=5e16, type=float, help="abundance")
parser.add_argument("-t", "--correlation-time", default=280, type=float, help="correlation time (ns).")
parser.add_argument("-d", "--decay-depth", default=-1, type=int, help="decay depth")

args = parser.parse_args()

import Sniper
#-----------------------------------------------------------------
# Global Setting
#-----------------------------------------------------------------
task = Sniper.Task("task")
task.setEvtMax(10)
task.setLogLevel(0)

#-----------------------------------------------------------------
# Svc Related
#-----------------------------------------------------------------
# Data Buffer Svc
import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")

# Random service
import RandomSvc
task.property("svcs").append("RandomSvc")
rndm = task.find("RandomSvc")
rndm.property("Seed").set(42)

# = root writer =
import RootWriter
rootwriter = task.createSvc("RootWriter")

rootwriter.property("Output").set({"GENEVT":"genevt.root"})

#-----------------------------------------------------------------
# Tool Related
#-----------------------------------------------------------------

import GenTools
gt = task.createAlg("GenTools")
print gt

# == GenDecay ==
Sniper.loadDll("libGenDecay.so")
era = gt.createTool("GtDecayerator")
era.property("ParentNuclide").set(args.nuclear)
era.property("ParentAbundance").set(args.parent_abundance)
era.property("CorrelationTime").set(args.correlation_time)
era.property("DecayDepth").set(args.decay_depth)
era.property("StopNuclide").set(args.stop_nuclear)

# == GtTimeOffsetTool ==
toffset = gt.createTool("GtTimeOffsetTool")

gt.property("GenToolNames").set([era.objName(), toffset.objName()])

# pgt = task.createAlg("PostGenTools")

#-----------------------------------------------------------------
# Run
#-----------------------------------------------------------------
task.show()
task.run()
