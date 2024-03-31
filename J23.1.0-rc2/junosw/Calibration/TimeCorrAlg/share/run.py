#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author: yuzy@ihep.ac.cn
# Oct 13, 2016


import Sniper
import sys
import os

import platform
print("*** NODE Info:", platform.uname(), "***")
print("*** CURRENT PID: ", os.getpid(), "***")

def get_parser():

    import argparse

    class MakeTVAction(argparse.Action):
        def __init__(self, option_strings, dest, nargs=None, **kwargs):
            #print "__init__ begin"
            #print option_strings 
            #print dest 
            #print nargs
            #print kwargs
            #print "__init__ end"
            super(MakeTVAction, self).__init__(option_strings, dest, nargs, **kwargs)
        def __call__(self, parser, namespace, values, option_string=None):
            #print "__call__ begin"
            #print parser
            #print namespace
            #print values
            # == convert a list into 3-tuple ==
            if len(values) % 3:
                print(("please set %s like x1 y1 z1 x2 y2 z2 ..." %option_string))
                sys.exit(-1)
            it = iter(values)
            values = list(zip(*([it]*3)))
            setattr(namespace, self.dest, values)
            #print option_string
            #print "__call__ end"

    parser = argparse.ArgumentParser(description='PMT spe waveform calibration.')

    import argparse

    parser = argparse.ArgumentParser(description='Waveform Reconstrtion.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default="sample_elecsim.root", help="input file name")
    parser.add_argument("--input_list", default=None, help="input file name")
    parser.add_argument("--output", default="sample_calib.root", help="output file name")
    parser.add_argument("--user-output",default="time_corr.root", help="output user file name")
    parser.add_argument("--user-output1",default="time_corr1.root", help="output user file name")
    parser.add_argument("--loglevel", default="Info",
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            )
    parser.add_argument("--step", default="1", type = int, help="Step of the calibration")
    parser.add_argument("--calibfile", default="Integral.txt", help="PMT spe integral file")
    parser.add_argument("--store", default="0", type = int, help="Store the average spe waveform or not")
    parser.add_argument("--option",default="fixed", help="timing alg option")
    parser.add_argument("--trigger", default="force", help="trigger mode")
    parser.add_argument("--forceTriggerSigma", default=0., type=float, help="force trigger sigma")
    parser.add_argument("--hitsOption", default="all", help="hits selection", 
                            choices = ["all", "1PE"])
    parser.add_argument("--positions",default=[(0,0,0)], nargs='+',
                            type=float, action=MakeTVAction,
                            help="Positions(mm) x1 y1 z1 x2 y2 z2 ....")
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6 }

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])

    #create buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0])

    # Create PMTCalib Svc
    import PMTCalibSvc
    calibsvc = task.createSvc("PMTCalibSvc");

    #Geometry service
    import Geometry
    rgSvc = task.createSvc("RecGeomSvc")
    rgSvc.property("GeomFile").set("default")
    rgSvc.property("GeomPathInRoot").set("JunoGeom")
    rgSvc.property("FastInit").set(True)

    # Create IO Svc
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputs = []
    if args.input_list:
        import sys
        import os.path
        if not os.path.exists(args.input_list):
            sys.exit(-1)
        with open(args.input_list) as f:
            for line in f:
                line = line.strip()
                inputs.append(line)
    else:
        inputs.append(args.input)
    inputsvc.property("InputFile").set(inputs)

#    if args.input.find('list') > -1:
#	arr = []
#        file1 = open(args.input)
#        for line in file1 :
#	    line = line.strip()
#	    arr.append(line)
#	inputsvc.property("InputFile").set(arr)
#    else:
#        inputsvc.property("InputFile").set([args.input])

    # Create RoOT Writer
    import RootWriter
#    task.property("svcs").append("RootWriter")
#    rw = task.find("RootWriter")
#    rw.property("Output").set({"FILE1": args.output})
    rootwriter = task.createSvc("RootWriter")
    rootwriter.property("Output").set({"CALIBEVT": args.user_output})
    #rootwriter.property("Output").set({"CALIBEVT1": args.user_output})


    #import TimeCorrAlg
    from GenTools import makeTV
    Sniper.loadDll("libTimeCorrAlg.so")
    pmtCalib=task.createAlg("TimeCorrAlg")
    pmtCalib.property("option").set(args.option) 
    pmtCalib.property("trigger").set(args.trigger)
    pmtCalib.property("hitsOption").set(args.hitsOption)
    pmtCalib.property("Positions").set([makeTV(x,y,z) for x,y,z in args.positions])
    pmtCalib.property("forceTriggerSigma").set(args.forceTriggerSigma)
    #pmtCalib.property("CalibFile").set(args.calibfile)
    #pmtCalib.property("Store").set(args.store)
    task.show()
    task.run()


