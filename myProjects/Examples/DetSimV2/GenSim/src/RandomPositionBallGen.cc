#include "DYB2PositionGenInterface.hh"
#include "RandomPositionGen.hh"
#include "RandomPositionBallGen.hh"

namespace DYB2 {
namespace Ball {

RandomPositionBallGen::RandomPositionBallGen(double ball_r)
    :m_ball_r(ball_r){
    m_gen = new RandomPositionGen(
                                    - m_ball_r,
                                    m_ball_r,
                                    -m_ball_r,
                                    m_ball_r,
                                    -m_ball_r,
                                    m_ball_r
                                    );

}
void
RandomPositionBallGen::setSeed(long seed) 
{
    m_gen -> setSeed(seed);
}

G4ThreeVector
RandomPositionBallGen::next()
{
    G4ThreeVector tmpvec;

    tmpvec = m_gen -> next();

    while( ! isOk(tmpvec) ) {
        tmpvec = m_gen -> next();
    }

    return tmpvec;
}
bool
RandomPositionBallGen::isOk(G4ThreeVector pos)
{
    double x = pos.x();
    double y = pos.y();
    double z = pos.z();

    return (x*x+y*y+z*z) < (m_ball_r*m_ball_r);
}

}
}
