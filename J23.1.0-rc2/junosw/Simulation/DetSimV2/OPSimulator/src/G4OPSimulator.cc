#include "G4OPSimulator.h"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4StackManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
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
#include "G4Cerenkov.hh"
#include "G4Cerenkov_modified.hh"

#include "NormalTrackInfo.hh"

#include <algorithm>
#include <iostream>

// #define DEBUG_OP_SIMULATOR

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
	m_ratio_vs_timeconstant = 0;
        // cerenkov
        m_ceren_proc = nullptr;
        m_ceren_table = nullptr;
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
        }

        return true;
    }

    bool G4OpSimulator::init_ceren() {
        if (!m_ceren_proc && !m_ceren_table) {
            G4ProcessManager* ElectronManager =
                G4Electron::Electron()->GetProcessManager();
            if (ElectronManager) {
                G4int N =
                    ElectronManager->GetProcessList()->entries();
                G4ProcessVector* fProcessVector =
                    ElectronManager->GetProcessList();
                for ( G4int i=0; i<N; i++) {
                    G4VProcess* fCurrentProcess = (*fProcessVector)[i];
                    if (fCurrentProcess && fCurrentProcess->GetProcessName() == "Cerenkov") {
                        std::cout << __FILE__ << ":" << __LINE__ << ": "
                                  << "Found Cerenkov process " << fCurrentProcess << std::endl;
                        m_ceren_proc = fCurrentProcess;
                    }
                }
            }

        }

        // Because there are several impl of Cerenkov, we need to check whether the proc
        // is compatible with OPSimulator
        if (m_ceren_proc) {
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                      << "Will dynamic_cast of Cerenkov process " << m_ceren_proc << std::endl;
            if (dynamic_cast<G4Cerenkov_modified*>(m_ceren_proc)) {
                auto _cerenkov_proc = dynamic_cast<G4Cerenkov_modified*>(m_ceren_proc);
                m_ceren_table = _cerenkov_proc->GetPhysicsTable();
                std::cout << "Found Physics Table " << m_ceren_table
                          << "for G4Cerenkov_modified in OPSimulator." << std::endl;
                // todo: switch off the production of cerenkov in Python script.
            } else if (dynamic_cast<G4Cerenkov*>(m_ceren_proc)) {
                std::cerr << "Unsupported Cerenkov impl 'G4Cerenkov' for OPSimulator. " << std::endl;
                return false;

            } else {
                std::cerr << "Unsupported Cerenkov impl for OPSimulator. " << std::endl;
                return false;
            }
        }

        if (m_ceren_table) {

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

        bool st_init = true;

        if (!m_scint_proc) {
            bool st = init_scint();
            if (!st) {
                st_init = false;
            }
        }
        if (!m_ceren_proc) {
            bool st = init_ceren();
            if (!st) {
                st_init = false;
            }
        }

#ifdef DEBUG_OP_SIMULATOR
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "G4OpSimulator::simulate starts. "
                  << "The st init is " << st_init 
                  << std::endl;
#endif

        if (st_init) {
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

#ifdef DEBUG_OP_SIMULATOR
            std::cout << __FILE__ << ":" << __LINE__ << ": "
                      << " m_gs_len: " << m_gs_len
                      << " m_gs_idx: " << m_gs_idx
                      << " m_gs_photon_len: " << m_gs_photon_len
                      << " m_gs_photon_idx: " << m_gs_photon_idx
                      << std::endl;
#endif

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

        }

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

#ifdef DEBUG_OP_SIMULATOR
        std::cout << __FILE__ << ":" << __LINE__ << ": "
                  << "G4OpSimulator::nextTrack() "
                  << std::endl;

        std::cout << "--> m_gs_photon_idx/m_gs_photon_len: "
                  << m_gs_photon_idx << "/"
                  << m_gs_photon_len
                  << std::endl;
#endif        

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

#ifdef DEBUG_OP_SIMULATOR
        std::cout << "--> m_gs_photon_idx/m_scint_photons_from_gs/m_gs_photon_len: "
                  << m_gs_photon_idx << "/"
                  << m_scint_photons_from_gs << "/"
                  << m_gs_photon_len
                  << std::endl;
#endif

	std::string pro_type = "none";

        if (m_gs_photon_idx < m_scint_photons_from_gs) {
            // scint
            track = create_scint_photon();
	    pro_type = "scint";
        } else if (m_gs_photon_idx < m_gs_photon_len) {
            // ceren
            track = create_ceren_photon();
	    pro_type = "ceren";
        } else {
            // error
            std::cerr << "ERROR FOUND: "
                      << " --> m_gs_photon_idx/m_scint_photons_from_gs/m_gs_photon_len: "
                      << m_gs_photon_idx << "/"
                      << m_scint_photons_from_gs << "/"
                      << m_gs_photon_len
                      << std::endl;
        }

	assert(pro_type != "none");

        // associate the track ID
        if (track) {
            track->SetParentID(m_currentTrackParentID);
        }

	// create  Normal track info
	
	NormalTrackInfo* aninfo = new NormalTrackInfo(track);

        G4Track* theTrack = (G4Track*)track;
        theTrack->SetUserInformation(aninfo);

	aninfo->setOriginalOPStartTime(track->GetGlobalTime());
	aninfo->setOriginalOP();
	if (pro_type == "ceren") {
		aninfo -> setFromCerenkov();
	}
	
	     

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

#ifdef DEBUG_OP_SIMULATOR
        std::cout << " the parent track ID: " 
                  << m_currentTrackParentID
                  << " create scint/ceren/total photons: "
                  << m_scint_photons_from_gs << "/"
                  << m_ceren_photons_from_gs << "/"
                  << m_gs_photon_len
                  << std::endl;
#endif

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
	
        

        int aParticlePDG = m_gs_container.pdgcode(m_gs_idx);
        
        const G4ParticleDefinition* aParticle = 
            G4ParticleTable::GetParticleTable()->FindParticle(aParticlePDG);
        current_step_particle = aParticle;
	
	
        if (aParticlePDG == 22 || aParticlePDG == -11 || aParticlePDG == 11) {
	    m_ratio_vs_timeconstant = aMaterialPropertiesTable->GetProperty("GammaCONSTANT");
		
        } else if (aParticlePDG == 1000020040) {
	    m_ratio_vs_timeconstant = aMaterialPropertiesTable->GetProperty("AlphaCONSTANT");
        } else {
	    m_ratio_vs_timeconstant = aMaterialPropertiesTable->GetProperty("NeutronCONSTANT");
        }

        const G4MaterialPropertyVector* Fast_Intensity = 
            aMaterialPropertiesTable->GetProperty("FASTCOMPONENT"); 
        const G4MaterialPropertyVector* Slow_Intensity =
            aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");

        if (!Fast_Intensity && !Slow_Intensity ) {
            return 0;
        }


        G4ThreeVector x0(m_gs_container.pre_x(m_gs_idx),
                         m_gs_container.pre_y(m_gs_idx),
                         m_gs_container.pre_z(m_gs_idx));
        G4ThreeVector p0 = G4ThreeVector(m_gs_container.delta_x(m_gs_idx),
                                         m_gs_container.delta_y(m_gs_idx),
                                         m_gs_container.delta_z(m_gs_idx)).unit();
        G4double      t0 = m_gs_container.pre_t(m_gs_idx);

        // Birks Law
        G4double ScintillationYield = 0;
        if (aMaterialPropertiesTable->ConstPropertyExists("SCINTILLATIONYIELD")) {// Yield.  Material must have this or we lack raisins dayetras
            ScintillationYield = 
                aMaterialPropertiesTable->GetConstProperty("SCINTILLATIONYIELD");
        } else {
            G4cerr << "Failed to get SCINTILLATIONYIELD..." << G4endl;
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
        /*
	*/
        // if the spectrum is not initialize:
        G4int materialIndex = aMaterial->GetIndex();
        if (!fastScintillationIntegral) {
            G4PhysicsTable* fastIntegral = m_scint_proc->getFastIntegralTable();
            fastScintillationIntegral = dynamic_cast<G4PhysicsOrderedFreeVector*>((*fastIntegral)(materialIndex));
        }
	
        return NumTracks;
    }

    int G4OpSimulator::num_ceren_photons_from_gs() {
        int num = 0;

        if (true) {
            num = num_ceren_photons_from_gs_G4Cerenkov_modified();
        }

        return num;
    }

    int G4OpSimulator::num_ceren_photons_from_gs_G4Cerenkov_modified() {

        // assume the current particle is same for both scint and ceren
        const G4ParticleDefinition* aParticle = current_step_particle;
        const G4Material* aMaterial = G4Material::GetMaterial(m_gs_container.matname(m_gs_idx));
        G4MaterialPropertiesTable* aMaterialPropertiesTable =
            aMaterial->GetMaterialPropertiesTable();
        if (!aMaterialPropertiesTable) return 0;
        G4MaterialPropertyVector* Rindex = 
            aMaterialPropertiesTable->GetProperty(kRINDEX);
        m_current_Cerenkov_Rindex = Rindex; // cache the Rindex
        if (!Rindex) return 0;

        // shortcut
        return m_gs_container.num_ceren(m_gs_idx);
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

/*  m_ratio_vs_timeconstant save the scintillation time and the ratio of each component
 *
 *  we use random sampling to determined which component the photon belongs to 
 *  */
	G4int nscnt = m_ratio_vs_timeconstant -> GetVectorLength();
	G4double p = G4UniformRand();
        G4double p_count = 0;
	G4int idx_scnt = 0 ;
        for( G4int j = 0 ; j < nscnt; j++ )
        {
            p_count += (*m_ratio_vs_timeconstant)[j] ;
            if( p < p_count ){
               idx_scnt = j ;
               break;
            }
        } 
	 
	assert(idx_scnt >= 0 );
/*
 * FIXME: we only use fastScintillationIntegral to generate the photon energy as
 *        fastScintillationIntegral and slowScintillationIntegral are same now
 * TODO : Consider fastScintillationIntegral and slowScintillationIntegral
 *
*/
	assert(fastScintillationIntegral);
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
        
        G4double ScintillationTime = m_ratio_vs_timeconstant->Energy(idx_scnt);

        // assuming gs_idx starts from 0.
        //  fast: [0, m_scint_photons_from_gs_fast)
        //  slow: [m_scint_photons_from_gs_fast, NumTracks)

        /*
	*/

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
        G4MaterialPropertyVector* Rindex = m_current_Cerenkov_Rindex;

        G4ThreeVector x0(m_gs_container.pre_x(m_gs_idx),
                         m_gs_container.pre_y(m_gs_idx),
                         m_gs_container.pre_z(m_gs_idx));
        G4ThreeVector p0 = G4ThreeVector(m_gs_container.delta_x(m_gs_idx),
                                         m_gs_container.delta_y(m_gs_idx),
                                         m_gs_container.delta_z(m_gs_idx)).unit();
        G4double t0 = m_gs_container.pre_t(m_gs_idx);

        G4double beta = (m_gs_container.pre_beta(m_gs_idx) + 
                         m_gs_container.post_beta(m_gs_idx))*0.5; // todo

        // For each genstep, the following part is same {
        G4double Pmin = Rindex->GetMinLowEdgeEnergy();
        G4double Pmax = Rindex->GetMaxLowEdgeEnergy();
        G4double dp = Pmax - Pmin;

        G4double nMax = Rindex->GetMaxValue();
        if (Rindex) {
            // nMax = Rindex->GetMaxValue();
            size_t ri_sz = Rindex->GetVectorLength();
   
            for (size_t i = 0; i < ri_sz; ++i) {
                if ((*Rindex)[i]>nMax) nMax = (*Rindex)[i];
            }
        }

        G4double BetaInverse = 1./beta;

        G4double maxCos = BetaInverse / nMax; 
        G4double maxSin2 = (1.0 - maxCos) * (1.0 + maxCos);

        // }


        // Determine photon energy

        G4double rand;
        G4double sampledEnergy, sampledRI; 
        G4double cosTheta, sin2Theta;

        // sample an energy

        while (true) {
            rand = G4UniformRand();  
            sampledEnergy = Pmin + rand * dp; 
            sampledRI = Rindex->Value(sampledEnergy);
            // check if n(E) > 1/beta
            if (sampledRI < BetaInverse) {
                continue;
            }
            cosTheta = BetaInverse / sampledRI;  

            sin2Theta = (1.0 - cosTheta)*(1.0 + cosTheta);
            rand = G4UniformRand();  

            // Loop checking, 07-Aug-2015, Vladimir Ivanchenko
            if (rand*maxSin2 <= sin2Theta) {
                break;
            }
        }

        // Generate random position of photon on cone surface 
        // defined by Theta 

        rand = G4UniformRand();

        G4double phi = twopi*rand;
        G4double sinPhi = std::sin(phi);
        G4double cosPhi = std::cos(phi);

        // calculate x,y, and z components of photon energy
        // (in coord system with primary particle direction 
        //  aligned with the z axis)

        G4double sinTheta = std::sqrt(sin2Theta); 
        G4double px = sinTheta*cosPhi;
        G4double py = sinTheta*sinPhi;
        G4double pz = cosTheta;

        // Create photon momentum direction vector 
        // The momentum direction is still with respect
        // to the coordinate system where the primary
        // particle direction is aligned with the z axis  

        G4ParticleMomentum photonMomentum(px, py, pz);

        // Rotate momentum direction back to global reference
        // system 

        photonMomentum.rotateUz(p0);

        // Determine polarization of new photon 

        G4double sx = cosTheta*cosPhi;
        G4double sy = cosTheta*sinPhi; 
        G4double sz = -sinTheta;

        G4ThreeVector photonPolarization(sx, sy, sz);

        // Rotate back to original coord system 

        photonPolarization.rotateUz(p0);

        // Generate a new photon:

        G4DynamicParticle* aCerenkovPhoton =
            new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(),photonMomentum);

        aCerenkovPhoton->SetPolarization(photonPolarization.x(),
                                         photonPolarization.y(),
                                         photonPolarization.z());

        aCerenkovPhoton->SetKineticEnergy(sampledEnergy);

        // Generate new G4Track object:

        G4double NumberOfPhotons, N;

        G4double MeanNumberOfPhotons1 = m_gs_container.num_ceren_1(m_gs_idx);
        G4double MeanNumberOfPhotons2 = m_gs_container.num_ceren_2(m_gs_idx);

        do {
            rand = G4UniformRand();
            NumberOfPhotons = MeanNumberOfPhotons1 - rand *
                (MeanNumberOfPhotons1-MeanNumberOfPhotons2);
            N = G4UniformRand() *
                std::max(MeanNumberOfPhotons1,MeanNumberOfPhotons2);
            // Loop checking, 07-Aug-2015, Vladimir Ivanchenko
        } while (N > NumberOfPhotons);

        G4double delta = rand * m_gs_container.steplen(m_gs_idx);

        G4double deltaTime = delta / (m_gs_container.pre_velocity(m_gs_idx) +
                                      rand*(m_gs_container.post_velocity(m_gs_idx)-
                                            m_gs_container.pre_velocity(m_gs_idx))*0.5);

        G4double aSecondaryTime = t0 + deltaTime;

        G4ThreeVector aSecondaryPosition = x0 + 
            rand * G4ThreeVector(m_gs_container.delta_x(m_gs_idx),
                                 m_gs_container.delta_y(m_gs_idx),
                                 m_gs_container.delta_z(m_gs_idx));

        G4Track* aSecondaryTrack = 
            new G4Track(aCerenkovPhoton,aSecondaryTime,aSecondaryPosition);

        return aSecondaryTrack;
    }

} // namespace opsimulator
