#ifndef HexagonPosBall_hh
#define HexagonPosBall_hh

#include "DetSimAlg/IDetElementPos.h"

#include <string>
#include <vector>
#include "G4String.hh"

namespace JUNO {

namespace Ball {

class HexagonPosBall: public IDetElementPos {
public:
    // this r is the distance between center of ball
    // and the center of PMT Equator
    HexagonPosBall(G4String filename, G4double r, G4bool lpmt);
    ~HexagonPosBall();

    G4bool hasNext();
    G4Transform3D next();
private:
    void initialize();
private:
    G4String m_filename;
    G4double m_ball_r;
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
    
    G4bool lpmt_flag;

};

}

}

#endif
