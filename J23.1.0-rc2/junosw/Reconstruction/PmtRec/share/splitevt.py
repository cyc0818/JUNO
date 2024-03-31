#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def config_detsim_task_nosim(task):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(["sample_detsim.root"])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

if __name__ == "__main__":

    Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(-1)
    task.setLogLevel(0)

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(42)

    # = second task =
    sec_task = task.createTask("Task/detsimtask")
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(sec_task)

    # = create alg in top task =
    # = Split Event =
    # == RootIO ==
    import RootIOSvc
    ro = task.createSvc("RootOutputSvc/OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": "sample_split.root"})
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # == Elec Sim ==
    Sniper.loadDll("libPmtRec.so")
    split_sim = task.createAlg("DummySplitByTimeAlg")
    split_sim.property("DetSimTaskName").set("detsimtask")
    split_sim.property("SplitTimeGap").set(10000)
    # = begin run =
    task.show()
    task.run()
