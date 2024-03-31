#ifndef OPSimAnaMgr_hh
#define OPSimAnaMgr_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include "TTree.h"
#include "TH1I.h"
#include <map>

class IOPSimSvc;
class BirksLawCalculator;

class OPSimAnaMgr: public IAnalysisElement,
                   public ToolBase{
public:

    OPSimAnaMgr(const std::string& name);
    ~OPSimAnaMgr();
    // Run Action
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    // Event Action
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    virtual void PreUserTrackingAction(const G4Track* aTrack);
    virtual void PostUserTrackingAction(const G4Track* aTrack);

    virtual void UserSteppingAction(const G4Step* step);

private:
    BirksLawCalculator* m_birksLawCalculator;

    Int_t m_eventID;
    IOPSimSvc* m_opsimsvc;

private:
    // = for testing only =
    // == ratio: the ratio used to simulate with OP ==
    double m_testing_ratio;
};

#endif
