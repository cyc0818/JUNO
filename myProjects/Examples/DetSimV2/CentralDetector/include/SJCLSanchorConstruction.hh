#ifndef SJCLSanchorConstruction_hh
#define SJCLSanchorConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class SJCLSanchorConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    SJCLSanchorConstruction(const std::string& name);
    ~SJCLSanchorConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeSJCLSanchorLogical();

private:
    G4VSolid* solidSJCLSanchor_up;
    G4VSolid* solidSJCLSanchor_down;
    G4VSolid* solidSJCLSanchor_box;
    G4VSolid* solidSJCLSanchor_ball;

    G4LogicalVolume* logicSJCLSanchor;

private:
    G4Material* Acrylic;

};

#endif
