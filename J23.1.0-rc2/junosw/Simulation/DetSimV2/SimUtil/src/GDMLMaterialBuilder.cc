#include "GDMLMaterialBuilder.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"

#include "G4GDMLParser.hh"
using namespace CLHEP;
DECLARE_TOOL(GDMLMaterialBuilder);

GDMLMaterialBuilder::GDMLMaterialBuilder(const std::string& name)
    : ToolBase(name)
      , m_is_initialized(false)
      , m_validate(false)
{
    declProp("GdmlFiles", m_gdml_files);
    declProp("GdmlValidation", m_validate);
}

GDMLMaterialBuilder::~GDMLMaterialBuilder()
{

}

G4LogicalVolume*
GDMLMaterialBuilder::getLV() 
{
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return 0;
}

bool
GDMLMaterialBuilder::init()
{
    std::vector<std::string>::iterator it;
    for (it = m_gdml_files.begin(); it != m_gdml_files.end(); ++it) {
        G4GDMLParser parser;
        parser.Read(*it, m_validate);
    }

    return true;
}
