#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper
import MCC15A

DEFAULT_GDML_OUTPUT = {"Acrylic": "geometry_acrylic.gdml", 
                       "Balloon": "geometry_balloon.gdml"}

if __name__ == "__main__":
    parser = MCC15A.get_parser()
    args = parser.parse_args()
    print args

    # == gdml ==
    gdml_filename = None
    if args.gdml:
        gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]

    # = top level task =
    task = Sniper.Task("detsimtask")
    task.asTop()
    task.setEvtMax(args.evtmax)
    task.setLogLevel(3)
    # = I/O Related =
    # == data registration ==
    # == RootIOSvc == 
    import RootIOSvc
    task.createSvc("RootOutputSvc/OutputSvc")
    ro = task.find("OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": args.output})
    # == Root Writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")
    rootwriter.property("Output").set({"SIMEVT":args.user_output})
    # == Data Buffer ==
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    # = Random Svc =
    import RandomSvc
    rndm = task.createSvc("RandomSvc")
    rndm.property("Seed").set(args.seed)
    # = JobInfoSvc =
    import JobInfoSvc
    jobinfosvc = task.createSvc("JobInfoSvc")
    jobinfosvc.property("OfflineVersion").set("MCC15A")
    import sys
    cmdhist = " ".join("\"%s\""%a for a in sys.argv)
    jobinfosvc.property("CommandLine").set(cmdhist)
#############################################################################
    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")
    # == different mode ==
    if args.gentool_mode == "calib":
        MCC15A.setup_generator_calib(gt, args)
    elif args.gentool_mode == "ibd":
        MCC15A.setup_generator_ibd(gt, args)
    elif args.gentool_mode == "b12":
        MCC15A.setup_generator_b12(gt, args)
    elif args.gentool_mode == "neutron":
        MCC15A.setup_generator_neutron(gt, args)
    # == global position ==
    if args.global_position:
        print "Using Global Position Mode"
        gun_pos = gt.findTool("GtPositionerTool")
        if not gun_pos:
            gun_pos = gt.createTool("GtPositionerTool")
            gt.property("GenToolNames").append([gun_pos.objName()])
        gun_pos.property("PositionMode").set("GenInGlobal")
        gun_pos.property("Positions").set(args.global_position[0])
    # == GtTimeOffsetTool ==
    toffset = gt.createTool("GtTimeOffsetTool")
    gt.property("GenToolNames").append([toffset.objName()])
#############################################################################
    # = detsim related =
    import DetSimOptions
    if args.detoption == "Acrylic":
        from DetSimOptions.ConfAcrylic import ConfAcrylic
        acrylic_conf = ConfAcrylic(task)
        acrylic_conf.configure()
        if gdml_filename:
            acrylic_conf.set_gdml_output(gdml_filename)
        # == geant4 run mac ==
        detsimalg = acrylic_conf.detsimalg()
        detsimalg.property("RunCmds").set([
                     #"/run/initialize",
                     #"/tracking/verbose 2",
                     #"/process/inactivate Scintillation",
                     #"/process/inactivate Cerenkov",
                 ])
        # == QE scale ==
        acrylic_conf.set_qe_scale(args.qescale)
        # == enable or disable 3inch PMTs ==
        if not args.pmt3inch:
            acrylic_conf.disable_3inch_PMT()
        # == GDMLMaterialBuilder ==
        if args.secret_file:
            material_builder = detsimalg.createTool("GDMLMaterialBuilder")
            material_builder.property("GdmlFiles").set([args.secret_file])
        # == DsPhysConsOptical ==
        dsphyscons_op = detsimalg.createTool("DsPhysConsOptical")
        dsphyscons_op.property("GammaSlowerTime").set(args.gamma_slower_time)
        dsphyscons_op.property("GammaSlowerRatio").set(args.gamma_slower_ratio)
    elif args.detoption == "Balloon":
        import sys
        print "In MCC15A, Balloon option is not support"
        sys.exit(-1)
#############################################################################
    # = begin run =
    task.show()
    task.run()
