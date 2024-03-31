#!/usr/bin/python
# -*- coding:utf-8 -*-
#

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='stuff')
    parser.add_argument("--evtmax", type=int, default=1, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--input", default=["sample_muonrec.root"],nargs='+',  help="input file name list")
    parser.add_argument("--output", default="sample_muonrec_output.root", help="output file name")
    parser.add_argument("--userout", default="recOutUser.root", help="user output file name")

    parser.add_argument("--detoption", default="Acrylic",
                                       choices=["Acrylic", "Balloon"],
                                       help="Det Option")
    parser.add_argument("--gdml", action="store_true", help="Use GDML.")
    return parser

TOTALPMTS = {"Acrylic": 17739, "Balloon": 18306}
DEFAULT_GDML_OUTPUT = {"Acrylic": "geometry_acrylic.gdml",
                       "Balloon": "geometry_balloon.gdml"}

if __name__  ==  "__main__":
    parser = get_parser()
    args = parser.parse_args()

    total_pmt = TOTALPMTS[args.detoption]
    gdml_filename = "default"
    if args.gdml:
        gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]

    #Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(1)

    Sniper.loadDll("libCLHEPDict.so")

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    #bufMgr.property("TimeWindow").set([0, 0]);

    import RootIOSvc
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    roSvc.property("OutputStreams").set({
             "/Event/Sim":args.output, 
             "/Event/RecTrack": args.output})
    riSvc = task.createSvc("RootInputSvc/InputSvc")
    riSvc.property("InputFile").set(args.input)

    import Geometry
    geom = task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set(gdml_filename)
    geom_path_inroot = "JunoGeom"
    geom.property("GeomPathInRoot").set(geom_path_inroot)
    geom.property("FastInit").set(True)
    
    Sniper.loadDll("libPmtRec.so")
    task.property("algs").append("PullSimHeaderAlg")

    import WPChargeClusterRec
    wpalg = task.createAlg("WPChargeClusterRec")
    wpalg.setLogLevel(1)
    #wpalg.property("OutputPmtPos").set(True)

    task.setEvtMax(args.evtmax)
    task.show()
    task.run()
