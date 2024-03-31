#ifndef MgrOfAnaMgr_hh
#define MgrOfAnaMgr_hh

#include <map>
#include <vector>
#include <string>
class IAnalysisElement;
class G4Run;
class G4Event;
class G4Track;
class G4Step;

class MgrOfAnaElem {
public:
    static MgrOfAnaElem& instance();

    bool reg(const std::string& key, IAnalysisElement*);
    bool get(const std::string& key, IAnalysisElement*&);

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    // TODO: Add Stacking Action
    void PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);
    void UserSteppingAction(const G4Step*);
   
private:
    // The reg order is important
    typedef std::map<std::string, IAnalysisElement*> K2AM;
    typedef std::vector<IAnalysisElement*> EXEORD;
    K2AM m_anamgrs;
    EXEORD m_order;

};

#endif
