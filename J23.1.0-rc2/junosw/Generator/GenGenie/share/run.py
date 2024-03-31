#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Author: Tao Lin <lintao AT ihep.ac.cn>

import argparse
import Sniper

def get_parsers():
    parser = argparse.ArgumentParser(description="GENIE gEvGen in SNiPER")
    # here, we keep the similar interface to GENIE
    # below is an example from Jie Cheng:
    #
    #gevgen -n 120000
    #       -p 16
    #       -t 1000060120[0.87924],1000010010[0.1201],1000080160[0.00034],1000070140[0.00027],1000160320[0.00005]
    #       -e 0.1,20.
    #       --run 100000101
    #       --seed 154748252
    #       -f /junofs/users/junoprotondecay/chengj/atm-gen/atmflux/juno-nutau-numu.data
    #       --cross-sections /junofs/users/chengjie/workdir/atmgen-202203/data/xsec/genie_xsec/v3_00_06/NULL/G1810b0211a-k250-e1000/data/gxspl-FNALsmall.xml
    #       --tune G18_10b_02_11a

    parser.add_argument("-n", "--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--run", type=int, default=42, help='run')

    parser.add_argument("-e", "--energy", help='E or E,range')
    parser.add_argument("-p", "--pdg", type=int, help="pdgcode of neutrino")
    parser.add_argument("-t", "--targets", help="pdg1[weight1],pdg2[weight2],...")
    parser.add_argument("-f", "--flux", help="flux")
    parser.add_argument("--cross-sections", help="cross section")
    parser.add_argument("--tune", default="Default", help="genie tune.")
    parser.add_argument("--output", default="sample.root", help="output file name, including the EDM")
    parser.add_argument("--user-output", default="user_sample.root", help="output file name, including the user tree")

    return parser

if __name__ == "__main__":

    parser = get_parsers()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    # = Data Buffer Mgr =
    import RootIOSvc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    readout=task.createSvc("RootOutputSvc/OutputSvc")
    outputdata={"/Event/Gen": args.output}
    readout.property("OutputStreams").set(outputdata)

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)

    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    # rootwriter.property("Output").set({"GENEVT":"genie_user.root"})
    rootwriter.property("Output").set({"SIMEVT": args.user_output})
    # = gen tools =
    import GenTools
    gt = task.createAlg("GenTools")

    Sniper.loadDll("libGenGenie.so")

    genie_evgen = gt.createTool("GtGenieEvGenTool")

    # Energy
    energy = 42
    energy_range = 1.
    energy_list = args.energy.split(",")
    if len(energy_list) == 1:
        energy = float(energy_list[0])
        energy_range = -1
    elif len(energy_list) == 2:
        energy = float(energy_list[0])
        energy_range = float(energy_list[1])
    genie_evgen.property("NuEnergy").set(energy)
    genie_evgen.property("NuEnergyRange").set(energy_range)

    genie_evgen.property("NuPdgCode").set(args.pdg)

    # Target
    target_mix = {}
    for pw in args.targets.split(","):
        i1 = pw.find("[")
        i2 = pw.find("]")
        p = int(pw[:i1])
        w = float(pw[i1+1:i2])
        target_mix[p] = w
    genie_evgen.property("TgtMix").set(target_mix)

    genie_evgen.property("Flux").set(args.flux)
    genie_evgen.property("RanSeed").set(args.seed)
    genie_evgen.property("InpXSecFile").set(args.cross_sections)

    genie_evgen.property("Tune").set(args.tune)

    # register genie evgen
    gt.property("GenToolNames").set([genie_evgen.objName()])


    task.run()
