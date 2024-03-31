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
    parser.add_argument("--output", default="sample_oec.root", help="output file name")
    parser.add_argument("--outputType", default="", choices=["", "waveform", "TQ"], help="output file name")
    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

    #import testAlg
    #inputTask.createAlg("testAlg")
if __name__ == "__main__":
    parser=get_parser()
    args=parser.parse_args()

    topTask=Sniper.TopTask("topTask")
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
    oecprocessoralg.property("TimeWindow").set([-3,3])
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
    print "file name: ", filelist;
    inputTask=topTask.createTask("Task/input_Task")
    import RootIOSvc
    import BufferMemMgr
    bufmgr=inputTask.createSvc("BufferMemMgr")
    bufmgr.property("StopTaskOpt").set(1)
    #bufmgr.property("TimeWindow").set([-10,10])
    readin=inputTask.createSvc("RootInputSvc/InputSvc")
    readin.property("InputFile").set(filelist)

    #==================LEC subtask=======================
    LECTask=topTask.createTask("Task/LEC_Task")
    lecbufmgr=LECTask.createSvc("BufferMemMgr")

    #read xml 
    import OECTagSvc
    lectagsvc=LECTask.createSvc("OECTagSvc")
    lectagsvc.property("OECTagFile").set(os.getenv('OECTAGSVCROOT')+"/share/tag.xml")
    import EvtConfig
    evtconfig=LECTask.createSvc("EvtConfigSvc")
    evtconfig.property("seqListFile").set(os.getenv('EVTCONFIGROOT')+"/share/seq.xml")
    evtconfig.property("sigListFile").set(os.getenv('EVTCONFIGROOT')+"/share/sig.xml")

    #store energy and time for tag
    import EvtStore
    LECTask.property("svcs").append("EvtStoreSvc")

    #store result
    import EvtResult
    LECTask.property("svcs").append("EvtResultSvc")

    #for fast reconstruction
    import Geometry
    geosvc=LECTask.createSvc("RecGeomSvc")
    geosvc.property("GeomFile").set("default")
    geosvc.property("GeomPathInRoot").set("JunoGeom")
    geosvc.property("FastInit").set(True)
    import JunoTimer
    LECTask.createSvc("JunoTimerSvc")

    import EvtAlgorithms
    import EvtSteering
    import SimpleRecAlg
    import RecCdMuonAlg
    import SpmtMuonRecTool
    #subTask RecCdMuonAlg
    subTask_RecMuon = topTask.createTask("Task/subTask_RecCdMuonAlg")
    recMuonAlg = RecCdMuonAlg.createAlg(subTask_RecMuon)
    recMuonAlg.property("Use3inchPMT").set(True)
    recMuonAlg.useRecTool("SpmtMuonRecTool")
    LECTask.property("algs").append("StepHandler")

    #===================HEC subtask======================
    HECTask=topTask.createTask("Task/HEC_Task")
    import OECTagSvc
    hectagsvc=HECTask.createSvc("OECTagSvc")
    hectagsvc.property("OECTagFile").set(os.getenv('OECTAGSVCROOT')+"/share/tag.xml")
    import OECConfigSvc
    OECConf = HECTask.createSvc("OECConfigSvc")
    OECConf.property("OECListFile").set(os.getenv('OECCONFIGSVCROOT')+"/share/sig.xml")
    #import HECAlg
    #hecalg=HECTask.createAlg("HECAlg")
    import HECMuonAlg
    hecmuonalg=HECTask.createAlg("HECMuonAlg")

    #===================output subtask===================
    outputTask=topTask.createTask("Task/output_Task")
    outbufmgr=outputTask.createSvc("BufferMemMgr")
    #readout=outputTask.createSvc("RootOutputSvc/OutputSvc")
    #outputdata={"/Event/TQ": args.output,
    #            "/Event/waveform": args.output,
    #            "/Event/OEC": args.output}
    #readout.property("OutputStreams").set(outputdata)


#$OECPROCESSORROOT


    #==============================================================================
    #RUN
    topTask.show()
    topTask.run()
