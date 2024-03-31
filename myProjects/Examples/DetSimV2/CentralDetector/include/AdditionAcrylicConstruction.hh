#ifndef AdditionAcrylicConstruction_hh
#define AdditionAcrylicConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Sphere;
class G4Polycone;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class AdditionAcrylicConstruction : public IDetElement,
                            public ToolBase{
                            
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    AdditionAcrylicConstruction(const std::string& name);
    ~AdditionAcrylicConstruction();

private:
    void initMaterials();
    void initVariables();

    void makeAdditionLogical();

private:

    G4VSolid*   solidAddition_down;
    G4VSolid*   solidAddition_up;
    G4VSolid*   solidAddition_up1;
    G4VSolid*   solidAddition_up2;
    G4VSolid*   uni_acrylic1;
    G4VSolid*   uni_acrylic2;
    G4VSolid*   uni_acrylic3;

    G4LogicalVolume* logicAddition;

private:
    G4Material* Acrylic;
    G4Material* Water;

private:
    double ZNodes[502];
    double RminNodes[502];
    double RmaxNodes[502];

private:
    std::string option;                                    
                            
};

#endif
