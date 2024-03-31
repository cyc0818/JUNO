#ifndef XJfixtureConstruction_hh
#define XJfixtureConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
//class G4SubtractionSolid;
class XJfixtureConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    virtual double geom_info(const std::string& /* param */);

    XJfixtureConstruction(const std::string& name);
    ~XJfixtureConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeXJfixtureLogical();

private:
    G4VSolid* solidXJfixture_down1;
    G4VSolid* solidXJfixture_down2;
    G4VSolid* solidXJfixture_down3;
    G4VSolid* solidXJfixture_down_uni1;
    G4VSolid* solidXJfixture_down_uni2;
    G4VSolid* solidXJfixture_down_uni3;
    G4VSolid* solidXJfixture_down_uni4;
    G4VSolid* solidXJfixture_down_uni5;

    G4VSolid* solidXJfixture_up1;
    G4VSolid* solidXJfixture_up2;
    G4VSolid* solidXJfixture_up3;
    G4VSolid* solidXJfixture_up_uni1;
    G4VSolid* solidXJfixture_up_uni2;

    G4VSolid* solidXJfixture;

    G4LogicalVolume* logicXJfixture;

private:
    G4Material* PE_PA ;
    G4double m_xjfixture_pos;

};

#endif
