#ifndef PrototypeConstruction_hh
#define PrototypeConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"
#include <string>

class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSolid;

class PrototypeConstruction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    PrototypeConstruction(const std::string& name);
    ~PrototypeConstruction();

private:

    void initMaterials();
    void initVariables();

    // + Steel Tube
    void makeSteelTubeLogical();

    // ++ LAB
    void makeBufferLogical();
    void makeBufferPhysical();

    // +++ Acrylic Ball or nylon ball
    void makeAcrylicLogical();
    void makeAcrylicWithChimneyLogical();
    void makeAcrylicPhysical();

    // ++++ LS
    void makeLSLogical();
    void makeLSWithChimneyLogical();
    void makeLSPhysical();

    // ++ 20 inch PMTs
    // ++ 8 inch PMTs
private:
    G4Tubs*          solidSteelTube;
    G4LogicalVolume* logicSteelTube;

    G4Tubs*            solidBuffer;
    G4LogicalVolume*   logicBuffer;
    G4VPhysicalVolume* physiBuffer;

    G4VSolid*           solidAcrylic;
    G4LogicalVolume*    logicAcrylic;
    G4VPhysicalVolume*  physiAcrylic;

    G4VSolid*           solidTarget;   // pointer to the solid Target
    G4LogicalVolume*    logicTarget;   // pointer to the logical Target
    G4VPhysicalVolume*  physiTarget;   // pointer to the physical Target

private: // Material
    G4Material* Steel;
    G4Material* LS;           // Target material
    G4Material* LAB;           // Target material
    G4Material* Water;           // Target material
    G4Material* Acrylic;         

private:
    double m_steel_tube_height;
    double m_steel_tube_radius;
    double m_steel_tube_thickness;

    double m_buffer_tube_height;
    double m_buffer_tube_radius;

    double m_acrylic_ball_radius; // outer
    double m_acrylic_ball_thickness;
    double m_acrylic_chimney_radius;

    double m_acrylic_circle_radius;
    double m_acrylic_circle_height;

    double m_target_radius;
    double m_target_chimney_radius;

    double m_ChimneyTopToCenter;

    std::string m_buffer_material;

    bool m_use_chimney;
    bool m_use_equator_circle;
};

#endif
