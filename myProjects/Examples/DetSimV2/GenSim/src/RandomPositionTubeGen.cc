#include "DYB2PositionGenInterface.hh"
#include "RandomPositionGen.hh"
#include "RandomPositionTubeGen.hh"

namespace DYB2 {
namespace Tube {

RandomPositionTubeGen::RandomPositionTubeGen(
        double tube_r,
        double tube_h
        ) : m_tube_r(tube_r), m_tube_h(tube_h){

    m_gen = new RandomPositionGen(
                                    -m_tube_r,
                                    m_tube_r,
                                    -m_tube_r,
                                    m_tube_r,
                                    -m_tube_h/2,
                                    m_tube_h/2);

}

void
RandomPositionTubeGen::setSeed(long seed) 
{
    m_gen -> setSeed(seed);
}

G4ThreeVector
RandomPositionTubeGen::next()
{
    G4ThreeVector tmpvec;

    tmpvec = m_gen -> next();

    while( ! isOk(tmpvec) ) {
        tmpvec = m_gen -> next();
    }

    return tmpvec;
}

bool
RandomPositionTubeGen::isOk(G4ThreeVector pos)
{
    double x = pos.x();
    double y = pos.y();

    return (x*x+y*y) < (m_tube_r*m_tube_r);
}

}

}
