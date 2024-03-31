#!/usr/bin/env python
#-*- coding: utf-8 -*-
from __future__ import print_function

import sys
import os
import logging
from .JUNOModule import JUNOModule
from .JUNOUtils import mh

class JUNOElecSimModule(JUNOModule):

    def __init__(self):
        pass

    def setup_profiling(self, task):
        import SniperProfiling
        task.createSvc("SniperProfiling")

    def register_options(self, parser):
        self.register_options_common(parser)
        self.register_options_input_and_rate(parser)
        self.register_options_detector_components(parser)

        self.register_options_supernova(parser)

        self.register_options_others(parser)
        pass

    def add_output_vec(self, output_vec, args):
        # generator and detector simulation
        output_vec.append("/Event/Gen")
        output_vec.append("/Event/Sim")

        # # electronics simulation
        output_vec.append("/Event/CdWaveform")
        output_vec.append("/Event/CdLpmtElec")
        output_vec.append("/Event/CdSpmtElec")

        output_vec.append("/Event/WpWaveform")
        output_vec.append("/Event/WpElec")

        output_vec.append("/Event/TtElec")

        output_vec.append("/Event/CdTrigger")
        output_vec.append("/Event/WpTrigger")
        # output_vec.append("/Event/TtTrigger")

        # # truth
        output_vec.append("/Event/CdLpmtTruth")
        output_vec.append("/Event/CdSpmtTruth")
        output_vec.append("/Event/WpTruth")
        # output_vec.append("/Event/TtTruth")
        output_vec.append("/Event/TrackTruth")
        pass

    def add_user_output_vec(self, user_output_vec, args):
        if "USER_OUTPUT" not in user_output_vec:
            user_output_vec.append("USER_OUTPUT")
        pass

    def init(self, toptask, args):

        self.init_common(toptask, args)
        self.init_input_and_rate(toptask, args)
        self.init_juno_timer(toptask, args)
        self.init_frontier(toptask, args)
        self.init_elec_sim_services(toptask, args)

        self.init_elec_sim_alg(toptask, args)
        pass

    ##########################################################################
    # Split the arguments into different categories
    ##########################################################################
    def register_options_common(self, parser):
        parser.add_argument("--seed", type=int, default=5, help='seed')
        parser.add_argument("--start", default="1970-01-01 00:00:00", help='starting time')#year, month, day, h, min, s; don't chang the format
        parser.add_argument("--end", default="2014-09-01 12:05:00", help='ending time')#year, month, day, h, min, s

        parser.add_argument("--run-number", type=int, default=0, help='run number (note: for simulation, it should be negative)')
        parser.add_argument("--start-event-number", type=int, default=0, help='start event number (>=0)')

    
        parser.add_argument('--enableUserRootFile', dest='enableUserRootFile', action='store_true')
        parser.add_argument('--disableUserRootFile', dest='enableUserRootFile', action='store_false')
        parser.set_defaults(enableUserRootFile=True)
    

    def register_options_input_and_rate(self, parser):
        # parser.add_argument("--input", action="append", help="input file name for TAG: IBD:ibd.root, U:ibd.root")
        parser.add_argument("--splitted-input", action="append", help="enable splitted input for TAG: IBD:0 for non-split input, Muon:1 for split input")
        parser.add_argument("--rate", action="append", # default="1.0", Hz
                            help="rate for TAG: IBD:1.0")
        parser.add_argument("--startidx", action="append", # default is 0.
                            help="startidx for TAG. example: IBD:0")
        parser.add_argument("--loop", action="append", # default is 0.
                            help="Whether circularly for TAG, 0 for disable loop, 1 for enable. example: --loop IBD:0 --loop U:1")
        parser.add_argument("--savetruths", action="append",#default is 1.
                            help="save TAG electronics simulation truths or not. example: IBD:1")

    def register_options_detector_components(self, parser):
        parser.add_argument("--TotalLpmt", type=int, default=17613,  help="total pmt number")

        parser.add_argument('--enableLPMT', dest='enableLPMT', action='store_true')
        parser.add_argument('--disableLPMT', dest='enableLPMT', action='store_false')
        parser.set_defaults(enableLPMT=True)
        parser.add_argument('--enableSPMT', dest='enableSPMT', action='store_true')
        parser.add_argument('--disableSPMT', dest='enableSPMT', action='store_false')
        parser.set_defaults(enableSPMT=True)
        parser.add_argument('--enableTT', dest='enableTT', action='store_true')
        parser.add_argument('--disableTT', dest='enableTT', action='store_false')
        parser.set_defaults(enableTT=True)
        parser.add_argument('--enableWP', dest='enableWP', action='store_true')
        parser.add_argument('--disableWP', dest='enableWP', action='store_false')
        parser.set_defaults(enableWP=False)
    
    def register_options_supernova(self, parser):
        parser.add_argument('--enableSNMode', dest='enableSNMode', action='store_true')
        parser.add_argument('--disableSNMode', dest='enableSNMode', action='store_false')
        parser.set_defaults(enableSNMode=False)

    def register_options_others(self, parser):
        parser.add_argument("--darkRateScale", default=1., type=float, help="scale of LPMT DCR");
        parser.add_argument("--darkRateScale_SPMT", default=1., type=float, help="scale of SPMT DCR");
        parser.add_argument("--darkRateScale_WP", default=1., type=float, help="scale of WP_PMT DCR");
    
        parser.add_argument('--enableEfficiency', dest='enableEfficiency', action='store_true')
        parser.add_argument('--disableEfficiency', dest='enableEfficiency', action='store_false')
        parser.set_defaults(enableEfficiency=False)
    
        parser.add_argument('--enableAfterPulse', dest='enableAfterPulse', action='store_true')
        parser.add_argument('--disableAfterPulse', dest='enableAfterPulse', action='store_false')
        parser.set_defaults(enableAfterPulse=True)
    
        parser.add_argument('--enableMergeLPMTPulse', dest='enableMergeLPMTPulse', action='store_true')
        parser.add_argument('--disableMergeLPMTPulse', dest='enableMergeLPMTPulse', action='store_false')
        parser.set_defaults(enableMergeLPMTPulse=False)
    
        parser.add_argument('--enableDarkPulse', dest='enableDarkPulse', action='store_true')
        parser.add_argument('--disableDarkPulse', dest='enableDarkPulse', action='store_false')
        parser.set_defaults(enableDarkPulse=True)

        parser.add_argument('--enableSaveDarkPulseTruths', dest='enableSaveDarkPulseTruths', action='store_true')
        parser.add_argument('--disableSaveDarkPulseTruths', dest='enableSaveDarkPulseTruths', action='store_false')
        parser.set_defaults(enableSaveDarkPulseTruths=True)

        parser.add_argument('--enableSaveTruths', dest='enableSaveTruths', action='store_true')
        parser.add_argument('--disableSaveTruths', dest='enableSaveTruths', action='store_false')
        parser.set_defaults(enableSaveTruths=True)


    
        parser.add_argument('--enablePmtTTS', dest='enablePmtTTS', action='store_true')
        parser.add_argument('--disablePmtTTS', dest='enablePmtTTS', action='store_false')
        parser.set_defaults(enablePmtTTS=True)
    
        parser.add_argument('--enableDynodeTTS2theta', dest='enableDynodeTT2theta', action='store_true')
        parser.add_argument('--disableDynodeTTS2theta', dest='enableDynodeTTS2theta', action='store_false')
        parser.set_defaults(enableDynodeTTS2theta=True)
    
        parser.add_argument('--enableMCPTTS2theta', dest='enableMCPTT2theta', action='store_true')
        parser.add_argument('--disableMCPTTS2theta', dest='enableMCPTTS2theta', action='store_false')
        parser.set_defaults(enableMCPTTS2theta=True)
    
        parser.add_argument('--enableDynodeTT2theta', dest='enableDynodeTT2theta', action='store_true')
        parser.add_argument('--disableDynodeTT2theta', dest='enableDynodeTT2theta', action='store_false')
        parser.set_defaults(enableDynodeTT2theta=True)
    
        parser.add_argument('--enableMCPTT2theta', dest='enableMCPTT2theta', action='store_true')
        parser.add_argument('--disableMCPTT2theta', dest='enableMCPTT2theta', action='store_false')
        parser.set_defaults(enableMCPTT2theta=True)
    
        parser.add_argument('--enableWPDarkPulse', dest='enableWPDarkPulse', action='store_true')
        parser.add_argument('--disableWPDarkPulse', dest='enableWPDarkPulse', action='store_false')
        parser.set_defaults(enableWPDarkPulse=False)
    
        parser.add_argument('--enableMCPLargeSignal', dest='enableMCPLargeSignal', action='store_true')
        parser.add_argument('--disableMCPLargeSignal', dest='enableMCPLargeSignal', action='store_false')
        parser.set_defaults(enableMCPLargeSignal=True)

        parser.add_argument("--enableChargeSmearing", dest="enableChargeSmearing", action='store_true')
        parser.add_argument("--disableChargeSmearing", dest="enableChargeSmearing", action='store_false')
        parser.set_defaults(enableChargeSmearing=True)
    
        parser.add_argument("--evtBufferLength", default=5000, type=float, help="the event BufferLength")
        parser.add_argument("--nHitsThreshold", default=200, type=float, help="the nHits Threshold")
    
        parser.add_argument("--enableTriggerByGroup", dest="enableTriggerByGroup", action="store_true")
        parser.add_argument("--disableTriggerByGroup", dest="enableTriggerByGroup", action="store_false")
        parser.set_defaults(enableTriggerByGroup=True)

        parser.add_argument("--LpmtTrigger_alg", default="MC-based", choices=["MC-based", "real-logic", "MM-trigger"], help="Set Lpmt Trigger Tool")
        parser.add_argument("--WPLpmtTrigger_alg", default="WP-logic", choices=["WP-logic"], help="Set WPLpmt Trigger Tool")
        parser.add_argument("--enableForceTrigger",  dest="enableForceTrigger",  action="store_true")
        parser.add_argument("--disableForceTrigger", dest="enableForceTrigger", action="store_false")
        parser.set_defaults(enableForceTrigger=False)
        parser.add_argument("--enableLaserTrigger",  dest="enableLaserTrigger",  action="store_true")
        parser.add_argument("--disableLaserTrigger", dest="enableLaserTrigger", action="store_false")
        parser.set_defaults(enableLaserTrigger=False)
        parser.add_argument("--enableMultiLaserLoad",  dest="enableMultiLaserLoad",  action="store_true")
        parser.add_argument("--disableMultiLaserLoad", dest="enableMultiLaserLoad", action="store_false")
        parser.set_defaults(enableMultiLaserLoad=False)
        parser.add_argument("--nLaserEventLoad", type=int, default=1,  help="Number of Laser events loaded into one timing window.")
        
        parser.add_argument("--enableTimeOffsetCorrection",  dest="enableTimeOffsetCorrection", action="store_true")
        parser.add_argument("--disableTimeOffsetCorrection", dest="enableTimeOffsetCorrection", action="store_false")
        parser.set_defaults(enableTimeOffsetCorrection=False)
    
        parser.add_argument("--Trigger_FiredPmtNum", type=float, default=200,  help="Trigger Num threshold")
        parser.add_argument("--Trigger_window", type=float, default=80,  help="trigger window default 80ns")
        parser.add_argument("--Trigger_slip_window", type=float, default=16,  help="slip window default 16ns")
    
        parser.add_argument("--StdTrigger_FiredPmtNum", type=float, default=300,  help="Standard Trigger Num threshold default 300")
        parser.add_argument("--StdTrigger_window", type=float, default=300,  help="Standard trigger window default 300ns")
        parser.add_argument("--StdTrigger_slip_window", type=float, default=16,  help="Standard slip window default 16ns")
        parser.add_argument("--VFLTrigger_FiredPmtNum", type=float, default=100,  help="VFL Trigger Num threshold default 100")
        parser.add_argument("--VFLTrigger_window", type=float, default=48,  help="VFL trigger window default 48ns")
        parser.add_argument("--VFLTrigger_slip_window", type=float, default=16,  help="VFL slip window default 16ns")
        parser.add_argument("--VFLTrigger_ConfFile", default="default", help="179 thresholds and windows for hardware trigger")
        parser.add_argument("--Trigger_Mode", default="OR-ed", choices=["onlyStd", "onlyVFL", "OR-ed"], help="Trigger Mode Option")
    
    
        parser.add_argument("--Trigger_FiredPmtNum_WP", type=float, default=17,  help="Trigger Num threshold for single region in WP")
        parser.add_argument("--Trigger_FiredPmtNum_WP_low", type=float, default=13,  help="Trigger Num low threshold for adjacent region in WP")
        parser.add_argument("--Trigger_window_WP", type=float, default=240,  help="trigger window default 240ns WP")
        parser.add_argument("--Trigger_slip_window_WP", type=float, default=16,  help="slip window default 16ns WP")
    
        parser.add_argument('--enableOvershoot', dest='enableOvershoot', action='store_true')
        parser.add_argument('--disableOvershoot', dest='enableOvershoot', action='store_false')
        parser.set_defaults(enableOvershoot=True)
    
        parser.add_argument('--enableSaturation', dest='enableSaturation', action='store_true')
        parser.add_argument('--disableSaturation', dest='enableSaturation', action='store_false')
        parser.set_defaults(enableSaturation=False)
    
        parser.add_argument('--enableNoise', dest='enableNoise', action='store_true')
        parser.add_argument('--disableNoise', dest='enableNoise', action='store_false')
        parser.set_defaults(enableNoise=True)
    
        parser.add_argument('--enableFADC', dest='enableFADC', action='store_true')
        parser.add_argument('--disableFADC', dest='enableFADC', action='store_false')
        parser.set_defaults(enableFADC=True)

        parser.add_argument('--enableCDLpmtTQ',dest='enableCDLpmtTQ', action='store_true')
        parser.add_argument('--disableCDLpmtTQ',dest='enableCDLpmtTQ', action='store_false')
        parser.set_defaults(enableCDLpmtTQ=True)

        parser.add_argument('--enableTagNotOverThresholdTQ',dest='enableTagNotOverThresholdTQ', action='store_true')
        parser.add_argument('--disableTagNotOverThresholdTQ',dest='enableTagNotOverThresholdTQ', action='store_false')
        parser.set_defaults(enableTagNotOverThresholdTQ=False)

        parser.add_argument('--enableSplitTrackRange',dest='enableSplitTrackRange',action='store_true')
        parser.add_argument('--disableSplitTrackRange',dest='enableSplitTrackRange',action='store_false')
        parser.set_defaults(enableSplitTrackRange=False)
    
        # Note: please only use split input mode for muon. For others, always use non-split input.
        # To enable split input mode, use --splitted-input Muon:1
        parser.add_argument('--enableSplitInput', dest='enableSplitInput', action='store_true')
        parser.add_argument('--disableSplitInput', dest='enableSplitInput', action='store_false')
        parser.set_defaults(enableSplitInput=False)

        parser.add_argument('--preTimeTolerance',  type=float, default=300, help="Pre trigger readout window")
        parser.add_argument('--postTimeTolerance', type=float, default=700, help="Post trigger readout window")
        #parser.add_argument('--preWaveSimTolerance', type=float, default=500, help="Pre wave sim window")

        parser.add_argument('--FadcOffset',  type=float, default=0.25, help="Offset of waveform baseline")

        parser.add_argument("--enableStoreElecTruthEDM", dest="storeElecTruthEDM", action="store_true", help="Store the ElecTruthEDM in TriggerHandlerLpmt")
        parser.add_argument("--disableStoreElecTruthEDM", dest="storeElecTruthEDM", action="store_false", help="Don't Store the ElecTruthEDM in TriggerHandlerLpmt")
        parser.set_defaults(storeElecTruthEDM=True)

        parser.add_argument("--enableTriggerSPMTbyLPMT",  dest="enableTriggerSPMTbyLPMT",  action="store_true")
        parser.add_argument("--disableTriggerSPMTbyLPMT", dest="enableTriggerSPMTbyLPMT", action="store_false")
        parser.set_defaults(enableTriggerSPMTbyLPMT=True)

        parser.add_argument("--enableGen",  dest="enableGen",  action="store_true")
        parser.add_argument("--disableGen", dest="enableGen", action="store_false")
        parser.set_defaults(enableGen=True)

        parser.add_argument("--enableOEC",dest="enableOEC",action = "store_true")
        parser.add_argument("--disenableOEC",dest="enableOEC",action = "store_false")
        parser.set_defaults(enableOEC=False)
    ##########################################################################
    # init
    ##########################################################################
    def init_common(self, toptask, args):
        # check PmtData
        import os
        import os.path


    def init_input_and_rate(self, task_top, args):
        filelists = {}
        ratelists = {}
        savetruthslists = {}
        startidxlist = {}
        looplist = {}
        splittedinputlist = {}
    
        if args.input is None:
            args.input = [ "sample_detsim.root" ]
        if args.rate is None:
            args.rate = [ "1.0" ]
        if args.startidx is None:
            args.startidx = ["0"]
        if args.loop is None:
            args.loop = ["0"]
        if args.savetruths is None:
            args.savetruths = ["1"]
        if args.splitted_input is None:
            args.splitted_input = ["0"]

    
        for f in args.input:
            kv = f.split(":", 1)
            if len(kv) == 1: kv.insert(0, "default")
            k, v = kv
    
            if v.endswith(".root"):
                filelists.setdefault(k, [])
                filelists[k].append(v)
            elif v.endswith(".txt"):
                filelists.setdefault(k, [])
                import os.path
                if not os.path.exists(v):
                    import sys
                    print("ERROR: Can't find file:",v)
                    sys.exit(-1)
                with open(v) as f:
                    for line in f:
                        line = line.strip()
                        filelists[k].append(line)
            else:
                import sys
                print("ERROR: Incorrect file type:",v)
                sys.exit(-1)

        for r in args.rate:
            kv = r.split(":")
            if len(kv) == 1: kv.insert(0, "default")
            k, v = kv
            if k in ratelists:
                print("ERROR: Don't allow specify different rate values for same tag")
                import sys
                sys.exit(-1)
            ratelists[k] = float(v)

        for t in args.savetruths:
            kv = t.split(":")
            if len(kv) == 1: kv.insert(0, "default")
            k, v = kv
            if k in savetruthslists:
                print("ERROR: Don't allow specify different savetruths values for same tag")
                import sys
                sys.exit(-1)
            savetruthslists[k] = int(v)

    
    
        for i in args.startidx:
            kv = i.split(":")
            if len(kv) == 1: kv.insert(0, "default")
            k, v = kv
            # for rate, we should avoid different values for same tag
            if k in startidxlist:
                print("ERROR: Don't allow specify different start index for same tag")
                import sys
                sys.exit(-1)
            # convert from string to float
            startidxlist[k] = int(v)
    
    
        for c in args.loop:
            kv = c.split(":")
            if len(kv) == 1: kv.insert(0,"default")
            k, v = kv
            if k in looplist:
                print("ERROR: Don't allow specify different loop option for same tag")
                import sys
                sys.exit(-1)
            looplist[k] = int(v)

        for c in args.splitted_input:
            kv = c.split(":")
            if len(kv) == 1: kv.insert(0,"default")
            k, v = kv
            if k in splittedinputlist:
                print("ERROR: Don't allow specify multiple splitted_input option for same tag")
                import sys
                sys.exit(-1)
            splittedinputlist[k] = int(v)
    
    
        samples = []
        for tag, fl in list(filelists.items()):
            d = {}
            d["name"] = tag
            d["input"] = fl
            if tag not in ratelists:
                print(("ERROR: missing rate for tag: %s"%tag))
                import sys
                sys.exit(-1)
            d["rate"] = ratelists[tag]

            if tag not in savetruthslists:
                d["savetruths"] = 1
            else:
                d["savetruths"] = savetruthslists[tag]

            if tag in looplist:
                d["loop"] = looplist[tag]
            if tag in startidxlist:
                d["start_index"] = startidxlist[tag]

            if tag in splittedinputlist:
                d["splitted_input"] = splittedinputlist[tag]
            else:
                d["splitted_input"] = 0 # use non-split input service by default

            samples.append(d)
    
        print(samples)
        
        sampleNum = len(samples)
        taskList=[]
        splitList = []
        inputList={}
        self.rateMap={}
        self.taskMap={}
        self.startidxMap = {}
        self.savetruthsMap = {}
        self.loopMap = {}
        for i in range(sampleNum):
            key_sample_name = (samples[i]["name"])
            rate = (samples[i]["rate"])
            savetruths = bool(samples[i]["savetruths"])
            inputList[i] = samples[i]["input"]
            taskList.append(samples[i]["name"])
            self.rateMap[key_sample_name]=rate
            self.savetruthsMap[key_sample_name] = savetruths
            self.taskMap[key_sample_name]=taskList[i]
            if "start_index" in samples[i]:
                self.startidxMap[key_sample_name] = samples[i]["start_index"]
            if "loop" in samples[i]:
                self.loopMap[key_sample_name] = samples[i]["loop"]
            splitList.append(samples[i]["splitted_input"])
    
        #add input task
        subTask={}
        readin={}

        import RootIOSvc
        import BufferMemMgr

        # Show the message when user use enableSplitInput
        if args.enableSplitInput:
            print("ERROR: option --enableSplitInput should be replaced by --splitted-input TAG:1")
            import sys
            sys.exit(-1)

        for i in range(sampleNum):
            subTask[i] = task_top.createTask("Task/"+taskList[i])    #taskList[2]: sample2
            #subTask[i].setLogLevel(0)
            if splitList[i]:
                import RootIOTools
                readin[i]=subTask[i].createSvc("MergingInputSvc/InputSvc")
                readin[i].property("MergingEventType").set("SimEvent")
                readin[i].property("MergingEventPath").set("/Event/Sim")
                print("Using split input mode for %s"%taskList[i])
            else:
                readin[i]=subTask[i].createSvc("RootInputSvc/InputSvc")
                print("Using non-split input mode for %s"%taskList[i])
            readin[i].property("InputFile").set(inputList[i])
            bufMgrTmp=subTask[i].createSvc("BufferMemMgr")
            stop_opt = 1 # 0: stop global, 1: stop local 
            bufMgrTmp.property("StopTaskOpt").set(stop_opt)


    def init_juno_timer(self, task_top, args):
    
        try:
            import JunoTimer
            task_top.createSvc("JunoTimerSvc")
        except:
            pass


    def init_frontier(self, task_top, args):
        # = Frontier Svc = #
        if (args.pmtsimparam_dbtype=="Frontier"):
            import FrontierSvc
            frontiersvc = task_top.createSvc("FrontierSvc")
            frontiersvc.property("ServerUrls").set(["http://junodb1.ihep.ac.cn:8080/Frontier"])
            frontiersvc.property("ProxyUrls").set(["http://squid-01.ihep.ac.cn:3128"])

    
    def init_elec_sim_services(self, task_top, args):
        import ElecSimAlg
        elecsimsvc = task_top.createSvc("ElecSimSvc")
        elecsimsvc.property("start").set(args.start)
        elecsimsvc.property("end").set(args.end)
        elecsimsvc.property("TriggerByGroup").set(args.enableTriggerByGroup)

        elecsimsvc.property("RunID").set(args.run_number)
        elecsimsvc.property("EventID").set(args.start_event_number)
       
        elecsimsvc.property("preTimeTolerance").set(args.preTimeTolerance)
        elecsimsvc.property("postTimeTolerance").set(args.postTimeTolerance)

        abcboardsvc = task_top.createSvc("AbcBoardSvc")
    

    def init_elec_sim_alg(self, task_top, args):
        task = task_top
        if(args.enableOEC):
            task = task_top.find("input_Task")
        elecsimalg = task.createAlg("ElecSimAlg")
        elecsimalg.property("TriggerSPMTbyLPMT").set(args.enableTriggerSPMTbyLPMT)
        elecsimalg.property("Gen").set(args.enableGen)
        
        
        # register handler
        #elecsimalg.createTool("TriggerHandlerLpmt")
        readoutspmt = elecsimalg.createTool("TriggerHandlerSpmt")
        readoutspmt.property("storeElecTruth").set(args.storeElecTruthEDM)

        elecsimalg.createTool("TriggerHandlerTT")
        
    
        # = task for trigger simulation =
        task_trigger = task_top.createTask("Task/TriggerSimTask")

        if args.profiling:
            self.setup_profiling(task_trigger)

        triggersimalg = task_trigger.createAlg("TriggerSimAlg")
        triggersimalg.property("LpmtTrigger_alg").set(args.LpmtTrigger_alg)
        triggersimalg.property("TriggerSPMTbyLPMT").set(args.enableTriggerSPMTbyLPMT)

        if not args.enableTriggerSPMTbyLPMT:
            triggersimalg.createTool("TriggerToolSpmt")
        
        if args.LpmtTrigger_alg == "MC-based":
            triggerLpmt = triggersimalg.createTool("TriggerToolLpmt")
            triggerLpmt.property("Trigger_FiredPmtNum").set(args.Trigger_FiredPmtNum)
            triggerLpmt.property("Trigger_window").set(args.Trigger_window)
            triggerLpmt.property("Trigger_slip_window").set(args.Trigger_slip_window)
    
        if args.LpmtTrigger_alg == "real-logic":
            triggerLpmt_VFL = triggersimalg.createTool("TriggerVFLToolLpmt")
            triggerLpmt_VFL.property("StdTrigger_FiredPmtNum").set(args.StdTrigger_FiredPmtNum)
            triggerLpmt_VFL.property("StdTrigger_window").set(args.StdTrigger_window)
            triggerLpmt_VFL.property("StdTrigger_slip_window").set(args.StdTrigger_slip_window)
            triggerLpmt_VFL.property("VFLTrigger_FiredPmtNum").set(args.VFLTrigger_FiredPmtNum)
            triggerLpmt_VFL.property("VFLTrigger_window").set(args.VFLTrigger_window)
            triggerLpmt_VFL.property("VFLTrigger_ConfFile").set(args.VFLTrigger_ConfFile)
            triggerLpmt_VFL.property("VFLTrigger_slip_window").set(args.VFLTrigger_slip_window)
            triggerLpmt_VFL.property("Trigger_Mode").set(args.Trigger_Mode)
            triggerLpmt_VFL.property("doForceTrigger").set(args.enableForceTrigger)
            triggerLpmt_VFL.property("doLaserTrigger").set(args.enableLaserTrigger)
            triggerLpmt_VFL.property("doTimeOffsetCorrection").set(args.enableTimeOffsetCorrection)
        
        if args.LpmtTrigger_alg == "MM-trigger":
            triggerLpmt = triggersimalg.createTool("MMTriggerToolLpmt")
            triggerLpmt.property("Trigger_FiredPmtNum").set(args.Trigger_FiredPmtNum)
            triggerLpmt.property("Trigger_window").set(args.Trigger_window)
            triggerLpmt.property("Trigger_slip_window").set(args.Trigger_slip_window)
            triggerLpmt.property("darkRateScale").set(args.darkRateScale)
    
        
        # = task for WP trigger simulation
        triggerWPpmt = triggersimalg.createTool("TriggerToolWPLpmt")
        triggerWPpmt.property("Trigger_FiredPmtNum_WP").set(args.Trigger_FiredPmtNum_WP)
        triggerWPpmt.property("Trigger_FiredPmtNum_WP_low").set(args.Trigger_FiredPmtNum_WP_low)
        triggerWPpmt.property("Trigger_window_WP").set(args.Trigger_window_WP)
        triggerWPpmt.property("Trigger_slip_window_WP").set(args.Trigger_slip_window_WP)
        
        triggerTT = triggersimalg.createTool("TriggerToolTT")
        
        # = task for pmt simulation =
        task_pmtsim = task_top.createTask("Task/PMTSimTask")
        import Geometry
        tt_geom_svc = task_pmtsim.createSvc("TTGeomSvc")

        if args.profiling:
            self.setup_profiling(task_pmtsim)

        pmtsimalg = task_pmtsim.createAlg("PMTSimAlg")
        pmtsimalg.property("Seed").set(args.seed)
        pmtsimalg.property("TotalLpmt").set(args.TotalLpmt)
        pmtsimalg.property("enableLPMT").set(args.enableLPMT)
        pmtsimalg.property("enableSPMT").set(args.enableSPMT)
        pmtsimalg.property("enableWP").set(args.enableWP)
        pmtsimalg.property("enableTT").set(args.enableTT)
        
        pmtsimalg.property("enableSNMode").set(args.enableSNMode)
        pmtsimalg.property("enableEfficiency").set(args.enableEfficiency)
        pmtsimalg.property("enableAfterPulse").set(args.enableAfterPulse)
        pmtsimalg.property("enableDarkPulse").set(args.enableDarkPulse)
        pmtsimalg.property("enablePmtTTS").set(args.enablePmtTTS)
        pmtsimalg.property("enableDynodeTTS2theta").set(args.enableDynodeTTS2theta)
        pmtsimalg.property("enableMCPTTS2theta").set(args.enableMCPTTS2theta)
        pmtsimalg.property("enableDynodeTT2theta").set(args.enableDynodeTT2theta)
        pmtsimalg.property("enableMCPTT2theta").set(args.enableMCPTT2theta)
        pmtsimalg.property("enableWPDarkPulse").set(args.enableWPDarkPulse)
        pmtsimalg.property("enableMergeLPMTPulse").set(args.enableMergeLPMTPulse)
        pmtsimalg.property("enableChargeSmearing").set(args.enableChargeSmearing)
        pmtsimalg.property("enableSaveDarkPulseTruths").set(args.enableSaveDarkPulseTruths)
        pmtsimalg.property("enableSaveTruths").set(args.enableSaveTruths)
        if not args.enableChargeSmearing:
            args.enableMCPLargeSignal = False
        pmtsimalg.property("enableMCPLargeSignal").set(args.enableMCPLargeSignal)
        pmtsimalg.property("darkRateScale").set(args.darkRateScale)
        pmtsimalg.property("darkRateScale_SPMT").set(args.darkRateScale_SPMT)
        pmtsimalg.property("darkRateScale_WP").set(args.darkRateScale_WP)
        pmtsimalg.property("nHitsThreshold").set(args.nHitsThreshold)
        pmtsimalg.property("evtBufferLength").set(args.evtBufferLength)
        pmtsimalg.property("RateMap").set(self.rateMap)
        pmtsimalg.property("TaskMap").set(self.taskMap)
        pmtsimalg.property("LoopMap").set(self.loopMap)
        pmtsimalg.property("StartIdxMap").set(self.startidxMap)
        pmtsimalg.property("SaveTruthsMap").set(self.savetruthsMap)

        pmtsimalg.property("doForceTrigger").set(args.enableForceTrigger)
        pmtsimalg.property("doLaserTrigger").set(args.enableLaserTrigger)
        pmtsimalg.property("enableMultiLaserLoad").set(args.enableMultiLaserLoad)
        pmtsimalg.property("nLaserEventLoad").set(args.nLaserEventLoad)
        
        WaveAlg = elecsimalg.createTool("TriggerHandlerLpmt")
        WaveAlg.property("enableOvershoot").set(args.enableOvershoot)
        WaveAlg.property("enableSaturation").set(args.enableSaturation)
        WaveAlg.property("enableNoise").set(args.enableNoise)
        WaveAlg.property("enableFADC").set(args.enableFADC)
        WaveAlg.property("enableCDLpmtTQ").set(args.enableCDLpmtTQ)
        WaveAlg.property("enableTagNotOverThresholdTQ").set(args.enableTagNotOverThresholdTQ)
        WaveAlg.property("enableSplitTrackRange").set(args.enableSplitTrackRange)
        WaveAlg.property("preTimeTolerance").set(args.preTimeTolerance)
        WaveAlg.property("postTimeTolerance").set(args.postTimeTolerance)
        WaveAlg.property("FadcOffset").set(args.FadcOffset)
        #WaveAlg.property("preWaveSimWindow").set(args.preWaveSimTolerance)
        WaveAlg.property("storeElecTruth").set(args.storeElecTruthEDM)

        # = for WP PMT
        WaveAlg = elecsimalg.createTool("TriggerHandlerWPLpmt")
        WaveAlg.property("enableOvershoot").set(args.enableOvershoot)
        WaveAlg.property("enableSaturation").set(args.enableSaturation)
        WaveAlg.property("enableNoise").set(args.enableNoise)
        WaveAlg.property("enableFADC").set(args.enableFADC)
        WaveAlg.property("storeElecTruth").set(args.storeElecTruthEDM)
        
        # we create two instances of PMTTool for LPMT and SPMT
        # their properties will be configured separately.
        lpmttool = pmtsimalg.createTool("PMTTool/LPMTTool")
        spmttool = pmtsimalg.createTool("SPMTTool/SPMTTool")
        ttpmttool = pmtsimalg.createTool("PMTTool/MaPMTTool")
        #spmttool.setLogLevel(0)
