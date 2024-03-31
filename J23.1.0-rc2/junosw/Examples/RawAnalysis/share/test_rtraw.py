#!/usr/bin/python
# -*- coding:utf-8 -*-

if __name__ == "__main__":

    import Sniper
    Sniper.setLogLevel(3)

    task = Sniper.Task("task")

    import RootIOSvc
    iSvc = task.createSvc("RootInputSvc/InputSvc")
    iSvc.property("InputFile").set(["events_edm.root"])
    oSvc = task.createSvc("RootOutputSvc/OutputSvc")
    oSvc.property("OutputStreams").set({"/Event/CdLpmtCalib":"events_rec.root"})

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0]);

    import RawAnalysis
    alg = task.createAlg("ElecEvtAnalysis")

    ## run waveform reconstruction
    import Geometry
    PMTParamSvc = task.createSvc("PMTParamSvc")
    import PMTCalibSvc
    PMTCalibSvc = task.createSvc("PMTCalibSvc")
    Sniper.loadDll("libDeconvolution.so")
    decon=task.createAlg("Deconvolution")
    #decon.property("TotalPMT").set(17746)
    #decon.property("Length").set(1250)
    #decon.property("Para1").set(50)
    #decon.property("Para2").set(120)
    #decon.property("Para3").set(30)
    #decon.property("Window").set(3)
    #decon.property("HitCounting").set(0)

    import RootWriter
    task.property("svcs").append("RootWriter")
    rw = task.find("RootWriter")
    rw.property("Output").set({
        "waveform": "tree_from_edm.root"
    })

    task.setEvtMax(10)
    task.run()
