//---------------------------------------------------------------------------
//                                  junoHit_TT
//
// The data members of a hit on the TT sensitive detector is defined.
//---------------------------------------------------------------------------
// Author: Karaparambil Rajan Rebin, J. P. A. M. de Andre (IPHC)
// Date: 12/12/2022
//---------------------------------------------------------------------------

#ifndef junoHit_TT_h
#define junoHit_TT_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

//---------------------------------------------------------------------------

class junoHit_TT : public G4VHit
{
public:
  junoHit_TT(G4int z);
  junoHit_TT(G4int i, G4double t);
  junoHit_TT();
  ~junoHit_TT();
  junoHit_TT(const junoHit_TT &right);
  const junoHit_TT& operator=(const junoHit_TT &right);
  G4int operator==(const junoHit_TT &) const {return 0;};
  void Init();

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // for visulization of hits
  void Draw();
  void Print();
  void Dump(const char* prefix);


private:
  G4double edep;     // the energy deposited after OPERA parametrization
  G4double time;     // the time of the hit (time at scintillator + time to reach MA-PMT parametrized by OPERA detector response)
  G4int channelID;   // the channel ID of the detected hit 
  G4int nDeposits;   // the total number of energy deposits per event

  //Variables for user file
  G4int trackID;     // the track ID
  G4int pdgID;       // the particle ID
  G4float depE;      // the deposited energy in scintillator
  G4float vertexKE;  // the kinetic energy at vertex
  G4int nbar;        // the scintillator strip number of the deposit
  G4int npanel;      // the panel number of the deposit
  G4int nplane;      // the plane number of the deposit
  G4int nwall;       // the wall number of the deposit
  G4float depX;      // the "x" position of the deposit
  G4float depY;      // the "y" position of the deposit
  G4float depZ;      // the "z" position of the deposit
  G4float depT;      // the time of the deposit
  G4float depDL;     // distance to the left PMT from the deposit position
  G4float depDR;     // distance to the right PMT from the deposit position
  G4float depPEL;    // PE in the left PMT
  G4float depPER;    // PE in the right PMT
  G4float depTL;     // Time at the left PMT
  G4float depTR;     // Time at the right PMT
  G4float isMuonDeposit;

public:
  inline G4double GetEdep() const { return edep; }
  inline void SetEdep(G4double val) { edep = val; }

  inline G4double GetTime() const { return time; }
  inline void SetTime(G4double val) { time = val; }

  inline G4int GetChannelID() const { return channelID; }
  inline void SetChannelID(G4int val) { channelID = val; }

  inline G4int GetNDeposits() const { return nDeposits; }
  inline void SetNDeposits(G4int val) { nDeposits = val; }

  //Functions for user file
  inline G4int GetTrackID() const { return trackID; }
  inline void SetTrackID(G4int val) { trackID = val; }

  inline G4int GetPdgID() const { return pdgID; }
  inline void SetPdgID(G4int val) { pdgID = val; }

  inline G4float GetDepE() const { return depE; }
  inline void SetDepE(G4float val) { depE = val; }

  inline G4float GetVertexKE() const { return vertexKE; }
  inline void SetVertexKE(G4float val) { vertexKE = val; }

  inline G4float GetNbar() const { return nbar; }
  inline void SetNbar(G4float val) { nbar = val; }

  inline G4float GetNpanel() const { return npanel; }
  inline void SetNpanel(G4float val) { npanel = val; }

  inline G4float GetNplane() const { return nplane; }
  inline void SetNplane(G4float val) { nplane = val; }

  inline G4float GetNwall() const { return nwall; }
  inline void SetNwall(G4float val) { nwall = val; }

  inline G4float GetDepX() const { return depX; }
  inline void SetDepX(G4float val) { depX = val; }

  inline G4float GetDepY() const { return depY; }
  inline void SetDepY(G4float val) { depY = val; }

  inline G4float GetDepZ() const { return depZ; }
  inline void SetDepZ(G4float val) { depZ = val; }

  inline G4float GetDepT() const { return depT; }
  inline void SetDepT(G4float val) { depT = val; }

  inline G4float GetDepDL() const { return depDL; }
  inline void SetDepDL(G4float val) { depDL = val; }

  inline G4float GetDepDR() const { return depDR; }
  inline void SetDepDR(G4float val) { depDR = val; }

  inline G4float GetDepPEL() const { return depPEL; }
  inline void SetDepPEL(G4float val) { depPEL = val; }

  inline G4float GetDepPER() const { return depPER; }
  inline void SetDepPER(G4float val) { depPER = val; }

  inline G4float GetDepTL() const { return depTL; }
  inline void SetDepTL(G4float val) { depTL = val; }

  inline G4float GetDepTR() const { return depTR; }
  inline void SetDepTR(G4float val) { depTR = val; }

  inline G4float GetIsMuonDeposit() const { return isMuonDeposit; }
  inline void SetIsMuonDeposit(G4float val) { isMuonDeposit = val; }

};

//junoHit_TT_Collection is a vector of hits
typedef G4THitsCollection<junoHit_TT> junoHit_TT_Collection;

extern G4ThreadLocal G4Allocator<junoHit_TT>* junoHit_TT_Allocator;

inline void* junoHit_TT::operator new(size_t)
{
  void * aHit;
  if(!junoHit_TT_Allocator) {
    junoHit_TT_Allocator = new G4Allocator<junoHit_TT>;
  }
  aHit = (void *) junoHit_TT_Allocator->MallocSingle();
  return aHit;
}

inline void junoHit_TT::operator delete(void *aHit)
{
  junoHit_TT_Allocator->FreeSingle((junoHit_TT*) aHit);
}

#endif
