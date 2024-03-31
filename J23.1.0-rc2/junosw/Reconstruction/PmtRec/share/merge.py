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
    input_files = ["sample_detsim.root", "sample_detsim2.root"]
    sec_tasks = []
    for i, input_file in enumerate(input_files):
        sec_task_name = "detsimtask%d"%i
        sec_task = task.createTask("Task/%s"%sec_task_name)
        #config_detsim_task(sec_task)
        config_detsim_task_nosim(sec_task, input_file)
        sec_tasks.append(sec_task_name)

    # = create alg in top task =
    # = Split Event =
    # == RootIO ==
    import RootIOSvc
    ro = task.createSvc("RootOutputSvc/OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": "sample_split.root"})
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # == Merge ==
    Sniper.loadDll("libPmtRec.so")
    merge_sim = task.createAlg("MergeSimEventAlg")
    merge_sim.property("InputTasks").set(sec_tasks)

    # = begin run =
    task.show()
    task.run()
    
