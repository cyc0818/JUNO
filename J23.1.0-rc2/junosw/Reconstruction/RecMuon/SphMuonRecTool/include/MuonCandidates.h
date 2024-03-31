/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      26.09.2019
# Filename:	MuonCandidates.h
# Description: 
=============================================================================*/
#ifndef MuonCandidates_h
#define MuonCandidates_h

// #ifdef _OPENMP
// #include <omp.h>
// #endif

#include <fstream>
#include <map>

#include "SphTrack.h"

class TF2;

class MuonCandidates 
{
 public:
  
  MuonCandidates(double radius, int pmtType, bool muonCalibration); 
  ~MuonCandidates(); 

  int  DefineNumberOfMuons(double par[], int n_tracks);
  void DefineMuonCandidates(TF2 *func, std::vector<std::vector<track_t>> &muonCandidate, TH2F *hEventLegendre);
  void EnergyDepositionMuonTrue(std::vector <trackSim_t> muonSim);
  void SetWPsignal(PmtTable &pmtTable);
  void SetEventNumber(int eventNumber) {fEventNumber = eventNumber;}
  void SetProductionMode(bool productionMode) {fProductionMode = productionMode;}
  
private:

  int     fPMTtype;
  int     fEventNumber;
  double  pi;
  double  fRadius;
  double  fMaxSlope;
  double  fDegToRad;
  double  fRadToDeg;
  bool    fProductionMode;
  TH2F   *fHistoCD;

  std::ofstream fOutputFileEnergy;
  PmtTable fWPsignal;
  PmtTable fCDsignal;
  
  int  EnergyDepositionPoints(std::vector<track_t> &muonTrack);
  void GetIOpointsCD(std::vector<TVector3> &CDioPoints, TF2 *func);
  void GetIOpointsWP(std::vector<TVector3> &WPioPoints);
  void GetConfirmedMuonTracks(std::vector<track_t> &muonTrack, std::vector<TVector3> &CDioPoints, std::vector<TVector3> &WPioPoints, std::vector<TVector3> &unusedWPioPoints);
  void SearchTrackCombinations(std::vector<track_t> &muonTrack, std::vector<TVector3> &CDioPoints, std::vector<std::vector<track_t>> &muonCandidates);
  void DefineParallelMuons(std::vector<TVector3>::iterator &point1,
			   std::vector<TVector3>::iterator &point2,
			   std::vector<TVector3> &points,
			   std::vector<track_t> &muon_candidate);  

  class IncreasingTracksNumber
  {
  public:
    bool operator()(const std::vector<track_t> &a,
		    const std::vector<track_t> &b) const
    {
      return (a.size() < b.size());
    }
  };
}; 

#endif // MuonCandidates_h
