#include "PSDClassification/RecEventHelper.h"

#include "TTimeStamp.h"

#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "EvtNavigator/EvtNavigator.h"
#include "Event/CdVertexRecHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"


#include "Event/CdVertexRecHeader.h"
#include "Event/SimHeader.h"
#include "Event/CdVertexRecEvt.h"

double RecEventHelper::GetTriggerTime(JM::EvtNavigator* nav){

	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}

	double trigger_time = -1.0;
	trigger_time= (nav->TimeStamp()).GetSec()*1.0e9 + (nav->TimeStamp()).GetNanoSec();
	//assert(trigger_time > 0);
	if ( not (trigger_time > 0)){
		LogError<< "Error: trigger_time is less than 0"<<std::endl;
		throw SniperException("Make sure you have right trigger time ");
		
	}
	return trigger_time;

}

double RecEventHelper::GetRecEnergy(JM::EvtNavigator* nav){

	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}


	double energy = -1.0;

	auto CVRechdr = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
	if(not CVRechdr){
		LogError<<"Failed to find the CdVertexHeader "<<std::endl;
		throw SniperException("please check the CdVertexRecHeader!!");
	}
	
	auto recevt = dynamic_cast<JM::CdVertexRecEvt*>(CVRechdr->event());
	if (not recevt){
		LogError<<"Failed to find RecEvent"<<std::endl;
		throw SniperException("please check the CdVertexRecEvt !!");
	}

	auto vertex = recevt->getVertex(0);
	if (not vertex){
		LogError<<"Failed to find vertex"<<std::endl;
		throw SniperException("please check the vertex !!");
	}

	// The follow energy scale will be removed after reconstruction software set the default scale factor 
	double energy_scale = 0.91539;
	energy = vertex->energy()*energy_scale;
	assert(energy > 0);
	
	return energy;

}
	
	/*
 *	  x y z , default unit is mm.
 *  */
std::vector<std::array<double,3>> RecEventHelper::GetRecXYZ(JM::EvtNavigator* nav){
	
	
	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}

	auto CVRechdr = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
	if(not CVRechdr){
		LogError<<"Failed to find the CdVertexRecHeader "<<std::endl;
		throw SniperException("please check the CdVertexRecHeader!!");
	}
	
	auto recevt = dynamic_cast<JM::CdVertexRecEvt*>(CVRechdr->event());
	if (not recevt){
		LogError<<"Failed to find RecEvent"<<std::endl;
		throw SniperException("please check the CdVertexRecEvt !!");
	}

	int num_vertex = recevt-> nVertices();
	
	if(not (num_vertex > 0)){
		LogError <<" num_vertex > 0 is failed !!"<< std::endl;
		throw SniperException("please check the num_vertex !!");
	}

	//assert(num_vertex > 0);
	//loop each vertex 
	std::vector<std::array<double,3 >>	vertexs_xyz;	
	for(int v_idx = 0 ; v_idx < num_vertex ; v_idx++){
		auto vertex = recevt->getVertex(v_idx);
		if (not vertex){
			LogError<<"Failed to find vertex "<< v_idx 
					<< std::endl;
			throw SniperException("please check the vertex !!");
		}
		std::array<double, 3 > xyz = { vertex->x(),vertex->y(),vertex->z() };
		vertexs_xyz.push_back(xyz);
		
	}
	
	assert(vertexs_xyz.size() == num_vertex);
	return vertexs_xyz ;
}


std::vector<std::string> RecEventHelper::SplitString(std::string str, std::string delimiter){

	std::vector<std::string>  result;
	//size_t delimiter_length = delimiter.length();
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		result.push_back(str.substr(0, pos));
		
		str.erase(0, pos + delimiter.length());
	}

	if( not (result.size() > 0)){
		LogError << "result.size > 0 failed !!"<<std::endl;
		throw SniperException("please check the result.size !!");
	}

	//assert(result.size() > 0 );
	return result;

}
/*
 *   m_event_type
 *   format:
 *   	 Label:entry:ntrks:nhits
 *   example:
 *		 radio:35167:1:852;radio:35168:1:1
 *   so we need split the above string to obtain info.
 *
 *
 */ 


std::vector<std::string> RecEventHelper::GetSplitMixedEventsType(JM::EvtNavigator* nav){

	
	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}

	auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
	if (not simhdr) {
		LogError<<"Failed to find SimHeader."<<std::endl;
	}
	std::string event_type =  simhdr ->getEventType();
	std::vector<std::string> split_mix_event_type = SplitString(event_type, ";");
	
	for(int idx = 0; idx < split_mix_event_type.size(); idx++){
		LogDebug<<" "<<split_mix_event_type[idx];
	}
	return split_mix_event_type;

}


std::vector<int> RecEventHelper::GetDetSimEventIDs(JM::EvtNavigator* nav){
	
	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}

	std::vector<int> detsim_event_ids;
	std::vector<std::string> split_mix_event_type = GetSplitMixedEventsType(nav);
	for(int idx = 0 ; idx < split_mix_event_type.size(); idx++){
		std::vector<std::string> a_event_type = SplitString( split_mix_event_type[idx],":" );
		int evtid = std::atoi(a_event_type[1].c_str());
		detsim_event_ids.push_back(evtid); 
	}

	if(not (detsim_event_ids.size() == split_mix_event_type.size())){
		LogError << "detsim_event_ids.size() == split_mix_event_type.size() failed"<<std::endl;
		throw SniperException("please check detsim_event_ids!!");	
	}

	//assert(detsim_event_ids.size() == split_mix_event_type.size());
	return detsim_event_ids; 
}



std::vector<std::string> RecEventHelper::GetEventTypes(JM::EvtNavigator* nav){

	
	if (not nav){
		LogError << "nav is a null ptr"<<std::endl;
		throw SniperException("please check the nav ptr");
	}

	std::vector<std::string> event_types;
	std::vector<std::string> split_mix_event_type = GetSplitMixedEventsType(nav);
	for(int idx = 0 ; idx < split_mix_event_type.size(); idx++ )
	{
		std::vector<std::string> a_event_type = SplitString(split_mix_event_type[idx], ":");
		event_types.push_back(a_event_type[0]);			
	}

	if(not (event_types.size() == split_mix_event_type.size())){
		LogError << "event_types.size() == split_mix_event_type.size()"<<std::endl;
		throw SniperException("please check the event_type!!");
	}

	//assert(event_types.size() == split_mix_event_type.size());
	return event_types; 
}




