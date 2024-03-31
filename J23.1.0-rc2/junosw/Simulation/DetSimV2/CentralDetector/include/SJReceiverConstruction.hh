#ifndef SJReceiverConstruction_hh
#define SJReceiverConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class SJReceiverConstruction : public IDetElement,
			public ToolBase{

public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);
    virtual double geom_info(const std::string& /* param */);

    SJReceiverConstruction(const std::string& name);
    ~SJReceiverConstruction();

private:
   void initMaterials();
   void initVariables();

   void makeSJReceiverLogical();

private:
    G4VSolid* solidSJReceiver_up;
    G4VSolid* solidSJReceiver_down;
    G4VSolid* solidSJReceiver_box;
    G4VSolid* solidSJReceiver_ball;

    G4LogicalVolume* logicSJReceiver;

private:
    G4Material* Acrylic;
    G4double m_radLS;
    G4double m_sjreceiver_pos;
private:
    bool m_simplify_calib_anchor;


};

#endif
