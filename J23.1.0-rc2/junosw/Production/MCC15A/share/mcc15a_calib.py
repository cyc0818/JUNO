#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper
import MCC15A.Calib

def config_detsim_task_nosim(task):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

if __name__ == "__main__":
    parser = MCC15A.Calib.get_parser()
    args = parser.parse_args()
    print args

    # = top level task =
    task = Sniper.Task("calibtask")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(3)
    # = I/O Related =
    # == data registration ==
    # == RootIOSvc == 
    import RootIOSvc
    ro = task.createSvc("RootOutputSvc/OutputSvc")
    ro.property("OutputStreams").set(
            {"/Event/Sim": args.sim_output,
            "/Event/Calib": args.calib_output,
            })
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # = Random Svc =
    import RandomSvc
    rndm = task.createSvc("RandomSvc")
    rndm.property("Seed").set(args.seed)
    # = JobInfoSvc =
    import JobInfoSvc
    jobinfosvc = task.createSvc("JobInfoSvc")
    jobinfosvc.property("OfflineVersion").set("MCC15A")
    import sys
    cmdhist = " ".join("\"%s\""%a for a in sys.argv)
    jobinfosvc.property("CommandLine").set(cmdhist)

    # == detsim task ==
    detsimtask = task.createTask("Task/detsimtask")
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(detsimtask)

    # == Split and PMT REC ==
    Sniper.loadDll("libPmtRec.so")
    split_sim = task.createAlg("DummySplitByTimeAlg")
    split_sim.property("DetSimTaskName").set("detsimtask")
    split_sim.property("SplitTimeGap").set(args.split_gap_time)

    cnv_to_calib = task.createAlg("PullSimHeaderAlg")
    cnv_to_calib.property("QEScale").set(1.0)
    cnv_to_calib.property("SmearQE").set(args.smearqe)
    import os
    elecroot = os.environ["ELECSIMROOT"]
    pmt_data = os.path.join(elecroot, "share", "PmtData.root")
    if args.smear_file:
        pmt_data = args.smear_file
    if not os.path.exists(pmt_data):
        print "PmtData: %s does not exist" %pmt_data
        import sys
        sys.exit(-1)
    cnv_to_calib.property("SmearQEFile").set(pmt_data)
    # = begin run =
    task.show()
    task.run()
