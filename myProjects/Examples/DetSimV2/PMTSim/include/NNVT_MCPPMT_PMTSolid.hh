#ifndef NNVT_MCPPMT_PMTSolid_hh
#define NNVT_MCPPMT_PMTSolid_hh

/* Implemantation of NNVT MCP-PMT geometry.
 * 
 * -- Tao Lin, 2017/05/29
 */

#include "globals.hh"
class G4VSolid;

class NNVT_MCPPMT_PMTSolid {
public:
  // We fix the whole geometry parameters. 
  // Developers could use thickness to get different dimension.
  NNVT_MCPPMT_PMTSolid();

  G4VSolid* GetSolid(G4String solidname, double thickness=0.0, char mode=' ');

private:
  // Some key parameters
  // m_R: Radius: 254.mm
  // m_H: Height: 570.mm
  // m_Htop: Height from top to equator: 184.mm
  // m_Hbtm: Height from bottom of torus to bottom: 172.50mm
  // m_Rbtm: 50.mm
  // m_Rtorus: 43.mm
  // 
  // m_Heq2torus: Height from equator to torus = m_H - m_Htop - m_Hbtm
  // m_Htubetorus: tube-torus part height
  // m_Rtubetorus: tube-torus part radius
  //
  //               '<--- 254 --->'
  //         ..  --|--  ..         <- top
  //     *                   *
  //   *                       *
  // |                           | <- equator              PART I: ellipse
  //   *                       *
  //     *  .             .  *     <- ellipse and torus
  //          .         .          Radius of torus: 43.0   PART II: Tube-Torus
  //          .         .      -   <- torus and tube
  //          '         '      ^
  //          |         |    172.50                        PART III: Tube
  //          |         |      v
  //           ----|----       -   <- bottom
  //               ' 50 '

  double m_R;
  double m_H;
  double m_Htop;
  double m_Hbtm;
  double m_Rbtm;
  double m_Rtorus;
  bool   m_polycone_neck; 
 
  double m_Heq2torus; 
  double m_theta;
  double m_Htubetorus;
  double m_Rtubetorus;

};

#endif
