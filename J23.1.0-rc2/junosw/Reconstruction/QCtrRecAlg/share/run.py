#!/usr/bin/env python
#-*- coding: utf-8 -*-

import Sniper
import os

def get_parser():
    import argparse
    parser=argparse.ArgumentParser(description='run QCtrRecAlg')
    parser.add_argument("--loglevel", default="Debug", 
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            help="Set the Log Level")
    parser.add_argument("--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--input", help="input file name")
    parser.add_argument("--inputList", action="append", help="input file list name")
    parser.add_argument("--output", default="sample.root", help="output file name")

    parser.add_argument("--recMethod", default="zy", help="the method used to reconstruct E/V: zy or yzy")
    parser.add_argument("--recMode", default="normal", help="the reconstruction mode:normal or for OEC")
    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

    #import QCtrRecAlg
    #inputTask.createAlg("QCtrRecAlg")
if __name__ == "__main__":
    parser=get_parser()
    args=parser.parse_args()

    topTask=Sniper.TopTask("topTask")
    topTask.setLogLevel(DATA_LOG_MAP[args.loglevel])
    topTask.setEvtMax(args.evtmax)

    #================for input================
    filelist=[]
    if args.input:
        filelist.append(args.input)
    if args.inputList:
        for fname in args.inputList:
            with open(fname) as hxf:
                for line in hxf:
                    line=line.strip()
                    filelist.append(line)
    import RootIOSvc
    import BufferMemMgr
    bufmgr=topTask.createSvc("BufferMemMgr")
    bufmgr.property("StopTaskOpt").set(1)
    #bufmgr.property("TimeWindow").set([-10,10])
    readin=topTask.createSvc("RootInputSvc/InputSvc")
    readin.property("InputFile").set(filelist)

    #================for output================
    readout=topTask.createSvc("RootOutputSvc/OutputSvc")
    outputdata={"/Event/Rec": args.output}
    readout.property("OutputStreams").set(outputdata)

    #================for reconstruction================
    #--------------services--------------
    import Geometry
    geosvc=topTask.createSvc("RecGeomSvc")
    geosvc.property("GeomFile").set("default")
    #geosvc.property("GeomFile").set(os.getenv('JUNOTOP')+"/offline/Examples/Tutorial/share/sample_detsim.root")
    geosvc.property("GeomPathInRoot").set("JunoGeom")
    geosvc.property("FastInit").set(True)

    #import Geometry
    pmtsvc = topTask.createSvc("PMTParamSvc")
    import MCParamsSvc
    mcparamssvc = topTask.createSvc("MCParamsSvc")
    import PMTSimParamSvc
    pmtsimsvc = topTask.createSvc("PMTSimParamSvc")

    import QCtrRecAlg
    #import RecTools
    qctrrecalg=topTask.createAlg("QCtrRecAlg")
    qctrrecalg.property("recMethod").set(args.recMethod)
    #qctrrecalg.property("recMode").set(args.recMode)
    #tools to be used by this algorithm
    if args.recMethod == 'zy':
        zyqctool = qctrrecalg.createTool("zyQCTool")
    elif args.recMethod == 'yzy':
        zyqctool = qctrrecalg.createTool("yzyQCTool")
    elif args.recMethod == 'default':
        import RecTimeLikeAlg
        zyqctool = qctrrecalg.createTool("zyQCTool")
        qcrectool = qctrrecalg.createTool("QcRecTool")
        qcrectool.property("Pdf_Value").set(True)
        ptfrectool = qctrrecalg.createTool("PtfRecTool")
        ptfrectool.property("Pdf_Value").set(True)
    qsumtool = qctrrecalg.createTool("QSumTool")
    qsumtool.property("lightYield").set(1350)
    import OMILREC
    ecorrtool = qctrrecalg.createTool("NPEUnifCTool")
    #import OECTagSvc
    #tagsvc=topTask.createSvc("OECTagSvc")
    #tagsvc.property("OECTagFile").set("/junofs/users/huangx/huangxinihep/oec-prototype/OECConfig/OECTagSvc/share/tag.xml")

    #==============================================================================
    #RUN
    topTask.show()
    topTask.run()
