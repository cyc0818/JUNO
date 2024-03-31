//#include "PSDClassification/RecEventHelper.h"
//#include "RecEventHelper.h"
#pragma once

#include "TTimeStamp.h"

#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/NavBuffer.h"



struct RecEventHelper{


	static double GetTriggerTime(JM::EvtNavigator* nav);
	
	static double GetRecEnergy(JM::EvtNavigator* nav);
	
	static std::vector< std::array<double,3> > GetRecXYZ(JM::EvtNavigator* nav);
	static std::vector<std::string> SplitString(std::string str, std::string delimiter);
	
	static std::vector<std::string> GetSplitMixedEventsType(JM::EvtNavigator* nav);

	//static std::vector<std::string> GetSplitEventType(JM::EvtNavigator* nav);
	static std::vector<int> GetDetSimEventIDs(JM::EvtNavigator* nav);
	
	static std::vector<std::string> GetEventTypes(JM::EvtNavigator* nav);
	

 };

