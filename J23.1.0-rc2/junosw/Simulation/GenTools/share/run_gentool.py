#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

# using the new Mgr.


import Sniper

def test_gun():
    from GenTools import makeTV
    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["gamma", "gamma", "e+"])
    gun.property("particleMomentums").set([1.173, 1.333, 1.0])
    gun.property("DirectionMode").set("Fix")
    gun.property("Directions").set([makeTV(0,0,-1), makeTV(0,0,-1), makeTV(0,0,-1)])

    #gun.setProp("PositionMode", "FixOne")
    #gun.setProp("Positions", [makeTV(0,0,15500)])

    gun.property("PositionMode").set("FixMany")
    gun.property("Positions").set([makeTV(0,0,15500), makeTV(0,0,15500), makeTV(0,0,-155)])

    return gun

def test_hepmc():
    gun = gt.createTool("GtHepEvtGenTool/gun")
    #gun.setProp("Source", "k40.asc")
    gun.property("Source").set("K40.exe -seed 42 -n 100|")
    return gun

def test_optical_gun():
    from GenTools import makeTV
    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set(["opticalphoton"])
    gun.property("particleMomentums").set([3e-3]) # 3eV

    return gun

def test_pelletron_gun():
    from GenTools import makeTV
    gun = gt.createTool("GtPelletronBeamerTool/gun")
    gun.property("particleName").set("e-")
    gun.property("planeCentrePos").set(makeTV(0,0,1e3)) # (0,0,1m)
    gun.property("planeDirection").set(makeTV(0,0,-1)) # down
    gun.property("planeRadius").set(20) # 20mm
    import math
    gun.property("beamThetaMax").set(math.radians(10)) # 10deg -> rad
    gun.property("beamMomentum").set(1.) # 1MeV
    gun.property("beamMomentumSpread").set(0.1) # 0.1MeV
    return gun

def test_supernova_gun():
    gun = gt.createTool("GtSNTool/gun")
    gun.property("inputSNFile").set("one-evt-supernova.root")
    gun.property("StartIndex").set(10000)
    return gun

def test_neutron_gun():
    gun = gt.createTool("GtNeutronTool/gun")
    gun.property("inputFile").set("neutron.root")
    gun.property("startIndex").set(10000)
    return gun

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.setEvtMax(10)
    task.setLogLevel(0)

    # = Data Buffer Mgr =
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

    rootwriter.property("Output").set({"GENEVT":"genevt.root"})

    # = gen tools =
    import GenTools
    gt = task.createAlg("GenTools")
    print gt

    dumper = gt.createTool("GtHepMCDumper")
    print dumper
    
    # TODO
    #gun = test_hepmc()
    #gun = test_gun()
    #gun = test_optical_gun()
    #gun = test_pelletron_gun()
    #gun = test_supernova_gun()
    gun = test_neutron_gun()

    gt.property("GenToolNames").set([gun.objName(), dumper.objName()])

    gt = task.createAlg("PostGenTools")

    task.show()
    task.run()
