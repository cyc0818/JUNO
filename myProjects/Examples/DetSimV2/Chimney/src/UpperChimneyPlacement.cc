#include <boost/python.hpp>
#include "Dimensions.hh"
#include "UpperChimneyPlacement.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

DECLARE_TOOL(UpperChimneyPlacement);

UpperChimneyPlacement::UpperChimneyPlacement(const std::string& name) 
    : ToolBase(name) {
    m_is_initialized = false;
    m_position_iter = m_position.end();
    declProp("UpperChimneyTop", m_TopToWater=0.);
}

UpperChimneyPlacement::~UpperChimneyPlacement() {

}

G4bool 
UpperChimneyPlacement::hasNext() {
    if (not m_is_initialized) {
        init();
        m_is_initialized = true;
    }
    return m_position_iter != m_position.end();
}

G4Transform3D
UpperChimneyPlacement::next() {
    return *(m_position_iter++);
}

void
UpperChimneyPlacement::init() {
  //  double   m_expHallZ=expHallZ;
  //  double x = m_x_offset_in_cd;
  //  double y = m_y_offset_in_cd;
  //  double z = m_z_offset_in_cd;
    double x = 0;
    double y = 0;
   // double z = 42.5/2.+3.5/2.;
    double z = m_TopToWater*m/2.- expHallZ/2. ;
    G4cout<<"Top Chimney Position: "<<z<<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<G4endl;
    G4ThreeVector pos(x, y, z);
    G4RotationMatrix rot;
    G4Transform3D trans(rot, pos); 
    m_position.push_back(trans);

    m_position_iter = m_position.begin();
}
