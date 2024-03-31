#ifndef SJReceiverFasternConstruction_hh
#define SJReceiverFasternConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class SJReceiverFasternConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    virtual double geom_info(const std::string& /* param */);

    SJReceiverFasternConstruction(const std::string& name);
    ~SJReceiverFasternConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeSJReceiverFasternLogical();

private:
    G4VSolid* solidSJReceiverFastern_down1;
    G4VSolid* solidSJReceiverFastern_down2;
    G4VSolid* solidSJReceiverFastern_down3;
    G4VSolid* solidSJReceiverFastern_down_uni1;
    G4VSolid* solidSJReceiverFastern_down_uni2;
    G4VSolid* solidSJReceiverFastern_down_uni3;
    G4VSolid* solidSJReceiverFastern_down_uni4;
    G4VSolid* solidSJReceiverFastern_down_uni5;

    G4VSolid* solidSJReceiverFastern_up1;
    G4VSolid* solidSJReceiverFastern_up2;
    G4VSolid* solidSJReceiverFastern_up_uni;

    G4VSolid* solidSJReceiverFastern;

    G4LogicalVolume* logicSJReceiverFastern;

private:
    G4Material* PE_PA ;
    G4double m_sjreceiverfastern_pos ;

};

#endif
