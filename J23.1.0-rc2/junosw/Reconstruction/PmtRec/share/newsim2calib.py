#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao
import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(10)
    task.setLogLevel(0)

    # Create Data Buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # Create IO Svc
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(["sample.root"])

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/Calib":"sample_calib.root",
                  "/Event/Sim": "sample_calib.root"}
    roSvc.property("OutputStreams").set(outputdata)

    #alg

    Sniper.loadDll("libPmtRec.so")
    #pullSimAlg = task.createAlg("PullSimHeaderAlg")
    task.property("algs").append("PullSimHeaderAlg")

    task.show()
    task.run()
