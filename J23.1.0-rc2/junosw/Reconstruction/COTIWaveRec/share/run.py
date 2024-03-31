#!/usr/bin/env python
# -*- coding:utf-8 -*-

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Waveform Reconstrtion.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default="sample_elecsim.root", help="input file name")
    parser.add_argument("--input-list", default=None, help="input file name")
    parser.add_argument("--output", default="sample_calib.root", help="output file name")
    parser.add_argument("--user-output", default="sample_calib_user.root", help="output user file name")
    parser.add_argument("--loglevel", default="Info",
            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
            )
    # different methods
    parser.add_argument("--method", default="contovthd",
            choices=["pmtrec", "wavefit", "decon", "muon", "cotiwaverec"],
            help="pmtrec: Integral PMT Rec, wavefit: Waveform fit, decon: Deconvolution, muon: Muon Waveform Reco, cotiwaverec: Continous Over-Threshold Integral")
    # parameters for the deconvolution method
    parser.add_argument("--Calib", default="0", type = int,  help="Calib parameters extraction mode, 1 = on, 0 = off")
    parser.add_argument("--Algrithm", default="2", type = int,  help="Baseline & threshold algrithom, 1 = constant baseline, 2 = dynamic baseline (default)")
    parser.add_argument("--BslBufLen", default="32", type = int,  help="Baseline buffer length, default = 32 ns")
    parser.add_argument("--NTimesRMS", default="1.5", type = float,  help="N times of rms as threshold, default = 1.5x")
    parser.add_argument("--NHeadPoints", default="5", type = int,  help="Head points number, default = 5")
    parser.add_argument("--NTailPoints", default="3", type = int,  help="Tail points number, default = 3")
    parser.add_argument("--DebugMode", default="0", type = int,  help="Debug mode, 1 = on, 0 = off, default = 0")
    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()
    print args

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

    # Create IO Svc
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    # input files could be one or a list of files
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

    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    roSvc.property("OutputStreams").set({"/Event/Calib":args.output})

    # user output
    import RootWriter
    rootwriter = task.createSvc("RootWriter")
    rootwriter.property("Output").set({"CALIBEVT":args.user_output})

    # waveform reconstruction alg
    if args.method == "contovthd":
        Sniper.loadDll("/junofs/users/zhangxt/github/calibRec_juno/COTIWaveRec/amd64_linux26/libCOTIWaveRec.so")
        contovthd = task.createAlg("COTIWaveRec")
        contovthd.property("Calib").set(args.Calib)
        contovthd.property("Algrithm").set(args.Algrithm)
        contovthd.property("BslBufLen").set(args.BslBufLen)
        contovthd.property("NTimesRMS").set(args.NTimesRMS)
        contovthd.property("NHeadPoints").set(args.NHeadPoints)
        contovthd.property("NTailPoints").set(args.NTailPoints)
        contovthd.property("DebugMode").set(args.DebugMode)
    task.show()
    task.run()
