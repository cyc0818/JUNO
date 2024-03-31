
#include "RoundBottomFlaskSolidMaker.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include <cassert>
#include <cmath>
using namespace CLHEP;
RoundBottomFlaskSolidMaker::RoundBottomFlaskSolidMaker(
        const std::string& name, double BallR, double TubeR, double TubeH,
        const HeightIndicator& indicator)
    : m_solid_name(name), m_BallR(BallR), m_TubeR(TubeR), m_TubeH(TubeH), 
      m_use_offset(false), m_offset(0)
{
    m_tmp_h = std::sqrt(m_BallR*m_BallR - m_TubeR*m_TubeR);

    if (indicator == TOPTOEQUATORH) {
        m_TubeH -= m_tmp_h;
    }
    
}

RoundBottomFlaskSolidMaker::RoundBottomFlaskSolidMaker(
        const std::string& name, double BallR, double TubeR, double TubeH, double offset)
    : m_solid_name(name), m_BallR(BallR), m_TubeR(TubeR), m_TubeH(TubeH), 
      m_use_offset(true), m_offset(offset)
{
    
}

G4VSolid* 
RoundBottomFlaskSolidMaker::getSolid()
{
    
    // = calculate the offset =
    if (not m_use_offset) {
        calculate();
    }
    // = create ball =
    G4Sphere* centerball_solid_sphere = new G4Sphere(
                                            m_solid_name+"_bottom_ball",
                                            0*mm, // R min
                                            m_BallR, // R max
                                            0*deg, // Start Phi
                                            360*deg, // Delta Phi
                                            0*deg, // Start Theta
                                            180*deg  // Delta Theta
                                            );
    // = create tube =
    G4Tubs* chimney_solid_tube = new G4Tubs(
                                        m_solid_name+"_top_tube",
                                        0*mm,      // pRMin,
                                        m_TubeR,   // pRMax,
                                        m_TubeH/2, // pDz,
                                        0*deg,     // pSPhi,
                                        360*deg    // pDPhi
                                        );
    // = union =
    G4UnionSolid* rb_solid = new G4UnionSolid(
                                    m_solid_name,
                                    centerball_solid_sphere,
                                    chimney_solid_tube,
                                    0,
                                    G4ThreeVector(0, 0, m_offset)
                                    );
    return rb_solid;
}

void 
RoundBottomFlaskSolidMaker::calculate()
{
    assert(not m_use_offset);

    m_offset = m_tmp_h + m_TubeH/2.;
}
