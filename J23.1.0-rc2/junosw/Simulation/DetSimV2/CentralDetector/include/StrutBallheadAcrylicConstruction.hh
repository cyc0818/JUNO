#ifndef StrutBallheadAcrylicConstruction_hh
#define StrutBallheadAcrylicConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4Orb;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class StrutBallheadAcrylicConstruction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    
    virtual double geom_info(const std::string& /* param */);  

    StrutBallheadAcrylicConstruction(const std::string& name);
    ~StrutBallheadAcrylicConstruction();

private:
    void initMaterials();
    void initVariables();

    // ++Strut 
    void makeStrutLogical();

private:
    
    G4Orb*          solidStrut;
    G4LogicalVolume* logicStrut;

private:
    G4Material* Steel;           // Target material

private:
   // double m_radStrut_in ;
   // double m_radStrut_out;
   // double m_lengthStrut;
   double m_rad;
    
    double gap;
    double strut_pos;
    
    G4double m_radLS ;
    G4double m_radAcrylic ;
    G4double m_anchorHeight;

};

#endif
