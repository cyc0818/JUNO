#ifndef IDetElementPos_h
#define IDetElementPos_h

#include "globals.hh"
#include "G4Transform3D.hh"

class IDetElementPos {
public:
    virtual G4bool hasNext()=0;
    virtual G4Transform3D next()=0;
    virtual ~IDetElementPos(){}
};

#endif
