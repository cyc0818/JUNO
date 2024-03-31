#ifndef UpperAcrylicConstruction_hh
#define UpperAcrylicConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Polycone;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class UpperAcrylicConstruction : public IDetElement,
                        public ToolBase{
                        
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    UpperAcrylicConstruction(const std::string& name);
    ~UpperAcrylicConstruction();

private:
    void initMaterials();
    void initVariables();

    void makeUpperLogical();

private:

     G4VSolid* base_steel;
     G4LogicalVolume* logicUpper;

private:
     G4Material* Steel;

private:
     double ZUpper[504];
     double RminUpper[504];
     double RmaxUpper[504];

private:
    std::string option;
                           
};
#endif
