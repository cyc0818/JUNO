#ifndef LowerChimney_hh
#define LowerChimney_hh

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

class LowerChimney : public IDetElement,
                     public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    LowerChimney(const std::string& name);
    ~LowerChimney();

private:

    void initMaterials();
//    void initVariables();

    void makeLowerChimneyLogicalAndPhysical();
    void setupLowerChimneyReflectorInCD();
	void makeNoShutterLowerChimneyLogicalAndPhysical();
//    void setupBlockerReflectorInCD();

 

private:

/////////
    G4VSolid*  solidLowerChimney;
    G4LogicalVolume* logicLowerChimney;

    G4VSolid*  solidLowerChimneyTyvek ;
    G4LogicalVolume* logicLowerChimneyTyvek;
    G4VPhysicalVolume* physiLowerChimneyTyvek;

    G4VSolid*  solidLowerChimneyAcrylic;
    G4LogicalVolume* logicLowerChimneyAcrylic;
    G4VPhysicalVolume* physiLowerChimneyAcrylic;

    G4VSolid*  solidLowerChimneySteel  ;
    G4LogicalVolume* logicLowerChimneySteel;
    G4VPhysicalVolume* physiLowerChimneySteel;

    G4VSolid*  solidLowerChimneyLS     ;
    G4LogicalVolume* logicLowerChimneyLS;
    G4VPhysicalVolume* physiLowerChimneyLS;

    G4VSolid*  solidLowerChimneyBlocker;
    G4LogicalVolume* logicLowerChimneyBlocker;
    G4VPhysicalVolume* physiLowerChimneyBlocker;
///////////////////////



private:

    G4Material* LS;           // Target material
    G4Material* Air;         
    G4Material* Acrylic;         
    G4Material* Water;
    G4Material* Tyvek;
    G4Material* Steel;
private:
    double m_Blocker2Btm;
    double m_reflectivity;
private:
    // properties
 //   bool m_use_chimney;
//    bool m_upper_chimney;
    bool m_lower_chimney;
    bool m_use_shutter;
};

#endif
