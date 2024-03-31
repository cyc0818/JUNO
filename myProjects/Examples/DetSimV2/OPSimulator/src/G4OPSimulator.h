#ifndef G4OpSimulator_h
#define G4OpSimulator_h

/*
 * G4OpSimulator:
 *   simulator based on Geant4 engine.
 */

#include "OPSimulator/IOPSimulator.h"

#include "G4GenStepContainer.h"
#include "G4PhysicsOrderedFreeVector.hh"

class DsG4Scintillation;

class G4Track;
class G4ParticleDefinition;

namespace opsimulator {

class G4OpSimulator: public IOPSimulator {
public:
    G4OpSimulator();

    // fill the genstep
    virtual IGenStep* create_genstep();
    virtual bool clear_gensteps();

    // simulate an event or multiple events
    virtual bool simulate();

    // get the hit
    virtual int get_nhits();
    virtual IHit* get_hit(int idx);
    virtual bool clear_hits();

private:
    // create a track
    G4Track* nextTrack();
    int m_trackCounter;
    int m_currentTrackParentID;
private:
    G4GenStepContainer m_gs_container;
    int m_gs_len;
    int m_gs_idx;
    int m_gs_photon_len; // scintillation + cerenkov
    int m_gs_photon_idx;

    bool create_photons_from_gs();
    int num_scint_photons_from_gs();
    int num_ceren_photons_from_gs();
    int m_scint_photons_from_gs; // cache the number of photons from scint.
    int m_scint_photons_from_gs_fast;
    int m_scint_photons_from_gs_slow;
    int m_scint_photons_from_gs_slower;
    int m_ceren_photons_from_gs; //                             from ceren.

    G4Track* create_scint_photon();
    G4Track* create_ceren_photon();

    // related to scintillation
    bool init_scint();
    DsG4Scintillation* m_scint_proc;
    G4PhysicsOrderedFreeVector* fastScintillationIntegral;
    double fastTimeConstant;
    double slowTimeConstant;
    double slowerTimeConstant;
    double slowerRatio;

    double gammaSlowerTime;
    double gammaSlowerRatio;

    double alphaSlowerTime;
    double alphaSlowerRatio;

    double neutronSlowerTime;
    double neutronSlowerRatio;


    const G4ParticleDefinition* current_step_particle;
};

} // namespace

#endif
