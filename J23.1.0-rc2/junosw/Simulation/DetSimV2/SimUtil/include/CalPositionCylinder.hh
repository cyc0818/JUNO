#ifndef CalPositionCylinder_hh
#define CalPositionCylinder_hh

#include "DetSimAlg/IDetElementPos.h"

#include <vector>

namespace JUNO {

namespace Cylinder{

class CalPositionCylinder: public IDetElementPos {
public:
    CalPositionCylinder(G4double cyl_r,
                    G4double cyl_h,
                    G4double pmt_r,
                    G4double pmt_h,
                    G4double pmt_interval);
    ~CalPositionCylinder();
    G4bool hasNext();
    G4Transform3D next();

private:
    void initialize();
    void calculate();

    G4int GetMaxiumNinCircle(G4double r_tube,
                             G4double gap);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
    G4double m_cyl_r;
    G4double m_cyl_h;
    G4double m_pmt_r;
    G4double m_pmt_h;
    G4double m_pmt_interval;


    G4double m_per_phi;

};

}
}

#endif
