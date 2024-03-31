#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=1, help='events to be processed')
    parser.add_argument("--input", default="input.root", help="input file name")
    parser.add_argument("--output", default="sample_rec.root", help="output file name")

    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon"],
                                       help="Detector Option")
    parser.add_argument("--gdml", action="store_true", help="Use GDML.")
    return parser

TOTALPMTS = {"Acrylic": 17746, "Balloon": 18306}
DEFAULT_GDML_OUTPUT = {"Acrylic": "geometry_acrylic.gdml", 
                       "Balloon": "geometry_balloon.gdml"}

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()

    total_pmt = TOTALPMTS[args.detoption]
    gdml_filename = "default"
    #if args.gdml:
    gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]

    task = Sniper.Task("rectask")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    # == Create Data Buffer Svc ==

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # == Create IO Svc ==
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    # === load dict ===
    Sniper.loadDll("libSimEventV2.so")
    Sniper.loadDll("libCalibEvent.so")
    Sniper.loadDll("libRecEvent.so")
    Sniper.loadDll("libCLHEPDict.so")
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/Rec": args.output,
                  "/Event/Calib": args.output,
                  #"/Event/Sim": args.output
                  }
    roSvc.property("OutputStreams").set(outputdata)

    # == Geometry Svc ==
    import Geometry
    geom = task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set(gdml_filename)

#-------------------------------alg-------------------------------
    # == PullSimHeaderAlg ==
    Sniper.loadDll("libPmtRec.so")
    task.property("algs").append("PullSimHeaderAlg")    

    # == RecTimeLikeAlg ==
    import PushAndPullAlg
    import os
    alg = task.createAlg("PushAndPullAlg")
    alg.property("Det_Type").set(args.detoption)
    alg.setLogLevel(2)

    task.show()
    task.run()
