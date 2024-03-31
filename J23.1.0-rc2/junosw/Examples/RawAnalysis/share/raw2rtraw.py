#!/usr/bin/python
# -*- coding:utf-8 -*-

def get_parser():
    import argparse
    parser = argparse.ArgumentParser(description="fill raw evt to root")
    parser.add_argument("--evtmax", type=int, default=-1, help="events to be processed")
    parser.add_argument("--input", default=None, help="input file name")
    parser.add_argument("--type", default=None, choices=["GCU", "DAQ"], help="input file type: GCU or DAQ")
    parser.add_argument("--output-root", default=None, help="output file name of RootWriter(Plain TTree)")
    parser.add_argument("--output-elec", default=None, help="output file name of RootOutputSvc")
    parser.add_argument("--output-reproducible", 
                        dest="output_reproducible",
                        action='store_true',
                        help='enable output reproducible')
    parser.add_argument("--no-output-reproducible", 
                        dest="output_reproducible",
                        action='store_false',
                        help='disable output reproducible (default)')
    parser.set_defaults(output_reproducible=False)

    return parser

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()

    if not args.input:
        print("please set input file")
        exit(1)

    if not args.type:
        print("please set input file type: GCU or DAQ")
        exit(1)

    import Sniper
    #Sniper.setLogLevel(2)

    task = Sniper.Task("task")

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0]);

    import RawDataIOSvc
    iSvc = task.createSvc("RawDataInputSvc/InputSvc")

    # use args.type and match...case for more file types, requires official file_type_names
    if args.type == "GCU":
        iSvc.property("Type").set("GCU")
        print("Input file type: GCU test file")
    else:
        iSvc.property("Type").set("DAQ")
        print("Input file type: DAQ test file")

    iSvc.property("InputFile").set([
        args.input
    ])

    import RawAnalysis
    alg = task.createAlg("RawEvtAnalysis")

    if args.output_root:
        output = args.output_root
        alg.property("FillRoot").set(True)
        #config the RootWriter
        import RootWriter
        rw = task.createSvc("RootWriter")

        if args.output_reproducible and '?reproducible' not in output:
            output += '?reproducible'

        rw.property("Output").set({
            "waveform": output
        })

    if args.output_elec:
        output = args.output_elec
        alg.property("FillElec").set(True)
        #config the RootOutputSvc
        import RootIOSvc
        oSvc = task.createSvc("RootOutputSvc/OutputSvc")
        oSvc.property("OutputStreams").set({
            "/Event/CdWaveform" : output
            })
        oSvc.property("IsReproducible").set(args.output_reproducible)

    task.setEvtMax(args.evtmax)
    task.run()
