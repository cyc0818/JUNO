#!/usr/bin/python

import Sniper

task = Sniper.Task("task")
task.asTop()
task.setLogLevel(2)

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
bufMgr.property("TimeWindow").set([-0.1, 0.1]);

import RootIOSvc
riSvc = task.createSvc("RootInputSvc/InputSvc")
riSvc.property("InputFile").set(["output9.root"])
roSvc = task.createSvc("RootOutputSvc/OutputSvc")
roSvc.property("OutputStreams").set({"/Event/PhyEvent": "zo.root"})

import FirstAlg
task.property("algs").append("FirstAlg/alg_example")

task.setEvtMax(10)
task.show()
task.run()
