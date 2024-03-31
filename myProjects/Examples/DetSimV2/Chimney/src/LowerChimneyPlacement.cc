#include <boost/python.hpp>
#include "Dimensions.hh"
#include "LowerChimneyPlacement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

DECLARE_TOOL(LowerChimneyPlacement);

LowerChimneyPlacement::LowerChimneyPlacement(const std::string& name) 
    : ToolBase(name) {
    m_is_initialized = false;
    m_position_iter = m_position.end();
}

LowerChimneyPlacement::~LowerChimneyPlacement() {

}

G4bool 
LowerChimneyPlacement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
LowerChimneyPlacement::next() {
    return *(m_position_iter++);
}

void
LowerChimneyPlacement::init() {
    double x = 0;
    double y = 0;
    double z = ChimBtm_2_Center + LowerTubeH/2.0; 
   // double z = 0; 
    G4cout<<"Lower Chimney Position: "<<z<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<G4endl;
    G4ThreeVector pos(x, y, z);
    G4RotationMatrix rot;
    G4Transform3D trans(rot, pos); 
    m_position.push_back(trans);

    m_position_iter = m_position.begin();
}
