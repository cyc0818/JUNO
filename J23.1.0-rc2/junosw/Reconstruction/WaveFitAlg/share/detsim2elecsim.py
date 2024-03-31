#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


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
    inputsvc.property("InputFile").set(["detsim_sample.root"])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

if __name__ == "__main__":

    Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    #task.setEvtMax(-1)
    task.setEvtMax(1)
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

    # = second task =
    sec_task = task.createTask("Task/detsimtask")
    #config_detsim_task(sec_task)
    config_detsim_task_nosim(sec_task)

    # = create alg in top task =
    #Sniper.loadDll("libTwoTask.so")
    #task.createAlg("AlgWithFire")
    # = Elec Sim =
    # = I/O Related =
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])
    # == Output Svc ==
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/Calib":"sample_calib.root",
                  "/Event/Elec":"sample_calib.root"}
    roSvc.property("OutputStreams").set(outputdata)
    # == Elec Sim ==
    Sniper.loadDll("libElecSim.so")
    elecsim = task.createAlg("ElecSimAlg")
    elecsim.property("enableOvershoot").set("true")
    elecsim.property("PmtTotal").set("17746")
    elecsim.property("split_evt_time").set("10000")
    elecsim.property("simFrequency").set("1e9")
    elecsim.property("noiseAmp").set("3.5e-3")
    elecsim.property("speAmp").set("0.010")
    elecsim.property("preTimeTolerance").set("300")
    elecsim.property("postTimeTolerance").set("2000")
    # == Wave Fit ==
    Sniper.loadDll("libWaveFit.so")
    wavefit = task.createAlg("WaveFitAlg")
    #wavefit.property("PmtTotal").set(17746);
    wavefit.property("PmtTotal").set(1);
    wavefit.property("OutFile").set("newjuno_test.root");
    # = begin run =
    task.show()
    task.run()
