#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"


#include "Event/TtElecHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/DelayedSignalClassifier.h"
//#include "DelayedSignalClassifier.h"
#include "PSDClassification/RecEventHelper.h"
//#include "RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"

DECLARE_TOOL(DelayedSignalClassifier);

DelayedSignalClassifier::DelayedSignalClassifier(const std::string& name)
    : 
	ToolBase(name)
{
}

DelayedSignalClassifier::~DelayedSignalClassifier() {

}

bool DelayedSignalClassifier::configure() {
	
		
	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
        LogError << "Can't get EventRecordSvc" << std::endl;
        return false;
    }
	m_event_record = recordsvc.data();

    return true;

}

bool DelayedSignalClassifier::is_delayed_signal(JM::EvtNavigator* nav , JM::EvtNavigator* pre_nav ){
	
	std::string evt_type = (RecEventHelper::GetEventTypes(nav))[0];
	std::string pre_evt_type = (RecEventHelper::GetEventTypes(pre_nav))[0];
	if (evt_type != pre_evt_type){
		return false;
	}
	
	int detsim_id = (RecEventHelper::GetDetSimEventIDs(nav))[0];
	int pre_detsim_id = (RecEventHelper::GetDetSimEventIDs(pre_nav))[0];
	
	if (detsim_id != pre_detsim_id ){
		return false;
	}

	return true;
}

ClassificationType DelayedSignalClassifier::classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it){


    ClassificationType classified_type = ClassificationType::kUnknown;
	
	if (m_event_record-> has_tag(it,ClassificationType::kAfterPulse))
	{  
		return classified_type;
	}
	

	//find the first pre event which is not afterpulse.
	//JM::NavBuffer::Iterator it = m_buf->current();                 
	m_buf = buffer;

	if ( it == m_buf->begin()){
		LogInfo<<" the current event is begin event !"<<std::endl;
		return classified_type;
	}

    JM::NavBuffer::Iterator pre_it = it;                          
    while(true){
        pre_it--;
		if (not m_event_record->has_tag(pre_it,ClassificationType::kAfterPulse)){
			break;
		}
	
		
        if( pre_it == m_buf->begin()){                              
            return classified_type;
        }                                             
    }


	auto nav = (*it).get();
	if(not nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the nav");
	}

	auto pre_nav = (*pre_it).get();
	if(not pre_nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the nav");
	}

	bool delayed_flag = is_delayed_signal(nav,pre_nav);
	if (delayed_flag){
		classified_type = ClassificationType::kDelayedSignal;
		//record the current.
		m_event_record->add_tag(it, classified_type);
		return classified_type;		
	}

	return classified_type;

}

ClassificationType DelayedSignalClassifier::classify(JM::NavBuffer* buffer) {
	

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
