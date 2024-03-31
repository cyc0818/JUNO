//--------------------------------------------------------------------------
//                            junoSD_PMT
//
// PMTs are difined as sensitive detector. They collect hits on them.
// The data members of hits are set up here using the information of G4Step.
// -------------------------------------------------------------------------
// Author: Liang Zhan, 2006/01/27
// Modified by: Weili Zhong, 2006/03/01
// -------------------------------------------------------------------------

#include "junoSD_PMT.hh"
#include "junoHit_PMT.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include <cassert>
#include "NormalTrackInfo.hh"
using namespace CLHEP;

////////////////////////////////////////////////////////////////////////////////

junoSD_PMT::junoSD_PMT(const std::string& name)
:G4VSensitiveDetector(name),hitCollection(0)
{
  G4String HCname;
  collectionName.insert(HCname="hitCollection");

  m_debug = false;
  m_merge_flag = false;
  m_time_window = 1000*ns;

}

junoSD_PMT::~junoSD_PMT()
{;}

void junoSD_PMT::Initialize(G4HCofThisEvent *HCE)
{
  if (m_debug) {
    G4cout << "junoSD_PMT::Initialize" << G4endl;
  }
  hitCollection = new junoHit_PMT_Collection(SensitiveDetectorName,collectionName[0]);

  // changed from a static variable to a normal variable by Jillings, August 2, 2006
  int HCID = -1;
  if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection);
  HCE->AddHitsCollection( HCID, hitCollection ); 

  m_pmtid2idincol.clear();
  assert( m_pmtid2idincol.size() == 0 );
}

G4bool junoSD_PMT::ProcessHits(G4Step *,G4TouchableHistory*)
{
  // NOTE: ProcessHits should never be called anymore, because
  // hits are decided by junoPMTOpticalModel, which calls junoPMTSD::SimpleHit()
  // in order to avoid having to create a bogus G4Step object.
  ////////////////////////////////////////////////////////////////

  return false;  
}

// Here is the real "hit" routine, used by GLG4PMTOpticalModel and by ProcessHits
// It is more efficient in some ways.
void junoSD_PMT::SimpleHit( const ParamsForSD_PMT& params )
{
  if (m_merge_flag) {
      bool ok = doMerge(params);
      if (ok) {
          return;
      }
  }
  // create new juno hit photon, the way of recording photo hits on PMTs
  junoHit_PMT* hit_photon = new junoHit_PMT();
  hit_photon->SetPMTID((int)params.ipmt);
  hit_photon->SetWeight((int)params.weight);
  hit_photon->SetTime((double) params.time );
  hit_photon->SetWavelength((double)params.wavelength);
  hit_photon->SetKineticEnergy((double) params.kineticEnergy );
  hit_photon->SetPosition(params.position);
  hit_photon->SetMomentum(params.momentum); 
  hit_photon->SetPolarization(params.polarization); 
  hit_photon->SetCount( params.iHitPhotonCount );
  // = User can handle the track info here =
  G4VUserTrackInformation* trkinfo = params.trackInfo;

    double t_start = 0.0; // the generated time of primary optical photon
    G4ThreeVector p_start;
    G4bool is_from_cerenkov = false;
    G4bool is_reemission = false;
    int producerID = params.producerID;
    bool is_original_op = false;
    if (trkinfo) {
        // try to load

        NormalTrackInfo* normaltrk = dynamic_cast<NormalTrackInfo*>(trkinfo);
        if (normaltrk) {
            producerID = normaltrk->GetOriginalTrackID();
            is_from_cerenkov = normaltrk->isFromCerenkov();
            is_reemission = normaltrk->isReemission();

            t_start = normaltrk->getOriginalOPStartTime();

            is_original_op = normaltrk->isOriginalOP();
        }
    }

  hit_photon->SetProducerID(producerID);
  hit_photon->SetFromCerenkov(is_from_cerenkov);
  hit_photon->SetReemission(is_reemission);
  hit_photon->SetOriginalOP(is_original_op);
  hit_photon->SetOriginalOPStartT(t_start);

  G4int idx = hitCollection->insert(hit_photon);
  if (m_merge_flag) {
      m_pmtid2idincol.insert( std::pair<int, int>( params.ipmt, idx-1 ) );
  }
}
void junoSD_PMT::EndOfEvent(G4HCofThisEvent*){
  if (m_debug) {
    G4cout << "junoSD_PMT::EndOfEvent" << G4endl;
  }
  if (m_merge_flag) {
    if (m_debug) {
      G4cout << "hitCollection.size: " << hitCollection->entries() << G4endl;
      G4cout << "m_pmtid2idincol.size: " << m_pmtid2idincol.size() << G4endl;
    }
    assert(hitCollection->entries() == (int)m_pmtid2idincol.size());
  }

}

void junoSD_PMT::clear(){}

void junoSD_PMT::DrawAll(){} 

void junoSD_PMT::PrintAll(){} 

bool junoSD_PMT::doMerge( const ParamsForSD_PMT& params) {
  G4double time_window = m_time_window;
  // magic here for muon simulation
  m_pmt_iter = m_pmtid2idincol.equal_range(params.ipmt);
  for(PMTID2COLIDS::iterator it = m_pmt_iter.first;
        it != m_pmt_iter.second; ++it) {
    assert( params.ipmt == it->first );
    int index_in_current_col = it->second;
    // get the hit
    junoHit_PMT* hits = dynamic_cast<junoHit_PMT*>(hitCollection->GetHit(index_in_current_col));
    if (hits == NULL) {
        continue;
    }
    // compare the time 
    // check whether they are in the same bin
    if ( static_cast<int>( params.time/time_window)
       ==static_cast<int>( hits->GetTime()/time_window) ) {
      // TODO: update the data
      if ( params.time < hits->GetTime() ) {
          if (m_debug) {
              G4cout << "PMTID: " << params.ipmt
                     << " Time Window: " << time_window 
                     << " current hit time: " << params.time
                     << " previous hit time: " << hits->GetTime()
                     << G4endl;
          }
          // update the hit because the new is more earlier.
          hits->SetTime((double) params.time );
          hits->SetWavelength((double)params.wavelength);
          hits->SetKineticEnergy((double) params.kineticEnergy );
          hits->SetPosition(params.position);
          hits->SetMomentum(params.momentum); 
          hits->SetPolarization(params.polarization); 
          hits->SetProducerID(params.producerID);
          // FIXME Merge two hits from different process
      }
      G4int new_count = params.iHitPhotonCount + hits->GetCount();
      hits->SetCount( new_count );
      return true;
    }
  }

  return false;
}


