#!/usr/bin/env python

import argparse

def get_parser():
    parser = argparse.ArgumentParser(description="TBB DetSim")
    parser.add_argument("--evtmax", type=int, default=10,
                   help="total events")
    parser.add_argument("--ntasks", type=int, default=4,
                   help="number of tasks.")
    parser.add_argument("--loglevel", type=int, default=3,
                   help="Log Level: 0: TEST, 1: DEBUG, 3: INFO, 4: WARN, 5: ERROR, 6: FATAL")
    parser.add_argument("--actions", action="append", choices=["normal"], default=[],
                   help="User Actions to be used.")
    parser.add_argument("--output", default="detsim", help="output file name pattern")
    return parser

# global task
import os
import SniperMuster
import DetSimMT
import DetSimMTUtil
import DetSimOptions
import Geometry
import PMTSimParamSvc

def HelloJob():
    # configuration is just the same as a single thread job
    import Sniper
    global ith
    ith += 1
    task = Sniper.Task("Job%d"%ith)
    task.setLogLevel(3)

    SlaveRunMgrSvc = task.createSvc("SlaveRunMgrSvc")
    SimAlg = task.createAlg("SimAlg")


    global first_time
    if first_time:
        task.show()
        first_time = False
    # instead of task.run(), we must return the task object here
    return task

def GInput():
    import Sniper
    task = Sniper.Task("GInput")
    genalg = task.createAlg("GlobalGenAlg")

    return task

def GOutput():
    import Sniper
    task = Sniper.Task("GOutput")

    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    import RootIOSvc
    outputsvc = task.createSvc("RootOutputSvc/OutputSvc")
    output_streams = {}
    output_streams["/Event/Sim"] = "%s.root"%args.output
    
    outputsvc.property("OutputStreams").set(output_streams)

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    outputmap = {"SIMEVT": "user-%s.root"%(args.output)}
    rootwriter.property("Output").set(outputmap)

    outalg = task.createAlg("GlobalDetSimOutAlg")

    return task

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()
    NTHREADS = args.ntasks
    EVTMAX = args.evtmax
    LOGLEVEL = args.loglevel
    
    import Sniper
    Sniper.setLogLevel(LOGLEVEL)
    global_task = DetSimMT.GlobalTask("globaltask")
    global_task.setLogLevel(LOGLEVEL)
    global_task.setEvtMax(EVTMAX)

    os.environ["JUNO_SIMPLIFY_CALIB_ANCHOR"] = "Enable"

    Sniper.loadDll("libMCParamsSvc.so")
    pmt_param_svc = global_task.createSvc("PMTParamSvc")
    pmt_sim_param_svc = global_task.createSvc("PMTSimParamSvc")
    mcsvc = global_task.createSvc("MCParamsFileSvc/MCParamsSvc")

    MasterRunMgrSvc = global_task.createSvc("MasterRunMgrSvc")
    MasterRunMgrSvc.property("RunMac").set("tbb.mac")
    MasterRunMgrSvc.property("NTasks").set(NTHREADS)
    MasterRunMgrSvc.property("Recorders").set(args.actions)
    MasterRunMgrSvc.property("DetFactory").set("DetFactorySvc")

    # service
    detfactory = global_task.createSvc("DetFactorySvc")

    detfactory.property("StrutPosFile").set(DetSimOptions.data_load("Strut_Acrylic.csv"))
    detfactory.property("Strut2PosFile").set(DetSimOptions.data_load("StrutBar2_Acrylic.csv"))
    detfactory.property("FastenerPosFile").set(DetSimOptions.data_load("Strut_Anchor_Acrylic.csv"))
    detfactory.property("XJanchorPosFile").set(DetSimOptions.data_load("XJanchor.csv"))
    detfactory.property("SJCLSanchorPosFile").set(DetSimOptions.data_load("SJCLSanchor.csv"))
    detfactory.property("SJReceiverPosFile").set(DetSimOptions.data_load("SJReceiverPos.csv"))
    detfactory.property("SJFixturePosFile").set(DetSimOptions.data_load("SJFixturePos.csv"))


    # algo

    detsimalg = global_task.createAlg("DetSimAlg")
    detsimalg.property("isMT").set(True) # disable the real work.

    # several tools
    wp = detsimalg.createTool("WaterPoolConstruction")
    wp.property("enabledLatticedShell").set(False)

    global_task.show()
    global_task.run()

    if True:
        import SniperMuster
        muster = SniperMuster.Muster()

        global ith
        ith = 0
        global first_time
        first_time = True

        # the EvtMax in Sniper.Task is deactivated by Muster
        muster.setEvtMax(EVTMAX)

        # we will execute the HelloJob maximumly in 4 threads
        muster.config(HelloJob, NTHREADS)

        # global buffer
        gs = SniperMuster.createGlobalStream("JunoGlobalStream/GEvtStream")
        gs.configInput(GInput)
        gs.configOutput(GOutput)
        gs.configBuffer(50, 20)

        # TODO: show the configurations
        #muster.show()

        # spawn the threads and begin to run 
        muster.run()
        gs.join()
    global_task.global_finalize()
