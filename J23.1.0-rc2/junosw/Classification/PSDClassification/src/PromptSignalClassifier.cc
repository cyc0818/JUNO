#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"


//#include "Event/TtElecHeader.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/PromptSignalClassifier.h"
//#include "PromptSignalClassifier.h"
#include "PSDClassification/RecEventHelper.h"
//#include "RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"

#include "EventRecordSvc/EventRecordSvc.h"


DECLARE_TOOL(PromptSignalClassifier);

PromptSignalClassifier::PromptSignalClassifier(const std::string& name)
    :
	ToolBase(name) 
{
}

PromptSignalClassifier::~PromptSignalClassifier() {

}

bool PromptSignalClassifier::configure() {

	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
		LogError << "Can't get EventRecordSvc" << std::endl;
		return false;
    }
	m_event_record = recordsvc.data();

    return true;
}


bool PromptSignalClassifier::populate_data_model(JM::NavBuffer* buffer ){
	//check buffer
	if (not buffer){
		LogError << "event buffer is empty. " << std::endl;
		return false;
	}
	m_buf = buffer;

	m_nav = buffer->curEvt();
	if( not m_nav){
		LogError << "Cannot load event navigator. " << std::endl;
		return false;
	}
	
	m_sim_hdr = JM::getHeaderObject<JM::SimHeader>(m_nav);
	if( not m_sim_hdr){
		LogError << "Cannot load simheader. " << std::endl;
		return false;
	}

	m_cd_vertex_rec_hdr = JM::getHeaderObject<JM::CdVertexRecHeader>(m_nav);
	if( not m_cd_vertex_rec_hdr){
		LogError << "Cannot load cd vertex header. " << std::endl;
		return false;
	}

	m_cd_vertex_rec_evt = dynamic_cast<JM::CdVertexRecEvt*>(m_cd_vertex_rec_hdr->event());
	if (not m_cd_vertex_rec_evt){
		LogError << "Cannot load cd vertex evt. " << std::endl;
		return false;
	}

	return true;

}

bool PromptSignalClassifier::is_prompt_signal(JM::EvtNavigator* nav , JM::EvtNavigator* post_nav ){

	std::string evt_type = (RecEventHelper::GetEventTypes(nav))[0];
	std::string post_evt_type = (RecEventHelper::GetEventTypes(post_nav))[0];
	if (evt_type != post_evt_type){
		return false;
	}
	
	int detsim_id = (RecEventHelper::GetDetSimEventIDs(nav))[0];
	int post_detsim_id = (RecEventHelper::GetDetSimEventIDs(post_nav))[0];
	
	if (detsim_id != post_detsim_id ){
		return false;
	}

	return true;
}

ClassificationType PromptSignalClassifier::classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it ){
	
	LogDebug<< "PromptSignalClassifier::classify start"<<std::endl;   
 	ClassificationType classified_type = ClassificationType::kUnknown;
	m_buf = buffer;
	
	//check afterpulse
	
	if (m_event_record-> has_tag(it,ClassificationType::kAfterPulse))
	{  
		return classified_type;
	}

	if (! (m_event_record-> has_tag(it,ClassificationType::kInEnergyRange))){
		return classified_type;
	}

	if (! (m_event_record-> has_tag(it,ClassificationType::kInFiducialVolume))){
		return classified_type;
	}


	// find the next event which is not afterpulse
	JM::NavBuffer::Iterator post_it = it;
	while(true){
		post_it++;
		if( post_it == m_buf->end()){
			LogDebug<<"post event is end !!"<<std::endl;
			return classified_type;
		}

		if (m_event_record-> has_tag(post_it,ClassificationType::kAfterPulse)){
		
			LogDebug<<" this post event is after pulse" << std::endl;
			continue;
		}


		break;
	}
	

	auto nav = (*it).get();
	if(not nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the nav!!");
	}

	auto post_nav = (*post_it).get();
	if(not post_nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the nav!!");
	}

	bool prompt_flag = is_prompt_signal(nav,post_nav);
	if (prompt_flag){
		classified_type = ClassificationType::kPromptSignal;
		m_event_record->add_tag(it, classified_type);
		return classified_type;		
	}

	return classified_type;

}

ClassificationType PromptSignalClassifier::classify(JM::NavBuffer* buffer) {
	
	
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
