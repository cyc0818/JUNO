#!/usr/bin/env python
#-*- coding: utf-8 -*-
import sys
import Sniper
import TestSpmtElecAlg

# Sniper.setLogLevel(2)
# = main task =
task_top = Sniper.Task("task_top")
task_top.asTop()
task_top.setLogLevel(2)

# buffer management and input
import BufferMemMgr
buffermgr = task_top.createSvc("BufferMemMgr")

# 
import RootIOSvc
input_svc = task_top.createSvc("RootInputSvc/InputSvc")
input_svc.property("InputFile").set([sys.argv[1]])

TestAlg = task_top.createAlg("TestSpmtElecAlg")
TestAlg.property("ShowEvent").set(-1);
TestAlg.property("ShowLPMT").set(False);
TestAlg.property("OutFile").set(sys.argv[2])
TestAlg.setLogLevel(1)

task_top.setEvtMax(-1)
task_top.show()
task_top.run()
