#ifndef InteresingProcessAnaMgr_hh
#define InteresingProcessAnaMgr_hh

/*
 * Description:
 *   This class is a tool to collect the interested processes. 
 *   It will save all the related tracks and vertices.
 *
 * Author: 
 *   Tao Lin <lintao AT ihep.ac.cn>
 */


#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IAnalysisElement.h"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "TTree.h"
#include <vector>
#include <unordered_map>
#include <string>

class ISimTrackSvc;

class InteresingProcessAnaMgr: public IAnalysisElement,
                               public ToolBase{
public:

    InteresingProcessAnaMgr(const std::string& name);
    ~InteresingProcessAnaMgr();
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
    void selectMichel(const G4Track* aTrack);
    void selectNeutronCapture(const G4Track* aTrack);
    
    // Store the decay secondaries (preliminary impl)
    // -- Tao Lin <lintao@ihep.ac.cn>, 16 Aug 2020
    void selectDecay(const G4Track* aTrack);

    // ====================================================================== //
    // Store secondaries with customized processes.
    // The idea is looping the secondaries and create vertices according to 
    // the process and position/time
    // -- Tao Lin, 21 March 2023

    struct InterestedProcessMgr {
        std::unordered_map<std::string, int> procname2use;
        std::unordered_map<const G4VProcess*, int> proc2use;
        std::unordered_map<G4ParticleDefinition*, int> particle2skip;
    } ipm;

    void selectInterested(const G4Track* parentTrack);

private:
    void saveMichel(const G4Track* aTrack);
    void saveNeutronCapture(const G4Track* aTrack);
    void saveNeutronCaptureSecondary(const G4Track* aTrack);

    void saveDecayInit(const G4Track* aTrack);
    void saveDecayExit(const G4Track* aTrack);

    // a general function to store the secondaries to the SimTrackSvc
    void saveSecondaryInit(const G4Track* aTrack);
    void saveSecondaryExit(const G4Track* aTrack);

private:
    // Evt Data
    TTree* m_michel_tree = nullptr;
    int m_eventID;
    // - michael electron
    std::vector<int>    michael_pdgid;
    std::vector<float>  michael_kine ;
    std::vector<float>  michael_px   ;
    std::vector<float>  michael_py   ;
    std::vector<float>  michael_pz   ;
    std::vector<float>  michael_pos_x;
    std::vector<float>  michael_pos_y;
    std::vector<float>  michael_pos_z;
    std::vector<double> michael_t    ;
    
    // - neutron capture in LS? TODO: Only select the specific material
    // -- detron + gamma (2.2MeV)
    // -- C13    + gamma (4.95MeV)
    // -- C13    + gamma (3.68MeV)
    //           + gamma (1.26MeV)
    TTree* m_neutron_tree = nullptr;
    // This only contains the neutron's info
    std::vector<int>    neutron_only_trkid  ;
    std::vector<float>  neutron_only_kine   ;
    std::vector<double> neutron_only_t      ;
    std::vector<float>  neutron_only_start_x;
    std::vector<float>  neutron_only_start_y;
    std::vector<float>  neutron_only_start_z;
    std::vector<float>  neutron_only_stop_x ;
    std::vector<float>  neutron_only_stop_y ;
    std::vector<float>  neutron_only_stop_z ;
    std::vector<float>  neutron_only_track_length;

    // It will contain all secondaries
    // please use the track id to seperate different
    // captures (assume it will have several captures)
    std::vector<int>    neutron_trkid; // parent track id (i.e. neutron)
    std::vector<int>    neutron_pdgid;
    std::vector<float>  neutron_kine ;
    std::vector<float>  neutron_px   ;
    std::vector<float>  neutron_py   ;
    std::vector<float>  neutron_pz   ;
    std::vector<float>  neutron_pos_x;
    std::vector<float>  neutron_pos_y;
    std::vector<float>  neutron_pos_z;
    std::vector<double> neutron_t    ;
private:
    ISimTrackSvc* simtracksvc = nullptr;
private:
   std::vector<int>  michel_trkid;
   std::vector<int>  neutron_sec_trkid;

private:
    // for debugging:
    // collect all the physics processes
    std::unordered_map<const G4VProcess*, int> proc_map;

    // user defined properties
    // - extra physics process names
    std::vector<std::string> m_extra_procnames;
    // - legacy selection
    bool m_legacy_michel_electron = false;
    bool m_legacy_ncapture = false;
    bool m_legacy_decay = false; // include decay and radioactivity decay
};

#endif
