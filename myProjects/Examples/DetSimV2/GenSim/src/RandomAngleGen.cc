
#include "RandomAngleGen.hh"
#include "Randomize.hh"

namespace DYB2 {

RandomAngleGen::RandomAngleGen(G4double angle_start,
                               G4double angle_stop) {
    m_angle_start = angle_start;
    m_angle_stop = angle_stop;
}

void 
RandomAngleGen::setSeed(long seed) {
    CLHEP::HepRandom::setTheSeed(seed);
}

G4double
RandomAngleGen::next() {
    G4double val = m_angle_start + 
                        (m_angle_stop - m_angle_start) * G4UniformRand();
    return val;
}

}
