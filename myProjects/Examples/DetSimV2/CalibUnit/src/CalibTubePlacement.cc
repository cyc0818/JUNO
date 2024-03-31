#include "CalibTubePlacement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
using namespace CLHEP;

DECLARE_TOOL(CalibTubePlacement);

CalibTubePlacement::CalibTubePlacement(const std::string& name) 
    : ToolBase(name) {
    m_is_initialized = false;
    m_position_iter = m_position.end();

    declProp("OffsetInZ", m_offset_in_cd=0.);
}

CalibTubePlacement::~CalibTubePlacement() {

}

G4bool 
CalibTubePlacement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
CalibTubePlacement::next() {
    return *(m_position_iter++);
}

void
CalibTubePlacement::init() {
    double x = 0.;
    double y = 0.;
    double z = m_offset_in_cd;
    G4ThreeVector pos(x, y, z);
    G4RotationMatrix rot;
    G4Transform3D trans(rot, pos); 
    m_position.push_back(trans);

    m_position_iter = m_position.begin();
}
