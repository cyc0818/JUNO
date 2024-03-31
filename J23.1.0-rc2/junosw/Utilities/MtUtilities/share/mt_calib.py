#!/usr/bin/env python

class SharedElemMgr():
    '''the class to manage all of the shared elements'''
    def __init__(self):
        self.elems = {}

    def create(self, identifier):
        elem = Sniper.create(identifier)
        self.elems[elem.objName()] = elem
        return elem

    #TODO: use index in case of duplicated names
    def get(self, name, index = None):
        return self.elems[name]

def config_shared_svcs(shared_elem_mgr):
    srgeo = shared_elem_mgr.create("SharedElem<RecGeomSvc>/RecGeomSvc")
    srgeo.property("FastInit").set(True)
    srgeo.property("GeomFile").set("default")
    srgeo.property("GeomPathInRoot").set("JunoGeom")

    shared_elem_mgr.create("SharedElem<MCParamsFileSvc>/MCParamsSvc")
    shared_elem_mgr.create("SharedElem<PMTParamSvc>/PMTParamSvc")

    ssec = shared_elem_mgr.create("SharedElem<SpmtElecConfigSvc>/SpmtElecConfigSvc")
    ssec.property("ConfigFile").set("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/offline/Examples/TestSpmtElecAlg/share/SpmtElecConfig.txt")
    ssec.setLogLevel(4)

    shared_elem_mgr.create("SharedElem<PMTSimParamSvc>/PMTSimParamSvc")
    shared_elem_mgr.create("SharedElem<PMTCalibSvc>/PMTCalibSvc").property("DBcur").set(20201219)

    scdb = shared_elem_mgr.create("SharedElem<CondDBSvc>/CondDBSvc")
    scdb.property("GlobalTag").set("gtname")
    scdb.property("RepoTypes").set({"frontier": "Frontier"})
    scdb.property("RepoURIs").set({"frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"})

    shared_elem_mgr.create("MtRootWriter/RootWriter").property("Output").set({"USER_OUTPUT": "sample_calib_user.root"})

    return shared_elem_mgr

def CalibJob():
    global shared_elem_mgr
    task = SniperMuster.createWorker("TopTask/junotoptask")

    task.createSvc("BufferMemMgr").property("TimeWindow").set([0, 0])
    task.createSvc("MtNavBufferMgr")
    task.addSvc(shared_elem_mgr.get("RootWriter"))
    task.addSvc(shared_elem_mgr.get("RecGeomSvc"))
    task.addSvc(shared_elem_mgr.get("MCParamsSvc"))
    task.addSvc(shared_elem_mgr.get("PMTParamSvc"))
    task.addSvc(shared_elem_mgr.get("SpmtElecConfigSvc"))
    task.addSvc(shared_elem_mgr.get("PMTSimParamSvc"))
    task.addSvc(shared_elem_mgr.get("PMTCalibSvc"))
    task.addSvc(shared_elem_mgr.get("CondDBSvc"))
    
    adec = task.createAlg("Deconvolution")
    adec.property("DBcur").set(20201219)
    adec.property("Threshold1").set(0.03)
    adec.property("Window1").set(7)
    adec.property("Window2").set(8)
    task.createAlg("TimeRec").property("AlgOption").set("fit1")
    task.createAlg("SPMTCalibAlg")

    return task

def GlobalInput():
    task = Sniper.Task("GlobalInput")
    task.createSvc("BufferMemMgr").property("TimeWindow").set([0, 0])
    task.createSvc("RootInputSvc/InputSvc").property("InputFile").set("sample_elecsim.root")
    falg = task.createAlg("FillGlobalBuffer")
    falg.property("ReadAhead").set(["/Event/Elec"])
    return task

def GlobalOutput():
    task = Sniper.Task("GlobalOutput")
    task.createSvc("BufferMemMgr").property("TimeWindow").set([0, 0])
    task.createSvc("RootOutputSvc/OutputSvc").property("OutputStreams").set({"/Event/Calib": "sample_calib.root"})
    task.createAlg("PruneGlobalBuffer")
    return task

if __name__ == "__main__":
    ## load libraries
    import Sniper, SniperMuster
    import BufferMemMgr, RootIOSvc, RootWriter, Geometry
    Sniper.loadDll("libMCParamsSvc.so")
    Sniper.loadDll("libSpmtElecConfigSvc.so")
    Sniper.loadDll("libPMTSimParamSvc.so")
    Sniper.loadDll("libPMTCalibSvc.so")
    Sniper.loadDll("libCondDB.so")
    Sniper.loadDll("libDeconvolution.so")
    Sniper.loadDll("libTimeRec.so")
    Sniper.loadDll("libSPMTCalibAlg.so")
    Sniper.loadDll("libMtUtilities.so")

    ## config shared services
    global shared_elem_mgr
    shared_elem_mgr = config_shared_svcs(SharedElemMgr())

    ## I/O and global buffer
    gs = SniperMuster.createGlobalStream("JunoGlobalStream")
    gs.configBuffer(20, 10)  #the buffer must be created before the I/O tasks
    gs.configInput(GlobalInput)
    gs.configOutput(GlobalOutput)

    ## config the muster
    muster = SniperMuster.Muster()
    muster.config(CalibJob, 4)
    muster.setIO(gs)
    muster.setEvtMax(-1)

    ## show the configuration and run
    SniperMuster.show()
    muster.run()
