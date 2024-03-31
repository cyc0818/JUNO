#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os.path
import Sniper

task = Sniper.Task("testtask")
#task.asTop()

import CondDB

conddbsvc = task.createSvc("CondDBSvc")

conddbsvc.property("RepoTypes").set({
    "frontier": "Frontier"
})
conddbsvc.property("RepoURIs").set({
#    "local": os.path.join("dummy-repo"),
    "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"
})
conddbsvc.property("GlobalTag").set(
    "gtname"
)

Sniper.loadDll("libCondDBExample.so")
testalg = task.createAlg("TestingCondDBAlg")

task.setEvtMax(5)
task.show()
task.run()
