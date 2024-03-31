#ifndef StrutBalloonConstruction_hh
#define StrutBalloonConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class StrutBalloonConstruction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    StrutBalloonConstruction(const std::string& name);
    ~StrutBalloonConstruction();

private:
    void initMaterials();
    void initVariables();

    // ++Strut 
    void makeStrutLogical();

private:
    
    G4Tubs*          solidStrut;
    G4LogicalVolume* logicStrut;

private:
    G4Material* Steel;           // Target material
    G4Material* Acrylic;           // Target material

private:
    double m_radStrut_in ;
    double m_radStrut_out;
    double m_lengthStrut;
    
    double gap;
    double strut_pos;
};

#endif
