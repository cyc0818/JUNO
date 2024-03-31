test_var_evtNum = 3

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
    geoSvc = shared_elem_mgr.create("SharedElem<RecGeomSvc>/RecGeomSvc")
    geoSvc.property("FastInit").set(True)
    geoSvc.property("GeomFile").set("default")
    geoSvc.property("GeomPathInRoot").set("JunoGeom")

    shared_elem_mgr.create("SharedElem<MCParamsFileSvc>/MCParamsSvc")
    shared_elem_mgr.create("SharedElem<PMTParamSvc>/PMTParamSvc")

    ssec = shared_elem_mgr.create("SharedElem<SpmtElecConfigSvc>/SpmtElecConfigSvc")
    ssec.property("ConfigFile").set("/sharefs/bes/zoujh/juno/MtOffline/mt.offline/Examples/TestSpmtElecAlg/share/SpmtElecConfig.txt")

    shared_elem_mgr.create("SharedElem<PMTSimParamSvc>/PMTSimParamSvc")
    shared_elem_mgr.create("SharedElem<PMTCalibSvc>/PMTCalibSvc").property("DBcur").set(20201219)
    
    scdb = shared_elem_mgr.create("SharedElem<CondDBSvc>/CondDBSvc")
    scdb.property("GlobalTag").set("gtname")
    scdb.property("RepoTypes").set({"frontier": "Frontier"})
    scdb.property("RepoURIs").set({"frontier": "http://junodb1.ihep.ac.cn:8080/Frontier"})

    shared_elem_mgr.create("MtRootWriter/RootWriter").property("Output").set({"USER_OUTPUT": "sample_MTrec_user_"+str(test_var_evtNum)+".root"})

    return shared_elem_mgr


def RecTimeLikeJob():
    global shared_elem_mgr
    task = SniperMuster.createWorker("TopTask/junotoptask")

    task.createSvc("BufferMemMgr").property("TimeWindow").set([0,0])
    task.createSvc("MtNavBufferMgr")
    task.addSvc(shared_elem_mgr.get("RootWriter"))
    task.addSvc(shared_elem_mgr.get("RecGeomSvc"))
    task.addSvc(shared_elem_mgr.get("MCParamsSvc"))
    task.addSvc(shared_elem_mgr.get("PMTParamSvc"))
    task.addSvc(shared_elem_mgr.get("SpmtElecConfigSvc"))
    task.addSvc(shared_elem_mgr.get("PMTSimParamSvc"))
    task.addSvc(shared_elem_mgr.get("PMTCalibSvc"))
    task.addSvc(shared_elem_mgr.get("CondDBSvc"))

    alg = task.createAlg("RecTimeLikeAlg")
    alg.property("TotalPMT").set(54318)
    alg.property("SwitchAlg").set(1)
    alg.property("File_path").set("/sharefs/bes/zoujh/juno/MtOffline/mt.offline/Reconstruction/RecTimeLikeAlg/share/no-elec")

    return task

def GlobalInput():
    task = Sniper.Task("GlobalInput")
    task.createSvc("BufferMemMgr").property("TimeWindow").set([0, 0])
    task.createSvc("RootInputSvc/InputSvc").property("InputFile").set("ok_calib.root")
    falg = task.createAlg("FillGlobalBuffer")
    falg.property("ReadAhead").set(["/Event/Calib"])
    return task

def GlobalOutput():
    task = Sniper.Task("GlobalOutput")
    task.createSvc("BufferMemMgr").property("TimeWindow").set([0, 0])
    task.createSvc("RootOutputSvc/OutputSvc").property("OutputStreams").set({"/Event/Calib": "sample_MTrec_"+str(test_var_evtNum)+"_Mt.root", "/Event/Rec": "sample_MTrec_"+str(test_var_evtNum)+"_Mt.root"})
    task.createAlg("PruneGlobalBuffer")
    return task


if __name__ == "__main__":
    ##ROOT thread safety
    import ROOT
    ROOT.EnableThreadSafety()

    ## load libraries
    import Sniper, SniperMuster
    import BufferMemMgr, RootIOSvc, RootWriter, Geometry
    Sniper.loadDll("libRecTimeLikeAlg.so")
    Sniper.loadDll("libPMTSimParamSvc.so")
    Sniper.loadDll("libMCParamsSvc.so")
    Sniper.loadDll("libCondDB.so")
    Sniper.loadDll("libPMTCalibSvc.so")
    Sniper.loadDll("libGeometry.so")
    Sniper.loadDll("libSpmtElecConfigSvc.so")
    Sniper.loadDll("libMtUtilities.so")


    ## config shared services
    global shared_elem_mgr
    shared_elem_mgr = config_shared_svcs(SharedElemMgr())

    ## I/O and global buffer
    gs = SniperMuster.createGlobalStream("JunoGlobalStream")
    gs.configBuffer(20, 8)  #the buffer must be created before the I/O tasks
    gs.configInput(GlobalInput)
    gs.configOutput(GlobalOutput)

    ## config the muster
    muster = SniperMuster.Muster()
    muster.config(RecTimeLikeJob, 8)
    muster.setIO(gs)
    muster.setEvtMax(test_var_evtNum)

    ## show the configuration and run
    SniperMuster.show()
    muster.run()
