#include "Calib_GuideTube_Placement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
using namespace CLHEP;
DECLARE_TOOL(Calib_GuideTube_Placement);

Calib_GuideTube_Placement::Calib_GuideTube_Placement(const std::string& name) 
    : ToolBase(name) {
    m_is_initialized = false;
    m_position_iter = m_position.end();

    declProp("OffsetInZ", m_offsetZ_in_cd=0.);
    declProp("OffsetInX", m_offsetX_in_cd=0.);
    declProp("OffsetInY", m_offsetY_in_cd=0.);

    declProp("Phi", m_phi=267.4*deg); // along z axis
}

Calib_GuideTube_Placement::~Calib_GuideTube_Placement() {

}

G4bool 
Calib_GuideTube_Placement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
Calib_GuideTube_Placement::next() {
    return *(m_position_iter++);
}

void
Calib_GuideTube_Placement::init() {
    double x = m_offsetX_in_cd;
    double y = m_offsetY_in_cd;
    double z = m_offsetZ_in_cd;
    G4ThreeVector pos(x, y, z);
    G4RotationMatrix rot;
    rot.rotateY(-90*deg);
    rot.rotateZ(m_phi);
    G4cout << "Rotate Z " << m_phi << G4endl;
    G4Transform3D trans(rot, pos);
    for (int i = 0; i < GT_number; i++)
    {
	m_position.push_back(trans);
    }

    m_position_iter = m_position.begin();
}
