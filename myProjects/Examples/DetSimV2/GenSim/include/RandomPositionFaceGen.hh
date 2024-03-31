#ifndef RandomPositionFaceGen_hh
#define RandomPositionFaceGen_hh

#include "DYB2PositionGenInterface.hh"

namespace DYB2 {
  namespace Ball {

class RandomPositionFaceGen: public IVector3dGen {

public:
  RandomPositionFaceGen(G4double r);

  G4ThreeVector next();
  void setSeed(long);

private:
  G4double m_r;
  
};
  }
}

#endif
