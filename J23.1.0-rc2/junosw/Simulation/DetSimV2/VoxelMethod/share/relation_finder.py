#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():
    import argparse
    parser = argparse.ArgumentParser(description='Run Analysis.')
    parser.add_argument("--evtmax", type=int, default=10, 
                            help='events to be processed')
    parser.add_argument("--user-output", default="relation.root", 
                            help="output file name")
    parser.add_argument("--inputs", default=["sample.root"], nargs="+", 
                            help="input file lists")
    parser.add_argument("--gdml-file", default="default", 
                            help="file contains gdml.")
    parser.add_argument("--geom", action="store_true",  help="save geometry.")
    parser.add_argument("--counting-only", default=False, action="store_true",
                            help="only do counting.")

    return parser

if __name__ == "__main__":
    # = parameter =
    parser = get_parser()
    args = parser.parse_args()
    print args

    evtmax = args.evtmax
    loglevel = 3
    input_lists = args.inputs
    gdml_filename = args.gdml_file
    geom_path_inroot = "JunoGeom"
    user_output = args.user_output
    # = create task =

    task = Sniper.Task("task")
    task.setEvtMax(evtmax)
    task.setLogLevel(loglevel)

    # == buffer related ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    #bufMgr.property("TimeWindow").set([-0.01, 0.01])

    # == IO Svc ==
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set(input_lists)

    # == RootWriter ==
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    out = {"SIMEVT":user_output}
    if args.geom:
        out["GEOM"]="geom-"+user_output
    rootwriter.property("Output").set(out)

    # == Geometry Svc ==
    import Geometry
    geom = task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set(gdml_filename)
    geom.property("GeomPathInRoot").set(geom_path_inroot)
    geom.property("FastInit").set(True)

    # == Alg ==
    # === Relation Finder ===
    Sniper.loadDll("libVoxelMethod.so")
    rf = task.createAlg("RelationFinder")
    rf.property("OutputGeom").set(args.geom)
    rf.property("CountingOnly").set(args.counting_only)

    task.show()
    task.run()
