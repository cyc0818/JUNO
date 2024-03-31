#pragma once

#ifdef WITH_G4CXOPTICKS

class G4VPhysicalVolume ; 
class G4VSensitiveDetector ;
struct PMTParamData ; 
struct PMTSimParamData ; 
struct NPFold ; 

struct LSExpDetectorConstruction_Opticks
{
    static void Setup(
          int opticksMode,
          const G4VPhysicalVolume* world, 
          const G4VSensitiveDetector* sd, 
          PMTParamData* ppd, 
          PMTSimParamData* psd,
          NPFold* pmtscan
          ); 


    static NPFold* SerializePMT( 
          PMTParamData* ppd, 
          PMTSimParamData* psd,
          NPFold* pmtscan 
     ); 


}; 

#endif


