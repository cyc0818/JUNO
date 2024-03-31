
#include "DYB2PositionGenInterface.hh"
#include "RandomPositionGen.hh"

#include "Randomize.hh"

namespace DYB2 {
RandomPositionGen::RandomPositionGen(
        double x_low, double x_up,
        double y_low, double y_up,
        double z_low, double z_up) {

    m_x_low = x_low;
    m_x_up = x_up;
    m_y_low = y_low;
    m_y_up = y_up;
    m_z_low = z_low;
    m_z_up = z_up;

}

void 
RandomPositionGen::setSeed(long seed) {
    CLHEP::HepRandom::setTheSeed(seed);
}

G4ThreeVector 
RandomPositionGen::next() {
    double x = m_x_low + (m_x_up - m_x_low) * G4UniformRand();
    double y = m_y_low + (m_y_up - m_y_low) * G4UniformRand();
    double z = m_z_low + (m_z_up - m_z_low) * G4UniformRand();
    return G4ThreeVector(x, y, z);
}
}
