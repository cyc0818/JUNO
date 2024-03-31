#ifndef IDetElement_h
#define IDetElement_h

#include <string>
class G4LogicalVolume;
#include "IDetElementPos.h"

class IDetElement {
public:
    virtual ~IDetElement() {}

    virtual G4LogicalVolume* getLV() = 0;
    virtual bool inject(std::string /* motherName */, IDetElement* /* other */, IDetElementPos* /* pos */) {return true;}

    virtual double geom_info(const std::string& /* param */) {return 0.0;}
    virtual double geom_info(const std::string& /* param */, int /* idx */) {return 0.0;}
};

#endif
