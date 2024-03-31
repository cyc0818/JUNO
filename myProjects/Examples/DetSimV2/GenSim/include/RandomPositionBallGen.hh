#ifndef RandomPositionBallGen_hh
#define RandomPositionBallGen_hh

#include "DYB2PositionGenInterface.hh"

namespace DYB2 {

namespace Ball {
class RandomPositionBallGen : public IVector3dGen {

public:
    RandomPositionBallGen(double ball_r);
    void setSeed(long);
    G4ThreeVector next();

private:
    bool isOk(G4ThreeVector);

private:
    IVector3dGen* m_gen;
    double m_ball_r;

};
}

}

#endif
