#ifndef WaterPoolConstruction_hh
#define WaterPoolConstruction_hh

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

class WaterPoolConstruction : public IDetElement,
                              public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    WaterPoolConstruction(const std::string& name);
    ~WaterPoolConstruction();

private:

    void initMaterials();
    void initVariables();

    // + WP: Water Pool
    void makeWPLogical();

    // ++ Latticed Stainless Steel Shell
    void makeLatticedShell();

private:
    G4Tubs*          solidWaterPool;
    G4LogicalVolume* logicWaterPool;

    G4Material* Water;
    G4Material* vetoWater;
    double m_radWP;
    double m_heightWP;

    // properties
    bool m_check_overlap;

    bool m_enabledLatticedShell;
private:
    // Control the daughters enabled or not in the WP
    // If IsPMT20inchEnabled = false, then the 20inch PMTs will not be constructed
    bool m_pmt20inch_enabled;
};

#endif
