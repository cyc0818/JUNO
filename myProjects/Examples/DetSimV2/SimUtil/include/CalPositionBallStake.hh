#ifndef CalPositionBallStake_hh
#define CalPositionBallStake_hh

#include "DYB2CalPositionInterface.hh"

#include <vector>

namespace DYB2 {
  namespace Stake{

    class CalPositionBallStake: public ICalPosition {
    public:
      CalPositionBallStake(
			   G4String posfile,
			   G4double ball_r,
			   G4double stake_h
			   );
      ~CalPositionBallStake();
      G4bool hasNext();
      G4Transform3D next();

    private:
      void initialize(G4String);
      void calculate();



    private:
      std::vector< G4Transform3D > m_position;
      std::vector< G4Transform3D >::iterator m_position_iter;

      G4double m_ball_r;
      G4double m_stake_h;
      G4double m_n_theta_max;
      G4double m_per_theta;
      G4int nDegree;
      G4int n_stake[5000];
      G4double DegreeA[5000],DegreeB[5000];

    };

  }
}

#endif
