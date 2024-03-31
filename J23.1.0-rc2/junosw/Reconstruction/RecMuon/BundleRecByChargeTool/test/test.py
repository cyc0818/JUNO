#!/usr/bin/python
# -*- coding:utf-8 -*-

import Sniper

def get_parser():

    import argparse
    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--input", default="sample_muon.root",help="input file name list")

    return parser

if __name__  ==  "__main__":
    parser = get_parser()
    args = parser.parse_args()

Sniper.setLogLevel(1)
task = Sniper.Task("task")
task.asTop()
task.setLogLevel(1)

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
bufMgr.property("TimeWindow").set([0, 0]);

import RootIOSvc
roSvc = task.createSvc("RootOutputSvc/OutputSvc")
roSvc.property("OutputStreams").set({"/Event/RecTrack": "recOut.root", "/Event/Calib":"recOut.root"})
riSvc = task.createSvc("RootInputSvc/InputSvc")
#riSvc.property("InputFile").set(["sample_muon.root" ])
riSvc.property("InputFile").set([args.input])

import Geometry
geom = task.createSvc("RecGeomSvc")
gdmlgeo = "/afs/ihep.ac.cn/users/z/zhangk/workfs/slc6/j16v1r2test/offline/Examples/Tutorial/share/geometry_acrylic.gdml"
geom.property("GeomFile").set(gdmlgeo)
geom_path_inroot = "JunoGeom"
geom.property("GeomPathInRoot").set(geom_path_inroot)
geom.property("FastInit").set(True)

Sniper.loadDll("libPmtRec.so")
task.property("algs").append("PullSimHeaderAlg")

import RecCdMuonAlg
#import BundleRecTool
Sniper.loadDll("libBundleRecTool.so")
recalg = RecCdMuonAlg.createAlg(task)
recalg.setLogLevel(1)
recalg.property("Use3inchPMT").set(True);
recalg.property("Use20inchPMT").set(True);
recalg.property("Pmt3inchTimeReso").set(1)            # ns
recalg.property("Pmt20inchTimeReso").set(3)            # ns
recalg.useRecTool("BundleRecTool")
recalg.rectool.property("qdistance").set(True)
recalg.rectool.property("x").set(3)
recalg.rectool.property("y").set(3)


task.setEvtMax(3)
task.show()
task.run()
