#!/usr/bin/env python
# -*- coding:utf-8 -*-
# Author: zhangxt@ihep.ac.cn
# Sep, 2019


import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='PMT spe waveform calibration.')
    parser.add_argument("--evtmax", type=int, default=-1, help='events to be processed')
    parser.add_argument("--input", default="sample_calib.root", help="input file name")
    parser.add_argument("--output", default="sample_calibCorr.root", help="output file name")
    parser.add_argument("--user-output", default="sample_calibCorr_user.root", help="output user file name")
    parser.add_argument("--loglevel", default="Info",
            choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
            )

    parser.add_argument("--calibfile", default="", help="PMT calibrated spe spectra root file, please use absolute directory")
    parser.add_argument("--SPE", type=int, default=0, help="Real SPE spectrum")
    parser.add_argument("--calibmode", default="calib", help="Default opration mode: \"calib\", or you can input the calibration parameters txt file's absolute directory")
    parser.add_argument("--recalg", default="deconv", help="Reconstruciton Algorithm: \"deconv\", \"simpleinte\"")
    parser.add_argument("--pecut", type=float, default=100000, help="PE number cut for calibration event selection")
    parser.add_argument("--dnstart", type=float, default=600, help="Dark noise start counting point, default = 600")
    parser.add_argument("--dnend", type=float, default=1000, help="Dark noist end counting point, default = 1000")
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6 }

if __name__ == "__main__":

    parser = get_parser()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])

    # create buffer Svc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([0, 0])

    # Geometry service
    import Geometry
    geom = task.createSvc("RecGeomSvc")
    geom.property("GeomFile").set("default")
    geom.property("FastInit").set(True)

    # Create IO Svc
    if args.calibmode == "calib":
        import RootIOSvc
        roSvc = task.createSvc("RootOutputSvc/OutputSvc")
        roSvc.property("OutputStreams").set({ "/Event/Calib": args.output})

        inputsvc = task.createSvc("RootInputSvc/InputSvc")
        if args.input.find('list') > -1:
            arr = []
            file1 = open(args.input)
            for line in file1 :
                line = line.strip()
                arr.append(line)
            inputsvc.property("InputFile").set(arr)
        else:
            inputsvc.property("InputFile").set([args.input])

    # Create PMTCalib Svc
    import PMTCalibSvc
    calibsvc = task.createSvc("PMTCalibSvc");

    # Create RoOT Writer
    import RootWriter
    # task.property("svcs").append("RootWriter")
    # rw = task.find("RootWriter")
    # rw.property("Output").set({"FILE1": args.output})
    rootwriter = task.createSvc("RootWriter")
    # rootwriter.property("Output").set({"FILE1": args.user_output})
    rootwriter.property("Output").set({"FILE2": args.user_output})

    # import PMTCalibAlg
    Sniper.loadDll("libPMTCalibAlg.so")
    pmtCalib=task.createAlg("PMTCalibAlg")
    pmtCalib.property("CalibMode").set(args.calibmode)
    pmtCalib.property("CalibFile").set(args.calibfile)
    pmtCalib.property("SPE").set(args.SPE)
    pmtCalib.property("Alg").set(args.recalg)
    pmtCalib.property("PECut").set(args.pecut)
    pmtCalib.property("DNStart").set(args.dnstart)
    pmtCalib.property("DNEnd").set(args.dnend)
    task.show()
    task.run()


