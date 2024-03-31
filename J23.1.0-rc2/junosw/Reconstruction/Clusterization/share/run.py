#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: kampmann

import Sniper

if __name__=="__main__":
    

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(100)
    task.setLogLevel(0)

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # Create IO Svc
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(["sample_calib.root"])

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/Calib":"sample_cluster.root",
                  "/Event/Cluster": "sample_cluster.root"}
    roSvc.property("OutputStreams").set(outputdata)

    #alg

    #Sniper.loadDll("libPmtRec.so")
    Sniper.loadDll("libClusterization.so")
    #pullSimAlg = task.createAlg("PullSimHeaderAlg")
    task.property("algs").append("ClusterizationAlg")

    task.show()
    task.run()

