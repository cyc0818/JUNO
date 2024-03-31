#include <boost/python.hpp>
#include <DetSimMT/SlaveRunMgrSvc.hh>

#include <SniperKernel/SvcFactory.h>
#include <SniperKernel/SniperLog.h>
#include <SniperKernel/Task.h>

#include <pthread.h>

#include "G4Threading.hh"
#include "G4String.hh"
#include "G4WorkerRunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UserWorkerThreadInitialization.hh"
#include "G4WorkerThread.hh"
#include "G4MTRunManagerKernel.hh"
#include "G4AutoLock.hh"
#include "G4UserWorkerInitialization.hh"
#include "G4VUserActionInitialization.hh"

DECLARE_SERVICE(SlaveRunMgrSvc);

#include <tbb/atomic.h>
namespace {
    tbb::atomic<int> counter;
}

SlaveRunMgrSvc::SlaveRunMgrSvc(const std::string& name)
    : SvcBase(name)
    , m_runmgr(0)
    , beamOnCondition(false)
{

}

SlaveRunMgrSvc::~SlaveRunMgrSvc()
{
    // delete m_runmgr;
}


bool
SlaveRunMgrSvc::initialize()
{
    const std::string& taskname = scope();
    unsigned threadid = (unsigned) pthread_self();
    LogDebug << "task " << taskname
            << " running in thread " << threadid
            << std::endl;

    // ===========================================================
    // based on examples/extended/parallel/TBB/B2b/src/tbbTask.cc
    // ===========================================================
    static G4ThreadLocal tbbWorkerRunManager* localRM = 0;

    // ===========================================================
    //  BEGIN initialize thread local worker 
    // ===========================================================
    if (!localRM) {
    G4MTRunManager* masterRM = G4MTRunManager::GetMasterRunManager();
    assert(masterRM);

    //============================
    //Step-0: Thread ID
    //============================
    //Initliazie per-thread stream-output
    //The following line is needed before we actually do I/O initialization
    //because the constructor of UI manager resets the I/O destination.
    G4int thisId = counter.fetch_and_increment();
    G4Threading::G4SetThreadId( thisId );
    G4UImanager::GetUIpointer()->SetUpForAThread( thisId );

    //============================
    //Step-1: Random number engine
    //============================
    //RNG Engine needs to be initialized by "cloning" the master one.
    const CLHEP::HepRandomEngine* masterEngine =
          masterRM->getMasterRandomEngine();
    assert(masterEngine);
    masterRM->GetUserWorkerThreadInitialization()->SetupRNGEngine(masterEngine);

    //============================
    //Step-2: Initialize worker thread
    //============================
    if(masterRM->GetUserWorkerInitialization()) {
        masterRM->GetUserWorkerInitialization()->WorkerInitialize();
    }
    if(masterRM->GetUserActionInitialization()) {
        G4VSteppingVerbose* sv =
        masterRM->GetUserActionInitialization()->InitializeSteppingVerbose();
        if (sv) G4VSteppingVerbose::SetInstance(sv);
    }
    //Now initialize worker part of shared objects (geometry/physics)
    G4WorkerThread::BuildGeometryAndPhysicsVector();
    // LT: should we create it directly?
    localRM = static_cast<tbbWorkerRunManager*>(
                    masterRM->GetUserWorkerThreadInitialization()->CreateWorkerRunManager()
                                                );
    G4cout<<localRM<<G4endl;
    //localRM->SetWorkerThread(wThreadContext);
//    G4AutoLock wrmm(&workerRMMutex);
//    G4MTRunManagerKernel::workerRMvector->push_back(localRM); //<<<<?????? ANDREA TBB
//    wrmm.unlock();

    //================================
    //Step-3: Setup worker run manager
    //================================
    // Set the detector and physics list to the worker thread. Share with master
    const G4VUserDetectorConstruction* detector = 
        masterRM->GetUserDetectorConstruction();
    localRM->G4RunManager::SetUserInitialization(
        const_cast<G4VUserDetectorConstruction*>(detector));
    const G4VUserPhysicsList* physicslist = masterRM->GetUserPhysicsList();
    localRM->SetUserInitialization(const_cast<G4VUserPhysicsList*>(physicslist));


    //================================
    //Step-4: Initialize worker run manager
    //================================
    if(masterRM->GetUserActionInitialization())
      { masterRM->GetNonConstUserActionInitialization()->Build(); }
    if(masterRM->GetUserWorkerInitialization())
      { masterRM->GetUserWorkerInitialization()->WorkerStart(); }
    localRM->Initialize();
    
    //Problem at this point is if there is more than one run...
    // Execute UI commands stored in the masther UI manager
    std::vector<G4String> cmds = masterRM->GetCommandStack();
    G4UImanager* uimgr = G4UImanager::GetUIpointer(); //TLS instance
    std::vector<G4String>::const_iterator it = cmds.begin();
    for(;it!=cmds.end();it++)
    { uimgr->ApplyCommand(*it); }
    //Start this run
    G4String macroFile = masterRM->GetSelectMacro();
    G4int numSelect = masterRM->GetNumberOfSelectEvents();
    G4int nEvents = dynamic_cast<Task*>(getParent())->evtMax();

    LogDebug << "LT macroFile: '" << macroFile << "'" << std::endl;
    LogDebug << "LT numSelect: " << numSelect << std::endl;
    LogDebug << "LT nEvents: " << nEvents << std::endl;
    if ( macroFile == "" || macroFile == " " )
    {
        // LT: 24th Oct. 2016
        //
        // I found Step-4 will do all the simulation. So we need to split this BeamOn method!
        // here, just skip it
        // localRM->BeamOn(nEvents,0,numSelect);
    }
    else
    {
        LogDebug << "LT call function localRM->BeamOn(nEvents,macroFile,numSelect);" << std::endl;
        localRM->BeamOn(nEvents,macroFile,numSelect);
    }

    LogDebug << "LT Step-4 done." << std::endl;

    //======= NEW TBB SPECIFIC ===========
    //Step-5: Initialize and start run
    //====================================
    // bla-bla-bla-bla
    // This is basically BeamOn 
    beamOnCondition = localRM->ConfirmBeamOnCondition();
    LogDebug << "LT Step-5 beamOnCondition: " << beamOnCondition << std::endl;
    if (beamOnCondition) {
        localRM->SetNumberOfEventsToBeProcessed( nEvents );
        localRM->ConstructScoringWorlds(); 
        localRM->RunInitialization();
        //Register this G4Run in output
        //Note: the idea is that we are going to accumulate everything in 
        //G4Run or derived class. We let the kernel know this is the object
        //where the output is accumulated for the tbb::tasks that run on 
        //this thread.
        // if ( output ) {}
    }

    LogDebug << "LT Step-5 done." << std::endl;


    // ===========================================================
    //   END  initialize thread local worker 
    // ===========================================================
    }

    m_runmgr = localRM;


    return true;
}

bool
SlaveRunMgrSvc::finalize()
{
    return true;
}

tbbWorkerRunManager*
SlaveRunMgrSvc::getRunMgr()
{
    return m_runmgr;
}
