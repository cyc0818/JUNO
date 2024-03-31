#include <boost/python.hpp>
#include "Dimensions.hh"
#include "UpperChimneyMaker.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include <cassert>
#include <cmath>
UpperChimneyMaker::~UpperChimneyMaker(){}
UpperChimneyMaker::UpperChimneyMaker(double TopToWater)
    :m_TopToWater(TopToWater)
{
     m_Tyvek_thickness  =Tyvek_thickness  ;
     m_Steel_thickness=Steel_thickness;
 

     m_TubeInnerR=TubeInnerR;
     m_TubeTyvekOuterR=m_TubeInnerR+m_Tyvek_thickness;
     m_TubeSteelOuterR=m_TubeTyvekOuterR+m_Steel_thickness;
}

G4VSolid* UpperChimneyMaker::getSolidALS(){
    
    G4Tubs* tube= new G4Tubs(
                                   "Upper_LS_tube",
                                   0*mm,      // pRMin,
                                   m_TubeInnerR,   // pRMax,
                                   m_TopToWater*m/2., // pDz,
                                   0*deg,     // pSPhi,
                                   360*deg    // pDPhi
                                   );
    return tube;
}

G4VSolid* UpperChimneyMaker::getSolidBTyvek(){

    G4VSolid* tube= new G4Tubs(
                                   "Upper_Tyvek_tube",
                                   m_TubeInnerR,   // pRMin,
                                   m_TubeTyvekOuterR,   // pRMax,
                                   m_TopToWater*m/2., // pDz,
                                   0*deg,     // pSPhi,
                                   360*deg    // pDPhi
                                   );
    return tube;
}


G4VSolid* UpperChimneyMaker::getSolidBSteel(){

    G4VSolid* tube= new G4Tubs(
                                   "Upper_Steel_tube",
                                   m_TubeTyvekOuterR,      // pRMin,
                                   m_TubeSteelOuterR,   // pRMax,
                                   m_TopToWater*m/2., // pDz,
                                   0*deg,     // pSPhi,
                                   360*deg    // pDPhi
                                   );
    

    
    return tube;
 }



G4VSolid* UpperChimneyMaker::getSolidUpperChimney(){
    
    G4Tubs* tube= new G4Tubs(
                             "Upper_Chimney",
                             0,   // pRMin,
                             m_TubeSteelOuterR+5.0*mm,   // pRMax,
                             m_TopToWater*m/2., // pDz,
                             0*deg,     // pSPhi,
                             360*deg    // pDPhi
                             );
    
    
    
    return tube;
}

