#include "OnePMTPlacement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
using namespace CLHEP;
DECLARE_TOOL(OnePMTPlacement);

OnePMTPlacement::OnePMTPlacement(const std::string& name)
    : ToolBase(name), m_is_initialized(false) {
    m_position_iter = m_position.end();

    // unit: mm
    declProp("x", x=0.0);
    declProp("y", y=0.0);
    declProp("z", z=0.0);

    // unit: degree
    declProp("0RotateZ", psi=0);   // rotate the PMT first along Z axis
    declProp("1RotateY", theta=0);
    declProp("2RotateZ", phi=0);
}

OnePMTPlacement::~OnePMTPlacement() {

}

G4bool
OnePMTPlacement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
OnePMTPlacement::next() {
    return *(m_position_iter++);
}

void
OnePMTPlacement::init() {
    G4ThreeVector pos(x*mm, y*mm, z*mm);
    G4RotationMatrix rot;
    rot.rotateZ(psi*deg);
    rot.rotateY(theta*deg);
    rot.rotateZ(phi*deg);
    G4Transform3D trans(rot, pos); 
    m_position.push_back(trans);

    m_position_iter = m_position.begin();
}
