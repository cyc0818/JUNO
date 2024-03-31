#ifndef IPMTElement_h
#define IPMTElement_h

#include "IDetElement.h"
#include "globals.hh"
#include "G4ThreeVector.hh"

class IPMTElement: public IDetElement {
public:

    virtual ~IPMTElement() {}

    virtual G4LogicalVolume* getLV() = 0;
    virtual bool inject(std::string /* motherName */, IDetElement* /* other */, IDetElementPos* /* pos */) {return false;};

    // geometry info
    virtual G4double GetPMTRadius() = 0;
    virtual G4double GetPMTHeight() = 0;
    virtual G4double GetZEquator() = 0;
    virtual G4ThreeVector GetPosInPMT() = 0;

};

#endif
