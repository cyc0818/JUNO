//--------------------------------------------------------------------------
//                            junoSD_PMT_v2
//
// PMTs are difined as sensitive detector. They collect hits on them.
// The data members of hits are set up here using the information of G4Step.
// -------------------------------------------------------------------------
// Author: Liang Zhan, 2006/01/27
// Modified by: Weili Zhong, 2006/03/01
// -------------------------------------------------------------------------

#include "junoSD_PMT_v2.hh"
#include "junoHit_PMT.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include <cassert>
#include "NormalTrackInfo.hh"
#include "G4OpticalPhoton.hh"
#include "Randomize.hh"
#include "G4DataInterpolation.hh"
#include "G4VProcess.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

#ifdef WITH_G4OPTICKS
#include <iomanip>
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Opticks.hh"

#include "PMTEfficiency.hh"
#include "PMTEfficiencyTable.hh"

#define WITH_G4OPTICKS_EFFICIENCY_CHECK 1

#endif


using namespace CLHEP;

////////////////////////////////////////////////////////////////////////////////

junoSD_PMT_v2::junoSD_PMT_v2(const std::string& name, int opticksMode)
:G4VSensitiveDetector(name),hitCollection(0), hitCollection_muon(0), hitCollection_opticks(0), m_opticksMode(opticksMode)
{
    G4String HCname;
    collectionName.insert(HCname="hitCollection");
    collectionName.insert(HCname="hitCollectionMuon");
#ifdef WITH_G4OPTICKS
    if(m_opticksMode > 0){
        collectionName.insert(HCname="hitCollectionOpticks");
    }
    m_PMTEfficiency = new PMTEfficiency ; 
    m_PMTEfficiencyTable = new PMTEfficiencyTable(m_PMTEfficiency) ; 
#endif

    m_debug = true;
    m_time_window = 1; // 1ns

    m_pmthitmerger = 0;
    m_pmthitmerger_opticks = 0;
    m_hit_type = 1; // 1 for normal, 2 for muon

    m_qescale = 1.0/0.8 ;  // scale back for 0.8 QE-scale in LsExpDetectorConstruction
    m_angle_response = 1.0;


    m_ce_flat_value = 0.9;

    MCP20inch_m_ce_flat_value = 0.85;
    MCP8inch_m_ce_flat_value = 0.85;
    Ham20inch_m_ce_flat_value = 0.95;
    Ham8inch_m_ce_flat_value = 0.7;
    HZC9inch_m_ce_flat_value = 0.67;

    MCP20inch_m_EAR_value = 1.;
    MCP8inch_m_EAR_value = 1.;
    Ham20inch_m_EAR_value = 0.93;
    Ham8inch_m_EAR_value = 0.88;
    HZC9inch_m_EAR_value = 0.92;

    m_disable = false;
    m_enable_optical_model = false;

    // 20inchfunc (1D)
    // FIXME: following are not used in current code
    m_ce_func_str = "0.9*[0]/(1+[1]*exp(-[2]*x))";
    //OLD
    //m_ce_func_params.push_back(1.006); // p0
    //m_ce_func_params.push_back(0.9023);// p1
    //m_ce_func_params.push_back(0.1273);// p2

    //NEW from the PMT test benches at Zhongshan
    m_ce_func_params.push_back(0.9194); // p0
    m_ce_func_params.push_back(0.504);  // p1
    m_ce_func_params.push_back(0.08076);// p2

    //These fit the Hamamatsu CE better..
    //m_ce_func_params.push_back(1.02557);// p0
    //m_ce_func_params.push_back(6.77639);// p1
    //m_ce_func_params.push_back(0.16419);// p2

    m_ce_func = 0;
    m_merge_count = 0 ; 
}

junoSD_PMT_v2::~junoSD_PMT_v2()
{;}

void junoSD_PMT_v2::Initialize(G4HCofThisEvent *HCE)
{
    m_merge_count = 0 ; 
    if (m_debug) {
        G4cout << "junoSD_PMT_v2::Initialize" << G4endl;
    }
    hitCollection = new junoHit_PMT_Collection(SensitiveDetectorName,collectionName[0]);
    hitCollection_muon = new junoHit_PMT_muon_Collection(SensitiveDetectorName,collectionName[1]);

    // changed from a static variable to a normal variable by Jillings, August 2, 2006
    int HCID = -1;
    if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection);
    HCE->AddHitsCollection( HCID, hitCollection ); 

    HCID = -1;
    if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection_muon);
    HCE->AddHitsCollection( HCID, hitCollection_muon ); 

    m_pmtid2idincol.clear();
    assert( m_pmtid2idincol.size() == 0 );

    if (m_pmthitmerger) {
        if (m_hit_type == 1) {
            m_pmthitmerger->init(hitCollection);
        } else if (m_hit_type == 2) {
            // TODO
            m_pmthitmerger->init(hitCollection_muon);
        } else {
            G4cout << "unknown hit type [" << m_hit_type << "]" << G4endl;
        }
    }
    // make sure the PMT merger exists.
    assert(m_pmthitmerger);
//    assert(m_PMTParamsvc);



#ifdef WITH_G4OPTICKS
    if(m_opticksMode > 0) // for now, a separate collection for GPU hits 
    {
        hitCollection_opticks = new junoHit_PMT_Collection(SensitiveDetectorName,collectionName[2]);
        HCID = -1;
        if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection_opticks);
        HCE->AddHitsCollection( HCID, hitCollection_opticks );
        assert(m_pmthitmerger_opticks); 
        if (m_hit_type == 1) {
            m_pmthitmerger_opticks->init(hitCollection_opticks);
        } else {
            G4cout << "FATAL : unknown hit type [" << m_hit_type << "]" << G4endl;
            assert(0); 
        }
    }
#endif

}

G4bool junoSD_PMT_v2::ProcessHits(G4Step * step,G4TouchableHistory*)
{
    if (m_disable) {
        return false;
    }
    // TODO: now it only support the single PE.
    // = only accept the optical photon
    G4Track* track = step->GetTrack();
    if (track->GetDefinition() != G4OpticalPhoton::Definition()) {
        return false;
    }
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    double edep = step->GetTotalEnergyDeposit();
    // = only when the photon is detected by the surface, the edep is non-zero.
    // = the QE is already applied in the OpBoundaryProcess::DoAbsorption
    if (edep<=0.0) {
        return false;
    }

    // LT
    // = Due to update of Geant4, now OpAbsorption will also cause non-zero edep.
    // = Hence we need to check the OP boundary.
    G4bool isOnBoundary = (postStepPoint->GetStepStatus() == fGeomBoundary);
    if (not isOnBoundary) {
        return false;
    }

    static G4ThreadLocal G4OpBoundaryProcess* boundary_proc=NULL;
    if (!boundary_proc) {
        G4ProcessManager* OpManager =
            G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
        if (OpManager) {
            G4int MAXofPostStepLoops =
                OpManager->GetPostStepProcessVector()->entries();
            G4ProcessVector* fPostStepDoItVector =
                OpManager->GetPostStepProcessVector(typeDoIt);
            for ( G4int i=0; i<MAXofPostStepLoops; i++) {
                G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
                G4OpBoundaryProcess* op =  dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
                if (op) {
                    boundary_proc = op;
                    break;
                }
            }
        }
     
    }

    if (!boundary_proc) {
        G4cout << "Can't locate OpBoundaryProcess." << G4endl;
        return false;
    }

    if(m_enable_optical_model == false) {
       G4OpBoundaryProcessStatus theStatus = Undefined;
       theStatus = boundary_proc->GetStatus();

       if (theStatus != Detection) {
           return false;
       }
    }

    // if (0) {
    //     std::cout << "DEBUG physics processes: " << std::endl;
    //     G4bool isOnBoundary = (postStepPoint->GetStepStatus() == fGeomBoundary);
    //     std::cout << "isOnBoundary: " << isOnBoundary << std::endl;

    //     // we should try to find the op status
    //     const G4VProcess* proc = postStepPoint->GetProcessDefinedStep();
    //     if (proc) {
    //         std::cout << (proc->GetProcessName()) << std::endl;
    //     }

    //     G4OpBoundaryProcessStatus theStatus = Undefined;
    //     G4ProcessManager* OpManager =
    //         G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    //     if (OpManager) {
    //         G4int MAXofPostStepLoops =
    //             OpManager->GetPostStepProcessVector()->entries();
    //         G4ProcessVector* fPostStepDoItVector =
    //             OpManager->GetPostStepProcessVector(typeDoIt);
    //         for ( G4int i=0; i<MAXofPostStepLoops; i++) {
    //             G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
    //             G4OpBoundaryProcess* fOpProcess =  dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
    //             if (fOpProcess) { theStatus = fOpProcess->GetStatus();  break;}
    //         }
    //     }
        
    //     if (theStatus == Undefined) {
    //         std::cout << "status: undefined." << std::endl;
    //     } else if (theStatus == Detection) {
    //         std::cout << "status: detect." << std::endl;
    //     } else {
    //         std::cout << "status: " << theStatus << std::endl;
    //     }

    //     std::cout << "END DEBUG physics processes: " << std::endl;

    // }

    // TODO: get CE and angle response from data.
    // = decide the CE (collection efficiency)
    // = the CE can be different at different position
    // == volume name
    std::string volname = track->GetVolume()->GetName(); // physical volume
    // == position
    const G4AffineTransform& trans = track->GetTouchable()->GetHistory()->GetTopTransform();
    const G4ThreeVector& global_pos = postStepPoint->GetPosition();
    G4ThreeVector local_pos = trans.TransformPoint(global_pos);

    // LT
    if (0) {
    std::cout << "DEBUG volname: " 
              << volname
              << " pre/post: "
              << preStepPoint->GetPhysicalVolume()->GetName() << "/"
              << postStepPoint->GetPhysicalVolume()->GetName()
              << " t: "
              << preStepPoint->GetGlobalTime() << "/"
              << postStepPoint->GetGlobalTime()
              << " pos: (" 
              << local_pos.x() << ", "
              << local_pos.y() << ", "
              << local_pos.z() << ") "
              << "r: " << sqrt(local_pos.x()*local_pos.x() + local_pos.y()*local_pos.y())
              << std::endl;
    }
    double qe = 1;
    bool pmt_type = true;
    bool qe_type  = true;
    double qescale;
    if(m_enable_optical_model){
        qescale = 1.0/0.4;   // scale back for 0.3 QE-scale in junoPMTOpticalModel
        m_qescale = qescale;
    }
    // == get the copy number -> pmt id
    int pmtID = get_pmtid(track);
    //if(pmtID<18000){
    //  qe =  m_PMTParamsvc->getPMTQE(pmtID);
    //  pmt_type =  m_PMTParamsvc->isHamamatsu(pmtID);
    //  qe_type  =  m_PMTParamsvc->isHighQE(pmtID);
    //}else if(pmtID<300000){
    //  qe = 0.3;
    //}else if(pmtID>=300000){
    //  qe = m_PMTParamsvc->getPMTQE(pmtID);
    //}

    // calculate QE from PDE from svc:
    double eff_pde = 1.;
    double qe_calc = 1.;
    if(pmtID<18000){
        eff_pde = m_PMTSimParsvc->get_pde(pmtID);
        if( m_PMTSimParsvc->isHamamatsu(pmtID)) {
            qe_calc = eff_pde / eff_ce_Hamamatsu;
        } else if ( m_PMTSimParsvc->isHiQENNVT(pmtID) ) {
            qe_calc = eff_pde / eff_ce_NNVT_HiQE;
        } else if ( m_PMTSimParsvc->isNormalNNVT(pmtID) ) {
            qe_calc = eff_pde / eff_ce_NNVT_Normal;
        }
    }
    else if(pmtID<300000) {
        qe_calc = 0.3;
    } else if(pmtID>=300000) {
        qe_calc = m_PMTParamsvc->getPMTQE(pmtID);
    } 

    double ce = get_ce(volname, local_pos, pmt_type, qe_type);  

    double f_angle_response = 1.0;
    // = final DE = QE * CE, but QE is already applied, so only CE is important.
    // = DE: Detection Efficiency
    double de = qe_calc*ce*f_angle_response*m_qescale;
    //double de = qe*ce*f_angle_response*m_qescale;
    //double de = qe*ce*f_angle_response;
    //std::cout << "test de: " << qe*ce << std::endl;
    if (de>1.0){
        std::cout<<"junoSD_PMT_v2:: de is larger than 1.0"<<std::endl;
    }

#ifdef WITH_G4OPTICKS_EFFICIENCY_CHECK
    {
        double epsilon = 1e-10 ; 
        double qe2 = getQuantumEfficiency(pmtID); 
        bool qe_match = std::abs(qe-qe2) < epsilon ; 
        assert( qe_match );  

        int pmtcat = m_PMTParamsvc->getPMTCategory(pmtID) ; 

        if(m_ce_mode == "20inch") 
        {
            double theta = local_pos.theta() ; 
            double ce2 = getCollectionEfficiency(theta,pmtID);  
            bool ce_match = std::abs(ce - ce2) < epsilon ; 
            if(!ce_match)
            {
                G4cout 
                   << __FILE__ << " +" << __LINE__ 
                   << " ce_match FAIL "
                   << " ce " << ce 
                   << " ce2 " << ce2 
                   << " ce - ce2 " << ce - ce2
                   << " epsilon " << epsilon 
                   << " theta " << theta
                   << " pmtID " << pmtID
                   << " pmtcat " << pmtcat
                   << " pmtcatname " << PMTParamSvc::PMTCategoryName(pmtcat)
                   << " volname " << volname
                   << G4endl 
                   ;
            }
            //assert( ce_match ); 

            double de2 = getDetectionEfficiency(theta, pmtID); 
            bool de_match = std::abs(de - de2) < epsilon ; 

            if(!de_match)
            {
                G4cout 
                   << __FILE__ << " +" << __LINE__ 
                   << " de_match FAIL "
                   << " de " << de 
                   << " de2 " << de2 
                   << " de - de2 " << de - de2
                   << " epsilon " << epsilon 
                   << " theta " << theta
                   << " pmtID " << pmtID
                   << " pmtcat " << pmtcat
                   << " pmtcatname " << PMTParamSvc::PMTCategoryName(pmtcat)
                   << " volname " << volname
                   << G4endl 
                   ;
            }
            //assert( de_match ); 
        }
    }
#endif

    if (G4UniformRand() > de) {
        return false;
    }
    // ========================================================================
    // create the transient PMT Hit Object
    // ========================================================================
    // == get the copy number -> pmt id
    int pmtid = get_pmtid(track);
    // == hit time
    double hittime = postStepPoint->GetGlobalTime();
    // == momentum and polarization
    G4ThreeVector local_pol = trans.TransformAxis(track->GetPolarization());
    local_pol = local_pol.unit();
    G4ThreeVector local_dir = trans.TransformAxis(track->GetMomentum());
    local_dir = local_dir.unit();
    // == wavelength 
    double wavelength = twopi*hbarc / edep;
    // == additional information from User Track information
    int producerID = -1;
    bool is_from_cerenkov = false;
    bool is_reemission = false;
    bool is_original_op = false;
    double t_start = 0;
    G4ThreeVector boundary_pos;
    G4VUserTrackInformation* trkinfo = track->GetUserInformation();
    if (trkinfo) {
        NormalTrackInfo* normaltrk = dynamic_cast<NormalTrackInfo*>(trkinfo);
        if (normaltrk) {
            producerID = normaltrk->GetOriginalTrackID();
            is_from_cerenkov = normaltrk->isFromCerenkov();
            is_reemission = normaltrk->isReemission();

            t_start = normaltrk->getOriginalOPStartTime();

            is_original_op = normaltrk->isOriginalOP();
            boundary_pos = normaltrk->getBoundaryPos();
        }
    }
    // ========================================================================
    // Save the Hit into hit collection
    // * without merge
    // * with merge
    //   + This is for the muon simulation or some events with a lot of hits
    //   + some values can't merge.
    //   + the flags such as producerID, is from cerenkov, is from scintillation,
    //     is reemission, is original op will be not right
    // ========================================================================
    // = check the merge flag first
    if (m_pmthitmerger and m_pmthitmerger->getMergeFlag()) {
        // == if merged, just return true. That means just update the hit
        // NOTE: only the time and count will be update here, the others 
        //       will not filled.
        bool ok = m_pmthitmerger->doMerge(pmtid, hittime);
        if (ok) {
            m_merge_count += 1 ; 
            return true;
        }
    }
    // = save the hit in the collection
    if (m_hit_type == 1) {
        junoHit_PMT* hit_photon = new junoHit_PMT();
        hit_photon->SetPMTID(pmtid);
        hit_photon->SetWeight(1.0);
        hit_photon->SetTime(hittime);
        hit_photon->SetWavelength(wavelength);
        hit_photon->SetKineticEnergy(edep);
        hit_photon->SetPosition(local_pos);
        hit_photon->SetTheta(local_pos.theta());
        hit_photon->SetPhi(local_pos.phi());
        hit_photon->SetMomentum(local_dir); 
        hit_photon->SetPolarization(local_pol); 

        hit_photon->SetGlobalPosition(global_pos);
        hit_photon->SetGlobalMomentum(track->GetMomentum()); 
        hit_photon->SetGlobalPolarization(track->GetPolarization()); 

        hit_photon->SetCount(1); // FIXME

        hit_photon->SetProducerID(producerID);
        hit_photon->SetFromCerenkov(is_from_cerenkov);
        hit_photon->SetReemission(is_reemission);
        hit_photon->SetOriginalOP(is_original_op);
        hit_photon->SetOriginalOPStartT(t_start);
        hit_photon->SetBoundaryPosition(boundary_pos);
        // == insert
        if (m_pmthitmerger) {
            m_pmthitmerger->saveHit(hit_photon);
        }
    } else if (m_hit_type == 2) {
        // save the muon only
        junoHit_PMT_muon* hit_photon = new junoHit_PMT_muon();
        hit_photon->SetPMTID(pmtid);
        hit_photon->SetTime(hittime);
        hit_photon->SetCount(1); // FIXME
        hit_photon->SetTheta(local_pos.theta());
        hit_photon->SetPhi(local_pos.phi());
        if (m_pmthitmerger) {
            m_pmthitmerger->saveHit(hit_photon);
        }
    } else {

    }
    return true;  
}




void junoSD_PMT_v2::EndOfEvent(G4HCofThisEvent* HCE){
    if (m_debug) {
        G4cout << "junoSD_PMT_v2::EndOfEvent" 
               << " opticksMode " << m_opticksMode 
               << G4endl;
    }

    if(m_opticksMode > 0)
    {
        EndOfEvent_Opticks(HCE);    
    }
}


void junoSD_PMT_v2::EndOfEvent_Opticks(G4HCofThisEvent* /*HCE*/)
{
    assert(m_opticksMode > 0 ); 

#ifdef WITH_G4OPTICKS
    const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent() ; 
    G4int eventID = event->GetEventID() ; 

    G4Opticks* g4ok = G4Opticks::Get() ; 

    G4cout 
       << "[[ junoSD_PMT_v2::EndOfEvent_Opticks "
       << " eventID " << eventID
       << " m_opticksMode " << m_opticksMode
       << " numGensteps " << g4ok->getNumGensteps()
       << " numPhotons " << g4ok->getNumPhotons()
       << G4endl
       ;

    int nhit = g4ok->propagateOpticalPhotons(eventID); 

    G4cout 
       << " junoSD_PMT_v2::EndOfEvent_Opticks.propagateOpticalPhotons "
       << " eventID " << eventID
       << " nhit " << nhit 
       << G4endl
       ;


    G4ThreeVector position ; 
    G4double time ; 
    G4ThreeVector direction ; 
    G4double weight ; 
    G4ThreeVector polarization ; 
    G4double wavelength ; 
    G4int flags_x ; 
    G4int flags_y ; 
    G4int flags_z ; 
    G4int flags_w ; 
    G4bool is_cerenkov ; 
    G4bool is_reemission ; 

    int merged_count(0); 
    for(int i=0 ; i < nhit ; i++)
    {
        g4ok->getHit(i,
                     &position, 
                     &time, 
                     &direction,
                     &weight,
                     &polarization,
                     &wavelength, 
                     &flags_x,
                     &flags_y,
                     &flags_z,
                     &flags_w,
                     &is_cerenkov,
                     &is_reemission
                    ); 

        int pmtid = flags_y ; 
        G4double hittime = time ;  

        bool merged = false ; 
        if (m_pmthitmerger_opticks and m_pmthitmerger_opticks->getMergeFlag()) {
            merged = m_pmthitmerger_opticks->doMerge(pmtid, hittime);
        }

        if(i < 100)
        G4cout << " getHit "
               << " i " << std::setw(6) << i 
               << " pos.x " << std::setw(10) << std::fixed << std::setprecision(2) << position.x()
               << " pos.y " << std::setw(10) << std::fixed << std::setprecision(2) << position.y()
               << " pos.z " << std::setw(10) << std::fixed << std::setprecision(2) << position.z()
               << " pmtid "   << std::setw(7) << pmtid
               << " merged " << ( merged ? "Y" : "N" )
               << " flags_x " << std::setw(7) << flags_x
               << " flags_y " << std::setw(7) << flags_y
               << " flags_z " << std::setw(7) << flags_z
               << " flags_w " << std::setw(7) << flags_w
               << G4endl
               ;

        if(merged)
        {
            merged_count += 1 ; 
        }
        else 
        {
            junoHit_PMT* hit_photon = new junoHit_PMT();
            hit_photon->SetPMTID(pmtid);
            hit_photon->SetWeight(weight);
            hit_photon->SetTime(hittime);
            hit_photon->SetWavelength(wavelength);
            //hit_photon->SetKineticEnergy(edep);

            // TODO: transform gymnastics to get the local_pos
            //       so can replicate the de random selection

            //hit_photon->SetPosition(local_pos);
            //hit_photon->SetTheta(local_pos.theta());
            //hit_photon->SetPhi(local_pos.phi());
            hit_photon->SetMomentum(direction); 
            hit_photon->SetPolarization(polarization); 

            hit_photon->SetGlobalPosition(position);
            hit_photon->SetGlobalMomentum(direction); 
            hit_photon->SetGlobalPolarization(polarization); 

            hit_photon->SetCount(1); 

            //hit_photon->SetProducerID(producerID);
            hit_photon->SetFromCerenkov(is_cerenkov);
            hit_photon->SetReemission(is_reemission);
            //hit_photon->SetOriginalOP(is_original_op);
            //hit_photon->SetOriginalOPStartT(t_start);
            //hit_photon->SetBoundaryPosition(boundary_pos);
            // == insert
            if (m_pmthitmerger_opticks) {
                m_pmthitmerger_opticks->saveHit(hit_photon);
            }
        }
    }      // nhit loop
    g4ok->reset(); 

    G4cout 
       << "]] junoSD_PMT_v2::EndOfEvent_Opticks "
       << " nhit " << nhit
       << " merged_count(opticks) " << merged_count
       << " merge_count(standard) " << m_merge_count 
       << " m_opticksMode " << m_opticksMode
       << " hitCollection " << hitCollection->entries()  
       << " hitCollection_muon " << hitCollection_muon->entries()  
       << " hitCollection_opticks " << hitCollection_opticks->entries()  
       << G4endl 
       ;
#else
    G4cout 
       << "junoSD_PMT_v2::EndOfEvent_Opticks "
       << " m_opticksMode " << m_opticksMode
       << " FATAL : opticksMode >  0 BUT WITH_G4OPTICKS not defined " 
       << G4endl 
       ;
    assert(0); 
#endif
}










void junoSD_PMT_v2::clear(){}

void junoSD_PMT_v2::DrawAll(){} 

void junoSD_PMT_v2::PrintAll(){} 

void junoSD_PMT_v2::SimpleHit(const ParamsForSD_PMT&){}

int junoSD_PMT_v2::get_pmtid(G4Track* track) {
    int ipmt= -1;
    // find which pmt we are in
    // The following doesn't work anymore (due to new geometry optimization?)
    //  ipmt=fastTrack.GetEnvelopePhysicalVolume()->GetMother()->GetCopyNo();
    // so we do this:
    {
        const G4VTouchable* touch= track->GetTouchable();
        int nd= touch->GetHistoryDepth();
        int id=0;
        for (id=0; id<nd; id++) {
            if (touch->GetVolume(id)==track->GetVolume()) {
                int idid=1;
                G4VPhysicalVolume* tmp_pv=NULL;
                for (idid=1; idid < (nd-id); ++idid) {
                    tmp_pv = touch->GetVolume(id+idid);

                    G4LogicalVolume* mother_vol = tmp_pv->GetLogicalVolume();
                    G4LogicalVolume* daughter_vol = touch->GetVolume(id+idid-1)->
                        GetLogicalVolume();
                    int no_daugh = mother_vol -> GetNoDaughters();
                    if (no_daugh > 1) {
                        int count = 0;
                        for (int i=0; (count<2) &&(i < no_daugh); ++i) {
                            if (daughter_vol->GetName()
                                    ==mother_vol->GetDaughter(i)->GetLogicalVolume()->GetName()) {
                                ++count;
                            }
                        }
                        if (count > 1) {
                            break;
                        }
                    }
                    // continue to find
                }
                ipmt= touch->GetReplicaNumber(id+idid-1);
                break;
            }
        }
        if (ipmt < 0) {
            G4Exception("junoPMTOpticalModel: could not find envelope -- where am I !?!", // issue
                    "", //Error Code
                    FatalException, // severity
                    "");
        }
    }

    return ipmt;
}

// ============================================================================
// = Collection Efficiency Related
// ============================================================================
// == change the Collection Efficiency Mode
void junoSD_PMT_v2::setCEMode(const std::string& mode) {
    m_ce_mode = mode;
}

// == get the Collection Efficiency 
double junoSD_PMT_v2::get_ce(const std::string& volname, const G4ThreeVector& localpos, bool pmt_type, bool qe_type) {
    // volname:
    // * PMT_20inch_body_phys
    // * PMT_3inch_body_phys
    if (m_ce_mode == "None") {
        return 1.0;
    } else if (m_ce_mode == "20inch") {
        // only 20inch PMT will be affected
         //G4cout << volname << G4endl;
        if (volname == "PMT_20inch_body_phys") {
            // calculate the angle theta
            double theta = localpos.theta();
            // do interpolate
            static double s_theta_NNVT[] = {
                0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg,
                77.5*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_NNVT[] =    {
                0.9,    0.9,   0.845,     0.801,    0.775,    0.802,
                0.802,   0.771,    0.66,
            };
            static double s_theta_hamamatsu[] = {
                0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
                79.*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_hamamatsu[] =    {
                0.873,    0.873,   0.888,     0.896,    0.881,    0.9,
                0.881,     0.627,    0.262,
            };
            static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT, 9, 0., 0.);
            if(pmt_type){
            static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);
            }

             return s_di.CubicSplineInterpolation(theta);
        }
/*
        if (volname == "PMT_20inch_body_phys") {
            // calculate the angle theta
            double theta = localpos.theta();
            // do interpolate
            static double s_theta_NNVT[] = {
                0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg,
                77.5*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_NNVT[] =    {
                1.0,    1.0,    0.9453,     0.9105,     0.8931,     0.9255, 
                0.9274,     0.8841,     0.734,  
            };
            static double s_ce_NNVT_highQE[] = {
               1.0,     1.0,    0.9772,     0.9723,     0.9699,     0.9697, 
               0.9452,  0.9103,     0.734,   
            };

            static double s_theta_hamamatsu[] = {
                0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
                79.*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_hamamatsu[] =    {
                0.911,    0.911,    0.9222,     0.9294,     0.9235,     0.93,
                0.9095, 0.6261, 0.2733, 
            };
            static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT, 9, 0., 0.);
            if(pmt_type){
            static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);
            }
            if(!pmt_type && qe_type) {
            static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT_highQE, 9, 0., 0.);
            }
             return s_di.CubicSplineInterpolation(theta);
        }
*/
        else if (volname == "HamamatsuR12860_PMT_20inch_body_phys") {
            double theta = localpos.theta();

            static double s_theta_hamamatsu[] = {
                0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
                79.*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_hamamatsu[] =    {
                0.911,    0.911,    0.9222,     0.9294,     0.9235,     0.93,
                0.9095, 0.6261, 0.2733, 
            };
            static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);  
            return s_di.CubicSplineInterpolation(theta);
        }


        else if (volname == "NNVTMCPPMT_PMT_20inch_body_phys") {
            // calculate the angle theta
            double theta = localpos.theta();
            // do interpolate
            static double s_theta_NNVT[] = {
                0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg,
                77.5*deg, 85.*deg, 90.*deg,
            };
            static double s_ce_NNVT[] =    {
                1.0,    1.0,    0.9453,     0.9105,     0.8931,     0.9255, 
                0.9274,     0.8841,     0.734,  
            };
            static double s_ce_NNVT_highQE[] = {
               1.0,     1.0,    0.9772,     0.9723,     0.9699,     0.9697, 
               0.9452,  0.9103,     0.734,   
            };

            if(!pmt_type && !qe_type){
                static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT, 9, 0., 0.);

                return s_di.CubicSplineInterpolation(theta);
            }
            else if(!pmt_type && qe_type) {
                static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT_highQE, 9, 0., 0.);
                return s_di.CubicSplineInterpolation(theta);
            }
        }

    } else if (m_ce_mode == "20inchflat"){
        // This is a flat mode which means no matter where the photon
        // hits, use the same CE.
        if (volname == "PMT_20inch_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            // -- 2015.10.10 Tao Lin <lintao@ihep.ac.cn>
            static double mean_val = m_ce_flat_value;
            return mean_val;
        }
    }else if (m_ce_mode == "flat"){
        // This is a flat mode which means no matter where the photon
        // hits, use the same CE.
        // G4cout << "PMT volume name : "<<volname << G4endl;
        if (volname == "R12860TorusPMTManager_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            static double Ham20inch_R12860_mean_val = Ham20inch_m_ce_flat_value*Ham20inch_m_EAR_value;
            return Ham20inch_R12860_mean_val;
        }
        else if (volname == "MCP20inchPMTManager_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            static double MCP20inch_mean_val = MCP20inch_m_ce_flat_value*MCP20inch_m_EAR_value;
            return MCP20inch_mean_val;
        }
        else if (volname == "Ham8inchPMTManager_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            static double Ham8inch_mean_val = Ham8inch_m_ce_flat_value*Ham8inch_m_EAR_value;
            return Ham8inch_mean_val;
        }
        else if (volname == "MCP8inchPMTManager_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            static double MCP8inch_mean_val = MCP8inch_m_ce_flat_value*MCP8inch_m_EAR_value;
            return MCP8inch_mean_val;
        }
        else if (volname == "HZC9inchPMTManager_body_phys") {
            // FIXME It's a fixed number here, we can make it a variable
            // if it is needed to be modified.
            static double HZC9inch_mean_val = HZC9inch_m_ce_flat_value*HZC9inch_m_EAR_value;
            return HZC9inch_mean_val;
        }
    }else if (m_ce_mode == "20inchfunc") {
        // In this mode, user needs to input:
        // 1. a function, which can be interpret by ROOT TF1.
        // 2. a list of parameters
        if (!m_ce_func) {
            G4cout << "WARNING: The CE Function is not defined." << G4endl;
            assert(m_ce_func);
        }
        // calculate the angle theta
        double theta = localpos.theta(); // unit: radians
        if (theta>CLHEP::halfpi) { theta = CLHEP::halfpi; }
        // convert angle
        // NOTE: the angle needs to be converted
        // 1. pi/2-theta
        // 2. radian -> degree
        theta = (CLHEP::halfpi-theta)/degree;
        return m_ce_func->Eval(theta);
    } else {
        G4cout << "WARNING: unknown CE mode " << m_ce_mode << G4endl;
    }

    return 1.0;
}

void
junoSD_PMT_v2::setCEFunc(const std::string& func, const std::vector<double>& param)
{
    // detele origial function
    if (m_ce_func) {
        delete m_ce_func;
        m_ce_func = 0;
    }

    // Info:
    std::cout << "Following is the CE Function detail:" << std::endl;
    std::cout << "CE Function: " << func << std::endl;
    // angle from 0 to 90 deg.
    // angle is from equator
    m_ce_func = new TF1("ce", func.c_str(), 0, 90);
    std::cout << "CE Params: ";
    for (size_t i = 0; i < param.size(); ++i) {
        std::cout << param[i] << " "; 
        m_ce_func->SetParameter(i, param[i]);
    }
    std::cout << std::endl;
}





#ifdef WITH_G4OPTICKS
double junoSD_PMT_v2::getCollectionEfficiency(double theta, int pmtID) const 
{
    int pmtcat = m_PMTParamsvc->getPMTCategory(pmtID); 
    return m_PMTEfficiency->getEfficiency(theta, pmtcat) ; 
}
double junoSD_PMT_v2::getQuantumEfficiency(int pmtID) const 
{
    double qe = 1. ;

    // Would be better for efficiency transitions to align with pmt category boundaries
    // so could then have a switch on the category. The reason it is better is that 
    // the categorization then happens in one place only (rather than being spead all over the code base)
    // making it easier to change.  Having selection smeared around means changing it will 
    // likely cause bugs. 

    if(pmtID<18000){
        qe =  m_PMTParamsvc->getPMTQE(pmtID);
    } 
    else if(pmtID<kOFFSET_CD_SPMT){
        qe = 0.3;   // WP PMTs all the same qe ?
    } 
    else if(pmtID>=kOFFSET_CD_SPMT){
        qe = m_PMTParamsvc->getPMTQE(pmtID);
    }   
    return qe ; 
}

double junoSD_PMT_v2::getEfficiencyScale() const 
{
    return m_angle_response*m_qescale ; 
}

double junoSD_PMT_v2::getDetectionEfficiency(double theta, int pmtID) const 
{
    double qe = getQuantumEfficiency(pmtID); 
    double ce = getCollectionEfficiency(theta, pmtID); 
    double es = getEfficiencyScale(); 
    double de = qe*ce*es; 
    return de ; 
}
PMTEfficiencyTable* junoSD_PMT_v2::getPMTEfficiencyTable() const 
{
    return m_PMTEfficiencyTable ; 
}

#endif


