#pragma once

/**
G4OpticksAnaMgr
===================

This class is used for Opticks validation, by instrumenting 
Geant4 to allow it to write full details of every step of optical photons 
in the same format OpticksEvent/NumPy arrays as those written by Opticks 
in non-production mode.

NB this class is not required for using Opticks, it
is only for validation. 

**/

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"

#ifdef WITH_G4OPTICKS
class DsG4Scintillation ; 
#endif

struct G4OpticksRecorder ; 

class G4OpticksAnaMgr: public IAnalysisElement,
                       public ToolBase {
public:

    G4OpticksAnaMgr(const std::string& name);
    ~G4OpticksAnaMgr();

    void BeginOfRun();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    virtual void UserSteppingAction(const G4Step*);

private:
#ifdef WITH_G4OPTICKS
     DsG4Scintillation* getScintillationProcess() const ;
     void               saveProperty(const char* prop, const char* dir, const char* name) const ;
     void               saveScintillationWavelengthSamples( const char* dir, const char* name ) const ;
     void               saveScintillationIntegral(const char* dir, const char* name) const ;
#endif
private:
    G4OpticksRecorder*  m_recorder ; 
};

