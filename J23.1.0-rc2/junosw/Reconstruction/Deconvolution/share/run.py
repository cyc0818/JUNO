#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author: yuzy@ihep.ac.cn
# July 12, 2016
# Usage: python $DECONVOLUTIONROOT/share/run.py --evtmax -1 --input elecsim.root --output calib.root

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='PMT waveform reconstruction')

    import argparse

    parser = argparse.ArgumentParser(description='Waveform Reconstrtion.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default="sample_elecsim.root", help="input file name")
    parser.add_argument("--output", default="sample_calib.root", help="output file name")
    parser.add_argument("--loglevel", default="Info",
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            )
    parser.add_argument("--para1", default="50", type = float, help="Filter parameter 1")
    parser.add_argument("--para2", default="120", type = float, help="Filter parameter 2")
    parser.add_argument("--para3", default="30", type = float, help="Filter parameter 3")
    parser.add_argument("--TotalPMT", default="17746", type = int,  help="Total PMT number")
    parser.add_argument("--Threshold", default="0.06", type = float,  help="threshold for hit finding")
    parser.add_argument("--Length", default="1250", type = int,  help="Readout window length ns")
    parser.add_argument("--Window", default="3", type = int,  help="Charge integral ns")
    parser.add_argument("--HitCounting", default="0", type = int,  help="Use the hit counting method or not")
    parser.add_argument("--save_features", default="0", type = int,  help="Save features or not")
    parser.add_argument("--user-output", default="calib_user.root", help="output user file name")
    #parser.add_argument("--CalibFile", default="/junofs/users/yuzy/Offline/Reconstruction/Deconvolution/share/SPE.root", help="SPE File location")
# The default parameters are for J16v2. Never modify them. 
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6 }

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])

    # Create Data Buffer Svc
    #create buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0])

    # Create IO Svc
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    if args.input.find('list') > -1:

        arr = []
        file1 = open(args.input)
        for line in file1 :
            line = line.strip()
            arr.append(line)
        inputsvc.property("InputFile").set(arr)
    else:
        inputsvc.property("InputFile").set([args.input])

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    roSvc.property("OutputStreams").set({"/Event/Calib":args.output})

    # user output
    import RootWriter
    rootwriter = task.createSvc("RootWriter")
    rootwriter.property("Output").set({"CALIBEVT":args.user_output})

    #import Deconvolution 
    Sniper.loadDll("libDeconvolution.so")
    decon=task.createAlg("Deconvolution")
    decon.property("TotalPMT").set(args.TotalPMT)
    decon.property("Threshold").set(args.Threshold) # Default value is 1/4 PE. The current version has relative large electronics noises
    #decon.property("CalibFile").set(args.CalibFile)
    decon.property("Length").set(args.Length)
    decon.property("save_features").set(args.save_features)
    decon.property("Para1").set(args.para1)
    decon.property("Para2").set(args.para2)
    decon.property("Para3").set(args.para3)
    decon.property("Window").set(args.Window)
    decon.property("HitCounting").set(args.HitCounting)
    task.show()
    task.run()


