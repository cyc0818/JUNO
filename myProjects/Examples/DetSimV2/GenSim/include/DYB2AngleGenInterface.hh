#ifndef DYB2AngleGenInterface_hh
#define DYB2AngleGenInterface_hh

#include "globals.hh"

namespace DYB2 {

class IAngleGen {
public:
    virtual G4double next()=0;
    virtual void setSeed(long)=0;
};

}

#endif
