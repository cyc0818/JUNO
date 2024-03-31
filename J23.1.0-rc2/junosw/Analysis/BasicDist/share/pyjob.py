#!/usr/bin/env python
# -*- coding: utf-8 -*-

#############################################################################
def get_parser():
    import argparse
    parser = argparse.ArgumentParser(description='Run Basic Distribution.')
    parser.add_argument("--evtmax", default=-1, type=int, help="total events")
    parser.add_argument("--inputs", default=["sample_detsim.root"], 
                                    nargs="+", help="input file names")
    parser.add_argument("--output", default="calib.root", help="output file name")
    parser.add_argument("-v", "--loglevel", default=6, type=int, choices = [0, 1, 2, 3, 4, 5, 6],
            help="log level. (0: test, 2: debug, 3: info, 4: warn, 5: error, 6: fatal)")
    parser.add_argument("--cut-min", default=1000, type=float, help="totalPE cut min")
    parser.add_argument("--cut-max", default=9000, type=float, help="totalPE cut max")
    parser.add_argument("--time-gap", default=300, type=float, help="time gap cut")

    parser.add_argument("--ibd-time-cut", default=800e3, type=float, help="ibd time cut (ns)")
    return parser
#############################################################################

parser = get_parser()
args = parser.parse_args()
print(args)

import Sniper

task = Sniper.Task("calibtask")
#task.setEvtMax(-1)
task.setEvtMax(args.evtmax)
task.setLogLevel(args.loglevel)

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")

import RootIOSvc
inputsvc = task.createSvc("RootInputSvc/InputSvc")
inputsvc.property("InputFile").set(args.inputs)

import RootWriter
rootwriter = task.createSvc("RootWriter")

rootwriter.property("Output").set({"CALIBEVT":args.output})

Sniper.loadDll("libBasicDist.so")
calibalg = task.createAlg("BasicDistAlg")
calibalg.property("SplitTimeGap").set(args.time_gap)
calibalg.property("TotalPECut").set([args.cut_min, args.cut_max])
calibalg.property("IBDTimeCut").set(args.ibd_time_cut)

task.show()
task.run()
