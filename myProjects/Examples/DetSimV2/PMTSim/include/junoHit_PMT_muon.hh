//------------------------------------------------------------------
//                       junoHit_PMT_muon
//                       
// The data members of a hit on sensitive detector is defined.
// Their values are obtained in junoSD_PMT using the information of
// steps which hit the sensitive detector.
// -----------------------------------------------------------------
//  Author: Liang Zhan,  2006/01/27
//  Modified by: Weili Zhong, 2006/03/01
// -----------------------------------------------------------------
// This is for muon only

#ifndef junoHit_PMT_muon_h
#define junoHit_PMT_muon_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

////////////////////////////////////////////////////////////////////////

class junoHit_PMT_muon : public G4VHit
{
  public:
    junoHit_PMT_muon(G4int z);  
    junoHit_PMT_muon(G4int i,G4double t);
    junoHit_PMT_muon();
    ~junoHit_PMT_muon();
    junoHit_PMT_muon(const junoHit_PMT_muon &right);
    const junoHit_PMT_muon& operator=(const junoHit_PMT_muon &right);
    G4int operator==(const junoHit_PMT_muon &/*right*/) const {return 0;};
    void Init();

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    // for visulization of hits
    void Draw();
    void Print();
    void Dump(const char* prefix);
    

  private:
    G4int pmtID;         // the ID of the PMT the photon hits
    G4double time;       // the time when photon hitting PMT
    G4int iHitCount;     // the Number of p.e. from a Hit
    G4double localtheta; // the photon local theth
    G4double localphi;   // the photon local phi
  public: 
    inline G4double GetTime() const { return time; }
    inline void SetTime(G4double val) { time = val; }

    inline void SetPMTID(G4int z) { pmtID = z; }
    inline G4int GetPMTID() const { return pmtID; }

    inline void SetCount(G4int n) { iHitCount = n; }
    inline G4int GetCount() const { return iHitCount; }

    G4int GetProducerID() const { return -1; }
    void SetProducerID(G4int /*pid*/) {}

    inline G4double GetTheta() const { return localtheta; }
    inline void SetTheta(G4double val) { localtheta = val; }

    inline G4double GetPhi() const { return localphi; }
    inline void SetPhi(G4double val) { localphi = val; }
};

// junoHit_PMT_muon_Collection is a vector of hits
typedef G4THitsCollection<junoHit_PMT_muon> junoHit_PMT_muon_Collection;

extern G4ThreadLocal G4Allocator<junoHit_PMT_muon>* junoHit_PMT_muon_Allocator;

inline void* junoHit_PMT_muon::operator new(size_t)
{
  void *aHit;
  if (!junoHit_PMT_muon_Allocator) {
      junoHit_PMT_muon_Allocator = new G4Allocator<junoHit_PMT_muon>;
  }
  aHit = (void *) junoHit_PMT_muon_Allocator->MallocSingle();
  return aHit;
}

inline void junoHit_PMT_muon::operator delete(void *aHit)
{
  junoHit_PMT_muon_Allocator->FreeSingle((junoHit_PMT_muon*) aHit);
}

#endif
