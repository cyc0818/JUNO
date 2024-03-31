#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

task = Sniper.Task("testtask")
task.asTop()
task.setEvtMax(1)

import RootWriter
rootwriter = task.createSvc("RootWriter")
rootwriter.property("Output").set({"SIMEVT": "params.root"})

Sniper.loadDll("libMCParamsSvc.so")

mssvc = None

backend = "DBI"

if backend == "File":
    mcsvc = task.createSvc("MCParamsFileSvc/MCParamsSvc")
elif backend == "Crest":
    mcsvc = task.createSvc("MCParamsCrestSvc/MCParamsSvc")
    mcsvc.property("URI").set("file://mc.json")
elif backend == "DBI":
    import DatabaseSvc
    dbsvc = task.createSvc("DatabaseSvc")
    dbsvc.property("Url").set("junodb1.ihep.ac.cn")
    dbsvc.property("User").set("juno")
    dbsvc.property("Password").set("jiangmen")

    mcsvc = task.createSvc("MCParamsDBSvc/MCParamsSvc")
    mcsvc.property("URI").set("file://mc.json")


Sniper.loadDll("libTestMCParamsSvc.so")
mcalg = task.createAlg("TestAlg")

task.show()
task.run()
