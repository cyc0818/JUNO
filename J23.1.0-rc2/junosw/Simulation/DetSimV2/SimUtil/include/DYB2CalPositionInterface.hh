#ifndef DYB2CalPositionInterface_hh
#define DYB2CalPositionInterface_hh

#include "globals.hh"
#include "G4Transform3D.hh"

namespace DYB2 {

class ICalPosition {
public:
    virtual G4bool hasNext()=0;
    virtual G4Transform3D next()=0;
    virtual ~ICalPosition()=0;
};

}

#endif
