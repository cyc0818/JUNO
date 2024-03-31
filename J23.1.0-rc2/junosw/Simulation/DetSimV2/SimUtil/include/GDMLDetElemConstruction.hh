#ifndef GDMLDetElemConstruction_hh
#define GDMLDetElemConstruction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

class G4Tubs;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class GDMLDetElemConstruction: public IDetElement,
                               public ToolBase {
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    GDMLDetElemConstruction(const std::string& name);
    ~GDMLDetElemConstruction();
private:
    bool init();
private:
    std::string m_gdml_file_name;
    std::string m_top_volume_name;

    G4LogicalVolume* m_top_volume;
};

#endif

