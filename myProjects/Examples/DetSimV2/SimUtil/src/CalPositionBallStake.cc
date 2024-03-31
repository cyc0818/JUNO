#include "CalPositionBallStake.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <cassert>
using namespace CLHEP;


namespace DYB2 {
  namespace Stake{
    CalPositionBallStake::CalPositionBallStake(G4String posfile,
					       G4double ball_r,
					       G4double stake_h
					       ) 
    {
      m_ball_r = ball_r;
      m_stake_h = stake_h;
      initialize(posfile);
      calculate();

      m_position_iter = m_position.begin();
    }

    CalPositionBallStake::~CalPositionBallStake() {

    }

    void CalPositionBallStake::initialize(G4String posfile) {
      G4cout << "Read-in Stake postion file: " << posfile << G4endl;
      FILE *fp=fopen(posfile,"rt");
      G4int i=0,j,n=4,nstake;
      G4double DA,DB;
      while(n==4)
	{
	  n=fscanf(fp,"%d %d %lf %lf\n",&j,&nstake,&DA,&DB);
	  if(n==4)
	    {
	      n_stake[i] = nstake;
	      DegreeA[i] = DA;
	      DegreeB[i] = DB;
	      i++;
	    }
	  else break;
	}
      nDegree = i;
      fclose(fp);
    }



    G4Transform3D CalPositionBallStake::next() {
      return *(m_position_iter++);
    }


    void CalPositionBallStake::calculate() {
      G4int n;
      for (G4int i = 0; i < nDegree ; ++i) {

	G4double theta = (90-DegreeA[i])*pi/180.;

 
	G4int n_one_circle = 1;
	G4double per_phi = 0;
	n=0;

	assert ( (0 <= theta) && (theta <=pi) );


	n_one_circle = n_stake[i];
	per_phi = 2*pi / n_one_circle;
	for (G4int phi_i=0; phi_i < n_one_circle; ++phi_i) {

	  G4double phi = per_phi * phi_i + DegreeB[i]*pi/180.;


	  G4double x = (m_stake_h/2 + m_ball_r) * sin(theta) * cos(phi);
	  G4double y = (m_stake_h/2 + m_ball_r) * sin(theta) * sin(phi);
	  G4double z = (m_stake_h/2 + m_ball_r) * cos(theta);

	  G4ThreeVector pos(x, y, z);
	  G4RotationMatrix rot;
	  rot.rotateY(pi + theta);
	  rot.rotateZ(phi);
	  G4Transform3D trans(rot, pos);

	  G4ThreeVector pos2(x, y, -z);
	  G4RotationMatrix rot2;
	  rot2.rotateY(2*pi - theta);
	  rot2.rotateZ(phi);
	  G4Transform3D trans2(rot2, pos2);

	  // Append trans
	  m_position.push_back(trans);
	  m_position.push_back(trans2);
	  n++;
	}
	printf("Stake Lay: %d NumStake=%d theta=%f StartAngle=%f\n",i+1,n,DegreeA[i],DegreeB[i]);
      }
    }

    G4bool CalPositionBallStake::hasNext() {
      return m_position_iter != m_position.end();
    }

  }
}
