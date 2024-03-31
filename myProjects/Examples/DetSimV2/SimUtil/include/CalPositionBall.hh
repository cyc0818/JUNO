#ifndef CalPositionBall_hh
#define CalPositionBall_hh

#include "DetSimAlg/IDetElementPos.h"

#include <vector>

namespace JUNO {

namespace Ball{

class CalPositionBall: public IDetElementPos {
public:
    CalPositionBall(G4double ball_r,
                    G4double pmt_r,
                    G4double pmt_offset,
                    G4double gap);
    ~CalPositionBall();
    G4bool hasNext();
    G4Transform3D next();

private:
    void initialize();
    void calculate();

    G4int GetMaxiumNinCircle(G4double r_tube,
                             G4double r_pmt,
                             G4double gap);

private:
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
    G4double m_ball_r;
    G4double m_pmt_r;
    G4double m_pmt_offset;
    G4double m_gap;


    G4double m_n_theta_max;
    G4double m_per_theta;

};

}
}

#endif
