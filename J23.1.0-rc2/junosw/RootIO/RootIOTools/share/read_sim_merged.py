#!/usr/bin/python

import Sniper
task = Sniper.TopTask("task")
task.setLogLevel(0)

import FirstAlg
alg = task.createAlg("ReadSimAlg/alg_example")

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")

import RootIOTools
riSvc = task.createSvc("MergingInputSvc/InputSvc")
riSvc.property("MergingEventType").set("SimEvent")
riSvc.property("MergingEventPath").set("/Event/Sim")
riSvc.property("InputFile").set(["SimEvent.root"])

task.setEvtMax(-1)
task.show()
task.run()
