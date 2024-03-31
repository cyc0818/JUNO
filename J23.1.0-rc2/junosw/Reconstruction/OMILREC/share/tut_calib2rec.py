#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():

    import argparse

    #####################################################################
    # General options (Tao Lin)
    #####################################################################
    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--seed", type=int, default=0, help='seed')
    parser.add_argument("--input", default="sample_calib.root", help="input file name")
    parser.add_argument("--output", default="sample_rec.root", help="output file name")
    parser.add_argument("--user-output", default="sample_rec_user.root", help="output user file name")

    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon", "TT"],
                                       help="Det Option")
    parser.add_argument("--gdml", default="True", action="store_true", help="Use GDML.")
    parser.add_argument("--gdml-file", help="the file contains the geometry info.")
    parser.add_argument("--elec",default="no",
                                 choices=["yes","no"],
                                 help="with electronics simulation or not.")

    #####################################################################
    # Cluster (kampmann)
    #####################################################################
    cluster_mode = parser.add_argument_group("clustermode", "Clusterization related") 
    cluster_mode.add_argument("--cluster", dest="cluster", action="store_true",
                              help="enable cluster")
    cluster_mode.add_argument("--no-cluster", dest="cluster", action="store_false",
                              help="disable cluster (default)")
    cluster_mode.set_defaults(cluster=False)
    cluster_mode.add_argument("--startThreshold"  , type=float, default=6.50131               , help="Start threshold in n sigma over DN (for cluster identification)")
    cluster_mode.add_argument("--endThreshold"    , type=float, default=-0.821532             , help="End threshold in n sigma over DN (for cluster borders)")
    cluster_mode.add_argument("--windowSize"      , type=float, default=50.                   , help="Sliding window size of the clusterization algorithm")
    cluster_mode.add_argument("--darkRate"        , type=float, default=27.36e3               , help="Average dark rate per PMT in Hz (default value from PmtData.root)")
    cluster_mode.add_argument("--sizeDAQwin"      , type=float, default=1250.                 , help="Size of the DAQ window in ns")

    #####################################################################
    # RecTimeLikeAlg (lizy)
    #####################################################################
    parser.add_argument("--SwitchAlg", default="1", type = int,  help="0:Old Time Likelihood; 1:New Time Likelihood with lizy PDF; 50:Peak Time Fitter Separate; 51:Peak Time Fitter Combine")
    parser.add_argument("--SaveMC", default="0", type = int,  help="save MC truth Information")
    parser.add_argument("--VtxCorr", default="2", type = int,  help="0:no corr; 1:corr with edep; 2:corr with rec_Energy")

    #####################################################################
    # OMILREC (huanggh) 
    #####################################################################

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


    parser.add_argument("--SignalWindowL",  type=float, default=280., help='Signal window length')
    parser.add_argument("--LPMTCalibEnergy",  type=float, default=1.022, help='Calibration energy of LPMT')
    parser.add_argument("--SPMTCalibEnergy",  type=float, default=1.022, help='Calibration energy of SPMT')
    parser.add_argument("--RfrIndxLS",  type=float, default=1.54, help='Refractive index of liquid scintillator')
    parser.add_argument("--RfrIndxWR",  type=float, default=1.355, help='Refractive index of water')
    parser.add_argument("--runID", type = int, default=0, help="runID")
    parser.add_argument("--fixX",   type=float, default=0., help='fix x')
    parser.add_argument("--fixY",   type=float, default=0., help='fix y')
    parser.add_argument("--fixZ",   type=float, default=0., help='fix z')
 

    # Muon switch
    parser.add_argument("--method", default="point", choices=["point", "track", "wp-track", "spmt-track", "energy-point", "tt-track"], 
                                    help="Rec for point-like or track-like (muon) events")
    
    return parser

TOTALPMTS = {"Acrylic": 54318, "Balloon": 18306, "TT": 0}
DEFAULT_GDML_OUTPUT = {"Acrylic": "default", #"sample_detsim.root",  #"geometry_acrylic.gdml"
                       "Balloon": "default",
                       "TT": "default",
                       }  #"geometry_balloon.gdml"
ENERGY_SCALE = {"no":2633.61,"yes":2668.69}; # yes:2640.90
SUBDIR = {"no":"no-elec","yes":"elec"};
IS_NEUTRON = {"no":0,"yes":1};
PDF_VALUE = {"no":0,"yes":1};

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()

    total_pmt = TOTALPMTS[args.detoption]
    energy_scale = ENERGY_SCALE[args.elec]
    subdir = SUBDIR[args.elec]
    pdf_value = PDF_VALUE[args.elec]
    # load the geom info from root file by default.
    gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]
    geom_path_inroot = "JunoGeom"
    if args.gdml:
        if args.gdml_file:
            gdml_filename = args.gdml_file
    if gdml_filename.endswith("gdml"):
        geom_path_inroot = ""
    # === check the existance of the geometry file ===
    import os.path
    if gdml_filename != "default" and not gdml_filename.startswith("root://") and not os.path.exists(gdml_filename):
        import sys
        print("can't find the geometry file %s"%gdml_filename)
        sys.exit(-1)

    task = Sniper.Task("rectask")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0])

    # == Create IO Svc ==
    import RootIOSvc
    inputsvc = task.createSvc("RootInputSvc/InputSvc")
    inputsvc.property("InputFile").set([args.input])

    # === load dict ===
    #Sniper.loadDll("libSimEventV2.so")
    Sniper.loadDll("libCalibEvent.so")
    Sniper.loadDll("libRecEvent.so")
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    outputdata = {
                  "/Event/Calib": args.output,
                  "/Event/Rec": args.output
                  }
    # For TT:
    if args.method == "tt-track":
        outputdata["/Event/Sim"] = args.output

    # For cluster output
    if args.cluster:
        outputdata["/Event/Cluster"] = args.output

    roSvc.property("OutputStreams").set(outputdata)

    # == Geometry Svc ==
    import Geometry
    geom = task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set(gdml_filename)
    geom.property("GeomPathInRoot").set(geom_path_inroot)
    geom.property("FastInit").set(True)


    # == cluster algorithm ==
    if args.cluster:
        Sniper.loadDll("libClusterization.so")
        clusteralg = task.createAlg("ClusterizationAlg")
        clusteralg.property("StartThreshold").set(args.startThreshold)
        clusteralg.property("EndThreshold"  ).set(args.endThreshold)
        clusteralg.property("windowSize"    ).set(args.windowSize)
        clusteralg.property("darkRate"      ).set(args.darkRate)
        clusteralg.property("sizeDAQwin"    ).set(args.sizeDAQwin)
        

    # == reconstruction algorithms ==

    # possibilities: Muon or other
    if args.method == "track":
        # do muon rec
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
        recalg.rectool.property("FhtCorrFile").set("$LSQMUONRECTOOLROOT/test/fhtcorr.root")
    elif args.method == "point":
        # == RecTimeLikeAlg ==
        import RecTimeLikeAlg
        import os
        import RootWriter
        import PMTCalibSvc
        rootwriter = task.createSvc("RootWriter");
        rootwriter.property("Output").set({"USER_OUTPUT":args.user_output})
        PMTCalibSvc = task.createSvc("PMTCalibSvc")
        conddbsvc = task.createSvc("CondDBSvc")
        conddbsvc.property("RepoTypes").set({
        #    "local": "LocalFSRepo",
        "frontier": "Frontier"
        })
        conddbsvc.property("RepoURIs").set({
        #    "local": os.path.join("dummy-repo"),
        "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"
        })
        conddbsvc.property("GlobalTag").set(
            "gtname"
        )
        # FIXME: should user set the PMT_R and Ball_R ???
        alg = task.createAlg("RecTimeLikeAlg")
        alg.property("TotalPMT").set(total_pmt)
        alg.property("PMT_R").set(19.434)
        alg.property("Ball_R").set(19.18)
        alg.property("Pdf_Value").set(pdf_value)
        alg.property("File_path").set( 
                        os.path.join(os.environ["RECTIMELIKEALGROOT"],"share",subdir))
        alg.property("SwitchAlg").set(args.SwitchAlg)
        alg.property("SaveMC").set(args.SaveMC)
        alg.property("VtxCorr").set(args.VtxCorr)
        alg.setLogLevel(2)

    elif args.method == "energy-point":
        # == Use RecTimeLikeAlg to rec. the vertex firstly == 
        # == Then use OMILREC to rec. the energy
        import RecTimeLikeAlg
        import os
        import RootWriter
        rootwriter = task.createSvc("RootWriter");

        if args.enableUseTLHVertex:
            conddbsvc = task.createSvc("CondDBSvc")
            conddbsvc.property("RepoTypes").set({
            #    "local": "LocalFSRepo",
            "frontier": "Frontier"
            })
            conddbsvc.property("RepoURIs").set({
            #    "local": os.path.join("dummy-repo"),
            "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"
            })
            conddbsvc.property("GlobalTag").set(
                "gtname"
            )

            #rootwriter.property("Output").set({"USER_OUTPUT":args.user_output})
            # FIXME: should user set the PMT_R and Ball_R ???
            alg = task.createAlg("RecTimeLikeAlg")
            alg.property("TotalPMT").set(total_pmt)
            alg.property("PMT_R").set(19.434)
            alg.property("Ball_R").set(19.18)
            alg.property("Pdf_Value").set(pdf_value)
            alg.property("File_path").set(
                            os.path.join(os.environ["RECTIMELIKEALGROOT"],"share",subdir))
            alg.property("SwitchAlg").set(args.SwitchAlg)
            alg.property("SaveMC").set(args.SaveMC)
            alg.property("VtxCorr").set(args.VtxCorr)
            alg.setLogLevel(2)

        import OMILREC
        rootwriter.property("Output").set({"TRec":args.user_output})

        import PMTCalibSvc
        PMTCalibSvc = task.createSvc("PMTCalibSvc")

        PMTParamSvc = task.createSvc("PMTParamSvc")

        import PMTSimParamSvc
        print(" == PMTSimParamSvc == ")
        PMTParamSvc = task.createSvc("PMTSimParamSvc")

        alg = task.createAlg("OMILREC")
        alg.property("SimFile").set(args.simfile)
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

    elif args.method == "wp-track":
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
        recalg.rectool.property("PECut").set(19)
        recalg.rectool.property("NpmtCut").set(1)
        recalg.rectool.property("DistanceCut").set(7000)
    elif args.method == "spmt-track":
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
        recalg.rectool.property("PECut").set(53)
        recalg.rectool.property("NpmtCut").set(1)
        recalg.rectool.property("DistanceCut").set(7000)
    elif args.method == "tt-track":
        # TODO: now we use detsim as input, 
        #       in the future, we will use calib data.
        # cmd:
        # $ python tut_calib2rec.py --detoption TT --method tt-track --input sample_detsim.root --output sample_rec_tt.root 

        tt_geom_svc = task.createSvc("TTGeomSvc")
        import RootWriter
        rootwriter = task.createSvc("RootWriter")
        rootwriter.property("Output").set({"SIMEVT":args.user_output})
        Sniper.loadDll("libSimEventV2.so")
        import TTTrackingAlg
        alg = task.createAlg("TTTrackingAlg")
        alg.property("Det_Type").set(args.detoption)
        alg.setLogLevel(0)


    task.show()
    task.run()
