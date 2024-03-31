#ifndef XJanchorConstruction_hh
#define XJanchorConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class XJanchorConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    XJanchorConstruction(const std::string& name);
    ~XJanchorConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeXJanchorLogical();

private:
    G4VSolid* solidXJanchor_up;
    G4VSolid* solidXJanchor_down;
    G4VSolid* solidXJanchor_ball;

    G4LogicalVolume* logicXJanchor;

private:
    G4Material* Acrylic;

};

#endif
