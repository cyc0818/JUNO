#!/usr/bin/env python

import argparse
import Sniper
import EventRecAlgMT

def get_parser():
    parser = argparse.ArgumentParser(description="TBB based dummy Event Rec")
    parser.add_argument("--evtmax", type=int, default=10,
                   help="total events")
    parser.add_argument("--ntasks", type=int, default=4,
                   help="number of tasks.")
    parser.add_argument("--loglevel", type=int, default=3,
                   help="Log Level: 0: TEST, 1: DEBUG, 3: INFO, 4: WARN, 5: ERROR, 6: FATAL")
    parser.add_argument("--input", default="calib.root", help="input file name")
    parser.add_argument("--output", default="rec", help="output file name pattern")
    return parser

def RecJob():
    import Sniper
    global ith
    ith += 1
    task = Sniper.Task("RecJob%d"%ith)
    # task = global_task.createTask("Task/RecJob%d"%ith)
    task.setLogLevel(3)

    mtgeom = task.createSvc("RecGeomSvcMT")


    recalg = task.createAlg("EventRecAlgMT")

    global first_time
    if first_time:
        task.show()
        first_time = False
    # instead of task.run(), we must return the task object here
    return task

def GInput():
    import Sniper

    task = Sniper.Task("GInput")

    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    genalg = task.createAlg("GlobalRecInputAlg")

    return task

def GOutput():
    import Sniper
    task = Sniper.Task("GOutput")

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0])

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {
                  "/Event/Rec": "%s.root"%args.output
                  }
    roSvc.property("OutputStreams").set(outputdata)

    outalg = task.createAlg("GlobalRecOutAlg")

    return task

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()
    NTHREADS = args.ntasks
    EVTMAX = args.evtmax
    LOGLEVEL = args.loglevel
    
    import Sniper
    Sniper.setLogLevel(LOGLEVEL)

    ##########################################################################
    # Global Service
    ##########################################################################
    global_task = Sniper.TopTask("global_task")


    ##########################################################################
    # Global Service / Geometry
    ##########################################################################

    gdml_filename = "default"
    geom_path_inroot = "JunoGeom"
    import Geometry
    geom = global_task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set(gdml_filename)
    geom.property("GeomPathInRoot").set(geom_path_inroot)
    geom.property("FastInit").set(True)

    mtgeom = global_task.createSvc("RecGeomSvcMT")

    global_task.initialize()

    ##########################################################################
    # Muster
    ##########################################################################

    import SniperMuster
    muster = SniperMuster.Muster()

    global ith
    ith = 0
    global first_time
    first_time = True

    # the EvtMax in Sniper.Task is deactivated by Muster
    muster.setEvtMax(EVTMAX)

    # we will execute the HelloJob maximumly in 4 threads
    muster.config(RecJob, NTHREADS)

    # global buffer
    gs = SniperMuster.GlobalStream("GEvtStream")
    gs.configInput(GInput)
    gs.configOutput(GOutput)
    gs.configBuffer(50, 20)

    # TODO: show the configurations
    #muster.show()

    # spawn the threads and begin to run 
    muster.run()
