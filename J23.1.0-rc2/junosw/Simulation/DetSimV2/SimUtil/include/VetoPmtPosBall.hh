#ifndef VetoPmtPosBall_hh
#define VetoPmtPosBall_hh

#include "DetSimAlg/IDetElementPos.h"

#include "globals.hh"
#include <string>
#include <vector>
#include "G4String.hh"

class Task;

namespace JUNO {

namespace Ball {

class VetoPmtPosBall: public IDetElementPos{
public:
    // this r is the distance between center of ball
    // and the center of PMT Equator
    VetoPmtPosBall(Task* scope);
    ~VetoPmtPosBall();

    G4bool hasNext();
    G4Transform3D next();
private:
    void initialize();
private:
    // G4String m_filename;
    // G4double m_ball_r;
    Task* m_scope;
    std::vector< G4Transform3D > m_position;
    std::vector< G4Transform3D >::iterator m_position_iter;
    
    // G4bool lpmt_flag;

};

}

}

#endif
