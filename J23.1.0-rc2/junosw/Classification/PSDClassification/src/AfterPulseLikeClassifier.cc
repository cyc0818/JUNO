#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/AfterPulseLikeClassifier.h"
#include "PSDClassification/RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"

DECLARE_TOOL(AfterPulseLikeClassifier);

AfterPulseLikeClassifier::AfterPulseLikeClassifier(const std::string& name)
    : ToolBase(name) {
	
	declProp("EnergyRatio",  m_energy_ratio = 0.05 );
	declProp("IntervalTime", m_interval_time = 1500);

}

AfterPulseLikeClassifier::~AfterPulseLikeClassifier() {

}

bool AfterPulseLikeClassifier::configure() {
	
	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
		LogError << "Can't get EventRecordSvc" << std::endl;
		return false;
	}
	m_event_record = recordsvc.data();
	return true;
}



ClassificationType AfterPulseLikeClassifier::classify(JM::NavBuffer* buf, JM::NavBuffer::Iterator it ) {

		
	ClassificationType classified_type = ClassificationType::kUnknown;

	if ( it == buf->begin()){
	    LogInfo<<" The current event is the begin event ! Can't check afterpulse in this buffer."<<std::endl;
	    return classified_type;
	}
	
	JM::NavBuffer::Iterator pre_it = it-1;
	
	auto nav = (*it).get();
	if(not nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the navigator!!");
	}
	
	auto pre_nav = (*pre_it).get();
	if(not pre_nav){
		LogError << "Cannot load event pre navigator. " << std::endl;
		throw SniperException("please check the pre navigator!!");
	}
	
	double energy = RecEventHelper::GetRecEnergy(nav);
	double pre_energy = RecEventHelper::GetRecEnergy(pre_nav);
	double post_vs_pre_ratio = energy/pre_energy;
	if (post_vs_pre_ratio > m_energy_ratio){
	    return classified_type;
	}
	
	double time = RecEventHelper::GetTriggerTime(nav);
	double pre_time = RecEventHelper::GetTriggerTime(pre_nav);
	double time_diff = time - pre_time;
	if (time_diff > m_interval_time){
	    return classified_type;
	}

	classified_type = ClassificationType::kAfterPulse;
	m_event_record->add_tag(it,classified_type);
   	
	return classified_type ;

}

ClassificationType AfterPulseLikeClassifier::classify(JM::NavBuffer* buffer) {
	LogDebug<< "AfterPulseLikeClassifier::classify start"<<std::endl;   
	ClassificationType classified_type_of_cur = ClassificationType::kUnknown; // type of current event

	for(auto it = buffer->begin() ; it != buffer->end() ; ++it )
    {
		ClassificationType type = classify(buffer,it); // tag event in current buffer
		if ( it == buffer->current() ){
			classified_type_of_cur = type; 
		}
	}

	return classified_type_of_cur ;
}
