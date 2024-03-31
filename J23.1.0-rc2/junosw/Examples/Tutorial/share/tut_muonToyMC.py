#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=1, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--output", default="sample_muon.root", help="output file name")
    parser.add_argument("--mac", default="run.mac", help="mac file")

    parser.add_argument("--genx", type=float, default=0, help='gen pos x (m)')
    parser.add_argument("--geny", type=float, default=0, help='gen pos y (m)')
    parser.add_argument("--genz", type=float, default=0, help='gen pos z (m)')

    parser.add_argument("--gentheta", type=float, default=180, help="gen theta (deg)")
    parser.add_argument("--genphi", type=float, default=180, help="gen phi (deg)")

    parser.add_argument("--genmom", type=float, default=1, help='gen momentum (MeV)')

    parser.add_argument("--timewindow", type=float, default=1, help='merge time window (ns)')
    parser.add_argument("--gdml", action="store_true", help="Save GDML.")
    return parser
DEFAULT_GDML_OUTPUT = {"Acrylic": "geometry_acrylic.gdml", 
                       "Balloon": "geometry_balloon.gdml"}

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()
    args.detoption = "Acrylic"
    print args


    gdml_filename = None
    if args.gdml:
        gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]
    import math
    theta_in_rad = math.radians(args.gentheta)
    phi_in_rad = math.radians(args.genphi)
    dirx = 1.*math.sin(theta_in_rad)*math.cos(phi_in_rad)
    diry = 1.*math.sin(theta_in_rad)*math.sin(phi_in_rad)
    dirz = 1.*math.cos(theta_in_rad)
    print "Direction: ", dirx, diry, dirz

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)
    # = I/O Related =
    import RootIOSvc
    task.createSvc("RootOutputSvc/OutputSvc")
    ro = task.find("OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": args.output})
    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)

    # = root writer =
    #import RootWriter
    #rootwriter = task.createSvc("RootWriter")

    #rootwriter.property("Output").set({"SIMEVT":"evt.root"})

    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")

    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["mu-"])
    gun.property("particleMomentums").set([args.genmom])
    gun.property("DirectionMode").set("Fix")
    gun.property("Directions").set([makeTV(dirx,diry,dirz)])
    gun.property("PositionMode").set("FixMany")
    gun.property("Positions").set([makeTV(args.genx*1e3,args.geny*1e3,args.genz*1e3)])

    gt.property("GenToolNames").set([gun.objName()])

    # = geant4 related =
    import DetSimOptions
    from DetSimOptions.ConfAcrylic import ConfAcrylic
    acrylic_conf = ConfAcrylic(task)
    acrylic_conf.configure()
    if gdml_filename:
        acrylic_conf.set_gdml_output(gdml_filename)
    detfactory = acrylic_conf.detsimfactory()
    detfactory.property("AnaMgrList").set(["MuonToySim"])
    detsimalg =  acrylic_conf.detsimalg()
    detsimalg.property("RunMac").set(args.mac)
    detsimalg.property("RunCmds").set([
            "/run/initialize",
            "/process/inactivate Scintillation",
            "/process/inactivate Cerenkov",
            ])

    pmtsd = acrylic_conf.tool("PMTSDMgr")
    pmtsd.property("EnableMergeHit").set(True)
    pmtsd.property("MergeTimeWindow").set(args.timewindow)
    # = begin run =
    task.show()
    task.run()
