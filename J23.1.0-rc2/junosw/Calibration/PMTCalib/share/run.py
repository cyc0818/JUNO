#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author: yuzy@ihep.ac.cn
# Oct 13, 2016


import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='PMT spe waveform calibration.')

    import argparse

    parser = argparse.ArgumentParser(description='Waveform Reconstrtion.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default="sample_elecsim.root", help="input file name")
    parser.add_argument("--output", default="SpeAvgWaveform.root", help="output file name")
    parser.add_argument("--loglevel", default="Info",
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            )
    parser.add_argument("--step", default="1", type = int, help="Step of the calibration")
    parser.add_argument("--calibfile", default="Integral.txt", help="PMT spe integral file")
    parser.add_argument("--store", default="1", type = int, help="Store the average spe waveform or not")
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

    # Create RoOT Writer
    import RootWriter
    task.property("svcs").append("RootWriter")
    rw = task.find("RootWriter")
    rw.property("Output").set({"FILE1": args.output})

    #import PMTCalib
    Sniper.loadDll("libPMTCalib.so")
    pmtCalib=task.createAlg("PMTCalib")
    pmtCalib.property("Step").set(args.step)
    pmtCalib.property("CalibFile").set(args.calibfile)
    pmtCalib.property("Store").set(args.store)
    task.show()
    task.run()


