#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Author: Tao Lin <lintao AT ihep.ac.cn>

import argparse
import Sniper

def get_parsers():
    parser = argparse.ArgumentParser(description="NuWro with Metropolis sampling in JUNO")
    
    parser.add_argument("--file", default="Default", help="NuWro input file path")
    parser.add_argument("-n", "--evtmax", type=int, default=50, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--run", type=int, default=1, help='run')
    
    return parser

if __name__ == "__main__":

    parser = get_parsers()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    # = Data Buffer Mgr =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)

    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    # rootwriter.property("Output").set({"GENEVT":"genie_user.root"})
    # = gen tools =
    import GenTools
    gt = task.createAlg("GenTools")

    Sniper.loadDll("libGenNuWro.so")

    nuwro_gen = gt.createTool("NuWroEvGen")

    # Energy
    nuwro_gen.property("NuWroConfigFile").set('/junofs/users/yanqy/juno-test/juno-1/ExternalLibs/nuwro/yan/data/params.txt')
    
    # register genie evgen
    gt.property("GenToolNames").set([nuwro_gen.objName()])
    task.run()

