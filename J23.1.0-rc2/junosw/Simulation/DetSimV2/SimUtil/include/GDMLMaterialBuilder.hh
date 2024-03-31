#ifndef GDMLMaterialBuilder_hh
#define GDMLMaterialBuilder_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"

#include <vector>

class GDMLMaterialBuilder: public IDetElement,
                           public ToolBase {
public:
    G4LogicalVolume* getLV();
    bool inject(std::string, IDetElement*, IDetElementPos*) {
        return true;
    }

    GDMLMaterialBuilder(const std::string& name);
    ~GDMLMaterialBuilder();
private:
    bool init();
private:
    bool m_is_initialized;
    bool m_validate;
    std::vector<std::string> m_gdml_files;
};

#endif


