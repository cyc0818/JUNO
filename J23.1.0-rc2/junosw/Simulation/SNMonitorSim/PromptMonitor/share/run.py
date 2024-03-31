#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: huangxin

import Sniper

def get_parser():
    import argparse
    parser=argparse.ArgumentParser(description='run supernova hardware trigger')
    parser.add_argument('--loglevel', default='Info', choices=['Test', 'Debug', 'Info', 'Warn', 'Error', 'Fatal'], help='set the log level')
    parser.add_argument('--evtmax', type=int, default=-1, help='events to be processed, do not change this value to complete the trigger for one SN burst')
    parser.add_argument("--input", action='append', help="input file name, if one SN burst is simulated in one file, use this parameter")
    parser.add_argument("--inputList", action="append", help="input file list name, if one SN burst is simulated in several files, use this parameter rather than --input")
    parser.add_argument('--output', default='sample.root', help='output file name')
    parser.add_argument('--user-output', default='user-sample.root', help='user output file name, stores the result of SN trigger')
    return parser

DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
        }

if __name__ == '__main__':
    parser=get_parser()
    args=parser.parse_args()

    topTask=Sniper.TopTask('topTask')
    topTask.setLogLevel(DATA_LOG_MAP[args.loglevel])
    topTask.setEvtMax(args.evtmax)

    #=============for input file==================
    filelist=[]
    if args.input:
        for f in args.input:
            filelist.append(f)
    if args.inputList:
        for fname in args.inputList:
            with open(fname) as hxf:
                for line in hxf:
                    line=line.strip()
                    filelist.append(line)
    #the data buffer
    import BufferMemMgr
    bufMgr=topTask.createSvc('BufferMemMgr')
    #input svc
    import RootIOSvc
    readin=topTask.createSvc('RootInputSvc/InputSvc')
    readin.property('InputFile').set(filelist)
    #output svc
    import RootWriter
    rootwriter=topTask.createSvc("RootWriter")
    rootwriter.property("Output").set({"SN_TRIGGER":args.user_output})

    import PromptMonitor
    topTask.createAlg('PromptMonitor')

    topTask.show()
    topTask.run()
