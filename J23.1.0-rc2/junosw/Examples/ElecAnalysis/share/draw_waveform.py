#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():         

    import argparse       

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.') 
    parser.add_argument("--evtmax", type=int, default=3, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--input", default="Elec_Output.root", help="input file name")
    parser.add_argument("--output", default="elec_analysis.root", help="output file name")
    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon"], 
                                       help="Det Option")
    return parser


TOTALPMTS = {"Acrylic": 17746, "Balloon": 18306}


if __name__ == "__main__":       
    parser = get_parser() 
    args = parser.parse_args() 

    total_pmt = TOTALPMTS[args.detoption]

    Sniper.setLogLevel(0)
    task = Sniper.Task("draw_waveform")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)


    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # == Data Registrition SVc ==
    # == I/O Svc ==

    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])   


    #add RootWriter
    import RootWriter
    rootwriter = task.createSvc("RootWriter")
    rootwriter.property("Output").set({"SIMEVT":args.output})



    # draw waveform
    #Sniper.loadDll("libElecSimV2.so")  
    Sniper.loadDll("libElecAnalysis.so")  
    draw_waveform=task.createAlg("TestBuffDataAlg")


    # = begin run =
    task.show()
    task.run()
