#pragma once
/**
U4RecorderAnaMgr
===================

This class depends on Opticks sources, specifically U4Recorder which 
enables full point-by-point details of optical photons to be recorded 
into Opticks/SEvt format and persisted into .npy files for NumPy based analysis.  

NB U4RecorderAnaMgr is not required for using Opticks, it is only used 
for Geant4 level validations. 

**/

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"

struct U4Recorder ; 

class U4RecorderAnaMgr: public IAnalysisElement,
                         public ToolBase {
public:

    U4RecorderAnaMgr(const std::string& name);
    ~U4RecorderAnaMgr();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    virtual void UserSteppingAction(const G4Step*);

    U4Recorder*  m_recorder ; 
};

