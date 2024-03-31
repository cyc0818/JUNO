#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: hxwang


import Sniper


def config_detsim_task(task):
    
    # = I/O Related =
    # = I/O =
    import RootIOSvc
    outsvc = task.createSvc("RootOutputSvc/OutputSvc")
    outsvc.property("OutputStreams").set({"/Event/Sim": "sample.root"})

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")

    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["gamma", "gamma", "e+"])
    gun.property("particleMomentums").set([1.173, 1.333, 1.0])
    gun.property("DirectionMode").set("Fix")
    gun.property("Directions").set([makeTV(0,0,-1), makeTV(0,0,-1), makeTV(0,0,-1)])
    gun.property("PositionMode").set("FixMany")
    gun.property("Positions").set([makeTV(0,0,15500), makeTV(0,0,15500), makeTV(0,0,-155)])

    gt.property("GenToolNames").set([gun.objName()])

    # = geant4 related =
    import DetSimOptions
    from DetSimOptions.ConfAcrylic import ConfAcrylic
    acrylic_conf = ConfAcrylic(task)
    acrylic_conf.configure()

def config_detsim_task_nosim(task):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(["sim.root"])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")


if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(10)
    task.setLogLevel(0)

    # Create Data Buffer Svc

    #create buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # Create IO Svc
    import RootIOSvc
    #inputsvc = task.createSvc("RootInputSvc/InputSvc")
    #inputsvc.property("InputFile").set(["sim.root"])
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    #outputdata = {"/Event/Calib":"sample_calib.root","/Event/Sim":"sample_calib.root"}
    roSvc.property("OutputStreams").set({"/Event/Calib":"calib.root"})


    #add algs
    sec_task = task.createTask("Task/detsimtask")
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(sec_task)


    #import ElecSim 
    Sniper.loadDll("libElecSim.so")
    elecSim = task.createAlg("ElecSimAlg/elecSim")
    elecSim.property("PmtTotal").set("17746")
    elecSim.property("split_evt_time").set("10000")
    elecSim.property("simFrequency").set("1e9")
    elecSim.property("noiseAmp").set("3.5e-3")
    elecSim.property("speAmp").set("0.010")
    elecSim.property("preTimeTolerance").set("300")
    elecSim.property("postTimeTolerance").set("2000")

    #import IntegralPmtRec 
    Sniper.loadDll("libIntegralPmtRec.so")
    intPmtRec=task.createAlg("IntegralPmtRec")
    intPmtRec.property("TotalPMT").set(17746)
    intPmtRec.property("GainFile").set("gain.root")
    intPmtRec.property("Threshold").set(0.25*0.0035) #1/4 PE


    task.show()
    task.run()


