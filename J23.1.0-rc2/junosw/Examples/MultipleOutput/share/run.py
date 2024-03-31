#!/usr/bin/env python
#-*- coding: utf-8 -*-

import Sniper
import RootIOSvc
import BufferMemMgr

task_top = Sniper.TopTask("task_top")
task_top.setEvtMax(10)
task_top.setLogLevel(0)

# two tasks
def create_task(name):
    task = task_top.createTask("Task/"+name)
    buffmgr = task.createSvc("BufferMemMgr")
    outsvc = task.createSvc("RootOutputSvc/OutputSvc")
    outsvc.property("OutputStreams").set({"/Event/Sim%s"%name: "output-%s.root"%name})

create_task("task0")
create_task("task1")

# Testing alg
Sniper.loadDll("libMultipleOutput.so")
testingalg = task_top.createAlg("TestingAlg")

task_top.run()

