#pragma once

#include "G4Polycone.hh"

namespace SimUtil
{
    /**
    SimUtil::MakeTubsWithPolycone
    -------------------------------
 
    Using G4Polycone to model a cylinder instead of G4Tubs has the 
    advantage that the cylinder can be z-offset directly in the 
    definition of the G4Polycone. This is very useful when a transform 
    needs to be applied to the other shape in CSG combinations. 

    **/
  
    inline G4VSolid* MakeTubsWithPolycone( 
                       const G4String& pName, 
                       G4double pRMin,
                       G4double pRMax,
                       G4double pDz,
                       G4double pSPhi,
                       G4double pDPhi, 
                       G4double zOffset=0. 
                      )   
    {   
        G4int numZPlanes = 2 ; 
        G4double zPlane[] = { zOffset - pDz, zOffset + pDz } ; 
        G4double rInner[] = { pRMin, pRMin } ; 
        G4double rOuter[] = { pRMax, pRMax } ; 

        return new G4Polycone(
                pName,
                pSPhi,  
                pDPhi,  
                numZPlanes,
                zPlane, 
                rInner,
                rOuter
                );  
    }   

}

