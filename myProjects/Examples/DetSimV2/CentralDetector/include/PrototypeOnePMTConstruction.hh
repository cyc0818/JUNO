#ifndef PrototypeOnePMTConstruction_hh
#define PrototypeOnePMTConstruction_hh

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

class PrototypeOnePMTConstruction: public IDetElement,
                             public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    PrototypeOnePMTConstruction(const std::string& name);
    ~PrototypeOnePMTConstruction();

private:

    void initMaterials();
    void initVariables();

    // + Steel Tube
    void makeSteelTubeLogical();

    // ++ LAB
    void makeBufferLogical();
    void makeBufferPhysical();

    // ++ 20 inch PMTs
    // ++ 8 inch PMTs
private:
    G4VSolid*        solidSteelTube;
    G4LogicalVolume* logicSteelTube;

    G4VSolid*          solidBuffer;
    G4LogicalVolume*   logicBuffer;
    G4VPhysicalVolume* physiBuffer;

private: // Material
    G4Material* Steel;
    G4Material* LS;           // Target material
    G4Material* LAB;           // Target material
    G4Material* Water;           // Target material
    G4Material* Acrylic;         
    G4Material* Air;
    G4Material* Oil;

private:
    double m_steel_tube_height;
    double m_steel_tube_radius;
    double m_steel_tube_thickness;

    double m_buffer_tube_height;
    double m_buffer_tube_radius;

    std::string m_buffer_material;

};

#endif
