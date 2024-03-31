#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "PSDClassification/EnergyRangeClassifier.h"
#include "PSDClassification/RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"
#include "EventRecordSvc/EventRecordSvc.h"


DECLARE_TOOL(EnergyRangeClassifier);

EnergyRangeClassifier::EnergyRangeClassifier(const std::string& name)
    :
	ToolBase(name) 
{
	declProp("EnergyRangeMax",  m_energy_range_max = 10 );
	declProp("EnergyRangeMin",  m_energy_range_min = 0.7);
}

EnergyRangeClassifier::~EnergyRangeClassifier() {

}

bool EnergyRangeClassifier::configure() {

	LogInfo << "m_energy_range_max = "<< m_energy_range_max
			<< "m_energy_range_min = "<< m_energy_range_min
			<< std::endl;

	SniperPtr<EventRecordSvc> recordsvc(*getParent(), "EventRecordSvc");
	if(recordsvc.invalid()){
        LogError << "Can't get EventRecordSvc" << std::endl;
       	return false;
    }
	m_event_record = recordsvc.data();

    return true;
}

bool EnergyRangeClassifier::is_in_energy_range(JM::EvtNavigator* nav ){
	
	double energy = RecEventHelper::GetRecEnergy(nav); //MeV
	if (energy > m_energy_range_max){
		return false;
	}

	if (energy < m_energy_range_min ){
		return false;
	}

	return true;
}

ClassificationType EnergyRangeClassifier::classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it ){
	
	LogDebug<< "EnergyRangeClassifier::classify start"<<std::endl;   
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
		return classified_type;
	}


	bool in_energy_flag = is_in_energy_range(nav);
	if (in_energy_flag){
		classified_type = ClassificationType::kInEnergyRange;
		m_event_record->add_tag(it, classified_type);
		return classified_type;		
	}

	return classified_type;

}

ClassificationType EnergyRangeClassifier::classify(JM::NavBuffer* buffer) {
	
	
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
