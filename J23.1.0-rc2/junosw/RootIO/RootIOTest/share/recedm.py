#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():
    import argparse

    parser = argparse.ArgumentParser("Rec EDM Test")
    parser.add_argument("--rw-mode", type=int, default=1,
                        help="1: write; 0: read")
    parser.add_argument("--evtmax", type=int, default=1000, help='events to be generated')
    parser.add_argument("--loglevel", default="Info", 
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            help="Set the Log Level")
    parser.add_argument("--output", default="sample_output.root", help='output file name')
    parser.add_argument("--input", default="sample_input.root", help='input file name')

    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    import RootIOSvc
    if args.rw_mode == 1:
        # write
        io_o = task.createSvc("RootOutputSvc/OutputSvc")
        io_o.property("OutputStreams").set({"/Event/Rec": args.output})
    elif args.rw_mode == 0:
        # read
        io_i = task.createSvc("RootInputSvc/InputSvc")
        io_i.property("InputFile").set([args.input])

    import RootIOTest

    alg = task.createAlg("TestRecEDM")
    alg.property("RWMode").set(args.rw_mode)

    task.show()
    task.run()
