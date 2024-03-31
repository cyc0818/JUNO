#!/usr/bin/env python
#-*- coding: utf-8 -*-
from __future__ import print_function

import sys
import os
import logging
from .JUNOModule import JUNOModule
from .JUNOUtils import mh

class JUNOWaveRecModule(JUNOModule):

    def __init__(self):
        pass

    def register_options(self, parser):
        self.register_options_common(parser)
        self.register_options_electruth(parser)
        self.register_options_database(parser)
        self.register_options_parameters(parser)
        pass

    def add_output_vec(self, output_vec, args):
        # generator and detector simulation
        if "/Event/Gen" not in output_vec:
            output_vec.append("/Event/Gen")
        if "/Event/Sim" not in output_vec:
            output_vec.append("/Event/Sim")

        # elec truth
        if args.enableElecTruth:
            output_vec.append("/Event/CdLpmtTruth")
            output_vec.append("/Event/CdSpmtTruth")
            output_vec.append("/Event/WpTruth")
            output_vec.append("/Event/TrackTruth")
            # output_vec.append("/Event/TtTruth")

        # calib
        output_vec.append("/Event/CdLpmtCalib")
        output_vec.append("/Event/CdSpmtCalib")
        output_vec.append("/Event/WpCalib")
        output_vec.append("/Event/TtCalib")
        pass

    def add_user_output_vec(self, user_output_vec, args):
        if "USER_OUTPUT" not in user_output_vec:
            user_output_vec.append("USER_OUTPUT")
        pass

    def init(self, toptask, args):
        self.init_common(toptask, args)

        self.init_waverec(toptask, args)
        self.init_CDWaveFormSaving(toptask, args)

        self.init_calibspmt(toptask, args)
        self.init_calibWppmt(toptask, args)
        self.init_calibTT(toptask, args)
        pass

    ##########################################################################
    # Split the arguments into different categories
    ##########################################################################
    def register_options_common(self, parser):
        # TODO: will be removed from this module
        parser.add_argument("--waverec", dest='waverec', action='store_true')
        parser.add_argument("--no-waverec", dest='waverec', action='store_true')
        parser.set_defaults(waverec=True)

        parser.add_argument("--waverec-method", default="decon",
                            choices=["pmtrec", "wavefit", "decon", "muon", "cotiwaverec", "oecwfrec"],
                            help="pmtrec: Integral PMT Rec, wavefit: Waveform fit, decon: Deconvolution, muon: Muon Waveform Reco, cotiwaverec: Continous Over-Threshold Integral")
        # parameters for the deconvolution method
        # The default parameters are for J20. Never modify them.
        parser.add_argument("--algOption",default="fit1",
                            choices=["fixed", "fit1", "cfd", "peak"],
                            help="fixed: fixed threshold, fit1: linear fitting, cfd: constant fraction timing, peak: peak position fitting")

    def register_options_electruth(self, parser):
    
        parser.add_argument("--enableElecTruth",  dest='enableElecTruth', action='store_true')
        parser.add_argument("--disableElecTruth", dest='enableElecTruth', action='store_false')
        parser.set_defaults(enableElecTruth=False)

    def register_options_database(self, parser):
        pass

    def register_options_parameters(self, parser):
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
        parser.add_argument("--Length", default="1000", type = int,  help="Readout window length ns")
        parser.add_argument("--Calib_Length", default="1000", type = int,  help="Calibration Readout window length ns")
        parser.add_argument("--Calib_Start_Time", default="300", type = int,  help="Calibration average SPE waveform pulse start time")
        parser.add_argument("--Window1", default="7", type = int,  help="Charge integral ns for NNVT")
        parser.add_argument("--Window2", default="8", type = int,  help="Charge integral ns for HMMT")
        parser.add_argument("--HitCounting", default="0", type = int,  help="Use the hit counting method or not")
        parser.add_argument("--Calib", default="0", type = int,  help="Calib parameters extraction mode, 1 = on, 0 = off")
        # parameters for the cotiwaverec method
        parser.add_argument("--Algrithm", default="2", type = int,  help="Baseline & threshold algrithom, 1 = constant baseline, 2 = dynamic baseline (default)")
        parser.add_argument("--BslBufLen", default="32", type = int,  help="Baseline buffer length, default = 32 ns")
        parser.add_argument("--NTimesRMS", default="2", type = float,  help="N times of rms as threshold, default = 2x")
        parser.add_argument("--NHeadPoints", default="5", type = int,  help="Head points number, default = 5")
        parser.add_argument("--NTailPoints", default="3", type = int,  help="Tail points number, default = 3")
        parser.add_argument("--DebugMode", default="0", type = int,  help="Debug mode, 1 = on, 0 = off, default = 0")
        parser.add_argument("--SaveFeatures", default="0", type = int,  help="not 0 to enable ML save features")

        parser.add_argument("--SteeringCut_nnvt", default="26", type = float,  help="OECWFrec nnvt Steering_Cut")
        parser.add_argument("--SteeringCut_hmmt", default="27", type = float,  help="OECWFrec hmmt Steering_Cut")
        parser.add_argument("--TTQ_method", default="SimpleIntegral", type=str, help="method to get total charge")
        parser.add_argument("--FHT_method", default="SimpleIntegral", type=str, help="method to get first hit time")
        parser.add_argument("--SimpleIntegral_SPEratio_nnvt", default="1.31", type = float,  help="OECWFrec nnvt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_SPEratio_hmmt", default="1.21", type = float,  help="OECWFrec hmmt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_AmpThreshold_nnvt", default="2.9", type = float,  help="OECWFrec nnvt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_AmpThreshold_hmmt", default="3.3", type = float,  help="OECWFrec hmmt SimpleIntegral_amp_threshold")
        parser.add_argument("--SimpleIntegral_IntegralThreshold_nnvt", default="0.084", type = float,  help="OECWFrec nnvt SimpleIntegral_itg_threshold")
        parser.add_argument("--SimpleIntegral_IntegralThreshold_hmmt", default="0.083", type = float,  help="OECWFrec nnvt SimpleIntegral_itg_threshold")
        parser.add_argument( "--enableSteering", dest="enableSteering", action= 'store_true', help="enable steering for OECWFrec")
        parser.add_argument("--disableSteering", dest="enableSteering", action='store_false')
        parser.set_defaults(enableSteering=True)
        parser.add_argument( "--enableCalibCorr", dest="enableCalibCorr", action= 'store_true', help="use calibration parameters for OECWFrec")
        parser.add_argument("--disableCalibCorr", dest="enableCalibCorr", action='store_false')
        parser.set_defaults(enableCalibCorr=True)
        parser.add_argument("--WFOverTurn", type=bool, help="overturn the waveform for OECWFrec")

        # parameter for CDWaveFormSaving algrothm
        #   default: disable algrothm, CD waveform won't be saved;
        #   not 0 (1.4e7 for example) to enable CD waveform saving: only save CD waveform for total nPE>WF_nPEcut.        
        parser.add_argument("--WF_nPEcut", default="0", type = float,  help="Saving CD WaveForms for events with total nPE above WF_nPEcut")

    ##########################################################################
    # init
    ##########################################################################
    def init_common(self, task, args):
        pass



    def init_waverec(self, task, args):
        import Sniper
        if args.waverec_method == "pmtrec":
            Sniper.loadDll("libIntegralPmtRec.so")
            import os
            elecroot = os.environ["ELECSIMROOT"]
            intPmtRec=task.createAlg("IntegralPmtRec")
            intPmtRec.property("TotalPMT").set(TOTAL_PMT)
            intPmtRec.property("Threshold").set(0.25*0.0035) #1/4 PE
            pass
        elif args.waverec_method == "wavefit":
            # == Wave Fit Alg ==
            Sniper.loadDll("libWaveFit.so")
            wavefit = task.createAlg("WaveFitAlg")
            wavefit.property("PmtTotal").set(TOTAL_PMT)
        elif args.waverec_method == "muon":
            Sniper.loadDll("libMuonWaveRec.so")
            muon = task.createAlg("MuonWaveRec")
            muon.property("Threshold").set(args.Threshold2)
            muon.property("Length").set(args.Length)
            muon.property("TotalPMT").set(TOTAL_PMT)
        elif args.waverec_method == "decon":
            Sniper.loadDll("libDeconvolution.so")
            deconv=task.createAlg("Deconvolution")
            deconv.property("TotalPMT").set(args.TotalPMT)
            deconv.property("Threshold1").set(args.Threshold1)
            deconv.property("Threshold2").set(args.Threshold2)
            deconv.property("WidthThreshold1").set(args.WidthThreshold1)
            deconv.property("WidthThreshold2").set(args.WidthThreshold2)
            deconv.property("Calib_Length").set(args.Calib_Length)
            deconv.property("Calib_Start_Time").set(args.Calib_Start_Time)
            deconv.property("Para1").set(args.Para1)
            deconv.property("Para2").set(args.Para2)
            deconv.property("Para3").set(args.Para3)
            deconv.property("Para4").set(args.Para4)
            deconv.property("Para5").set(args.Para5)
            deconv.property("Para6").set(args.Para6)
            deconv.property("Window1").set(args.Window1)
            deconv.property("Window2").set(args.Window2)
            deconv.property("Calib").set(args.Calib)
            deconv.property("DebugMode").set(args.DebugMode)
            deconv.property("SaveFeatures").set(args.SaveFeatures)
            deconv.property("HitCounting").set(args.HitCounting)
            Sniper.loadDll("libTimeRec.so");

            timerec=task.createAlg("TimeRec")
            timerec.property("AlgOption").set(args.algOption)
            timerec.property("enableElecTruth").set(args.enableElecTruth)
            timerec.property("WindowLength").set(args.Length)

        elif args.waverec_method == "cotiwaverec":
            Sniper.loadDll("libCOTIWaveRec.so")
            cotiwaverec=task.createAlg("COTIWaveRec")
            cotiwaverec.property("TotalPMT").set(args.TotalPMT)
            cotiwaverec.property("Calib").set(args.Calib)
            cotiwaverec.property("Algrithm").set(args.Algrithm)
            cotiwaverec.property("BslBufLen").set(args.BslBufLen)
            cotiwaverec.property("NTimesRMS").set(args.NTimesRMS)
            cotiwaverec.property("NHeadPoints").set(args.NHeadPoints)
            cotiwaverec.property("NTailPoints").set(args.NTailPoints)
            cotiwaverec.property("DebugMode").set(args.DebugMode)
        
        elif args.waverec_method == "oecwfrec":
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


    def init_calibspmt(self, task, args):
        import Sniper
        Sniper.loadDll("libSPMTCalibAlg.so")
        SPMTcal = task.createAlg("SPMTCalibAlg") 
    def init_calibWppmt(self, task, args):
        import Sniper
        Sniper.loadDll("libWpPMTCalibAlg.so")
        WpPMTcal = task.createAlg("WpPMTCalibAlg")
    def init_calibTT(self, task, args):
        import Sniper
        Sniper.loadDll("libTTCalibAlg.so")
        TTcal = task.createAlg("TTCalibAlg")

    def init_CDWaveFormSaving(self, task, args):
        if args.WF_nPEcut == 0: 
            return
        import Sniper
        Sniper.loadDll("libCDWaveFormSaving.so")
        WFsave = task.createAlg("CDWaveFormSaving")  
        WFsave.property("WF_nPEcut").set(args.WF_nPEcut)
