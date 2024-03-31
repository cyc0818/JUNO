#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(10)
    task.setLogLevel(0)

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(42)

    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    rootwriter.property("Output").set({"SIMEVT":"evt.root"})

    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")

    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["e+"])
    gun.property("particleMomentums").set([1.0])

    gt.property("GenToolNames").set([gun.objName()])

    # = geant4 related =
    # == G4Svc ==
    Sniper.loadDll("libG4Svc.so")
    g4svc = task.createSvc("G4Svc")
    print g4svc.objName()

    # == DetSimOptions ==
    Sniper.loadDll("libDetSimOptions.so")
    detsim0 = task.createSvc("DetSim0Svc")
    detsim0.property("AnaMgrList").set(["GenEvtInfoAnaMgr", "NormalAnaMgr"])
    print detsim0.objName()

    # == DetSimAlg ==
    Sniper.loadDll("libDetSimAlg.so")
    detsimalg = task.createAlg("DetSimAlg")
    detsimalg.property("DetFactory").set(detsim0.objName())
    detsimalg.property("RunMac").set("run.mac")
    
    # === Create Tools ===
    anamgr_normal = detsimalg.createTool("NormalAnaMgr")
    print anamgr_normal

    # = begin run =
    task.show()
    #task.run()
