#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao
# TT can no longer be used without ElecSim

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--loglevel", default="Info", 
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            help="Set the Log Level")
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--input", default="sample_detsim.root", help="input file name")
    parser.add_argument("--output", default="sample_calib.root", help="output file name")

    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon"],
                                       help="Det Option")

    # == Split Alg ==
    parser.add_argument("--split-gap-time", default=1000, 
                                        type=float,
                                        help="split gap time (ns)")
    parser.add_argument("--qe-scale", default=1.0, type=float, 
                                      help="QE scale in ElecSim")
    parser.add_argument("--qe-smear", dest="smearqe", action="store_true",
                                      help="Enable QE smear")
    parser.add_argument("--no-qe-smear", dest="smearqe", action="store_false",
                                      help="Disable QE smear")
    parser.set_defaults(smearqe=False)

    return parser

def config_detsim_task_nosim(task):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()

    task = Sniper.TopTask("calibtask")
    # task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)

    # Create Data Buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # Create IO Svc
    import RootIOSvc

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/CdLpmtCalib": args.output,
                  "/Event/CdSpmtCalib": args.output,
                  "/Event/WpCalib": args.output,
                  "/Event/TtCalib": args.output,
                  "/Event/Sim": args.output}
    roSvc.property("OutputStreams").set(outputdata)

    # == geometry service ==
    import Geometry
    tt_geom_svc = task.createSvc("TTGeomSvc")

    # == detsim task ==
    detsimtask = task.createTask("Task/detsimtask")
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(detsimtask)
    #alg

    Sniper.loadDll("libPmtRec.so")
    split_sim = task.createAlg("DummySplitByTimeAlg")
    split_sim.property("DetSimTaskName").set("detsimtask")
    split_sim.property("SplitTimeGap").set(args.split_gap_time)

    pullSimAlg = task.createAlg("PullSimHeaderAlg")
    pullSimAlg.property("QEScale").set(args.qe_scale)
    pullSimAlg.property("SmearQE").set(args.smearqe)
    if args.smearqe:
        import os
        elecroot = os.environ["ELECSIMROOT"]
        pmt_data = os.path.join(elecroot, "share", "PmtData.root")
        if not os.path.exists(pmt_data):
            print("PmtData: %s does not exist" %pmt_data)
            import sys
            sys.exit(-1)
        pullSimAlg.property("SmearQEFile").set(pmt_data)

    
    task.show()
    task.run()

