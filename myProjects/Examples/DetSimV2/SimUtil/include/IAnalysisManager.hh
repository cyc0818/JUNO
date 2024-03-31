#ifndef IAnalysisManager_hh
#define IAnalysisManager_hh

class G4Run;
class G4Event;
class G4Track;
class G4Step;

class IAnalysisManager {
public:
    // Run Action
    virtual void BeginOfRunAction(const G4Run*) = 0;
    virtual void EndOfRunAction(const G4Run*) = 0;
    // Event Action
    virtual void BeginOfEventAction(const G4Event*) = 0;
    virtual void EndOfEventAction(const G4Event*) = 0;
    // Stacking Action
    // Tracking Action
    virtual void PreUserTrackingAction(const G4Track*) {}
    virtual void PostUserTrackingAction(const G4Track*) {}
    // Stepping Action
    virtual void UserSteppingAction(const G4Step*) {}

    virtual ~IAnalysisManager() {}
};

#endif
