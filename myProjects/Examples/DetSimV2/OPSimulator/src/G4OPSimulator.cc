#include "G4OPSimulator.h"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4StackManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LossTableManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenStep.h"
#include "G4ProcessManager.hh"
#include "DsG4Scintillation.h"

#include <algorithm>
#include <iostream>

namespace opsimulator {

    G4OpSimulator::G4OpSimulator() {
        m_trackCounter = 0;
        m_currentTrackParentID = 0;

        m_gs_len = 0;
        m_gs_idx = 0;
        m_gs_photon_len = 0;
        m_gs_photon_idx = 0;

        m_scint_photons_from_gs = 0;
        m_scint_photons_from_gs_fast = 0;
        m_scint_photons_from_gs_slow = 0;
        m_scint_photons_from_gs_slower = 0;

        m_ceren_photons_from_gs = 0;

        m_scint_proc = 0;
        fastScintillationIntegral = 0;

        fastTimeConstant = 0.0;
        slowTimeConstant = 0.0;
        slowerTimeConstant = 0.0;
        slowerRatio = 0.0;

        gammaSlowerTime = 0.0;
        gammaSlowerRatio = 0.0;
        alphaSlowerTime = 0.0;
        alphaSlowerRatio = 0.0;
        neutronSlowerTime = 0.0;
        neutronSlowerRatio = 0.0;

    }

    bool G4OpSimulator::init_scint() {

        
        if (!m_scint_proc) {
            G4ProcessManager* OpManager =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
            if (OpManager) {
                G4int MAXofPostStepLoops =
                    OpManager->GetPostStepProcessVector()->entries();
                G4ProcessVector* fPostStepDoItVector =
                    OpManager->GetPostStepProcessVector(typeDoIt);
                for ( G4int i=0; i<MAXofPostStepLoops; i++) {
                    G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
                    DsG4Scintillation* op =  dynamic_cast<DsG4Scintillation*>(fCurrentProcess);
                    if (op) {
                        m_scint_proc = op;
                        break;
                    }
                }
            }
     
        }

        // loading parameters from scintillation process
        if (m_scint_proc) {
            gammaSlowerTime = m_scint_proc->GetGammaSlowerTimeConstant();
            gammaSlowerRatio = m_scint_proc->GetGammaSlowerRatio();

            alphaSlowerTime = m_scint_proc->GetAlphaSlowerTimeConstant();
            alphaSlowerRatio = m_scint_proc->GetAlphaSlowerRatio();

            neutronSlowerTime = m_scint_proc->GetNeutronSlowerTimeConstant();
            neutronSlowerRatio = m_scint_proc->GetNeutronSlowerRatio();
        }

        return true;
    }

    IGenStep* G4OpSimulator::create_genstep() {
        IGenStep* gs = new G4GenStep<G4GenStepContainer>(m_gs_container);

        return gs;
    }

    bool G4OpSimulator::clear_gensteps() {
        m_gs_container.clear();
        return true;
    }

    bool G4OpSimulator::simulate() {

        if (!m_scint_proc) {
            init_scint();
        }

        G4EventManager* g4evtmgr = G4EventManager::GetEventManager();
        G4TrackingManager* g4trkingmgr = g4evtmgr->GetTrackingManager();
        G4StackManager* g4stackmgr = g4evtmgr->GetStackManager();

        m_gs_len = m_gs_container.size();
        // if no genstep, just skip simulation.
        if (m_gs_len<=0) {
            return true;
        }
        m_gs_idx = -1;
        m_gs_photon_len = -1;
        m_gs_photon_idx = -1;

        int counter_poptrack = 0;
        int counter_gentrack = 0;
        m_trackCounter = 0;

        // create G4Track objects and invoke the tracking manager. 
        while (true) {

            // an example: produce a g4track
            G4Track* track = 0;


            G4VTrajectory* previousTrajectory;
            if (( track = g4stackmgr->PopNextTrack(&previousTrajectory) ) != 0 ) {
                // case 1: there is a track in the stackmgr
                ++counter_poptrack;
            } else if ((track = nextTrack()) != 0) {
                // case 2: produce a new track
                //      2.1: scintillation
                //      2.2: cerenkov
                ++counter_gentrack;

            } else if (m_gs_idx<m_gs_len) {
                // even though track is NULL, but we still have gensteps.
                continue;
            } else {
                // stop
                break;
            }

            ++m_trackCounter;

            track->SetTrackID(m_trackCounter);


            // process one track
            g4trkingmgr->ProcessOneTrack(track);

            // need to process secondaries.
            // Refer: G4/source/event/src/G4EventManager.cc

            G4TrackVector * secondaries = g4trkingmgr->GimmeSecondaries();
            // keep them
            if (secondaries && secondaries->size()) {
                for (auto newTrack: *secondaries) {
                    g4stackmgr->PushOneTrack(newTrack);
                }
                secondaries->clear();
            }

            delete track;

            // for debug
            // break;
        }

        std::cout << "Total number of simulated photon: " << m_trackCounter
                  << " including: " << counter_poptrack << " from stack "
                  << " and " << counter_gentrack << " from gentrack." << std::endl;

        // clear the buffer
        clear_gensteps();

        return true;
    }

    int G4OpSimulator::get_nhits() {
        return 0;
    }

    IHit* G4OpSimulator::get_hit(int idx) {
        return nullptr;
    }

    bool G4OpSimulator::clear_hits() {
        return true;
    }

    G4Track* G4OpSimulator::nextTrack() {
        G4Track* track = 0;
        // if (++m_trackCounter > 1000) {
        //     return track;
        // }

        // get the next index
        //
        //   gs: [ | | | ]
        //          ^
        //          `---.
        //              v
        //   photon:  [xxxxxxxxxxxxxxx]
        
        if (++m_gs_photon_idx >= m_gs_photon_len) {
            while (true) {
                bool st = create_photons_from_gs();
                // failed to create more photons
                if (!st) {
                    return 0;
                }
                // if there is photons, break the while loop
                if (m_gs_photon_len) {
                    break;
                }
            }
        }

        if (m_gs_photon_idx < m_scint_photons_from_gs) {
            // scint
            track = create_scint_photon();
        } else if (m_gs_photon_idx < m_gs_photon_len) {
            // ceren
        } else {
            // error
        }

        // associate the track ID
        track->SetParentID(m_currentTrackParentID);

        return track;
    }

    bool G4OpSimulator::create_photons_from_gs() {
        // the initial m_gs_idx is -1.
        if (++m_gs_idx >= m_gs_len) {
            // no genstep any more.
            return false;
        }

        // get the track ID of the current step
        m_currentTrackParentID = m_gs_container.trackid(m_gs_idx);

        // access the gs
        m_scint_photons_from_gs = num_scint_photons_from_gs();
        m_ceren_photons_from_gs = num_ceren_photons_from_gs();

        m_gs_photon_len = m_scint_photons_from_gs + m_ceren_photons_from_gs;

        // reset te photon index
        m_gs_photon_idx = 0;

        std::cout << " the parent track ID: " 
                  << m_currentTrackParentID
                  << " create scint/ceren/total photons: "
                  << m_scint_photons_from_gs << "/"
                  << m_ceren_photons_from_gs << "/"
                  << m_gs_photon_len
                  << std::endl;

        return true;
    }

    int G4OpSimulator::num_scint_photons_from_gs() {
        // current gs idx: m_gs_idx
        int NumTracks = 0;

        // copy & paste from Scintillation.
        const G4Material* aMaterial = G4Material::GetMaterial(m_gs_container.matname(m_gs_idx));

        G4MaterialPropertiesTable* aMaterialPropertiesTable =
            aMaterial->GetMaterialPropertiesTable();

        if (!aMaterialPropertiesTable) {
            return 0;
        }

        G4String FastTimeConstant = "FASTTIMECONSTANT";
        G4String SlowTimeConstant = "SLOWTIMECONSTANT";
        G4String strYieldRatio = "YIELDRATIO";

        slowerTimeConstant = 0.0;
        slowerRatio = 0.0;

        

        int aParticlePDG = m_gs_container.pdgcode(m_gs_idx);
        
        const G4ParticleDefinition* aParticle = 
            G4ParticleTable::GetParticleTable()->FindParticle(aParticlePDG);
        current_step_particle = aParticle;

        if (aParticlePDG == 22 || aParticlePDG == -11 || aParticlePDG == 11) {
            FastTimeConstant = "GammaFASTTIMECONSTANT";
            SlowTimeConstant = "GammaSLOWTIMECONSTANT";
            strYieldRatio = "GammaYIELDRATIO";
            slowerTimeConstant = gammaSlowerTime;
            slowerRatio = gammaSlowerRatio;
        } else if (aParticlePDG == 1000020040) {
            FastTimeConstant = "AlphaFASTTIMECONSTANT";
            SlowTimeConstant = "AlphaSLOWTIMECONSTANT";
            strYieldRatio = "AlphaYIELDRATIO";
            slowerTimeConstant = alphaSlowerTime;
            slowerRatio = alphaSlowerRatio;
        } else {
            FastTimeConstant = "NeutronFASTTIMECONSTANT";
            SlowTimeConstant = "NeutronSLOWTIMECONSTANT";
            strYieldRatio = "NeutronYIELDRATIO";
            slowerTimeConstant = neutronSlowerTime;
            slowerRatio = neutronSlowerRatio;
        }

        const G4MaterialPropertyVector* Fast_Intensity = 
            aMaterialPropertiesTable->GetProperty("FASTCOMPONENT"); 
        const G4MaterialPropertyVector* Slow_Intensity =
            aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");

        if (!Fast_Intensity && !Slow_Intensity ) {
            return 0;
        }

        G4int nscnt = 1;
        if (Fast_Intensity && Slow_Intensity) nscnt = 2;

        G4ThreeVector x0(m_gs_container.pre_x(m_gs_idx),
                         m_gs_container.pre_y(m_gs_idx),
                         m_gs_container.pre_z(m_gs_idx));
        G4ThreeVector p0 = G4ThreeVector(m_gs_container.delta_x(m_gs_idx),
                                         m_gs_container.delta_y(m_gs_idx),
                                         m_gs_container.delta_z(m_gs_idx)).unit();
        G4double      t0 = m_gs_container.pre_t(m_gs_idx);

        // Birks Law
        G4double ScintillationYield = 0;
        {// Yield.  Material must have this or we lack raisins dayetras
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty("SCINTILLATIONYIELD");
            if (!ptable) {
                G4cout << "ConstProperty: failed to get SCINTILLATIONYIELD"
                       << G4endl;
                return 0;
            }
            ScintillationYield = ptable->Value(0);
        }

        G4double ResolutionScale    = 1;
        {// Resolution Scale
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty("RESOLUTIONSCALE");
            if (ptable)
                ResolutionScale = ptable->Value(0);
        }

        G4double QuenchedTotalEnergyDeposit = m_gs_container.evis(m_gs_idx);

        G4double MeanNumberOfPhotons = ScintillationYield * QuenchedTotalEnergyDeposit;
        G4double MeanNumberOfTracks = MeanNumberOfPhotons; 

        if (MeanNumberOfTracks > 10.) {
            G4double sigma = ResolutionScale * sqrt(MeanNumberOfTracks);
            NumTracks = G4int(G4RandGauss::shoot(MeanNumberOfTracks,sigma)+0.5);
        }
        else {
            NumTracks = G4int(G4Poisson(MeanNumberOfTracks));
        }

        // before return the total tracks of scintillation, get the numbers of
        // * fast 
        // * slow
        // * slower
        G4double YieldRatio = 0.0;
        { // Slow Time Constant
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty(strYieldRatio.c_str());
            if(!ptable) ptable = aMaterialPropertiesTable->GetProperty("YIELDRATIO");
            if (ptable) {
                YieldRatio = ptable->Value(0);
            }
        }

        m_scint_photons_from_gs_fast = NumTracks;
        m_scint_photons_from_gs_slow = 0;
        m_scint_photons_from_gs_slower = 0;

        if (nscnt > 1) {
            double ExcitationRatio = 1.0;
            int Num;
            if ( ExcitationRatio == 1.0 ) {
                Num = G4int( 0.5 +  (std::min(YieldRatio,1.0) * NumTracks) );  // round off, not truncation
            }
            else {
                Num = G4int( 0.5 +  (std::min(ExcitationRatio,1.0) * NumTracks));
            }
            
            m_scint_photons_from_gs_fast = Num;
            m_scint_photons_from_gs_slow = NumTracks - Num;
        }

        // if the spectrum is not initialize:
        G4int materialIndex = aMaterial->GetIndex();
        if (!fastScintillationIntegral) {
            G4PhysicsTable* fastIntegral = m_scint_proc->getFastIntegralTable();
            fastScintillationIntegral = dynamic_cast<G4PhysicsOrderedFreeVector*>((*fastIntegral)(materialIndex));
        }

        // time constant
        fastTimeConstant = 0.0;
        if (true) { // Fast Time Constant
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty(FastTimeConstant.c_str());
            if (!ptable) ptable = aMaterialPropertiesTable->GetProperty("FASTTIMECONSTANT");
            if (ptable) {
                fastTimeConstant = ptable->Value(0);
            }
        }

        G4double slowTimeConstant = 0.0;
        if (true) { // Slow Time Constant
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty(SlowTimeConstant.c_str());
            if(!ptable) ptable = aMaterialPropertiesTable->GetProperty("SLOWTIMECONSTANT");
            if (ptable){
                slowTimeConstant = ptable->Value(0);
            }
        }


        return NumTracks;
    }

    int G4OpSimulator::num_ceren_photons_from_gs() {
        return 0;
    }

    G4Track* G4OpSimulator::create_scint_photon() {
        // this method will create a track according to the current state

        G4double cost = 1. - 2.*G4UniformRand();
        G4double sint = sqrt((1.-cost)*(1.+cost));

        G4double phi = twopi*G4UniformRand();
        G4double sinp = sin(phi);
        G4double cosp = cos(phi);

        G4double px = sint*cosp;
        G4double py = sint*sinp;
        G4double pz = cost;


        G4ParticleMomentum photonMomentum(px, py, pz);
        G4double sx = cost*cosp;
        G4double sy = cost*sinp; 
        G4double sz = -sint;

        G4ThreeVector photonPolarization(sx, sy, sz);

        G4ThreeVector perp = photonMomentum.cross(photonPolarization);

        phi = twopi*G4UniformRand();
        sinp = sin(phi);
        cosp = cos(phi);

        photonPolarization = cosp * photonPolarization + sinp * perp;

        photonPolarization = photonPolarization.unit();

        G4DynamicParticle* aScintillationPhoton =
            new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(), 
                                  photonMomentum);
        aScintillationPhoton->SetPolarization
            (photonPolarization.x(),
             photonPolarization.y(),
             photonPolarization.z());

        double sampledEnergy = 4.*eV;

        G4double CIIvalue = G4UniformRand()*
            fastScintillationIntegral->GetMaxValue();
        sampledEnergy=
            fastScintillationIntegral->GetEnergy(CIIvalue);


        aScintillationPhoton->SetKineticEnergy(sampledEnergy);

        G4double atime = 0.0;
        G4ThreeVector apos(0.,0.,0.);

        // calculate the delta time
        double rand = 1.0;
        // if charge != 0
        if (current_step_particle->GetPDGCharge() != 0) {
            rand = G4UniformRand();
        }

        G4double delta = rand * m_gs_container.steplen(m_gs_idx);
        G4double deltaTime = delta /
            ((m_gs_container.pre_velocity(m_gs_idx)+
              m_gs_container.post_velocity(m_gs_idx))/2.);
        // todo: use the fast/slow/slower time const
        G4double ScintillationTime = 0.0;
        // assuming gs_idx starts from 0.
        //  fast: [0, m_scint_photons_from_gs_fast)
        //  slow: [m_scint_photons_from_gs_fast, NumTracks)
        if (m_gs_photon_idx<m_scint_photons_from_gs_fast) {
            ScintillationTime = fastTimeConstant;
        } else if (m_gs_photon_idx<m_scint_photons_from_gs_fast+m_scint_photons_from_gs_slow) {
            ScintillationTime = slowTimeConstant;

            // slower
            if (G4UniformRand()<slowerRatio) {
                ScintillationTime = slowerTimeConstant;
            }
        }

        deltaTime = deltaTime - 
            ScintillationTime * log( G4UniformRand() );

        apos = // x0 + rand * aStep.GetDeltaPosition();
            G4ThreeVector(m_gs_container.pre_x(m_gs_idx),
                          m_gs_container.pre_y(m_gs_idx),
                          m_gs_container.pre_z(m_gs_idx))
            + rand * 
            G4ThreeVector(m_gs_container.delta_x(m_gs_idx),
                          m_gs_container.delta_y(m_gs_idx),
                          m_gs_container.delta_z(m_gs_idx));
        atime = m_gs_container.pre_t(m_gs_idx) + deltaTime;
        G4Track* track = new G4Track(aScintillationPhoton,
                                     atime,
                                     apos);
        return track;
    }

    G4Track* G4OpSimulator::create_ceren_photon() {
        return 0;
    }

} // namespace opsimulator
