#ifndef SphTrack_h
#define SphTrack_h

#include "RecTools/PmtProp.h"

#include <TVector3.h>
#include <TPolyMarker.h>
#include <TH1F.h>
#include <TH2F.h>

#include <vector>

typedef std::pair<TVector3, TVector3> track_t;

// event description

typedef struct {
  TVector3     initMuonMomentum;
  TVector3     endMuonMomentum;
  TVector3     initMuonPosition;
  TVector3     endMuonPosition;
  TVector3     intPoint; // interaction point
  double       intEnergy; // interaction energy
  TPolyMarker *markerMuonPosition;
} trackSim_t;

typedef struct {
  int     eventNumber;
  double  recDistCenter;  // reconstructed distance from muon
                          // track to detector center
  double  recDistMuon;    // distance between simulated and reconstructed muon
  double  chi2;           // chi2
  double  scaleEn;        // energy scale for found muon
  double  sumPE[2];       // registered energy in PMTs
  double  intEnergyTrue;  // interaction energy, true energy
  double  intEnergyRec;   // interaction energy, rec energy
  double  energyLossTrue; // energy loss by muon, true energy
  double  energyLossRec;  // energy loss by muon, rec energy
} trackRec_t;

typedef struct {
  TH2F *hEventRaw;
  TH2F *hEventLegendre;
  TH2F *hEventRecMuon;
  TH2F *hEventRecShower;
  std::vector <trackSim_t> muonSim;
  std::vector <std::vector<track_t>> muonCandidates;
} EventSimRec_t;

#endif // SphTrack_h
