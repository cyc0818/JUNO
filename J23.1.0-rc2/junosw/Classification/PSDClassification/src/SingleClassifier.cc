#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/SingleClassifier.h"
#include "PSDClassification/RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"
#include "EventRecordSvc/EventRecordSvc.h"


DECLARE_TOOL(SingleClassifier);

SingleClassifier::SingleClassifier(const std::string& name)
    :
	ToolBase(name) 
{
}

SingleClassifier::~SingleClassifier() {

}

bool SingleClassifier::configure() {

	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
        LogError << "Can't get EventRecordSvc" << std::endl;
        return false;
    }
	m_event_record = recordsvc.data();

    return true;
}


ClassificationType SingleClassifier::classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it ){
	
	LogDebug<< "SingleClassifier::classify start"<<std::endl;   
 	ClassificationType classified_type = ClassificationType::kUnknown;
	m_buf = buffer;
	
	//check afterpulse
	
	if (m_event_record-> has_tag(it,ClassificationType::kAfterPulse))
	{  
		return classified_type;
	}
	
	if (!( m_event_record -> has_tag(it,ClassificationType::kInEnergyRange)))
	{
		return classified_type;
	}

	if (!( m_event_record -> has_tag(it,ClassificationType::kInFiducialVolume)))
	{
		return classified_type;
	}

	classified_type = ClassificationType::kSingle;
	m_event_record->add_tag(it, classified_type);

	return classified_type;
}

ClassificationType SingleClassifier::classify(JM::NavBuffer* buffer) {
	

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
