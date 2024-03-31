#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(10)
    task.setLogLevel(3)
    # = Data Buffer =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(42)

    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    rootwriter.property("Output").set({"SIMEVT":"evt.root"})

    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")

    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["gamma", "gamma", "e+"])
    gun.property("particleMomentums").set([1.173, 1.333, 1.0])
    gun.property("DirectionMode").set("Fix")
    gun.property("Directions").set([makeTV(0,0,-1), makeTV(0,0,-1), makeTV(0,0,-1)])
    gun.property("PositionMode").set("FixMany")
    gun.property("Positions").set([makeTV(0,0,15500), makeTV(0,0,15500), makeTV(0,0,-155)])

    gt.property("GenToolNames").set([gun.objName()])

    # = geant4 related =
    # == G4Svc ==
    Sniper.loadDll("libG4Svc.so")
    g4svc = task.createSvc("G4Svc")
    print g4svc.objName()

    # == DetSimOptions ==
    Sniper.loadDll("libDetSimOptions.so")
    Sniper.loadDll("libAnalysisCode.so")
    Sniper.loadDll("libCentralDetector.so")
    Sniper.loadDll("libTopTracker.so")
    Sniper.loadDll("libCalibUnit.so")
    detsim0 = task.createSvc("DetSim0Svc")
    detsim0.property("AnaMgrList").set(["GenEvtInfoAnaMgr", 
                                        "NormalAnaMgr",
                                        "GeoAnaMgr"])
    #detsim0.property("PMTPosFile").set("Det1PMTPos_new.csv")
    detsim0.property("CDName").set("DetSim1")
    detsim0.property("TTName").set("") # disable TT
    detsim0.property("VetoPMTPosMode").set("")
    detsim0.property("CalibUnitEnable").set(True)
    #detsim0.property("StrutPosFile").set("Strut_Balloon.csv")
    #detsim0.property("FastenerPosFile").set("strut.csv")

    # == DetSimAlg ==
    Sniper.loadDll("libDetSimAlg.so")
    detsimalg = task.createAlg("DetSimAlg")
    detsimalg.property("DetFactory").set(detsim0.objName())

    # Calib Unit Related
    flag_use_gdml_calibtube = True
    calibtube = None
    if flag_use_gdml_calibtube:
        detsim0.property("CalibUnitName").set("lCT")
        tube = detsimalg.createTool("GDMLDetElemConstruction/lCT")
        #detsim0.property("CalibUnitName").set("Tube")
        #tube = detsimalg.createTool("GDMLDetElemConstruction/Tube")
        tube.property("GdmlFilename").set("calibtube.gdml")
        #tube.property("LogicalVolumeName").set("lCT")
        pass
    else:
        calibtube = detsimalg.createTool("CalibTubeConstruction")
        detsim0.property("CalibUnitName").set("CalibTube")
    print calibtube
    calibtubeplace = detsimalg.createTool("CalibTubePlacement")
    # FIXME a more general geometry service is needed.
    calibTubeLength1 = 17.3e3; # 17.3m
    calibTubeLength2 = 0.3e3   #  0.3m
    offset_z_in_cd = (calibTubeLength1+calibTubeLength2)/2.
    calibtubeplace.property("OffsetInZ").set(offset_z_in_cd)

    # === GeoAnaMgr ===
    geo = detsimalg.createTool("GeoAnaMgr")
    geo.property("GdmlEnable").set(True)
    geo.property("GdmlOutput").set("sample.gdml")
    geo.property("Gdml2RootEnable").set(False)
    geo.property("GdmlStoreRefs").set(True)

    # === Material Builder ===
    material_builder = detsimalg.createTool("GDMLMaterialBuilder")
    material_builder.property("GdmlFiles").set(["LS.gdml"])

    # = begin run =
    task.show()
    task.run()
