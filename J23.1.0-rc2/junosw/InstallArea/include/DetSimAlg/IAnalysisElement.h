#ifndef IAnalysisElement_h
#define IAnalysisElement_h

class G4Run;
class G4Event;
class G4Track;
class G4Step;

#include "G4ClassificationOfNewTrack.hh"


class IAnalysisElement {
public:
    virtual ~IAnalysisElement() {}

    // Run Action
    virtual void BeginOfRunAction(const G4Run*) = 0;
    virtual void EndOfRunAction(const G4Run*) = 0;
    //
    // Event Action
    virtual void BeginOfEventAction(const G4Event*) = 0;
    virtual void EndOfEventAction(const G4Event*) = 0;
    //
    // Stacking Action
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) {return fUrgent;}
    virtual void NewStage() {}
    virtual void PrepareNewEvent() {}
    //
    // Tracking Action
    virtual void PreUserTrackingAction(const G4Track*) {}
    virtual void PostUserTrackingAction(const G4Track*) {}
    //
    // Stepping Action
    virtual void UserSteppingAction(const G4Step*) {}
    //
};

#endif
