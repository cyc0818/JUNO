#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


import Sniper
import os
if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(3)
    task.setLogLevel(3)
    # = I/O Related =
    # import DataRegistritionSvc
    # task.createSvc("DataRegistritionSvc")
    
    import RootIOSvc
    task.createSvc("RootOutputSvc/OutputSvc")
    ro = task.find("OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": "sample.root"})
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

    # = geometry service for PMT =
    import Geometry
    pmt_param_svc = task.createSvc("PMTParamSvc")
    tt_geom_svc = task.createSvc("TTGeomSvc")


    # = generator related =
    import GenTools
    from GenTools import makeTV
    gt = task.createAlg("GenTools")

    gun = gt.createTool("GtGunGenTool/gun")
    #gun.property("particleNames").set(["mu-"])
    #gun.property("particleMomentums").set([100.0e3]) # 100 GeV
    gun.property("particleNames").set(["e-"])
    gun.property("particleMomentums").set([1.0]) # 1 MeV


    #gun.property("particleNames").set(["gamma"])
    #gun.property("particleMomentums").set([1.0])
    #gun.property("DirectionMode").set("Fix")
    #gun.property("Directions").set([makeTV(0,0,0), makeTV(0,0,-1), makeTV(0,0,-1)])
    #gun.property("PositionMode").set("FixMany")
    gun.property("Positions").set([makeTV(0,0,0)])

    gt.property("GenToolNames").set([gun.objName()])

    # = geant4 related =
    import DetSimOptions
    from DetSimOptions.ConfAcrylic import ConfAcrylic
    acrylic_conf = ConfAcrylic(task)
    acrylic_conf.configure()
    acrylic_conf.disable_pmts_and_struts_in_cd()
    acrylic_conf.set_tt_name("")
    acrylic_conf.set_veto_pmt_mode("")
    acrylic_conf.disable_3inch_PMT()
    #acrylic_conf.set_gdml_output("geometry_acrylic.gdml")
    #acrylic_conf.set_dae_output("geometry_acrylic.dae")

    acrylic_conf.add_anamgr("G4OpticksAnaMgr")
    #acrylic_conf.enable_chimney()
    detfactory = acrylic_conf.detsimfactory()
    #detfactory.property("3inchPMTPosFile").set("")
    # === PMT Mask ===
    #mask = acrylic_conf.mask()
    #print mask
    #mask.property("BufferMaterial").set("Water")
    # # === DAE Physics ===
    # Sniper.loadDll("libG4DAEChroma.so")
    # op = acrylic_conf.tool("DAEDsPhysConsOptical/DsPhysConsOptical")
    # op.property("UseCerenkov").set(True)
    # op.property("UseScintillation").set(True)
    # # = begin run =
    # anamgr = acrylic_conf.tool("G4DAEAnaMgr")
    # import os
    # p = "GBoundaryLibMetadataMaterialMap.json"
    # idpath = os.getenv("IDPATH")
    # if idpath:
    #     p = os.path.join(idpath, "GBoundaryLibMetadataMaterialMap.json")
    # if os.path.exists(p):
    #     anamgr.property("GGeoMatMap").set(p)
    # anamgr.property("ReduceFactor").set(1000)
    if not os.environ.get("OPTICKSDATAROOT", None):
        print("The envvar $OPTICKSDATAROOT is not set")
        import sys
        sys.exit(-1)
    os.environ["OPTICKSDATA_DAEPATH_J1707"] = os.path.join(os.environ["OPTICKSDATAROOT"], "export/juno1707/g4_00")
    os.environ["OPTICKS_GEOKEY"] = "OPTICKSDATA_DAEPATH_J1707"
    os.environ["OPTICKS_QUERY"] = "all"
    os.environ["OPTICKS_CTRL"] = "volnames"

    Sniper.loadDll("libG4Opticks.so")
    opticks_anamgr = acrylic_conf.tool("G4OpticksAnaMgr")

    op = acrylic_conf.tool("OKDsPhysConsOptical/DsPhysConsOptical")
    op.property("UseCerenkov").set(True)
    op.property("UseScintillation").set(True)

    task.show()
    task.run()
