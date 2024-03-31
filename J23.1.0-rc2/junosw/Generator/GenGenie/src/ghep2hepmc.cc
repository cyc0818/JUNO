#include "ghep2hepmc.h"
#include "CLHEP/Units/SystemOfUnits.h"

// BELOW is from GENIE gNtpConv.cxx
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "libxml/parser.h"
#include "libxml/xmlmemory.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TFolder.h>
#include <TBits.h>
#include <TObjString.h>
#include <TMath.h>
#include "Framework/ParticleData/BaryonResonance.h"
#include "Framework/ParticleData/BaryonResUtils.h"
#include "Framework/Conventions/GBuild.h"
#include "Framework/Conventions/Constants.h"
#include "Framework/Conventions/Units.h"
#include "Framework/Conventions/GVersion.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/GHEP/GHepStatus.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/GHEP/GHepUtils.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Numerical/RandomGen.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGUtils.h"
#include "Framework/ParticleData/PDGLibrary.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/RunOpt.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/SystemUtils.h"
#include "Framework/Utils/T2KEvGenMetaData.h"


bool ghep2hepmc(genie::EventRecord& event, HepMC::GenEvent& hepmc_event) {
    // based on ConvertToGST


    //input particles
    genie::GHepParticle * neutrino = event.Probe();
    genie::GHepParticle * target = event.Particle(1);
    assert(target);
    genie::GHepParticle * fsl = event.FinalStatePrimaryLepton();
    genie::GHepParticle * hitnucl = event.HitNucleon();

    int tgtZ = 0;
    int tgtA = 0;
    if(genie::pdg::IsIon(target->Pdg())) {
        tgtZ = genie::pdg::IonPdgCodeToZ(target->Pdg());
        tgtA = genie::pdg::IonPdgCodeToA(target->Pdg());
    } 
    if(target->Pdg() == genie::kPdgProton   ) { tgtZ = 1; tgtA = 1; }    
    if(target->Pdg() == genie::kPdgNeutron  ) { tgtZ = 0; tgtA = 1; }    


    const genie::Interaction * interaction = event.Summary();
    const genie::InitialState & init_state = interaction->InitState();
    const genie::ProcessInfo &  proc_info  = interaction->ProcInfo();
    const genie::Kinematics &   kine       = interaction->Kine();
    const genie::XclsTag &      xcls       = interaction->ExclTag();
    const genie::Target &       tgt        = init_state.Tgt();

    // Vertex in detector coord system
    TLorentzVector * vtx = event.Vertex();

    // Process id
    bool is_qel    = proc_info.IsQuasiElastic();
    bool is_res    = proc_info.IsResonant();
    bool is_dis    = proc_info.IsDeepInelastic();

#if __GENIE_RELEASE_CODE__ > GRELCODE(3,0,6) 
    bool is_coh    = proc_info.IsCoherentProduction();
#else
    bool is_coh    = proc_info.IsCoherent();
#endif


    bool is_dfr    = proc_info.IsDiffractive();
    bool is_imd    = proc_info.IsInverseMuDecay();
    bool is_imdanh = proc_info.IsIMDAnnihilation();
    bool is_singlek = proc_info.IsSingleKaon();    
    bool is_nuel      = proc_info.IsNuElectronElastic();
    bool is_em        = proc_info.IsEM();
    bool is_weakcc    = proc_info.IsWeakCC();
    bool is_weaknc    = proc_info.IsWeakNC();
    bool is_mec       = proc_info.IsMEC();
    bool is_amnugamma = proc_info.IsAMNuGamma();

    if (!hitnucl && neutrino) {
        assert(is_coh || is_imd || is_imdanh || is_nuel | is_amnugamma);
    }


    // Define own process id, and then add it to  HepMC event
    int procID = 0;
    if (is_weaknc) procID = 100000;
    if (is_weakcc) procID = 200000;

    if (is_qel) procID = procID + 10000;
    if (is_res) procID = procID + 1000;
    if (is_coh || is_dfr) procID = procID + 100;
    if (is_dis) procID = procID + 10;
    if (is_mec) procID = procID + 1;
    hepmc_event.set_signal_process_id(procID);
  
    // Hit quark - set only for DIS events
    int  qrk  = (is_dis) ? tgt.HitQrkPdg() : 0;     
    bool seaq = (is_dis) ? tgt.HitSeaQrk() : false; 

    // Resonance id ($GENIE/src/BaryonResonance/BaryonResonance.h) -
    // set only for resonance neutrinoproduction
    int resid = (is_res) ? genie::EResonance(xcls.Resonance()) : -99;

    // (qel or dis) charm production?
    bool charm = xcls.IsCharmEvent();

    // Get NEUT and NUANCE equivalent reaction codes (if any)
    int brCodeNeut    = genie::utils::ghep::NeutReactionCode(&event);
    int brCodeNuance  = genie::utils::ghep::NuanceReactionCode(&event);

    // Get event weight
    double weight = event.Weight();
    hepmc_event.weights().push_back(weight);

    // Access kinematical params _exactly_ as they were selected internally
    // (at the hit nucleon rest frame; 
    // for bound nucleons: taking into account fermi momentum and off-shell kinematics)
    //
    bool get_selected = true;
    double xs  = kine.x (get_selected);
    double ys  = kine.y (get_selected);
    double ts  = (is_coh || is_dfr) ? kine.t (get_selected) : -1;
    double Q2s = kine.Q2(get_selected);
    double Ws  = kine.W (get_selected);

    // Calculate the same kinematical params but now as an experimentalist would 
    // measure them by neglecting the fermi momentum and off-shellness of bound nucleons
    //
    TLorentzVector pdummy(0,0,0,0);

    const TLorentzVector & k1 = (neutrino) ? *(neutrino->P4()) : pdummy;  // v 4-p (k1)
    const TLorentzVector & k2 = (fsl)      ? *(fsl->P4())      : pdummy;  // l 4-p (k2)
    const TLorentzVector & p1 = (hitnucl)  ? *(hitnucl->P4())  : pdummy;  // N 4-p (p1)      

    double M  = genie::constants::kNucleonMass; 
    TLorentzVector q  = k1-k2;                     // q=k1-k2, 4-p transfer
    double Q2 = -1 * q.M2();                       // momemtum transfer
    
    double v  = (hitnucl) ? q.Energy()       : -1; // v (E transfer to the nucleus)
    double x, y, W2, W;
    if(!is_coh){ 
    
        x  = (hitnucl) ? 0.5*Q2/(M*v)     : -1; // Bjorken x
        y  = (hitnucl) ? v/k1.Energy()    : -1; // Inelasticity, y = q*P1/k1*P1

        W2 = (hitnucl) ? M*M + 2*M*v - Q2 : -1; // Hadronic Invariant mass ^ 2
        W  = (hitnucl) ? TMath::Sqrt(W2)  : -1; 
    } else{

        v = q.Energy();
        x  =  0.5*Q2/(M*v);      // Bjorken x
        y  = v/k1.Energy();    // Inelasticity, y = q*P1/k1*P1

        W2 = M*M + 2*M*v - Q2;  // Hadronic Invariant mass ^ 2
        W  = TMath::Sqrt(W2); 

    }

    double t  = (is_coh || is_dfr) ? kine.t (get_selected) : -1;

    // Get v 4-p at hit nucleon rest-frame
    TLorentzVector k1_rf = k1;         
    if(hitnucl) {
        k1_rf.Boost(-1.*p1.BoostVector());
    }

    bool study_hadsyst = (is_qel || is_res || is_dis || is_coh || is_dfr || is_mec || is_singlek);
    
    // =======================================================================
    // BELOW will loop all the particles
    // =======================================================================
    TObjArrayIter piter(&event);
    genie::GHepParticle * p = 0;
    int ip=-1;


    std::vector<int> final_had_syst;
    while( (p = (genie::GHepParticle *) piter.Next()) && study_hadsyst) {
        ip++;

        // don't count final state lepton as part hadronic system 
        //if(!is_coh && event.Particle(ip)->FirstMother()==0) continue;
        if(event.Particle(ip)->FirstMother()==0) continue;
        if(genie::pdg::IsPseudoParticle(p->Pdg())) continue;
        int pdgc = p->Pdg();
        int ist  = p->Status();
        if(ist==genie::kIStStableFinalState) {
            if (pdgc == genie::kPdgGamma || pdgc == genie::kPdgElectron || pdgc == genie::kPdgPositron)  {
                int igmom = p->FirstMother();
                if(igmom!=-1) {
                    // only count e+'s e-'s or gammas not from decay of pi0
                    if(event.Particle(igmom)->Pdg() != genie::kPdgPi0) { final_had_syst.push_back(ip); }
                }
            } else {
                final_had_syst.push_back(ip);
            }
        }
        // now add pi0's that were decayed as short lived particles
        else if(pdgc == genie::kPdgPi0){
            int ifd = p->FirstDaughter();
            int fd_pdgc = event.Particle(ifd)->Pdg();
            // just require that first daughter is one of gamma, e+ or e-  
            if(fd_pdgc == genie::kPdgGamma || fd_pdgc == genie::kPdgElectron || fd_pdgc == genie::kPdgPositron){
                final_had_syst.push_back(ip);
            }
        }
    }//particle-loop

    if( count(final_had_syst.begin(), final_had_syst.end(), -1) > 0) {
 	return false;
    }

    //
    // Extract info on the primary hadronic system (before any intranuclear rescattering)
    // looking for particles with status_code == kIStHadronInTheNucleus 
    // An exception is the coherent production and scattering off free nucleon targets 
    // (no intranuclear rescattering) in which case primary hadronic system is set to be 
    // 'identical' with the final  state hadronic system
    //

    ip = -1;
    TObjArrayIter piter_prim(&event);

    std::vector<int> prim_had_syst;
    if(study_hadsyst) {
        // if coherent or free nucleon target set primary states equal to final states
      
        if(!genie::pdg::IsIon(target->Pdg()) || (is_coh)) {

            for( std::vector<int>::const_iterator hiter = final_had_syst.begin();
                 hiter != final_had_syst.end(); ++hiter) {

                prim_had_syst.push_back(*hiter);
            }
        } 
      
        else {


            //to find the true particles emitted from the principal vertex,
            // looping over all Ist=14 particles ok for hA, but doesn't
            // work for hN.  We must now look specifically for these particles.
            int ist_store = -10;
            if(is_res){
                while( (p = (genie::GHepParticle *) piter_prim.Next()) ){
                    ip++;      
                    int ist_comp  = p->Status();
                    if(ist_comp==genie::kIStDecayedState) {
                        ist_store = ip;    //store this mother
                        continue;
                    }

                    if(p->FirstMother()==ist_store) {
                        prim_had_syst.push_back(ip);
                    }
                }
            }
            if(is_dis){
                while( (p = (genie::GHepParticle *) piter_prim.Next()) ){
                    ip++;      
                    int ist_comp  = p->Status();
                    if(ist_comp==genie::kIStDISPreFragmHadronicState) {
                        ist_store = ip;    //store this mother
                        continue;
                    }
                    if(p->FirstMother()==ist_store) {
                        prim_had_syst.push_back(ip);
                    }
                }
            }
            if(is_qel){
                while( (p = (genie::GHepParticle *) piter_prim.Next()) ){
                    ip++;      
                    int ist_comp  = p->Status();
                    if(ist_comp==genie::kIStNucleonTarget) {
                        ist_store = ip;    //store this mother
                        continue;
                    }

                    if(p->FirstMother()==ist_store) {
                        prim_had_syst.push_back(ip);
                    }
                }
            }      
            if(is_mec){
                while( (p = (genie::GHepParticle *) piter_prim.Next()) ){
                    ip++;      
                    int ist_comp  = p->Status();
                    if(ist_comp==genie::kIStDecayedState) {
                        ist_store = ip;    //store this mother
                        continue;
                    }

                    if(p->FirstMother()==ist_store) {
                        prim_had_syst.push_back(ip);
                    }
                }
            }
	
	
            // also include gammas from nuclear de-excitations (appearing in the daughter list of the 
            // hit nucleus, earlier than the primary hadronic system extracted above)
            for(int i = target->FirstDaughter(); i <= target->LastDaughter(); i++) {
                if(i<0) continue;
                if(event.Particle(i)->Status()==genie::kIStStableFinalState) { 
                    prim_had_syst.push_back(i); 
                }
            }      

	
        } // else from ( not ion or coherent ) 
      
    }//study_hadsystem?
    
    if( count(prim_had_syst.begin(), prim_had_syst.end(), -1) > 0) {
        return false;
    }

    //
    // Al information has been assembled -- Start filling up the tree branches
    //


    int brFSPrimLept = (fsl) ? fsl->Pdg() : 0;

    double brEl         = k2.Energy();      
    double brPxl        = k2.Px();      
    double brPyl        = k2.Py();      
    double brPzl        = k2.Pz();      
    double brPl         = k2.P();

    // Primary hadronic system (from primary neutrino interaction, before FSI)
    auto brNi = prim_had_syst.size();
    for(int j=0; j<brNi; j++) {
        p = event.Particle(prim_had_syst[j]);

    }


    // create HepMC event
    // get the vertex 
    HepMC::GenVertex* hepmc_vertex = hepmc_event.signal_process_vertex();
    if (not hepmc_vertex) {
        double brVtxX = 0.0;
        double brVtxY = 0.0;
        double brVtxZ = 0.0;
        double brVtxT = 0.0;

        const double m_scale_xyz = 1.0;
        const double m_scale_t = 1.0;

        hepmc_vertex = new HepMC::GenVertex(HepMC::FourVector(brVtxX*m_scale_xyz*CLHEP::meter,
                                                              brVtxY*m_scale_xyz*CLHEP::meter,
                                                              brVtxZ*m_scale_xyz*CLHEP::meter,
                                                              brVtxT*m_scale_t*CLHEP::second));
        hepmc_event.set_signal_process_vertex(hepmc_vertex);

    }

    //primary neutrino and target isotope
    double pPdg = neutrino->Pdg();
    double pPx = neutrino->Px();
    double pPy = neutrino->Py();
    double pPz = neutrino->Pz();
    double pEn = neutrino->Energy();

    double tPdg = target->Pdg();
    double tEn = target->Energy();

    auto hepmc_pnu = new HepMC::GenParticle( 
                                                 HepMC::FourVector(pPx*CLHEP::GeV, 
                                                                   pPy*CLHEP::GeV, 
                                                                   pPz*CLHEP::GeV, 
                                                                   pEn*CLHEP::GeV),
                                                 pPdg,
                                                 0 /* status */
                    ); 
    hepmc_vertex->add_particle_in(hepmc_pnu);

    auto hepmc_target = new HepMC::GenParticle( 
                                                 HepMC::FourVector(0.*CLHEP::GeV, 
                                                                   0.*CLHEP::GeV, 
                                                                   0.*CLHEP::GeV, 
                                                                   tEn*CLHEP::GeV),
                                                 tPdg,
                                                 0 /* status */
                    ); 
    hepmc_vertex->add_particle_in(hepmc_target);

    
    // lepton

    auto hepmc_particle = new HepMC::GenParticle( 
                                                 HepMC::FourVector(brPxl*CLHEP::GeV, 
                                                                   brPyl*CLHEP::GeV, 
                                                                   brPzl*CLHEP::GeV, 
                                                                   brEl*CLHEP::GeV),
                                                 brFSPrimLept,
                                                 1 /* status */
                    ); 
    hepmc_vertex->add_particle_out(hepmc_particle);


    // Final state (visible) hadronic system
    auto brNf = final_had_syst.size();
    for(int j=0; j<brNf; j++) {
        p = event.Particle(final_had_syst[j]);

        int    hpdg = p->Pdg();     
        double hE   = p->Energy();     
        double hKE  = p->KinE();     
        double hpx  = p->Px();     
        double hpy  = p->Py();     
        double hpz  = p->Pz();     
        double hp   = TMath::Sqrt(hpx*hpx + hpy*hpy + hpz*hpz);
        double hm   = p->Mass();     
        double hcth = TMath::Cos( p->P4()->Vect().Angle(k1.Vect()) );

        auto hepmc_particle = new HepMC::GenParticle( 
                                                     HepMC::FourVector(/*brPxf[i]*/ hpx*CLHEP::GeV, 
                                                                       /*brPyf[i]*/ hpy*CLHEP::GeV, 
                                                                       /*brPzf[i]*/ hpz*CLHEP::GeV, 
                                                                       /*brEf [i]*/ hE*CLHEP::GeV),
                                                     /*brPdgf[i]*/ hpdg,
                                                     1 /* status */
                                                      );
        hepmc_vertex->add_particle_out(hepmc_particle);

    }

    return true;
}
