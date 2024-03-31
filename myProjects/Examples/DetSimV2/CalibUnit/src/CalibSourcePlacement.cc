#include "CalibSourcePlacement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
using namespace CLHEP;

DECLARE_TOOL(CalibSourcePlacement);

CalibSourcePlacement::CalibSourcePlacement(const std::string& name) 
    : ToolBase(name) {
    m_is_initialized = false;
    m_position_iter = m_position.end();

    declProp("OffsetInZ", m_offsetZ_in_cd=0.);
    declProp("OffsetInX", m_offsetX_in_cd=0.);
    declProp("OffsetInY", m_offsetY_in_cd=0.);
}

CalibSourcePlacement::~CalibSourcePlacement() {

}

G4bool 
CalibSourcePlacement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
CalibSourcePlacement::next() {
    return *(m_position_iter++);
}

void
CalibSourcePlacement::init() {
    double x = m_offsetX_in_cd;
    double y = m_offsetY_in_cd;
    double z = m_offsetZ_in_cd;
    G4ThreeVector pos(x, y, z);
    G4RotationMatrix rot;
    G4Transform3D trans(rot, pos); 
    m_position.push_back(trans);

    m_position_iter = m_position.begin();
}
