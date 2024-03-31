#!/usr/bin/env python
#-*- coding: utf-8 -*-
from __future__ import print_function

import sys
import os
import logging
from .JUNOModule import JUNOModule
from .JUNOUtils import mh

class JUNOEventRecModule(JUNOModule):

    TOTALPMTS = {"Acrylic": 54318, "Balloon": 18306, "TT": 0}
    ENERGY_SCALE = {"no":2633.61,"yes":2668.69}; # yes:2640.90
    SUBDIR = {"no":"no-elec","yes":"elec"};
    IS_NEUTRON = {"no":0,"yes":1};
    PDF_VALUE = {"no":0,"yes":1};


    def __init__(self):
        pass

    def setup_profiling(self, task):
        import SniperProfiling
        task.createSvc("SniperProfiling")

    def register_options(self, parser):
        self.register_options_common(parser)
        self.register_options_cluster(parser)
        self.register_options_rectimelike(parser)
        self.register_options_omilrec(parser)
        self.register_options_qctr(parser)
        self.register_options_bdte(parser)

        self.register_options_recmethods(parser)
        pass

    def add_output_vec(self, output_vec, args):
        # == Sim ==
        output_path_sim = ["/Event/Gen",
                           "/Event/Sim",
                           ]
        for k in output_path_sim:
            if k not in output_vec:
                output_vec.append(k)

        # == Calib ==
        output_path_calib = ["/Event/CdLpmtCalib",
                             "/Event/CdSpmtCalib",
                             "/Event/WpCalib",
                             # "/Event/TtCalib"
                            ]
        for k in output_path_calib:
            if k not in output_vec:
                output_vec.append(k)
        # == Rec ==
        output_path_rec = ["/Event/CdVertexRec",
                           "/Event/CdTrackRec",
                           "/Event/WpRec",
                           "/Event/TtRec"
                          ]
        for k in output_path_rec:
            if k not in output_vec:
                output_vec.append(k)

        # == Others ==
        if args.method == "tt-track" and "/Event/Sim" not in output_vec:
            output_vec.append("/Event/Sim")
        if args.cluster:
            output_vec.append("/Event/Cluster")
        pass

    def add_user_output_vec(self, user_output_vec, args):
        if "USER_OUTPUT" not in user_output_vec:
            user_output_vec.append("USER_OUTPUT")


    def init(self, toptask, args):
        self.init_common(toptask, args)

        self.init_clusterization(toptask, args)
        self.init_event_rec(toptask, args)
        pass

    ##########################################################################
    # Split the arguments into different categories
    ##########################################################################
    def register_options_common(self, parser):
        parser.add_argument("--evtrec", dest='evtrec', action='store_true')
        parser.add_argument("--no-evtrec", dest='evtrec', action='store_true')
        parser.set_defaults(evtrec=True)


        parser.add_argument("--elec",default="yes",
                                     choices=["yes","no"],
                                     help="with electronics simulation or not.")
        pass

    def register_options_cluster(self, parser):
        cluster_mode = parser.add_argument_group("clustermode", "Clusterization related") 
        cluster_mode.add_argument("--cluster", dest="cluster", action="store_true",
                                  help="enable cluster")
        cluster_mode.add_argument("--no-cluster", dest="cluster", action="store_false",
                                  help="disable cluster (default)")
        cluster_mode.set_defaults(cluster=False)
        cluster_mode.add_argument("--startThreshold"  , type=float, default=6.50131               , help="Start threshold in n sigma over DN (for cluster identification)")
        cluster_mode.add_argument("--endThreshold"    , type=float, default=-0.821532             , help="End threshold in n sigma over DN (for cluster borders)")
        cluster_mode.add_argument("--windowSize"      , type=float, default=50.                   , help="Sliding window size of the clusterization algorithm")
        cluster_mode.add_argument("--darkRate"        , type=float, default=27.36e3               , help="Average dark rate per PMT in Hz (default value)")
        cluster_mode.add_argument("--sizeDAQwin"      , type=float, default=1250.                 , help="Size of the DAQ window in ns")


    def register_options_rectimelike(self, parser):
        parser.add_argument("--SwitchAlg", default="1", type = int,  help="1:1D PDF; 2:3D PDF")
        parser.add_argument("--SaveMC", default="0", type = int,  help="save MC truth information (only work for ideal case)")
        parser.add_argument("--VtxCorr", default="0", type = int,  help="0:no corr; 1:bias correction")
        parser.add_argument("--SwitchPMTType", default="0", type = int,  help="0:all; 1:Hamamastu PMT only; 2:MCP PMT only")



    def register_options_omilrec(self, parser):
        parser.add_argument("--TruthLevel", default="0", type = int,  help="0:for not saving any truth; 1:save true vertex and energy; 2:save all including LPMT and SPMT truth")
        parser.add_argument("--RecMode", default="Real",help="Real mode or Toy mode")
        parser.add_argument("--simfile", default="sample_detsim_user.root",help="For enable reading simulation truth")
        parser.add_argument("--RecMapPath", default="",help="The path of the nPEMap, TimePdf, ChargeSpec and PMTData file")
        parser.add_argument("--TimePdfPath", default="",help="The path of the TimePdf")
        parser.add_argument("--enableReadTruth", action="store_true", help="enable reading simulation truth (must specify simfile)")
        parser.set_defaults(enableReadTruth=False)
        parser.add_argument("--enableLTSPEs", action="store_true", help="enable mcp long-tail SPEs")
        parser.set_defaults(enableLTSPEs=False)
        parser.add_argument("--enableUseTrueVertex", action="store_true", help="enable using true vertex")
        parser.set_defaults(enableUseTrueVertex=False)
        parser.add_argument("--enableUseFixedVertex", action="store_true", help="enable using fix vertex")
        parser.set_defaults(enableFixTrueVertex=False)
        parser.add_argument("--enableUseTLHVertex", action="store_true", help="enable using RecTimeLikeAlg vertex")
        parser.set_defaults(enableUseTLHVertex=False)
        parser.add_argument("--enableTimeInfo", action="store_true", help="enable time information for QTMLE")
        parser.set_defaults(enableTimeInfo=False)
        parser.add_argument("--enableDynTimeInfoOnly", action="store_true", help="only enable time information of dynode-pmt")
        parser.set_defaults(enableDynTimeInfoOnly=False)
        parser.add_argument("--enableQTimePdf", action="store_true", help="enable using charge related time-pdfs for QTMLE")
        parser.set_defaults(enableQTimePdf=False)
        parser.add_argument("--enableTimeOffsetCorr", action="store_true", help="enable time offset corrtion for QTMLE")
        parser.set_defaults(enableTimeOffsetCorr=False)
        parser.add_argument("--enableDynInfoOnly", action="store_true", help="only enable hit information of dynode-pmt")
        parser.set_defaults(enableDynInfoOnly=False)
        parser.add_argument("--enableMcpInfoOnly", action="store_true", help="only enable hit information of mcp-pmt")
        parser.set_defaults(enableMcpInfoOnly=False)
        parser.add_argument("--enableSPMTInfo", action="store_true", help="enable small PMT information")
        parser.set_defaults(enableSPMTInfo=False)
        parser.add_argument("--enableSPMTOnly", action="store_true", help="enable using small PMT only (must enableSPMTInfo)")
        parser.set_defaults(enableSPMTOnly=False)
        parser.add_argument("--enableUpdatePMTPara", action="store_true", help="enable updating the calibrated parameters of PMTs")
        parser.set_defaults(enableUpdatePMTPara=False)
        parser.add_argument("--enableOccupancy", action="store_true", help="enable occupancy model")
        parser.set_defaults(enableOccupancy=False)
        parser.add_argument("--enableAddToyQ", action="store_true", help="enable adding toy charge smearing (must enableReadTruth)")
        parser.set_defaults(enableAddToyQ=False)
        parser.add_argument("--enableAddToyTTS", action="store_true", help="enable adding toy tts (must enableReadTruth)")
        parser.set_defaults(enableAddToyTTS=False)
        parser.add_argument("--enableAddToyDN", action="store_true", help="enable adding toy dark noise (must enableReadTruth)")
        parser.set_defaults(enableAddToyDN=False)
        parser.add_argument("--enableUserPMTParaTruth", action="store_true", help="enable using different pmt parameters")
        parser.set_defaults(enableUserPMTParaTruth=False)
        parser.add_argument("--enableDebug", action="store_true", help="enable debug output")
        parser.set_defaults(enableDebug=False)
        parser.add_argument("--enableUseEkMap", action="store_true", help="enable use Ek nPE map")
        parser.set_defaults(enableUseEkMap=False)
        parser.add_argument("--enableUsePhiMap", action="store_true", help="enable use Phi nPE map")
        parser.set_defaults(enableUsePhiMap=False)
    
    
        parser.add_argument("--SignalWindowL",  type=float, default=420., help='Signal window length') 
        parser.add_argument("--LPMTCalibEnergy",  type=float, default=1.022, help='Calibration energy of LPMT')
        parser.add_argument("--SPMTCalibEnergy",  type=float, default=1.022, help='Calibration energy of SPMT')
        parser.add_argument("--RfrIndxLS",  type=float, default=1.54, help='Refractive index of liquid scintillator')
        parser.add_argument("--RfrIndxWR",  type=float, default=1.355, help='Refractive index of water')
        parser.add_argument("--runID", type = int, default=0, help="runID")
        parser.add_argument("--fixX",   type=float, default=0., help='fix x')
        parser.add_argument("--fixY",   type=float, default=0., help='fix y')
        parser.add_argument("--fixZ",   type=float, default=0., help='fix z')

    def register_options_qctr(self, parser):
        parser.add_argument("--recMethod", default="sqmle", help="the method used to reconstruct E/V: ptf, zy, yzy, imb or sqmle")

    def register_options_recmethods(self, parser):
        parser.add_argument("--method", default="steering", 
                            choices=["steering", "point", "qctr", "track", "wp-track", "spmt-track", "lpmtQ-track", "energy-point", "tt-track", "cdwp-track", "sphfunc-track", "cdwclipping-track", "bdte"], 
                            help="Rec for point-like or track-like (muon) events. Use 'steering' for invoke the point-like or track-like automatically")
        parser.add_argument("--create-subsubtask-in", default="toptask",
                            choices=["toptask", "subtask"],
                            help="For event steering, create subsubtask in top or sub task")

    def register_options_bdte(self, parser):
        default_path = os.path.join(os.getenv("JUNOTOP"), "data", "Reconstruction", "BDTE", "xgb_opt_edep_flat.json")
        parser.add_argument("--bdteModel", default=default_path, help="Path to the model to use for BDTE")
        pass

    ##########################################################################
    # init
    ##########################################################################
    def init_common(self, task, args):
    
        self.total_pmt = self.TOTALPMTS[args.detoption]
        self.energy_scale = self.ENERGY_SCALE[args.elec]
        self.subdir = self.SUBDIR[args.elec]
        self.pdf_value = self.PDF_VALUE[args.elec]

        ########################################
        # setup the directory for RecTimeLikeAlg
        ########################################
        import os
        rectimelikedata = None
        for envvar in ["JUNOTOP", "WORKTOP"]:
            if not os.getenv(envvar):
                continue
            p = os.getenv(envvar) + "/data/Reconstruction/RecTimeLikeAlg/share"
            if os.path.exists(p):
                rectimelikedata = p
        if rectimelikedata:
            self.rectimelikedata = os.path.join(rectimelikedata, self.subdir)
        else:
            # set empty string
            self.rectimelikedata = ""


    
    
    def init_clusterization(self, task, args):
        import Sniper
        if args.cluster:
            Sniper.loadDll("libClusterization.so")
            clusteralg = task.createAlg("ClusterizationAlg")
            clusteralg.property("StartThreshold").set(args.startThreshold)
            clusteralg.property("EndThreshold"  ).set(args.endThreshold)
            clusteralg.property("windowSize"    ).set(args.windowSize)
            clusteralg.property("darkRate"      ).set(args.darkRate)
            clusteralg.property("sizeDAQwin"    ).set(args.sizeDAQwin)

    def init_event_rec(self, task, args):
        if args.method == "steering":
            self.init_event_rec_steering(task, args)
        elif args.method == "track":
            self.init_event_rec_track(task, args)
        elif args.method == "cdwp-track":
            self.init_event_rec_cdwp_track(task, args)
        elif args.method == "point":
            self.init_event_rec_point(task, args)
        elif args.method == "qctr":
            self.init_event_rec_qctr(task, args)
        elif args.method == "energy-point":
            self.init_event_rec_energy_point(task, args)
        elif args.method == "wp-track":
            self.init_event_rec_wp_track(task, args)
        elif args.method == "spmt-track":
            self.init_event_rec_spmt_track(task, args)
        elif args.method == "lpmtQ-track":
            self.init_event_rec_lpmtQ_track(task, args)
        elif args.method == "sphfunc-track":
            self.init_event_rec_sphfunc_track(task, args)
        elif args.method == "tt-track":
            self.init_event_rec_tt_track(task, args)
        elif args.method == "cdwclipping-track":
            self.init_event_rec_cdwclipping_track(task, args)    
        elif args.method == "bdte":
            self.init_event_rec_bdte(task, args)

    ##########################################################################
    # Event Reconstruction Steering
    ##########################################################################
    def init_event_rec_steering(self, task, args):
        ######################################################################
        # Classification
        ######################################################################
        import ClassificationAlg
        import DetClassification
        import MCTruthClassification

        classificationAlg = task.createAlg("ClassificationAlg")

        cd_classifier = classificationAlg.createTool("CentralDetectorClassifier")
        wp_classifier = classificationAlg.createTool("WaterPoolClassifier")
        tt_classifier = classificationAlg.createTool("TopTrackerClassifier")

        tracklike_classifier = classificationAlg.createTool("MCTruthTrackLikeClassifier")
        pointlike_classifier = classificationAlg.createTool("MCTruthPointLikeClassifier")

        muonbundle_classifier = classificationAlg.createTool("MCTruthMuonBundleClassifier")

        classificationAlg.property("ClassifierNames").set([cd_classifier.objName(),
                                                           wp_classifier.objName(),
                                                           tt_classifier.objName(),
                                                           tracklike_classifier.objName(),
                                                           pointlike_classifier.objName(),
                                                           muonbundle_classifier.objName()])


        ######################################################################
        # Steering
        ######################################################################
        import Sniper
        Sniper.loadDll("libTopSteering.so")
        topsteering = task.createAlg("TopSteering")

        ######################################################################
        ## RecPoint
        ######################################################################
        sub = task.createTask("Task/RecPoint")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(sub)

        self.init_event_rec_steering_common(task, sub)

        self.init_event_rec_energy_point(sub, args)

        ######################################################################
        ## RecTrack
        ######################################################################
        sub = task.createTask("TopTask/RecTrack")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(sub)

        self.init_event_rec_steering_common(task, sub)

        Sniper.loadDll("libMuSteering.so")
        musteering = sub.createAlg("MuSteering")

        mutask = task
        if args.create_subsubtask_in == "toptask":
            mutask = task
            musteering.property("IsInTopTask").set(True)
        elif args.create_subsubtask_in == "subtask":
            mutask = sub
            musteering.property("IsInTopTask").set(False)

        # if musteering.IsInTopTask is true: use task to create subsubtask
        #                           is false: use sub to create subsubtask

        subsub = mutask.createTask("Task/RecMuSingle")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(subsub)

        self.init_event_rec_steering_common(task, subsub)
        self.init_event_rec_track(subsub, args)

        subsub = mutask.createTask("Task/RecMuBundle")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(subsub)

        self.init_event_rec_steering_common(task, subsub)
        self.init_event_rec_spmt_track(subsub, args)

    
        ######################################################################
        ## RecWP
        ######################################################################
        sub = task.createTask("Task/RecWP")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(sub)

        self.init_event_rec_steering_common(task, sub)

        self.init_event_rec_wp_track(sub, args)

        ######################################################################
        ## RecTT
        ######################################################################
        sub = task.createTask("Task/RecTT")
        # Profiling for subtask
        if args.profiling:
            self.setup_profiling(sub)

        self.init_event_rec_steering_common(task, sub)

        self.init_event_rec_tt_track(sub, args)


    # register shared components into sub-task
    def init_event_rec_steering_common(self, toptask, subtask):
        # DataBuffer
        #this sub-task shares the same DataBuffer of the top-task:"/Event" via MappedDataSvc
        md = subtask.createSvc("MappedDataSvc")
        md.regist(toptask, "/Event", "/Event")

        # register a list of shared services
        shared_svc_names = ["RecGeomSvc", "RootWriter", "MCParamsSvc",
                            "PMTParamSvc", "SpmtElecConfigSvc", "PMTSimParamSvc",
                            "PMTCalibSvc", "CondDBSvc"]

        for svc_name in shared_svc_names:
            svc = toptask.find(svc_name)
            if svc:
                subtask.addSvc(svc)
            else:
                logging.warn("Skip mapping service %s from %s to %s due to null"%(svc_name, toptask.objName(), subtask.objName()))

    ##########################################################################
    # Track Only
    ##########################################################################
    def init_event_rec_track(self, task, args):
        import RecCdMuonAlg
        import LsqMuonRecTool
        recalg = RecCdMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        # whether to use data of 3 inch pmts
        recalg.property("Use3inchPMT").set(False);
        # whether to use data of 20 inch pmts
        recalg.property("Use20inchPMT").set(True);
        # 3inch pmt time resolution,  unit: ns
        recalg.property("Pmt3inchTimeReso").set(1)
        #20inch pmt time resolution,  unit: ns
        recalg.property("Pmt20inchTimeReso").set(3)

        #configure the specific rec tool
        recalg.useRecTool("LsqMuonRecTool")
        recalg.rectool.property("LSRadius").set(17700)
        recalg.rectool.property("LightSpeed").set(299.792458)
        recalg.rectool.property("LSRefraction").set(1.485)
        recalg.rectool.property("MuonSpeed").set(299.792458)
        recalg.rectool.property("FhtCorrFile").set("$RECMU_FHTCORR_DATA/fhtcorr.root")
        recalg.rectool.property("FhtCorrMethod").set("corr2dtad")

    def init_event_rec_cdwp_track(self, task, args):
        import RecMuonAlg
        import CdWpFHTMuonRecTool
        recalg = RecMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        # whether to use data of 3 inch pmts
        recalg.property("Use3inchPMT").set(False);
        # whether to use data of 20 inch pmts
        recalg.property("Use20inchPMT").set(True);
        # 3inch pmt time resolution,  unit: ns
        recalg.property("Pmt3inchTimeReso").set(1)
        #20inch pmt time resolution,  unit: ns
        recalg.property("Pmt20inchTimeReso").set(3)
        recalg.property("ChosenDetectors").set(3)

        #configure the specific rec tool
        recalg.useRecTool("CdWpFHTMuonRecTool")
        recalg.rectool.property("LSRadius").set(17700)
        recalg.rectool.property("LightSpeed").set(299.792458)
        recalg.rectool.property("LSRefraction").set(1.485)
        recalg.rectool.property("MuonSpeed").set(299.792458)
        recalg.rectool.property("FhtCorrFile").set("$RECMU_FHTCORR_DATA/fhtcorr.root")
        recalg.rectool.property("FhtCorrMethod").set("corr2dtad")

    ##########################################################################
    # Point Only
    ##########################################################################
    def init_event_rec_point(self, task, args):
        # == RecTimeLikeAlg ==
        import RecTimeLikeAlg
        import os
        # import RootWriter
        # import PMTCalibSvc

        # rootwriter = task.createSvc("RootWriter");
        # rootwriter.property("Output").set({"USER_OUTPUT":args.user_output})

        # PMTCalibSvc = task.createSvc("PMTCalibSvc")
        # conddbsvc = task.createSvc("CondDBSvc")
        # conddbsvc.property("RepoTypes").set({
        # #    "local": "LocalFSRepo",
        # "frontier": "Frontier"
        # })
        # conddbsvc.property("RepoURIs").set({
        # #    "local": os.path.join("dummy-repo"),
        # "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"
        # })
        # conddbsvc.property("GlobalTag").set(
        #     "gtname"
        # )
        # FIXME: should user set the PMT_R and Ball_R ???
        alg = task.createAlg("RecTimeLikeAlg")
        alg.property("TotalPMT").set(self.total_pmt)
        alg.property("PMT_R").set(19.434)
        alg.property("Ball_R").set(19.18)
        alg.property("Pdf_Value").set(self.pdf_value)
        alg.property("File_path").set(self.rectimelikedata)
        alg.property("SwitchAlg").set(args.SwitchAlg)
        alg.property("SaveMC").set(bool(args.SaveMC))
        alg.property("VtxCorr").set(args.VtxCorr)
        alg.property("SwitchPMTType").set(args.SwitchPMTType)
        alg.setLogLevel(2)

    def init_event_rec_qctr(self, task, args):
        import Sniper
        Sniper.loadDll("libElecEvent.so")

        import QCtrRecAlg
        alg = task.createAlg("QCtrRecAlg")
        alg.property("recMethod").set(args.recMethod)
        alg.setLogLevel(2)
        #alg.property("recMode").set(args.recMode)
        #tools to be used by this algorithm
        if args.recMethod == 'zy':
            zyqctool = alg.createTool("zyQCTool")
        elif args.recMethod == 'yzy':
            zyqctool = alg.createTool("yzyQCTool")
        elif args.recMethod == 'imb':
            zyqctool = alg.createTool("imbQCTool")
        elif args.recMethod == 'sqmle':
            zyqctool = alg.createTool("SQMLE")
        elif args.recMethod == 'ptf':
            import RecTimeLikeAlg
            zyqctool = alg.createTool("zyQCTool")
            qcrectool = alg.createTool("QcRecTool")
            qcrectool.property("Pdf_Value").set(True)
            ptfrectool = alg.createTool("PtfRecTool")
            ptfrectool.property("Pdf_Value").set(True)
        qsumtool = alg.createTool("QSumTool")
        import OMILREC
        ecorrtool = alg.createTool("NPEUnifCTool")


    def init_event_rec_energy_point(self, task, args):
        # == Use RecTimeLikeAlg to rec. the vertex firstly == 
        # == Then use OMILREC to rec. the energy
        import RecTimeLikeAlg
        import os
        # import RootWriter
        # rootwriter = task.createSvc("RootWriter");

        if args.enableUseTLHVertex:
            # conddbsvc = task.createSvc("CondDBSvc")
            # conddbsvc.property("RepoTypes").set({
            # #    "local": "LocalFSRepo",
            # "frontier": "Frontier"
            # })
            # conddbsvc.property("RepoURIs").set({
            # #    "local": os.path.join("dummy-repo"),
            # "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"
            # })
            # conddbsvc.property("GlobalTag").set(
            #     "gtname"
            # )

            #rootwriter.property("Output").set({"USER_OUTPUT":args.user_output})
            # FIXME: should user set the PMT_R and Ball_R ???
            alg = task.createAlg("RecTimeLikeAlg")
            alg.property("TotalPMT").set(self.total_pmt)
            alg.property("PMT_R").set(19.434)
            alg.property("Ball_R").set(19.18)
            alg.property("Pdf_Value").set(self.pdf_value)
            alg.property("File_path").set(self.rectimelikedata)
            alg.property("SwitchAlg").set(args.SwitchAlg)
            alg.property("SaveMC").set(bool(args.SaveMC))
            alg.property("VtxCorr").set(args.VtxCorr)
            alg.property("SwitchPMTType").set(args.SwitchPMTType)
            alg.setLogLevel(2)

        import OMILREC
        # rootwriter.property("Output").set({"TRec":args.user_output})

        # import PMTCalibSvc
        # PMTCalibSvc = task.createSvc("PMTCalibSvc")

        # PMTParamSvc = task.createSvc("PMTParamSvc")

        # import PMTSimParamSvc
        # print(" == PMTSimParamSvc == ")
        # PMTParamSvc = task.createSvc("PMTSimParamSvc")

        alg = task.createAlg("OMILREC")
        alg.property("SimFile").set(args.simfile)
        alg.property("TruthLevel").set(args.TruthLevel)
        alg.property("PMT_R").set(19.434)
        alg.property("Ball_R").set(19.246)
        alg.property("LPMTCalibEnergy").set(args.LPMTCalibEnergy)
        alg.property("SPMTCalibEnergy").set(args.SPMTCalibEnergy)
        alg.property("SignalWindowL").set(args.SignalWindowL)
        alg.property("RfrIndxLS").set(args.RfrIndxLS)
        alg.property("RfrIndxWR").set(args.RfrIndxWR)
        alg.property("fix_x").set(args.fixX)
        alg.property("fix_y").set(args.fixY)
        alg.property("fix_z").set(args.fixZ)

        alg.property("runID").set(args.runID)
        # if the path is set, use the options from cmd line.
        # otherwise, use the default one.
        import os
        RecMapPath = os.path.join(os.environ["JUNOTOP"], "data/Reconstruction/OMILREC/RecMap")
        if (os.environ.get("WORKTOP")):
            WorkRecMapPath = os.path.join(os.environ["WORKTOP"], "data/Reconstruction/OMILREC/RecMap")
            if (os.path.exists(WorkRecMapPath)):
                RecMapPath = WorkRecMapPath
        else: 
            os.environ["WORKTOP"]=os.environ.get("JUNOTOP")
        if len(args.RecMapPath):
            RecMapPath = args.RecMapPath
        alg.property("RecMapPath").set(RecMapPath)
        alg.property("TimePdfPath").set(args.TimePdfPath)
        alg.property("RecMode").set(args.RecMode)
        alg.property("enableDebug").set(args.enableDebug)
        alg.property("enableReadTruth").set(args.enableReadTruth)
        alg.property("enableLTSPEs").set(args.enableLTSPEs)
        alg.property("enableUseTrueVertex").set(args.enableUseTrueVertex)
        alg.property("enableUseFixedVertex").set(args.enableUseFixedVertex)
        alg.property("enableUseTLHVertex").set(args.enableUseTLHVertex)
        alg.property("enableTimeInfo").set(args.enableTimeInfo)
        alg.property("enableDynTimeInfoOnly").set(args.enableDynTimeInfoOnly)
        alg.property("enableUseEkMap").set(args.enableUseEkMap)
        alg.property("enableUsePhiMap").set(args.enableUsePhiMap)
        alg.property("enableQTimePdf").set(args.enableQTimePdf)
        alg.property("enableDynInfoOnly").set(args.enableDynInfoOnly)
        alg.property("enableMcpInfoOnly").set(args.enableMcpInfoOnly)
        alg.property("enableSPMTInfo").set(args.enableSPMTInfo)
        alg.property("enableSPMTOnly").set(args.enableSPMTOnly)
        alg.property("enableOccupancy").set(args.enableOccupancy)
        alg.property("enableAddToyQ").set(args.enableAddToyQ)
        alg.property("enableAddToyTTS").set(args.enableAddToyTTS)
        alg.property("enableAddToyDN").set(args.enableAddToyDN)
        alg.setLogLevel(2)
        
        if args.RecMode == "Real":
            PMTPara = alg.createTool("PMTRealParaTool/PMTParaTool")
            PMTPara.property("RecMapPath").set(RecMapPath)
            PMTPara.property("enableUpdatePMTPara").set(args.enableUpdatePMTPara)
            PMTPara.property("enableTimeOffsetCorr").set(args.enableTimeOffsetCorr)

            PMTInput = alg.createTool("PMTRealInputTool/PMTInputTool")


        elif args.RecMode == "Toy":
            PMTPara = alg.createTool("PMTToyParaTool/PMTParaTool")
            PMTPara.property("RecMapPath").set(RecMapPath)
            PMTPara.property("enableUserPMTParaTruth").set(args.enableUserPMTParaTruth)
            PMTPara.property("enableAddToyQ").set(args.enableAddToyQ)
            PMTPara.property("enableAddToyTTS").set(args.enableAddToyTTS)
            PMTPara.property("enableAddToyDN").set(args.enableAddToyDN)

            PMTInput = alg.createTool("PMTToyInputTool/PMTInputTool")
            PMTInput.property("SimFile").set(args.simfile)
            PMTInput.property("enableSPMTInfo").set(args.enableSPMTInfo)
            PMTInput.property("enableAddToyQ").set(args.enableAddToyQ)
            PMTInput.property("enableAddToyTTS").set(args.enableAddToyTTS)
            PMTInput.property("enableAddToyDN").set(args.enableAddToyDN)

    def init_event_rec_wp_track(self, task, args):
        import RecWpMuonAlg
        import PoolMuonRecTool
        recalg = RecWpMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        recalg.property("Use3inchPMT").set(True);
        recalg.property("Use20inchPMT").set(True);
        recalg.property("Pmt3inchTimeReso").set(1)            # ns
        recalg.property("Pmt20inchTimeReso").set(3)            # ns
        recalg.useRecTool("PoolMuonRecTool")
        recalg.rectool.property("MaxPoints").set(7)
        recalg.rectool.property("PECut").set(10)
        recalg.rectool.property("NpmtCut").set(1)
        recalg.rectool.property("DistanceCut").set(7000)

    def init_event_rec_spmt_track(self, task, args):
        import RecCdMuonAlg
        import SpmtMuonRecTool
        recalg = RecCdMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        recalg.property("Use3inchPMT").set(True);
        recalg.property("Use20inchPMT").set(True);
        recalg.property("Pmt3inchTimeReso").set(1)            # ns
        recalg.property("Pmt20inchTimeReso").set(3)            # ns
        recalg.useRecTool("SpmtMuonRecTool")
        recalg.rectool.property("MaxPoints").set(7)
        recalg.rectool.property("PECut").set(2)
        recalg.rectool.property("NpmtCut").set(1)
        recalg.rectool.property("DistanceCut").set(5000)


    def init_event_rec_lpmtQ_track(self, task, args):
        import RecCdMuonAlg
        import CdLpmtQSpecMuonRecTool
        recalg = RecCdMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        recalg.property("Use3inchPMT").set(True);
        recalg.property("Use20inchPMT").set(True);
        recalg.property("Pmt3inchTimeReso").set(1)            # ns
        recalg.property("Pmt20inchTimeReso").set(3)            # ns
        recalg.useRecTool("CdLpmtQSpecMuonRecTool")
        recalg.rectool.property("MaxPoints").set(7)
        recalg.rectool.property("PECut").set(100)
        recalg.rectool.property("NpmtCut").set(1)
        recalg.rectool.property("DistanceCut").set(5000)

    def init_event_rec_sphfunc_track(self, task, args):
        import RecMuonAlg
        import SphMuonRecTool
        recalg = RecMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        recalg.property("Use3inchPMT").set(True);
        recalg.property("Use20inchPMT").set(True);
        recalg.property("Pmt3inchTimeReso").set(1)
        recalg.property("Pmt20inchTimeReso").set(3)
        recalg.property("ChosenDetectors").set(3)
        recalg.useRecTool("SphMuonRecTool")

    def init_event_rec_tt_track(self, task, args):
        # cmd:
        # $ python tut_calib2rec.py --method tt-track --input sample_calib.root --output sample_rec_tt.root 

        tt_geom_svc = task.createSvc("TTGeomSvc")

        # import RootWriter
        # rootwriter = task.createSvc("RootWriter")
        # rootwriter.property("Output").set({"SIMEVT":args.user_output})
        import Sniper

        Sniper.loadDll("libCalibEvent.so")
        import TTTrackingAlg
        alg = task.createAlg("TTTrackingAlg")
        alg.setLogLevel(0)

    def init_event_rec_cdwclipping_track(self, task, args):
        import RecCdMuonAlg
        import CdWClippingMuonRecTool
        recalg = RecCdMuonAlg.createAlg(task)
        recalg.setLogLevel(1)
        # whether to use data of 3 inch pmts
        recalg.property("Use3inchPMT").set(False);
        # whether to use data of 20 inch pmts
        recalg.property("Use20inchPMT").set(True);
        # 3inch pmt time resolution,  unit: ns
        recalg.property("Pmt3inchTimeReso").set(1)
        #20inch pmt time resolution,  unit: ns
        recalg.property("Pmt20inchTimeReso").set(3)

        #configure the specific rec tool
        recalg.useRecTool("CdWClippingMuonRecTool")
        recalg.rectool.property("NetRack_R").set(20050.)
        recalg.rectool.property("NPmtRequired_min").set(100)
        recalg.rectool.property("NPmtRequired_max").set(17200)

    def init_event_rec_bdte(self, task, args):

        # Create a python store
        import SniperPython
        task.createSvc("PyDataStoreSvc/DataStore")

        import BDTE
        task.createAlg("EdmReader");

        self.alg_bdte_reco = BDTE.Reco("Reco", BDTE.Config(path = args.bdteModel, optimal_features = BDTE.SELECTED_FEATURES), mode=BDTE.Mode.INFERENCE);
        task.addAlg(self.alg_bdte_reco)

        task.createAlg("EdmWriter");

