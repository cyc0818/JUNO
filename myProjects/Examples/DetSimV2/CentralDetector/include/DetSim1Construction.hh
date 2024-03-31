#ifndef DetSim1Construction_hh
#define DetSim1Construction_hh

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

class DetSim1Construction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    DetSim1Construction(const std::string& name);
    ~DetSim1Construction();

private:

    void initMaterials();
    void initVariables();

    // + Tyvek film 
    //   the Tyvek film will contain the Inner Water Pool.
    //   and the surface will be added when the CD is injected into Outer Water
    //   Pool.
    void makeReflectorLogical();
    void makeReflectorChimneyLogical();

    // + WP: Water Pool
    void makeWPLogical();
    void makeWPWithChimneyLogical();
    void makeWPPhysical();

    // ++ Acrylic Ball
    void makeAcrylicLogical();
    void makeAcrylicWithChimneyLogical();
    void makeAcrylicPhysical();

    // +++ LS 
    void makeLSLogical();
    void makeLSWithChimneyLogical();
    void makeLSPhysical();

    // ++ PMT
    //    The PMTs are injected into WP.
    void makePMTLogical();
    void makePMTPhysical();

    // ++++ Commissioning Water
    void makeCommissioningWaterLogical();
    void makeCommissioningWaterWithChimneyLogical();
    void makeCommissioningWaterPhysical();

private:
    G4VSolid*          solidReflector;
    G4LogicalVolume*   logicReflector;

    G4VSolid*          solidWaterPool;
    G4LogicalVolume*   logicWaterPool;
    G4VPhysicalVolume* physiWaterPool;

    G4VSolid*           solidAcrylic;
    G4LogicalVolume*    logicAcrylic;
    G4VPhysicalVolume*  physiAcrylic;

    G4VSolid*           solidTarget;   // pointer to the solid Target
    G4LogicalVolume*    logicTarget;   // pointer to the logical Target
    G4VPhysicalVolume*  physiTarget;   // pointer to the physical Target

    G4VSolid*           solidCommissioningWater;   // pointer to the solid Target
    G4LogicalVolume*    logicCommissioningWater;   // pointer to the logical Target
    G4VPhysicalVolume*  physiCommissioningWater;   // pointer to the physical Target


    G4LogicalVolume*   pmttube_log;
    G4VPhysicalVolume* pmttube_phys;
private:

    G4Material* LS;           // Target material
    G4Material* Acrylic;         
    G4Material* Water;
    G4Material* Tyvek;
private:
    double m_radLS;
    double m_thicknessAcrylic;
    double m_radAcrylic;
    double m_radWP;
    double m_heightWP;

    double m_radInnerWater;        // the radius of ball;
    double m_radInnerWaterChimney; // the radius of tube

    double m_thicknessReflector;
    double m_radReflector;
    double m_radReflectorChimney;

    double m_ChimneyTopToCenter;
    double m_radLSChimney;
    double m_radAcrylicChimney;

    double m_blockerZ;
    double m_ChimneyoffSet;

private:
    // properties
    bool m_use_chimney;
    bool m_check_overlap;

    // commissioning stage.
    //           || chimney (need to be configured additionally.)
    //          .  . 
    //      .          . 
    //     --------------    zbelowwater     z > 0
    //    ----------------    equator        z = 0
    //                                       z < 0
    //      .          .
    //          .  .

    bool m_is_commissioning;
    double m_z_below_water; // below z is water
};

#endif
