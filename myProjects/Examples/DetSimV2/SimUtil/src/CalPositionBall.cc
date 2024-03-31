#include "CalPositionBall.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <cassert>
using namespace CLHEP;
namespace JUNO {
namespace Ball {

CalPositionBall::CalPositionBall(G4double ball_r,
                                 G4double pmt_r,
                                 G4double pmt_offset,
                                 G4double gap) 
{
    m_ball_r = ball_r;
    m_pmt_r = pmt_r;
    m_pmt_offset = pmt_offset;
    m_gap = gap;

    initialize();
    calculate();

    m_position_iter = m_position.begin();
}

CalPositionBall::~CalPositionBall() {

}

void CalPositionBall::initialize() {
  G4int n_x_z = GetMaxiumNinCircle(
                                    m_ball_r,
                                    m_pmt_r,
                                    m_gap);
  G4int n_x_z_half = n_x_z / 2;

  m_n_theta_max = n_x_z_half + 1;
  m_per_theta = pi / n_x_z_half;

}

// Helper:
// Copy from the original Tube Detector.
G4int CalPositionBall::GetMaxiumNinCircle(G4double r_tube,
    G4double r_pmt,
    G4double gap)
{
  G4int N=0;
  G4double theta = 2 * atan(r_pmt / r_tube);
  G4double phi = 2* asin(gap / (2*sqrt(r_pmt*r_pmt + r_tube*r_tube)));
  N = int(2*pi / (theta + phi));
  return N;
}

G4Transform3D CalPositionBall::next() {
    return *(m_position_iter++);
}

void CalPositionBall::calculate() {
  for (G4int theta_i = 0; theta_i < m_n_theta_max; ++theta_i) {
    //G4double theta = per_theta * n_x_z_half/2;
    G4double theta = m_per_theta * theta_i;

    G4double small_theta = atan( m_pmt_r / m_ball_r);

    //G4cout << "Small theta: " << small_theta << G4endl;
    //G4cout << "Theta: " << theta << G4endl;

    G4int n_one_circle = 1;
    G4double per_phi = 0;

    assert ( (0 <= theta) && (theta <=pi) );

    G4double theta_real=0;

    if ( theta < pi/2 ) {
      theta_real = theta - small_theta;
    } else if (theta >= pi/2) {
      theta_real = (pi - theta) - small_theta;
    }


    if ( (theta_real > small_theta) ) {

      assert ( (0 <= theta) && (theta <=pi) );

      //G4cout << "Theta: " << theta_real << G4endl;

      G4double ball_r_x_y = m_ball_r * sin(theta_real);

      assert (ball_r_x_y >= 0 || printf("Ball_r_x_y: %g m\n", ball_r_x_y/m));

      // Calculate the r - phi
      // TODO
      // The gap is the gap between the small Rs.

      n_one_circle = GetMaxiumNinCircle(
                                                    ball_r_x_y,
                                                    m_pmt_r,
                                                    m_gap
                                                            );
      assert ( n_one_circle > 0 );
      per_phi = 2*pi / n_one_circle;

    } else {

    }


    for (G4int phi_i=0; phi_i < n_one_circle; ++phi_i) {

      G4double phi = per_phi * phi_i;


      G4double x = (m_pmt_offset + m_ball_r) * sin(theta) * cos(phi);
      G4double y = (m_pmt_offset + m_ball_r) * sin(theta) * sin(phi);
      G4double z = (m_pmt_offset + m_ball_r) * cos(theta);

      G4ThreeVector pos(x, y, z);
      G4RotationMatrix rot;
      rot.rotateZ(pi/4); // The Cover is quadrate
      rot.rotateY(pi + theta);
      rot.rotateZ(phi);
      G4Transform3D trans(rot, pos);

      // Append trans
      m_position.push_back(trans);
    }
  }

}

G4bool CalPositionBall::hasNext() {
    return m_position_iter != m_position.end();
}


}
}
