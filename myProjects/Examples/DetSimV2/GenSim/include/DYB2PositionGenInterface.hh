#ifndef DYB2PositionGenInterface_hh
#define DYB2PositionGenInterface_hh

#include "globals.hh"
#include "G4ThreeVector.hh"

namespace DYB2 {

class IVector3dGen {
public:
    virtual G4ThreeVector next()=0;
    virtual void setSeed(long)=0;
    virtual ~IVector3dGen(){}
};

}

#endif
