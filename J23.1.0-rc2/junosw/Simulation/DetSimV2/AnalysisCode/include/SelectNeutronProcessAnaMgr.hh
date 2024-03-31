#ifndef SelectNeutronProcessAnaMgr_hh
#define SelectNeutronProcessAnaMgr_hh


/*
Author: Hu Yuxiang   
email:huyuxiang@ihep.ac.cn

This anamgr used to debug the neutron process
it will classify the neutron process and tag the event ID
the event ID will be saved in /tmp/  in .npy file (like the below):

===========================
[10:38:58][lxslc701.ihep.ac.cn]~/junofs/2021-2-23/NucleonDecay/Signal/SimAna % ls /tmp/m_tag_
m_tag_4_gamma.npy    m_tag_alpha.npy      m_tag_B11.npy        m_tag_C12.npy        m_tag_inelastic.npy  m_tag_noC12.npy

============================

You can use this anamgr by adding the option   "--anamgr-neutron-inelastic" 

*/

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include "TTree.h"
#include <vector>
class ISimTrackSvc;

class SelectNeutronProcessAnaMgr: public IAnalysisElement,
                               public ToolBase{
public:

    SelectNeutronProcessAnaMgr(const std::string& name);
    ~SelectNeutronProcessAnaMgr();
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
    ISimTrackSvc* simtracksvc;
    std::vector<int> m_tag_inelastic;
	std::vector<int> m_tag_B11;
	std::vector<int> m_tag_noC12;
	std::vector<int> m_tag_C11;
	std::vector<int> m_tag_C12;
	std::vector<int> m_tag_alpha;
	std::vector<int> m_tag_4_gamma;
    void SelectNeutronInelastic(const G4Step* aStep);
};

#endif
