#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def config_detsim_task_nosim(task, input_files):
    # = I/O Related =

    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(input_files)

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

def get_parser():

    import argparse
    parser = argparse.ArgumentParser(description='Merge LED simulation data.')

    parser.add_argument("--evtmax", type=int, default=-1, help='events to be produced')
    parser.add_argument("--output", default="sample_merged.root", help="output file name")
    parser.add_argument("--stride", type=int, default=1, help='merge N events into 1 event.')
    parser.add_argument("--input", help="Input file name")
    return parser

if __name__ == "__main__":

    parser = get_parser()
    import sys
    args = parser.parse_args()
    print args


    # Sniper.setLogLevel(0)
    task = Sniper.TopTask("task")
    task.setEvtMax(args.evtmax)
    # task.setLogLevel(0)

    # = second task =
    #input_files = ["sample_detsim.root"]
    # input_file = "/junofs/users/huangx/production/J19v1r0-Pre3/Lazer/p10/photon_10000/detsim/detsim-2219.root"
    input_file = args.input
    sec_task_name = "detsimtask%d"%(0)
    sec_task = task.createTask("Task/%s"%sec_task_name)
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(sec_task, [input_file])

    # = create alg in top task =
    # = Split Event =
    # == RootIO ==
    import RootIOSvc
    ro = task.createSvc("RootOutputSvc/OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": args.output})
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # == Merge ==
    Sniper.loadDll("libPmtRec.so")
    merge_sim = task.createAlg("MergeSimEventAlg")
    merge_sim.property("InputTasks").set([sec_task_name]*args.stride)

    # = begin run =
    task.show()
    task.run()
    

