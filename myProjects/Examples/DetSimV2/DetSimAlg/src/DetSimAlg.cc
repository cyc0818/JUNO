
#include "DetSimAlg/DetSimAlg.h"
#include "SniperKernel/AlgFactory.h"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4Svc/IG4Svc.h"

DECLARE_ALGORITHM(DetSimAlg);

DetSimAlg::DetSimAlg(const std::string& name)
    : AlgBase(name)
{
    det_factory = NULL;
    run_manager = NULL;
    i_event = 0;
    m_run_mac = "";
    declProp("G4Svc", m_g4svc_name="G4Svc");
    declProp("DetFactory", m_factory_name);
    declProp("RunMac", m_run_mac);
    declProp("RunCmds", m_run_cmds);
    declProp("VisMac", m_vis_mac);
    declProp("StartEvtID", i_event);
    declProp("isMT", is_mt=false); // single thread mode by default
}

DetSimAlg::~DetSimAlg()
{
}

bool 
DetSimAlg::initialize()
{
    if (is_mt) {
        return true;
    }

    LogInfo << objName()
            << " initialized successfully"
            << std::endl;
    LogInfo << objName()
            << " : Load Factory Name: "
            << m_factory_name
            << std::endl;
    SniperPtr<IDetSimFactory> ptr_det_factory(*getParent(), m_factory_name);
    det_factory = ptr_det_factory.data();
    if (det_factory == 0) {
        LogError << "Failed to Load the DetSim Factory "
                 << std::endl;
        return false;
    }

    // create the run manager
    SniperPtr<IG4Svc> g4svc(*getParent(), m_g4svc_name);
    if (g4svc.invalid()) {
        LogError << "Failed to Load the G4Svc " << m_g4svc_name
                 << std::endl;
        return false;
    }
    run_manager = g4svc->getRunMgr();
    if (run_manager==NULL) {
        return false;
    }

    SetG4Init();

    // Load mac file.
    //
    if (m_vis_mac.length()) {
        // If user set the vis mac file,
        // start to run the GUI
        StartG4Vis();
        return false;
    } 
    SetG4RunMac();
    SetG4RunCmds();

    // Initialize G4 Kernel
    run_manager->Initialize();

    // Initialize Run
    if (run_manager->initializeRM()) {

    } else {
        return false;
    }

    return true;
}

bool
DetSimAlg::execute()
{
    if (is_mt) {
        return true;
    }

    if (m_vis_mac.length()) {
        // If user set the vis mac file,
        // start to run the GUI
        return false;
    } 
    LogInfo << objName()
            << " Simulate An Event ("
            << i_event
            << ") "
            << std::endl;
    if (!run_manager) {
        return false;
    }
    if (run_manager->SimulateEvent(i_event) ) {
        ++i_event;
        return true;
    } else {
        return false;
    }
}

bool
DetSimAlg::finalize()
{
    if (is_mt) {
        return true;
    }

    LogInfo << objName()
            << " finalized successfully"
            << std::endl;
    if (m_vis_mac.length()) {
        // If user set the vis mac file,
        // start to run the GUI
        return true;
    } 
    if (run_manager) {
        run_manager->finalizeRM();
    }
    return true;
}

bool 
DetSimAlg::SetG4Init()
{

    // The Geant4 Related
    G4VUserDetectorConstruction* detector = det_factory 
        -> createDetectorConstruction();
    run_manager->SetUserInitialization(detector);

    G4VUserPhysicsList* physics = det_factory
        -> createPhysicsList();
    run_manager->SetUserInitialization(physics);

    G4VUserPrimaryGeneratorAction* gen_action = det_factory
        -> createPrimaryGenerator();
    run_manager->SetUserAction(gen_action);

    // User Action
    G4UserRunAction* run_action = det_factory
        -> createRunAction();
    run_manager->SetUserAction(run_action);

    G4UserEventAction* event_action = det_factory
        -> createEventAction();
    run_manager->SetUserAction(event_action);

    G4UserStackingAction* stacking_action = det_factory
        -> createStackingAction();
    run_manager->SetUserAction(stacking_action);

    G4UserTrackingAction* tracking_action = det_factory
        -> createTrackingAction();
    run_manager->SetUserAction(tracking_action);

    G4UserSteppingAction* stepping_action = det_factory
        -> createSteppingAction();
    run_manager->SetUserAction(stepping_action);

    return true;
}

bool
DetSimAlg::SetG4RunMac()
{
    if (m_run_mac.length()) {
        // If the run.mac is set,
        // Load the run.mac
        G4UImanager * UImanager = G4UImanager::GetUIpointer();
        std::string command = "/control/execute ";
        UImanager->ApplyCommand( command + m_run_mac);
        return true;
    }
    return false;

}

bool
DetSimAlg::SetG4RunCmds()
{
    G4UImanager * UImanager = G4UImanager::GetUIpointer();
    for (std::vector<std::string>::iterator it = m_run_cmds.begin();
            it != m_run_cmds.end(); ++it) {
        UImanager->ApplyCommand(*it);
    }
    return true;
}

bool
DetSimAlg::StartG4Vis()
{
    LogInfo << "Start Geant4 Visual" << std::endl;
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    LogInfo << "Initialize Geant4 Vis Manager" << std::endl;

    G4UImanager * UImanager = G4UImanager::GetUIpointer();
    LogInfo << "Get Geant4 UI Manager" << std::endl;
    
    LogInfo << "Create Geant4 UI Executive" << std::endl;
    char* argv[1] = {"dummy_argv"};
    G4UIExecutive * ui = new G4UIExecutive(1,argv);
    std::string viscmd("/control/execute ");
    viscmd += m_vis_mac;
    LogInfo << "Execute Geant4 command" << viscmd << std::endl;
    UImanager->ApplyCommand(viscmd);

    ui->SessionStart();
    delete ui;
    delete visManager;

    return true;
}


