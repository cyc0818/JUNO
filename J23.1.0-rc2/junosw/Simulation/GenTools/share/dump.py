#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

# Load the GenHeader and GenEvent from ROOT files.


import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.setEvtMax(-1)
    task.setLogLevel(0)

    # = Data Buffer Mgr =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = input svc =
    import RootIOSvc

    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set("sample_detsim.root")

    outputsvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputsvc.property("OutputStreams").set({"/Event/Gen": "sample_gen.root"})


    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    rootwriter.property("Output").set({"GENEVT":"genevt.root"})


    # = GenTools =
    import GenTools

    gt = task.createAlg("PostGenTools")

    task.show()
    task.run()
