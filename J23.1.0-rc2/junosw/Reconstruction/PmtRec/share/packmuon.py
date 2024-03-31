#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def config_detsim_task_nosim(task, input_file):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([input_file])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

if __name__ == "__main__":

    Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(-1)
    task.setLogLevel(0)

    # = second task =
    #input_files = ["sample_detsim.root"]
    input_file = "sample_detsim.root"
    sec_task_name = "detsimtask%d"%(0)
    sec_task = task.createTask("Task/%s"%sec_task_name)
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(sec_task, input_file)

    # = create alg in top task =
    # = Split Event =
    # == RootIO ==
    import RootIOSvc
    ro = task.createSvc("RootOutputSvc/OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": "sample_merge.root"})
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # == Merge ==
    Sniper.loadDll("libPmtRec.so")
    merge_sim = task.createAlg("PackSplitEventAlg")
    merge_sim.property("InputTask").set(sec_task_name)

    # = begin run =
    task.show()
    task.run()
    

