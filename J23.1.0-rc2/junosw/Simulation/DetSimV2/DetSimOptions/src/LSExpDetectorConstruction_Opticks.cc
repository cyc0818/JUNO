#ifdef WITH_G4CXOPTICKS

#include <vector>
#include <string>
#include "Geometry/PMTParamData.h"
#include "Geometry/_PMTParamData.h"
#include "PMTSimParamSvc/PMTSimParamData.h"
#include "PMTSimParamSvc/_PMTSimParamData.h"
#include "LSExpDetectorConstruction_Opticks.hh"

#include "G4CXOpticks.hh"
#include "SSim.hh"
#include "SEvt.hh"
#include "SLOG.hh"

void LSExpDetectorConstruction_Opticks::Setup(
          int opticksMode,
          const G4VPhysicalVolume* world, 
          const G4VSensitiveDetector* sd, 
          PMTParamData* ppd, 
          PMTSimParamData* psd,
          NPFold* pmtscan 
          )
{
    bool opticksMode_valid = opticksMode > -1 && opticksMode <= 3 ; 
    LOG_IF(fatal, !opticksMode_valid ) << " unexpected opticksMode " << opticksMode ; 
    assert( opticksMode_valid );  

    NPFold* jpmt = SerializePMT(ppd, psd, pmtscan) ; 

    LOG(info) << "[ WITH_G4CXOPTICKS opticksMode " << opticksMode << " sd " << sd  ; 
    if( opticksMode == 0 ) 
    { 
        SEvt::HighLevelCreateOrReuse();    // U4RecorderAnaMgr not active in opticksMode:0 
        SSim::Create();                    // done by G4CXOpticks::G4CXOpticks in opticksMode > 0
        SSim::AddSubfold("jpmt", jpmt );
    }
    else if( opticksMode == 1 || opticksMode == 3 || opticksMode == 2 )
    { 
        if(opticksMode == 2) G4CXOpticks::SetNoGPU() ; 
        G4CXOpticks::SetGeometry(world) ; 
        SSim::AddSubfold("jpmt", jpmt );  // needs to be before SaveGeometry 
        G4CXOpticks::SaveGeometry(); 
    } 
    LOG(info) << "] WITH_G4CXOPTICKS " ; 
}

NPFold* LSExpDetectorConstruction_Opticks::SerializePMT( 
          PMTParamData* ppd, 
          PMTSimParamData* psd,
          NPFold* pmtscan 
     )
{
    _PMTParamData    _ppd(*ppd) ; 
    _PMTSimParamData _psd(*psd) ; 

    NPFold* j = new NPFold ; 
    j->add_subfold( "PMTParamData",    _ppd.serialize() );
    j->add_subfold( "PMTSimParamData", _psd.serialize() );
    if(pmtscan) j->add_subfold( "PMTScan",  pmtscan );

    return j ; 
}

#endif
