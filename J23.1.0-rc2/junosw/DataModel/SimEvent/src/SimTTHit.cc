#include "Event/SimTTHit.h"

ClassImp(JM::SimTTHit)

namespace JM {

    SimTTHit::SimTTHit() {
        channelid = -1;
	edep = 0.0;
	time = 0.0;
	
	//-------------------------------------
	//To be removed in future
	//pe = 0.0;
	//ADC = 0.0;
        //x = 0.0;
        //y = 0.0;
        //z = 0.0;
	//------------------------------------- 
    }

    SimTTHit::~SimTTHit() {

    }
}
