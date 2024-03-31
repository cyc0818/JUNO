#ifndef SJFixtureConstruction_hh
#define SJFixtureConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class SJFixtureConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    SJFixtureConstruction(const std::string& name);
    ~SJFixtureConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeSJFixtureLogical();

private:
    G4VSolid* solidSJFixture_up;
    G4VSolid* solidSJFixture_down;
    G4VSolid* solidSJFixture_box;
    G4VSolid* solidSJFixture_ball;

    G4LogicalVolume* logicSJFixture;

private:
    G4Material* Acrylic;

};

#endif
