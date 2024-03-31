//------------------------------------------------------------------------------
//                                      junoSD_TT
//
// The Top-tracker is defined as a sensitive detector. They collect the hits.
//------------------------------------------------------------------------------
//Author: Karaparambil Rajan Rebin, J. P. A. M. de Andre (IPHC)
//Date: 12/12/2022
//------------------------------------------------------------------------------

#ifndef junoSD_TT_h
#define junoSD_TT_h 1

#include "G4VSensitiveDetector.hh"
#include "junoHit_TT.hh"

//------------------------------------------------------------------------------

class G4Step;
class G4HCofThisEvent;

class junoSD_TT : public G4VSensitiveDetector
{
public:
  junoSD_TT(const std::string& name);
  ~junoSD_TT();

  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep, G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);

  void clear();
  void DrawAll();
  void PrintAll();
  
  void InitParams();
  bool SaveHits();
private:
  junoHit_TT_Collection* hitCollectionTT;
  junoHit_TT_Collection* userhitCollectionTT;

  int m_NDeposits;
  std::vector<int>   m_dep_trackID;
  std::vector<int>   m_dep_pdg;
  std::vector<float> m_dep_E;
  std::vector<float> m_dep_E0;
  std::vector<int>   m_dep_nbar;
  std::vector<int>   m_dep_npanel;
  std::vector<int>   m_dep_nplane;
  std::vector<int>   m_dep_nwall;
  std::vector<float> m_dep_x;
  std::vector<float> m_dep_y;
  std::vector<float> m_dep_z;
  std::vector<float> m_dep_t;
  std::vector<float> m_dep_dL;
  std::vector<float> m_dep_dR;
  std::vector<float> m_dep_peL;
  std::vector<float> m_dep_peR;
  std::vector<float> m_dep_tL;
  std::vector<float> m_dep_tR;

  std::vector<int> m_isMuonDeposits;

  float coeff[4][16];
};

#endif
