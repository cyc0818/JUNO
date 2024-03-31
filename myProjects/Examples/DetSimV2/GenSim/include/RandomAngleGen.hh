
#ifndef RandomAngleGen_hh
#define RandomAngleGen_hh

#include "DYB2AngleGenInterface.hh"

namespace DYB2 {

class RandomAngleGen :public IAngleGen {
public:
    RandomAngleGen(G4double angle_start, 
                   G4double angle_stop);

    void setSeed(long);
    G4double next();

private:
    G4double m_angle_start;
    G4double m_angle_stop;
};

}

#endif
