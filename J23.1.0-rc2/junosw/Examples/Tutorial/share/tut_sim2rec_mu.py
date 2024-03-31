#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified: 2014-09-09 21:03
# Filename: test.py
# Description: 

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default=["sample_muonsim.root"],nargs='+',  help="input file name list")
    parser.add_argument("--output", default="sample_muonrec.root", help="output file name")
    parser.add_argument("--fhtcorr",default="$LSQMUONRECTOOLROOT/test/fhtcorr.root",
                                        help="name of file with correction data for first hit time")
    parser.add_argument("--geomfile",default='',help="name of file with geometry information")
    return parser

if __name__  ==  "__main__":
    parser = get_parser()
    args = parser.parse_args()

    #Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    task.setLogLevel(0)
    
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0]);
    
    import RootIOSvc
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    roSvc.property("OutputStreams").set({
             "/Event/Sim":args.output, 
             "/Event/RecTrack": args.output,
             "/Event/Calib":args.output})
    riSvc = task.createSvc("RootInputSvc/InputSvc")
    riSvc.property("InputFile").set(args.input)
    
    import Geometry
    geom = task.createSvc("RecGeomSvc")
    if(args.geomfile!=''):
      geom.property("GeomFile").set(args.geomfile)
    else:
      #TODO how to specify geometry for every input, especially 
      #for inputs with different geometry info.
      geom.property("GeomFile").set(args.input[0])
    geom_path_inroot = "JunoGeom"
    geom.property("GeomPathInRoot").set(geom_path_inroot)
    geom.property("FastInit").set(True)
    
    Sniper.loadDll("libPmtRec.so")
    task.property("algs").append("PullSimHeaderAlg")
    
    import RecCdMuonAlg
    import LsqMuonRecTool
    recalg = RecCdMuonAlg.createAlg(task)
    recalg.setLogLevel(1)
    # whether to use data of 3 inch pmts
    recalg.property("Use3inchPMT").set(False);
    # whether to use data of 20 inch pmts
    recalg.property("Use20inchPMT").set(True);
    # 3inch pmt time resolution,  unit: ns
    recalg.property("Pmt3inchTimeReso").set(1)
    #20inch pmt time resolution,  unit: ns
    recalg.property("Pmt20inchTimeReso").set(3)

    #configure the specific rec tool
    recalg.useRecTool("LsqMuonRecTool")
    recalg.rectool.property("LSRadius").set(17700)
    recalg.rectool.property("LightSpeed").set(299.792458)
    recalg.rectool.property("LSRefraction").set(1.485)
    recalg.rectool.property("MuonSpeed").set(299.792458)
    recalg.rectool.property("FhtCorrFile").set(args.fhtcorr)

    task.setEvtMax(args.evtmax)
    task.show()
    task.run()
