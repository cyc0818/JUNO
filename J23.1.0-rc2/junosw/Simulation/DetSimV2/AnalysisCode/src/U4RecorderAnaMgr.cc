#include "U4RecorderAnaMgr.hh"
#include "SniperKernel/ToolFactory.h"

#ifdef WITH_G4CXOPTICKS
#include "U4Recorder.hh"
#endif

DECLARE_TOOL(U4RecorderAnaMgr);

U4RecorderAnaMgr::U4RecorderAnaMgr(const std::string& name)
    : 
    ToolBase(name),
#ifdef WITH_G4CXOPTICKS
    m_recorder(new U4Recorder)
#else
    m_recorder(nullptr)
#endif
{
}

U4RecorderAnaMgr::~U4RecorderAnaMgr()
{
#ifdef WITH_G4CXOPTICKS
    delete m_recorder ; 
#endif
}

#ifdef WITH_G4CXOPTICKS
void U4RecorderAnaMgr::BeginOfRunAction(const G4Run* run) {         m_recorder->BeginOfRunAction(run);       }
void U4RecorderAnaMgr::EndOfRunAction(const G4Run* run) {           m_recorder->EndOfRunAction(run);         }
void U4RecorderAnaMgr::BeginOfEventAction(const G4Event* evt) {     m_recorder->BeginOfEventAction(evt);     }
void U4RecorderAnaMgr::EndOfEventAction(const G4Event* evt) {       m_recorder->EndOfEventAction(evt);       } 
void U4RecorderAnaMgr::PreUserTrackingAction(const G4Track* trk) {  m_recorder->PreUserTrackingAction(trk);  } 
void U4RecorderAnaMgr::PostUserTrackingAction(const G4Track* trk) { m_recorder->PostUserTrackingAction(trk); } 
void U4RecorderAnaMgr::UserSteppingAction(const G4Step* step) {     m_recorder->UserSteppingAction(step);    } 
#else
void U4RecorderAnaMgr::BeginOfRunAction(const G4Run* ) {} 
void U4RecorderAnaMgr::EndOfRunAction(const G4Run* ) {} 
void U4RecorderAnaMgr::BeginOfEventAction(const G4Event* ) {}
void U4RecorderAnaMgr::EndOfEventAction(const G4Event* ) {}
void U4RecorderAnaMgr::PreUserTrackingAction(const G4Track* ) {}
void U4RecorderAnaMgr::PostUserTrackingAction(const G4Track* ) {}
void U4RecorderAnaMgr::UserSteppingAction(const G4Step* ) {}
#endif

