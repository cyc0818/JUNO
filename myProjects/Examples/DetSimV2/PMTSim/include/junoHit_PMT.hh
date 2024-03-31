//------------------------------------------------------------------
//                       junoHit_PMT
//                       
// The data members of a hit on sensitive detector is defined.
// Their values are obtained in junoSD_PMT using the information of
// steps which hit the sensitive detector.
// -----------------------------------------------------------------
//  Author: Liang Zhan,  2006/01/27
//  Modified by: Weili Zhong, 2006/03/01
// -----------------------------------------------------------------

#ifndef junoHit_PMT_h
#define junoHit_PMT_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

////////////////////////////////////////////////////////////////////////

class junoHit_PMT : public G4VHit
{
  public:
    junoHit_PMT(G4int z);  
    junoHit_PMT(G4int i,G4double t);
    junoHit_PMT();
    ~junoHit_PMT();
    junoHit_PMT(const junoHit_PMT &right);
    const junoHit_PMT& operator=(const junoHit_PMT &right);
    G4int operator==(const junoHit_PMT &/*right*/) const {return 0;};
    void Init();

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    // for visulization of hits
    void Draw();
    void Print();
    void Dump(const char* prefix);
    

  private:
    G4double edep;
    G4double energy;     // the energy of the photon hitting PMT
    G4double localtheta; // the photon local theth
    G4double localphi;   // the photon local phi
    G4ThreeVector pos;   // the photon hitting position
    G4ThreeVector mom;   // the photon momentum when hitting PMT
    G4ThreeVector pol;   // the photon polarization when hitting PMT

    G4ThreeVector global_pos;   // (global) the photon hitting position
    G4ThreeVector global_mom;   // (global) the photon momentum when hitting PMT
    G4ThreeVector global_pol;   // (global) the photon polarization when hitting PMT

    G4int pmtID;         // the ID of the PMT the photon hits
    G4double time;       // the time when photon hitting PMT
    G4int iHitCount;     // the Number of p.e. from a Hit
    G4int weight;        // if the number of photons is scaled to speed up
                         // processing, this is the weight of the photon. 
                         // (ie for a muon, might track fewer photons and
                         // compensate with increased weight.)
    G4double wavelength; // wavelength of photon hitting PMT

    G4int producerID;    // track ID of parent of optical photon.  See
                         // junoTrajectory.
    G4bool isFromCerenkov;
    G4bool isReemission;

    G4bool isOriginalOP;
    G4double OriginalOPStartT;

    G4ThreeVector boundary_pos;

  public:
    inline void SetEdep(G4double de)
    { edep = de; };
    inline G4double GetEdep()
    { return edep; };
      
  public: 
    inline G4double GetTime() const { return time; }
    inline void SetTime(G4double val) { time = val; }

    inline G4double GetWavelength() const { return wavelength; }
    inline void SetWavelength(G4double val) { wavelength = val; }

    inline G4int GetWeight() const { return weight; }
    inline void SetWeight(G4int val) { weight = val; }
      
    inline G4double GetKineticEnergy() const { return energy; }
    inline void SetKineticEnergy(G4double val) { energy = val; }

    inline G4double GetTheta() const { return localtheta; }
    inline void SetTheta(G4double val) { localtheta = val; }

    inline G4double GetPhi() const { return localphi; }
    inline void SetPhi(G4double val) { localphi = val; }
      
    inline void SetPosition(G4ThreeVector xyz) { pos = xyz; }
    inline G4ThreeVector GetPosition() const { return pos; }
      
    inline void SetMomentum(G4ThreeVector mxyz) { mom = mxyz; }
    inline G4ThreeVector GetMomentum() const { return mom; }
    
    inline void SetPolarization(G4ThreeVector pxyz) { pol=pxyz; }
    inline G4ThreeVector GetPolarization() const {return pol;}   

    inline void SetGlobalPosition(G4ThreeVector xyz) { global_pos = xyz; }
    inline G4ThreeVector GetGlobalPosition() const { return global_pos; }
      
    inline void SetGlobalMomentum(G4ThreeVector mxyz) { global_mom = mxyz; }
    inline G4ThreeVector GetGlobalMomentum() const { return global_mom; }
    
    inline void SetGlobalPolarization(G4ThreeVector pxyz) { global_pol=pxyz; }
    inline G4ThreeVector GetGlobalPolarization() const {return global_pol;}   

    
    inline void SetPMTID(G4int z) { pmtID = z; }
    inline G4int GetPMTID() const { return pmtID; }

    inline void SetCount(G4int n) { iHitCount = n; }
    inline G4int GetCount() const { return iHitCount; }

    G4int GetProducerID() const { return producerID; }
    void SetProducerID(G4int pid) { producerID = pid; }

    G4bool IsFromCerenkov() const { return isFromCerenkov;}
    void SetFromCerenkov(G4bool flag) { isFromCerenkov = flag; }

    G4bool IsReemission() const { return isReemission;}
    void SetReemission(G4bool flag) { isReemission = flag; }

    G4bool IsOriginalOP() const { return isOriginalOP; }
    void SetOriginalOP(G4bool flag) { isOriginalOP = flag; }

    G4double GetOriginalOPStartT() { return OriginalOPStartT; }
    void SetOriginalOPStartT(double t) { OriginalOPStartT = t; }

    inline void SetBoundaryPosition(G4ThreeVector xyz) { boundary_pos = xyz; }
    inline G4ThreeVector GetBoundaryPosition() const { return boundary_pos; }
};

// junoHit_PMT_Collection is a vector of hits
typedef G4THitsCollection<junoHit_PMT> junoHit_PMT_Collection;

extern G4ThreadLocal G4Allocator<junoHit_PMT>* junoHit_PMT_Allocator;

inline void* junoHit_PMT::operator new(size_t)
{
  void *aHit;
  if (!junoHit_PMT_Allocator) {
      junoHit_PMT_Allocator = new G4Allocator<junoHit_PMT>;
  }
  aHit = (void *) junoHit_PMT_Allocator->MallocSingle();
  return aHit;
}

inline void junoHit_PMT::operator delete(void *aHit)
{
  junoHit_PMT_Allocator->FreeSingle((junoHit_PMT*) aHit);
}

#endif
