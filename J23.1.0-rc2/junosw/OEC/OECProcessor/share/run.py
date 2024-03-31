#!/usr/bin/env python
#-*- coding: utf-8 -*-

import Sniper
import os

def get_parser():
    import argparse
    parser=argparse.ArgumentParser(description='run OEC')
    parser.add_argument("--loglevel", default="Debug", 
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            help="Set the Log Level")
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", help="input file name")
    parser.add_argument("--inputList", action="append", help="input file list name")
    parser.add_argument("--enableEventRec", dest="useEventRec", action="store_true", help="perform reconstruction algorithms")
    parser.add_argument("--disableEventRec", dest="useEventRec", action="store_false", help="disable reconstruction algorithms, reading from input file instead.")
    parser.set_defaults(useEventRec=True)
    parser.add_argument("--output", default="sample_oec.root", help="output file name")
    parser.add_argument("--outputType", default="", choices=["", "waveform", "TQ"], help="output file name")
    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

if __name__ == "__main__":
    parser=get_parser()
    args=parser.parse_args()

    topTask=Sniper.TopTask("TopTask")
    topTask.setLogLevel(DATA_LOG_MAP[args.loglevel])
    topTask.setEvtMax(args.evtmax)

    #junotimer and rootwriter for test
    import JunoTimer
    topTask.createSvc("JunoTimerSvc")
    import RootWriter
    rootwriter=topTask.createSvc("RootWriter")
    rootwriter.property("Output").set({"USER_OUTPUT":"userOutput.root"})
    
    import OECProcessor
    oecprocessoralg=topTask.createAlg("OECProcessorAlg")
    oecprocessoralg.property("TimeWindow").set([0,3])
    oecprocessoralg.property("outputType").set(args.outputType)

    #=============input subtask or simulation/calibration subtask===============
    filelist=[]
    if args.input:
        filelist.append(args.input)
    if args.inputList:
        for fname in args.inputList:
            with open(fname) as hxf:
                for line in hxf:
                    line=line.strip()
                    filelist.append(line)
    inputTask=topTask.createTask("Task/input_Task")
    import RootIOSvc
    import BufferMemMgr
    bufmgr=inputTask.createSvc("BufferMemMgr")
    bufmgr.property("StopTaskOpt").set(1)
    #bufmgr.property("TimeWindow").set([-10,10])
    readin=inputTask.createSvc("RootInputSvc/InputSvc")
    readin.property("InputFile").set(filelist)


    #==================LEC subtask=======================
    LECTask=topTask.createTask("TopTask/LEC_Task")
    lecbufmgr=LECTask.createSvc("BufferMemMgr")

    #---------------temp------------------
    #read event tag values
    import OECTagSvc
    lectagsvc=LECTask.createSvc("OECTagSvc")
    lectagsvc.property("OECTagFile").set(os.getenv('OECTAGSVCROOT')+"/share/tag.xml")

    #store energy and time for tag
    import EvtStore
    LECTask.property("svcs").append("EvtStoreSvc")

    ##store result
    #import EvtResult
    #LECTask.property("svcs").append("EvtResultSvc")

    #timer
    import JunoTimer
    LECTask.createSvc("JunoTimerSvc")

    #import EvtAlgorithms

    #whether to use reconstruction or not
    if args.useEventRec:
        import Geometry
        geosvc=LECTask.createSvc("RecGeomSvc")
        geosvc.property("GeomFile").set("default")
        #geosvc.property("GeomFile").set(os.getenv('JUNOTOP')+"/offline/Examples/Tutorial/share/sample_detsim.root")
        geosvc.property("GeomPathInRoot").set("JunoGeom")
        geosvc.property("FastInit").set(True)
        pmtsvc = LECTask.createSvc("PMTParamSvc")
        Sniper.loadDll("libMCParamsSvc.so")
        mcparamssvc = LECTask.createSvc("MCParamsFileSvc/MCParamsSvc")
        import PMTSimParamSvc
        pmtsimsvc = LECTask.createSvc("PMTSimParamSvc")

        import QCtrRecAlg
        import OMILREC
        import RecTimeLikeAlg
        import RecCdMuonAlg
        import SpmtMuonRecTool
    else:
        #-----------subTask: readFromRecAlg--------------
        import readFromRecAlg
        #subTask_readFromRecAlg=LECTask.createTask("Task/subTask_readFromRecAlg")
        #readfromrecAlg=subTask_readFromRecAlg.createAlg("readFromRecAlg")
        readfromrecAlg=LECTask.createAlg("readFromRecAlg")

    import EvtSteering
    #Sniper.loadDll("libSniperCoreUsages.so")
    stephandler = LECTask.createAlg("StepHandler")
    stepsequencer = stephandler.createTool("StepSequencer")
    if args.useEventRec:
        stepsequencer.property("ConfigFile").set(os.getenv('EVTSTEERINGROOT')+"/share/seq.json")
    else:
        stepsequencer.property("ConfigFile").set("")
    stepdecision= stephandler.createTool("StepDecision")
    stepdecision.property("ConfigFile").set(os.getenv('EVTSTEERINGROOT')+"/share/sig.json")

    #===================HEC subtask======================
    HECTask=topTask.createTask("Task/HEC_Task")
    import OECTagSvc
    hectagsvc=HECTask.createSvc("OECTagSvc")
    hectagsvc.property("OECTagFile").set(os.getenv('OECTAGSVCROOT')+"/share/tag.xml")
    import OECConfigSvc
    OECConf = HECTask.createSvc("OECConfigSvc")
    OECConf.property("OECListFile").set(os.getenv('OECCONFIGSVCROOT')+"/share/sig.xml")
    import HECAlg
    hecalg=HECTask.createAlg("HECAlg")

    #===================output subtask===================
    outputTask=topTask.createTask("Task/output_Task")
    outbufmgr=outputTask.createSvc("BufferMemMgr")
    readout=outputTask.createSvc("RootOutputSvc/OutputSvc")
    outputdata={"/Event/TQ": args.output,
                "/Event/waveform": args.output,
                "/Event/OEC": args.output}
    readout.property("OutputStreams").set(outputdata)


#$OECPROCESSORROOT


    #==============================================================================
    #RUN
    topTask.show()
    topTask.run()
