#!/usr/bin/env python
# -*- coding:utf-8 -*-

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--input",default="sample_detsim.root", help="input file name")
    #parser.add_argument("--input",default="/publicfs/dyb/data/userdata/chengyp/ElecSim8Mev/e+_7.472_0.0/e+_0.root", help="input file name")
    parser.add_argument("--output",default="/workfs/dyw/chengyp/newjuno/offline/Reconstruction/TemplateFitAlg/share/sample_calib.root", help="output file name")
    #parser.add_argument("--output",default="/publicfs/dyb/data/userdata/chengyp/junotemplate/tmp8.root", help="output file name")

    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon"],
                                       help="Det Option")

    parser.add_argument("--splittime", default=10000, type=int, help="Split time (ns)") 
    parser.add_argument("--enableAfterPulse", default=False, help="enableAfterPulse or not")
    parser.add_argument("--enableDarkPulse", default=False, help="enableDarkPulse or not")
    parser.add_argument("--enableOvershoot", default=True, help="enableOvershoot or not")
    parser.add_argument("--enableRinging", default=False, help="enableRinging or not")
    parser.add_argument("--enableSatuation", default=False, help="enableSatuation or not")
    parser.add_argument("--enableNoise", default=True, help="enableNoise or not")
    parser.add_argument("--enablePretrigger", default=False, help="enablePretrigger or not")

    #############
    parser.add_argument("--simFrequency", default=1e9, help="the simulation frequerncy")
    parser.add_argument("--noiseAmp", default=0.2e-3, help="the electronic noise amplitude")
    parser.add_argument("--speAmp", default=3.5e-3, type=float, help="the single pe amplitude")
    parser.add_argument("--preTimeTolerance", default=300, help="the preTimeTolerance")
    parser.add_argument("--postTimeTolerance", default=2000, help="the postTimeTolerance")

    return parser

TOTALPMTS = {"Acrylic": 17746, "Balloon": 18306}

def config_detsim_task_nosim(task):
    # = I/O Related =
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

def create_elec_sim_alg(task):
    # = second task =
    sec_task = task.createTask("Task/detsimtask")
    config_detsim_task_nosim(sec_task)
    # = elec sim =
    Sniper.loadDll("libElecSim.so")
    elecsim = task.createAlg("ElecSimAlg")
    elecsim.property("PmtTotal").set(total_pmt)
    import os
    elecroot = os.environ["ELECSIMROOT"]
    pmt_data = os.path.join(elecroot, "share", "PmtData.root")
    elecsim.property("PmtDataFile").set(pmt_data)
    elecsim.property("split_evt_time").set(args.splittime)
    elecsim.property("enableAfterPulse").set(args.enableAfterPulse)
    elecsim.property("enableDarkPulse").set(args.enableDarkPulse)

    elecsim.property("enableOvershoot").set(args.enableOvershoot)
    elecsim.property("enableRinging").set(args.enableRinging)
    elecsim.property("enableSatuation").set(args.enableSatuation)
    elecsim.property("enableNoise").set(args.enableNoise)
    elecsim.property("enablePretrigger").set(args.enablePretrigger)

    elecsim.property("simFrequency").set(args.simFrequency)
    elecsim.property("noiseAmp").set(args.noiseAmp)
    elecsim.property("speAmp").set(args.speAmp)
    elecsim.property("preTimeTolerance").set(args.preTimeTolerance)
    elecsim.property("postTimeTolerance").set(args.postTimeTolerance)

    return elecsim

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()

    total_pmt = TOTALPMTS[args.detoption]

    Sniper.setLogLevel(0)
    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(args.evtmax)
    #task.setEvtMax(1)
    task.setLogLevel(0)

    # = I/O Related =
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # == Output Svc ==
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {"/Event/Calib":args.output,
                  }
    roSvc.property("OutputStreams").set(outputdata)
    # == root writer ==
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    rootwriter.property("Output").set({"TEMPLFIT":"/workfs/dyw/chengyp/newjuno/offline/Reconstruction/TemplateFitAlg/share/evt.root"})

    # == elec sim ==
    elecsimalg = create_elec_sim_alg(task)
    # == template fit alg ==
    Sniper.loadDll("libTemplateFitAlg.so")
    templatefitalg = task.createAlg("TemplateFitAlg")
    templatefitalg.property("InputRootFile").set("")
    templatefitalg.property("Chi2tolerance").set(2.5)

    # = begin run =
    task.show()
    task.run()
