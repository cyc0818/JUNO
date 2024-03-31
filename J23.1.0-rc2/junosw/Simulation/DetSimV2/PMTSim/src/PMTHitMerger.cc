#include "PMTHitMerger.hh"
#include "G4SDManager.hh"
using namespace CLHEP;
PMTHitMerger::PMTHitMerger() {
  hitCollection = 0;
  hitCollection_muon = 0;
  m_merge_flag = false;
  m_time_window = 1.0; // 1ns

  // initialize map capacity for all PMTs in central detector

  m_PMThit.clear();
  m_PMThit_muon.clear();
  for (int i = 0; i < 17746; ++i) {
    m_PMThit[i].clear();
    m_PMThit_muon[i].clear();
  }
}


bool
PMTHitMerger::doMerge(int pmtid, double hittime) {

  if (not m_merge_flag) return false;

  if (hitCollection != 0) {

    std::map<int, std::vector<junoHit_PMT*> >::iterator pmt = m_PMThit.find(pmtid);

    if (pmt != m_PMThit.end()) {

      int time1 = static_cast<int>(hittime/m_time_window);

      std::vector<junoHit_PMT*>::iterator it = pmt->second.begin();

      for ( ; it != pmt->second.end(); ++it) {

 	// compare the time 

 	if (time1 == static_cast<int>((*it)->GetTime()/m_time_window)) {
	      
 	  if (hittime < (*it)->GetTime()) (*it)->SetTime(hittime);

 	  // === update the count
 	  (*it)->SetCount(1 + (*it)->GetCount());
 	  return true;
 	}
      }
    }
  } else if (hitCollection_muon != 0) {

    std::map<int, std::vector<junoHit_PMT_muon*> >::iterator pmt = m_PMThit_muon.find(pmtid);

    if (pmt != m_PMThit_muon.end()) {

      int time1 = static_cast<int>(hittime/m_time_window);

      std::vector<junoHit_PMT_muon*>::iterator it = pmt->second.begin();

      for ( ; it != pmt->second.end(); ++it) {

 	// compare the time 

 	if (time1 == static_cast<int>((*it)->GetTime()/m_time_window)) {
	      
 	  if (hittime < (*it)->GetTime()) (*it)->SetTime(hittime);

 	  // === update the count
 	  (*it)->SetCount(1 + (*it)->GetCount());
 	  return true;
 	}
      }
    }
  }
  return false;
}


bool
PMTHitMerger::saveHit(junoHit_PMT* hit_photon) {
  if (not hasNormalHitType()) {
    std::cerr << "WARN: PMTHitMerger don't use normal hit type, however a normal hit is inserted."
	      << std::endl;
    return false;
  }

  if (m_merge_flag) {
    int pmtid = hit_photon->GetPMTID();
    std::map<int, std::vector<junoHit_PMT*> >::iterator pmt = m_PMThit.find(pmtid);
    if (pmt == m_PMThit.end()) {
      m_PMThit[pmtid].clear();
      m_PMThit[pmtid].push_back(hit_photon);
    }
    else pmt->second.push_back(hit_photon);
  }
  hitCollection->insert(hit_photon);

  return true;
}


bool
PMTHitMerger::saveHit(junoHit_PMT_muon* hit_photon) {
  if (not hasMuonHitType()) {
    std::cerr << "WARN: PMTHitMerger don't use muon hit type, however a muon hit is inserted."
	      << std::endl;
    return false;
  }

  if (m_merge_flag) {
    int pmtid = hit_photon->GetPMTID();
    std::map<int, std::vector<junoHit_PMT_muon*> >::iterator pmt = m_PMThit_muon.find(pmtid);
    if (pmt == m_PMThit_muon.end()) {
      m_PMThit_muon[pmtid].clear();
      m_PMThit_muon[pmtid].push_back(hit_photon);
    }
    else pmt->second.push_back(hit_photon);
  }
  hitCollection_muon->insert(hit_photon);

  return true;
}


bool
PMTHitMerger::init(junoHit_PMT_Collection* hitcol) {

  assert(m_time_window > 0);

  // during event begin, retrieve the hit collection
  hitCollection = hitcol;

  // clear all vectors but saving pmts number

  std::map<int, std::vector<junoHit_PMT*> >::iterator pmt = m_PMThit.begin();
  for ( ; pmt != m_PMThit.end(); ++pmt) pmt->second.clear();

  return true;
}


bool
PMTHitMerger::init(junoHit_PMT_muon_Collection* hitcol) {

  assert(m_time_window > 0);

  // during event begin, retrieve the hit collection
  hitCollection_muon = hitcol;

  // clear all vectors but saving pmts number

  std::map<int, std::vector<junoHit_PMT_muon*> >::iterator pmt = m_PMThit_muon.begin();
  for ( ; pmt != m_PMThit_muon.end(); ++pmt) pmt->second.clear();

  return true;
}
