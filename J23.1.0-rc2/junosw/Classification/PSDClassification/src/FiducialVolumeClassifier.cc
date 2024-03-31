#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"


#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/FiducialVolumeClassifier.h"
#include "PSDClassification/RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"
#include "EventRecordSvc/EventRecordSvc.h"


DECLARE_TOOL(FiducialVolumeClassifier);

FiducialVolumeClassifier::FiducialVolumeClassifier(const std::string& name)
    :
	ToolBase(name) 
{
	declProp("RadiusMax",  m_radius_max = 16000.); //mm
	declProp("RadiusMin",  m_radius_min = 0.); //mm
}

FiducialVolumeClassifier::~FiducialVolumeClassifier() {

}

bool FiducialVolumeClassifier::configure() {

	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
        LogError << "Can't get EventRecordSvc" << std::endl;
        return false;
    }
	m_event_record = recordsvc.data();

    return true;
}

bool FiducialVolumeClassifier::is_in_fiducial_volume(JM::EvtNavigator* nav ){
	
	std::vector< std::array<double,3>> xyzs =  RecEventHelper::GetRecXYZ(nav);
	auto xyz = xyzs[0];
	double r_square = xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2];
	double r = sqrt(r_square);

	if ( r > m_radius_max){
		return false;
	}

	if ( r < m_radius_min ){
		return false;
	}

	return true;
}

ClassificationType FiducialVolumeClassifier::classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it ){
	
	LogDebug<< "FiducialVolumeClassifier::classify start"<<std::endl;   
 	ClassificationType classified_type = ClassificationType::kUnknown;
	m_buf = buffer;
	
	//check afterpulse
	
	if (m_event_record-> has_tag(it,ClassificationType::kAfterPulse))
	{  
		return classified_type;
	}

	auto nav = (*it).get();
	if(not nav){
		LogError << "Cannot load event navigator. " << std::endl;
		throw SniperException("please check the nav!!");
	}


	bool in_fiducial_volume_flag = is_in_fiducial_volume(nav);
	if (in_fiducial_volume_flag){
		classified_type = ClassificationType::kInFiducialVolume;
		m_event_record->add_tag(it, classified_type);
		return classified_type;		
	}

	return classified_type;

}

ClassificationType FiducialVolumeClassifier::classify(JM::NavBuffer* buffer) {
	
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
