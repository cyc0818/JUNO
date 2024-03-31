#ifndef IToolForSD_PMT_h
#define IToolForSD_PMT_h

#include "G4ThreeVector.hh"
#include "G4VUserTrackInformation.hh"

struct ParamsForSD_PMT {
    // necessary info for a PMT Hit
    G4int ipmt;
    G4int weight;
    G4double time;
    G4double wavelength;
    G4double kineticEnergy;
    G4ThreeVector position;
    G4ThreeVector momentum;
    G4ThreeVector polarization;
    G4int iHitPhotonCount;
    // other user defined MC Truth
    G4int producerID;
    G4VUserTrackInformation* trackInfo;
};

class IToolForSD_PMT {
public:
    virtual ~IToolForSD_PMT() {}
    virtual void SimpleHit(const ParamsForSD_PMT&) = 0;
};

#endif
