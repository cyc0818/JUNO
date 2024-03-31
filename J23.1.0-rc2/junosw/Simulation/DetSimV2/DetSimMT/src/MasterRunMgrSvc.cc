#include <boost/python.hpp>

#include <DetSimAlg/IDetSimFactory.h>

#include <DetSimMT/MasterRunMgrSvc.hh>
#include <DetSimMT/tbbUserWorkerInitialization.hh>

#include <SniperKernel/SvcFactory.h>
#include <SniperKernel/Task.h>
#include <SniperKernel/SniperPtr.h>

#include "G4UImanager.hh"

// #include "JUNOPhysicsList.hh"
// #include "JUNODetectorConstruction.hh"
// #include "JUNOActionInitialization.hh"

DECLARE_SERVICE(MasterRunMgrSvc);

MasterRunMgrSvc::MasterRunMgrSvc(const std::string& name)
    : SvcBase(name)
    , m_runmgr(0)
{
    declProp("RunMac", m_runmac="");
    declProp("NTasks", m_ntasks=1);
    declProp("Modulo", m_event_modulo=1);
    declProp("SeedOnce", m_seed_once=1);

    // application specific
    declProp("Recorders", m_recorder_names);
    declProp("DetFactory", m_factory_name);
}

MasterRunMgrSvc::~MasterRunMgrSvc()
{
    // delete m_runmgr;
}

bool
MasterRunMgrSvc::initialize()
{
    LogDebug << "start MasterRunMgrSvc::initialize()" << std::endl; 
    std::cout << "... " << __LINE__ << std::endl;
    std::cout << "... " << __LINE__ << std::endl;
    m_runmgr = new tbbMasterRunManager();
    std::cout << "... " << __LINE__ << std::endl;
    LogInfo << "tbbMasterRunManager::GetEventModulo (original): "
            << m_runmgr->GetEventModulo()
            << std::endl;
    m_runmgr->SetEventModulo(m_event_modulo); // 1 event per modulo
    m_runmgr->SetUserInitialization(new tbbUserWorkerInitialization );

    // As a framework, register following classes via IDetSimFactory
    SniperPtr<IDetSimFactory> ptr_det_factory(*getParent(), m_factory_name);
    if (ptr_det_factory.invalid()) {
        LogError << "Failed to Load the DetSim Factory. " << std::endl;
        return false;
    }
    det_factory = ptr_det_factory.data();


    // Set mandatory initialization classes
    //
    // Detector construction
    LogDebug << "initialize detector construction. " << std::endl;
    G4VUserDetectorConstruction* detector = det_factory->createDetectorConstruction();
    m_runmgr->SetUserInitialization(detector);

    // Physics list
    // G4VModularPhysicsList* physicsList = new JUNOPhysicsList;
    // // physicsList->SetVerboseLevel(1);
    G4VUserPhysicsList* physicsList = det_factory->createPhysicsList();
    m_runmgr->SetUserInitialization(physicsList);

    // User action initialization
    // JUNOActionInitialization* useraction_init = new JUNOActionInitialization();
    // useraction_init->set_recorder_names(m_recorder_names);
    G4VUserActionInitialization* useraction_init = det_factory->createUserActionInitialization();
    if (!useraction_init) {
        LogError << "Failed to create UserActionInitialization. " << std::endl;
        return false;
    }
    m_runmgr->SetUserInitialization(useraction_init);

    m_runmgr->Initialize();

    // LT: call mac to initialize or set verbose
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4String cmd = "/control/execute ";
    if (m_runmac.size()) {
        cmd += m_runmac;
        LogInfo << "call geant4 macro: " << cmd << std::endl;
        UImanager->ApplyCommand(cmd);
    }

    // LT: try use BeamOn directly.
    //     But now, we are not smart. User give us:
    //       * ntasks
    //       * evtmax per task
    //     So we need to calculate it.
    G4int nEvents = dynamic_cast<Task*>(getParent())->evtMax();
    G4int Ntasks = m_ntasks;
    m_runmgr->BeamOn(nEvents*Ntasks);

    m_runmgr->SetSeedOncePerCommunication(m_seed_once);

    return true;
}

bool
MasterRunMgrSvc::finalize()
{
    m_runmgr->RunTermination();
    return true;
}

