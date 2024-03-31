#include "GDMLDetElemConstruction.hh"
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
DECLARE_TOOL(GDMLDetElemConstruction);

GDMLDetElemConstruction::GDMLDetElemConstruction(const std::string& name)
    : ToolBase(name)
      , m_top_volume(0)
{
    declProp("GdmlFilename", m_gdml_file_name);
    declProp("LogicalVolumeName", m_top_volume_name);
}

GDMLDetElemConstruction::~GDMLDetElemConstruction()
{

}

G4LogicalVolume*
GDMLDetElemConstruction::getLV()
{
    if (not m_top_volume) {
        init();
    }
    assert(m_top_volume);

    return m_top_volume;
}

bool
GDMLDetElemConstruction::inject(std::string /* motherName */, IDetElement* /* other */, IDetElementPos* /* pos */)
{
    return true;
}

bool
GDMLDetElemConstruction::init()
{
    // load the gdml file first.
    G4GDMLParser parser;
    parser.Read(m_gdml_file_name);

    G4LogicalVolume* tmp_volume = 0;
    if (m_top_volume_name.size()) {
        tmp_volume = parser.GetVolume(m_top_volume_name);
        if (not tmp_volume) {
            LogError << "Can't find Logical Volume [" << m_top_volume_name
                     << "] in file <" << m_gdml_file_name << ">" << std::endl;
            LogInfo << "Try to use name " << this->objName() << " to load "
                    << std::endl;
        }
    }
    if (not tmp_volume) {
        tmp_volume = parser.GetVolume( this->objName() );
        if (not tmp_volume) {
            LogError << "Can't find Logical Volume [" << this->objName() 
                     << "] in file <" << m_gdml_file_name << ">" << std::endl;
            return false;
        }
    }
    m_top_volume = tmp_volume;
    return true;
}
