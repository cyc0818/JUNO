#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: huangxin

import sys
import os
import logging
from Tutorial import JUNOModule

class JUNOOECTagModule(JUNOModule):
    def __init__(self):
        self.oectagsvc = None
        self.optRegistered = False
        pass

    def register_options(self, parser):
        if not self.optRegistered:
            parser.add_argument("--config-tags", default=os.getenv('OECTAGSVCROOT')+"/share/tag.json", help="The configuration json file for single tag condition")
            self.optRegistered = True
        pass

    def init(self, toptask, args):
        import OECTagSvc
        import Sniper
        if self.oectagsvc is None:
            self.oectagsvc = Sniper.create('SharedElem<OECTagSvc>/OECTagSvc')
            self.oectagsvc.property("OECTagFile").set(args.config_tags)
            pass
        toptask.addSvc(self.oectagsvc)
        pass
    pass

class JUNOOECInputModule(JUNOModule):
    def __init__(self):
        pass

    def init(self, toptask, args):
        import BufferMemMgr
        bufmgr = toptask.createSvc('BufferMemMgr')
        bufmgr.property("StopTaskOpt").set(1)
        pass
    pass

class JUNOOECTopModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--elecsim", dest="useElecSim", action="store_true", help="perform electronic simulation")
        parser.add_argument("--no-elecsim", dest="useElecSim", action="store_false", help="do not perform electronic simulation (default is disabled)")
        parser.set_defaults(useElecSim=False)
        parser.add_argument("--waverec", dest="useWaveRec", action="store_true", help="perform waveform reconstruction algorithms")
        parser.add_argument("--no-waverec", dest="useWaveRec", action="store_false", help="disable waveform reconstruction algorithms, reading from input file instead.")
        parser.set_defaults(useWaveRec=True)
        parser.add_argument("--evtrec", dest="useEventRec", action="store_true", help="perform event reconstruction algorithms")
        parser.add_argument("--no-evtrec", dest="useEventRec", action="store_false", help="disable event reconstruction algorithms, reading from input file instead.")
        parser.set_defaults(useEventRec=True)
        pass

    def add_output_vec(self, output_vec, args):
        output_vec.append('/Event/Oec') #OecEvt
        output_vec.extend(['/Event/CdTrigger', '/Event/WpTrigger', '/Event/TtTrigger']) #trigger
        output_vec.extend(['/Event/CdWaveform', '/Event/WpWaveform']) #Waveform
        output_vec.extend(['/Event/CdLpmtElec_FPGA', '/Event/CdLpmtElec', '/Event/CdSpmtElec', '/Event/WpElec']) #T/Q
        output_vec.extend(['/Event/Gen', '/Event/Sim']) #MC truth
        output_vec.extend(['/Event/CdLpmtTruth', '/Event/CdSpmtTruth', '/Event/TrackTruth', '/Event/WpTruth', '/Event/TtTruth']) #Elec truth
        pass

    def add_user_output_vec(self, user_output_vec, args):
        user_output_vec = ['USER_OUTPUT', 'TIME_REC']
        pass

    def init(self, toptask, args):
        if not args.useElecSim:
            import JunoTimer
            toptask.createSvc("JunoTimerSvc")
            if args.input is not None:
                args.input = args.input[0]
        else:
            args.enableOEC = True

        # if in multi-threading mode, disable OECProcessorAlg
        if "nthreads" not in args:
            import OECProcessor
            oecprocessoralg=toptask.createAlg("OECProcessorAlg")
            oecprocessoralg.property("TimeWindow").set([-0.01, 0.01])
        else: # multi-threaded
            oecpgba = toptask.createAlg("OECPruneGBAlg")
            oecpgba.property("TimeWindow").set([-0.01, 0.01])


        if args.useEventRec:
            import QCtrRecAlg
            import FBRalg
            import OMILREC
            import RecTimeLikeAlg
            import RecCdMuonAlg
            import CdLpmtQSpecMuonRecTool

            if not args.config_seq:
                args.config_seq = os.getenv('EVTSTEERINGROOT')+"/share/seq_withrec.json"
        else:
            import readFromRecAlg
            if not args.config_seq:
                args.config_seq = os.getenv('EVTSTEERINGROOT')+"/share/seq_withoutrec.json"
        pass
    pass

class JUNOLECModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--config-seq", help="The configuration json file for sequences")
        parser.add_argument("--config-sig", default=os.getenv('EVTSTEERINGROOT')+"/share/sig.json", help="The configuration json file for tag condition")

    def add_user_output_vec(self, user_output_vec, args):
        if "TIME_REC" not in user_output_vec:
            user_output_vec.append("TIME_REC")
        pass

    def init(self, toptask, args):
        import EvtStore
        toptask.createSvc('EvtStoreSvc')

        import JunoTimer
        toptask.createSvc("JunoTimerSvc")

        import OECProcessor
        toptask.createAlg("OECCreatorAlg")
        
        import EvtSteering
        stephandler = toptask.createAlg("StepHandler")
        stepsequencer = stephandler.createTool("StepSequencer")
        stepsequencer.property("ConfigFile").set(args.config_seq)
        stepdecision = stephandler.createTool("StepDecision")
        stepdecision.property("ConfigFile").set(args.config_sig)
    pass

class JUNOOECWaveRecModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--Para1", default="110", type = float, help="Filter parameter 1 for NNVT PMT")
        parser.add_argument("--Para2", default="160", type = float, help="Filter parameter 2 for NNVT PMT")
        parser.add_argument("--Para3", default="15", type = float, help="Filter parameter 3 for NNVT PMT")
        parser.add_argument("--Para4", default="80", type = float, help="Filter parameter 4 for HMMT PMT")
        parser.add_argument("--Para5", default="130", type = float, help="Filter parameter 5 for HMMT PMT")
        parser.add_argument("--Para6", default="15", type = float, help="Filter parameter 6 for HMMT PMT")
        parser.add_argument("--TotalPMT", default="17612", type = int,  help="Total PMT number")
        parser.add_argument("--Threshold1", default="0.030", type = float,  help="threshold for NNVT PMT hit finding") # This is a.u.. Don't change it.
        parser.add_argument("--Threshold2", default="0.025", type = float,  help="threshold for HMMT PMT hit finding") # This is a.u.. Don't change it.
        parser.add_argument("--WidthThreshold1", default="12", type = int,  help="width threshold for NNVT PMT hit finding") 
        parser.add_argument("--WidthThreshold2", default="12", type = int,  help="width threshold for HMMT PMT hit finding") # Specified for J20v2r0-Pre2
        parser.add_argument("--Calib_Length", default="1000", type = int,  help="Calibration Readout window length ns")
        parser.add_argument("--Calib_Start_Time", default="300", type = int,  help="Calibration average SPE waveform pulse start time")
        parser.add_argument("--Window1", default="7", type = int,  help="Charge integral ns for NNVT")
        parser.add_argument("--Window2", default="8", type = int,  help="Charge integral ns for HMMT")
        parser.add_argument("--HitCounting", default="0", type = int,  help="Use the hit counting method or not")
        parser.add_argument("--Calib", default="0", type = int,  help="Calib parameters extraction mode, 1 = on, 0 = off")

        parser.add_argument("--DebugMode", default="0", type = int,  help="Debug mode, 1 = on, 0 = off, default = 0")

        parser.add_argument("--SteeringCut_nnvt", default="26", type = float,  help="OECWFrec nnvt Steering_Cut")
        parser.add_argument("--SteeringCut_hmmt", default="27", type = float,  help="OECWFrec hmmt Steering_Cut")
        parser.add_argument("--BSL_method", default="3TW", type=str, choices=["3TW", "F50"], help="method to sub baseline")
        parser.add_argument("--TTQ_method", default="SimpleIntegral", type=str, choices=["Deconvolution","SimpleIntegral","OverZeroSum","AdcSum"], help="method to get total charge")
        parser.add_argument("--FHT_method", default="SimpleIntegral", type=str, choices=["Deconvolution","SimpleIntegral","CFD"], help="method to get first hit time")
        parser.add_argument("--SimpleIntegral_SPEratio_nnvt", default="1.31", type = float,  help="OECWFrec nnvt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_SPEratio_hmmt", default="1.21", type = float,  help="OECWFrec hmmt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_AmpThreshold_nnvt", default="2.9", type = float,  help="OECWFrec nnvt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_AmpThreshold_hmmt", default="3.3", type = float,  help="OECWFrec hmmt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_IntegralThreshold_nnvt", default="0.084", type = float,  help="OECWFrec nnvt SimpleIntegral_itg_threshold")
        parser.add_argument("--SimpleIntegral_IntegralThreshold_hmmt", default="0.083", type = float,  help="OECWFrec nnvt SimpleIntegral_itg_threshold")
        parser.add_argument( "--enableSteering", dest="enableSteering", action= 'store_true', help="enable steering for OECWFrec")
        parser.add_argument("--disableSteering", dest="enableSteering", action='store_false')
        parser.set_defaults(enableSteering=True)
        parser.add_argument( "--enableFilter", dest="enableFilter", action= 'store_true', help="enable filter for OECWFrec")
        parser.add_argument("--disableFilter", dest="enableFilter", action='store_false')
        parser.set_defaults(enableFilter=False)
        parser.add_argument( "--enableCalibCorr", dest="enableCalibCorr", action= 'store_true', help="use calibration parameters for OECWFrec")
        parser.add_argument("--disableCalibCorr", dest="enableCalibCorr", action='store_false')
        parser.set_defaults(enableCalibCorr=True)
        parser.add_argument("--WFOverTurn", action='store_true', help="overturn the waveform for OECWFrec")
        parser.set_defaults(WFOverTurn=False)
        pass

    def add_user_output_vec(self, user_output_vec, args):
        if "USER_OUTPUT" not in user_output_vec:
            user_output_vec.append("USER_OUTPUT")
        pass

    def init(self, task, args):
        import Sniper
        Sniper.loadDll("libSPMTCalibAlg.so")
        SPMTcal = task.createAlg("SPMTCalibAlg") 

        Sniper.loadDll("libWpPMTCalibAlg.so")
        WpPMTcal = task.createAlg("WpPMTCalibAlg")

        Sniper.loadDll("libOECWFrec.so")
        oecwfrec=task.createAlg("OECWFrec")
        wfTool = oecwfrec.createTool("OECWFrecTool")
        siTool = oecwfrec.createTool("SimpleIntegralTool")
        deTool = oecwfrec.createTool("DeconvolutionTool")
        adTool = oecwfrec.createTool("AdcsumTool")

        subTools = ['SimpleIntegralTool', 'DeconvolutionTool', 'AdcsumTool']

        wfTool.property("TotalPMT").set(args.TotalPMT)
        wfTool.property("TTQ_method").set(args.TTQ_method)
        wfTool.property("FHT_method").set(args.FHT_method)
        wfTool.property("enableSteering").set(args.enableSteering)
        wfTool.property("Steering_Cut").set((args.SteeringCut_nnvt, args.SteeringCut_hmmt))
        wfTool.property("WFOverTurn").set(args.WFOverTurn)
        # wfTool.property("SubTools").set(subTools)

        siTool.property("TotalPMT").set(args.TotalPMT)
        siTool.property("enableCalibCorr").set(args.enableCalibCorr)
        siTool.property("SimpleIntegral_SPEratio").set((args.SimpleIntegral_SPEratio_nnvt, args.SimpleIntegral_SPEratio_hmmt))
        siTool.property("SimpleIntegral_amp_threshold").set((args.SimpleIntegral_AmpThreshold_nnvt, args.SimpleIntegral_AmpThreshold_hmmt))
        siTool.property("SimpleIntegral_itg_threshold").set((args.SimpleIntegral_IntegralThreshold_nnvt, args.SimpleIntegral_IntegralThreshold_hmmt))
        
        deTool.property("TotalPMT").set(args.TotalPMT)
        deTool.property("enableCalibCorr").set(args.enableCalibCorr)
        deTool.property("Calib_Length").set(args.Calib_Length)
        deTool.property("Calib_Start_Time").set(args.Calib_Start_Time)
        deTool.property("Deconvolution_amp_threshold").set((args.Threshold1, args.Threshold2))
        deTool.property("Deconvolution_width_threshold").set((args.WidthThreshold1, args.WidthThreshold2))
        deTool.property("Deconvolution_itg_width").set((args.Window1, args.Window2))

        adTool.property("TotalPMT").set(args.TotalPMT)
        adTool.property("SimpleIntegral_SPEratio").set((args.SimpleIntegral_SPEratio_nnvt, args.SimpleIntegral_SPEratio_hmmt))

        subTools.append('OECWFrecTool')
        oecwfrec.property("TotalPMT").set(args.TotalPMT)
        oecwfrec.property("SubTools").set(subTools)

        pass
    pass

class JUNOHECModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--config-htagcond", default=os.getenv('OECCONFIGSVCROOT')+"/share/sig.json", help="The configuration json file for correlated tag condition")
        parser.add_argument( "--dT", default="2000", type=float, help="[ns] Internal time for triggers correlation")
        parser.add_argument( "--Qbf", default="0.1", type=float, help="Criterion for PC selection")
        parser.add_argument( "--thre_E", default="100", type=float, help="[MeV] Energy cut for atm nu")
        pass

    def init(self, toptask, args):
        import OECConfigSvc
        OECConf = toptask.createSvc("OECConfigSvc")
        OECConf.property("OECListFile").set(args.config_htagcond)
        import HECAlg
        hecalg = toptask.createAlg("HECAlg")
        hecalg.property("toolName").set(['HECSingleTool', 'HECDefaultTool','HECMuonTool'])
        hecdefaulttool = hecalg.createTool("HECDefaultTool")
        hecdefaulttool.property("dT").set(2000000)
        hecdefaulttool.property("maxN").set(100)
        hecdefaulttool.property("TpreMuonVeto").set(0)
        hecdefaulttool.property("TpostMuonVeto").set(7000)
        hecsingletool = hecalg.createTool("HECSingleTool")
        hecsingletool.property("TpreMuonVeto").set(0)
        hecsingletool.property("TpostMuonVeto").set(7000)
        hecmuontool = hecalg.createTool("HECMuonTool")
        hecmuontool.property("dT").set(2000000)
        hecmuontool.property("maxN").set(100)
        hecmuontool.property("TMuonWindow").set(200)
        import HECAtmAlg
        hecatmalg = toptask.createAlg("HECAtmAlg")
        hecatmalg.property("dT").set(args.dT)
        hecatmalg.property("Qbf").set(args.Qbf)
        hecatmalg.property("thre_E").set(args.thre_E)
        pass
    pass
