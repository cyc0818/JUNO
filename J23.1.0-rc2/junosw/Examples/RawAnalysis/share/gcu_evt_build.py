#!/usr/bin/python
# -*- coding:utf-8 -*-

def get_parser():
    import argparse
    parser = argparse.ArgumentParser(description='fill raw data to root')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default=None, help="input file name")
    parser.add_argument("--output-root", default=None, help="output file name of RootWriter(Plain TTree)")
    parser.add_argument("--output-elec", default=None, help="output file name of RootOutputSvc")

    return parser

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()

    if not args.input:
        print("please set input file")
        exit(1)

    output = args.output
    if not output:
        import os
        output = os.path.basename(args.input)

    ngcus = 0
    fgcus = open(args.input, 'r').readlines()

    import Sniper
    Sniper.setLogLevel(3)

    task = Sniper.TopTask("task")

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0]);

    import RawDataIOSvc
    for fgcu in fgcus:
        fgcu = fgcu.strip('\n')
        ngcus += 1
        iTask = task.createTask("Task/gcu%02d" % ngcus)
        iSvc = iTask.createSvc("RawDataInputSvc/InputSvc")
        iSvc.property("Type").set("GCU")
        iSvc.property("InputFile").set([fgcu])
        iBufMgr = iTask.createSvc("BufferMemMgr")
        iBufMgr.property("TimeWindow").set([0, 0]);
        iBufMgr.property("StopTaskOpt").set(1)

    import RawAnalysis
    alg1 = task.createAlg("RawEvtBuilder")
    alg1.property("NumGCUs").set(ngcus)
    alg1.setLogLevel(3)
    alg2 = task.createAlg("ElecEvtAnalysis")

    if args.output_root:
        output = args.output_root
        import RootWriter
        rw = task.createSvc("RootWriter")
        print(output)
        rw.property("Output").set({
            "waveform": output
        })

    if args.output_elec:
        output = args.output_elec
        import RootIOSvc
        oSvc = task.createSvc("RootOutputSvc/OutputSvc")
        print(output)
        oSvc.property("OutputStreams").set({
            "/Event/CdWaveform" : output
            })

    task.setEvtMax(args.evtmax)
    task.show()
    task.run()
