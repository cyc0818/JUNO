#ifndef UpperChimneyMaker_hh
#define UpperChimneyMaker_hh

/* This class is used to create a Ball with Chimney.

 */
#include <string>
#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSolid;


class UpperChimneyMaker {
    public:
    UpperChimneyMaker(double TopToWater);
    ~UpperChimneyMaker();
public:
    G4VSolid* getSolidUpperChimney();

    G4VSolid* getSolidALS();
    G4VSolid* getSolidBSteel();
    G4VSolid* getSolidBTyvek();


private:
    std::string m_solid_name;

    double m_TopToWater;
//    double m_GateCenter;

public:
    double m_TubeInnerR;

    double m_Tyvek_thickness;
    double m_Steel_thickness; 

    double m_TubeTyvekOuterR;
    double m_TubeSteelOuterR;
};

class UpperChimney : public IDetElement,
public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    
//    double geom_info(const std::string& param,  const int wallnumber);
    
    UpperChimney(const std::string& name);
    ~UpperChimney();
private:
    void initMaterials();
    void initVariables();
    void makeUpperChimneyLogicalandPhysical();
    void setupTopChimneyReflectorInCD();   
private:
    G4VSolid*          solidUpperChimney;
    G4LogicalVolume*   logicUpperChimney;

    G4VSolid*          solidUpperChimneyLS;
    G4LogicalVolume*   logicUpperChimneyLS;
    G4VPhysicalVolume* physiUpperChimneyLS;
    
    
    G4VSolid*          solidUpperChimneySteel;
    G4LogicalVolume*   logicUpperChimneySteel;
    G4VPhysicalVolume* physiUpperChimneySteel;
    
    
    G4VSolid*          solidUpperChimneyTyvek;
    G4LogicalVolume*   logicUpperChimneyTyvek;
    G4VPhysicalVolume* physiUpperChimneyTyvek;
    
private:
    
    G4Material* LS;           // Target material
    G4Material* Air;
    G4Material* Acrylic;
    G4Material* Water;
    G4Material* Tyvek;
    G4Material* Steel;
    
private:
    double m_heightWP;
    double m_reflectivity;
private:
    // properties
    bool m_use_UpperChimney;
    double m_TopToWater;

};
#endif
