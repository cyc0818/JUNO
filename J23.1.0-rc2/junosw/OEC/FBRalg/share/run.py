#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: huangxin

import string
import Sniper

def get_parser():
    import argparse
    parser=argparse.ArgumentParser(description='run PMT information')
    parser.add_argument('--output', default='sample.root', help='output file name')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", help="input file name")
    parser.add_argument("--inputList", action="append", help="input file list name")
    parser.add_argument("--loglevel", default="Debug", 
                            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                            help="Set the Log Level")   
    #set the parameters
    #-----------parameters of the PromptMonitor----------------
    #-----------parameters of the sliding window method and time interval method----------------
    #common
    '''
    parser.add_argument('--T', type=float, default=4, help='the length of the time window')
    parser.add_argument('--Nthr', type=int, default=3, help='the threshold of n to give alert')
    parser.add_argument('--startTime', type=float, default='-5', help='the threshold of n to give alert')
    #for sliding window method
    parser.add_argument('--dT', type=float, default=1e-3, help='the step of the')
    #for time interval method
    #-----------parameters of the bayesian block method tool----------------
    parser.add_argument('--ncp', type=float, default=7., help='the ncp_prior for BayesianBlock')
    '''
    #parser.add_argument('--infotype', type=int, default=1)
    parser.add_argument('--threQ', type=float, default="5")
    parser.add_argument('--Rf', type=float, default="15000")

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
    readin.property('InputFile').set(args.input)
    #output svc
    #import RootWriter
    #rootwriter=topTask.createSvc("RootWriter")
    #rootwriter.property("Output").set({"USER_OUTPUT":"userOutput.root"})
    import Geometry
    pmt_param_svc = topTask.createSvc("PMTParamSvc")

    import FBRalg
    ginf=topTask.createAlg('FBRalg')
    ginf.property('threQ').set(args.threQ)
    ginf.property('Rf').set(args.Rf)

    topTask.show()
    topTask.run()
