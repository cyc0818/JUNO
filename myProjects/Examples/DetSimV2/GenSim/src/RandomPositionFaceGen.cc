
#include "RandomPositionFaceGen.hh"
#include "Randomize.hh"
#include <cmath>

namespace DYB2 {
  namespace Ball {

RandomPositionFaceGen::RandomPositionFaceGen(G4double r) {
  m_r = r;
}

void 
RandomPositionFaceGen::setSeed(long seed) {
    CLHEP::HepRandom::setTheSeed(seed);
}

G4ThreeVector
RandomPositionFaceGen::next() {
  double costheta, theta;
  double phi;

  costheta = -1 + (2) * G4UniformRand();
  theta = acos(costheta);
  phi = 2 * pi * G4UniformRand();

  double x = m_r * sin(theta) * cos(phi);
  double y = m_r * sin(theta) * sin(phi);
  double z = m_r * cos(theta);
  return G4ThreeVector(x, y, z);

}

  }
}
