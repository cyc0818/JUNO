
#ifndef RandomPositionHollowGen_hh
#define RandomPositionHollowGen_hh

#include "DYB2PositionGenInterface.hh"

namespace DYB2 {

namespace Ball {
class RandomPositionHollowGen : public IVector3dGen {

public:
    RandomPositionHollowGen(double ball_r,double thick,G4String material);
    void setSeed(long);
    G4ThreeVector next();

private:
    bool isOk(G4ThreeVector);

private:
    IVector3dGen* m_gen;
    double m_ball_r;
    double m_thick;
    G4String m_material;

};
}

}

#endif
