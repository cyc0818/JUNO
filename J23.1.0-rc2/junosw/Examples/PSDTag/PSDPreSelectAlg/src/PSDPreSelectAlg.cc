#include "PSDPreSelectAlg.hh"

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperException.h"

#include "SniperPython/PyDataStore.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdVertexRecHeader.h"
#include "PSDClassification/RecEventHelper.h"
#include "ClassificationTool/ClassificationType.h"

namespace p = boost::python;
namespace np = boost::python::numpy;


DECLARE_ALGORITHM(PSDPreSelectAlg);

PSDPreSelectAlg::PSDPreSelectAlg(const std::string& name)
    : AlgBase(name) {

	declProp("ClassifierList", m_classifierNames);
	m_max_event_num = 1000000;
}

bool PSDPreSelectAlg::initialize() {

    SniperDataPtr<JM::NavBuffer>  navBuf(*getRoot(), "/Event");
	if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

	//np::initialize();
	//init_psd_tag_cache();
	if( not (init_classifiers() ) ){
		return false;
	}

	if( not (init_record_svc())){
		return false;
	}
	
	return true;
}
bool PSDPreSelectAlg::init_record_svc(){
	SniperPtr<EventRecordSvc> recordsvc(*getParent(),"EventRecordSvc");
	if(recordsvc.invalid()){
		LogError << " Can't find EventRecordSvc "<<std::endl;
		return false;	
	}
	m_event_record = recordsvc.data();
	return true;
	
}

bool PSDPreSelectAlg::init_classifiers(){

	if(!m_classifierNames.size()){
		LogError << " classifiers tool is empty !! need to register at least one tool" <<std::endl;
		return false;
	}

	for (const auto& name: m_classifierNames) {
		IClassificationTool* ct = tool<IClassificationTool>(name);
        if (not ct) {
            LogError << "Failed to find the ClassifierTool: " << name << std::endl;
            return false;
        }
       //initialize eash classifier
       	ct->configure();
   		m_classifiers.push_back(ct);
    }
	return true;
}

bool PSDPreSelectAlg::init_psd_tag_cache(){

    p::tuple shape = p::make_tuple(m_max_event_num);
    np::dtype dtype_int = np::dtype::get_builtin<int>();
	np::ndarray psd_tag_cache  = np::zeros(shape, dtype_int);


    // register them 
    SniperDataPtr<PyDataStore> pystore(*getRoot(), "DataStore");
    LogInfo << "Register the value to PyDataStore. " << std::endl;
    pystore->set("psd_tag", psd_tag_cache);
	return true;
}

bool PSDPreSelectAlg::test_rec_event_helper(){

	auto nav = m_buf->curEvt();
	if (not nav){
		LogError << "Failed to find the navigator. " << std::endl;
		return false;
	}

	LogInfo <<" trigger Event ID = " << nav->EventID()<<std::endl;
	double energy = RecEventHelper::GetRecEnergy(nav);
	double trigger_time = RecEventHelper::GetTriggerTime(nav);

	LogDebug<<" energy = "<< energy << " [MeV] "
		   	<<" trigger_time = " << trigger_time << " [ns]"
			<< std::endl;

	std::vector<std::array<double,3> > xyzs = RecEventHelper::GetRecXYZ(nav);
	std::vector<int> sim_event_ids = RecEventHelper::GetDetSimEventIDs(nav);
	std::vector<std::string> event_types =  RecEventHelper::GetEventTypes(nav);


	std::cout<<" xyzs.size() = "<<xyzs.size()
			 <<" sim_event_ids size = "<< sim_event_ids.size()
			 <<" event_types size = "<< event_types.size()
			 << std::endl;

	for(int idx = 0 ; idx < sim_event_ids.size();idx++){
		std::cout<<" Sim event ID = "<<sim_event_ids[idx]
				 <<" event_type = "<< event_types[idx]
				 << std::endl;
		
	}


	for(int idx = 0 ; idx < xyzs.size() ; idx++){
	   std::cout<<" x = " << xyzs[idx][0] << " [mm] "
				<<" y = " << xyzs[idx][1] << " [mm] "
				<<" z = " << xyzs[idx][2] << " [mm] "
				<< std::endl;
	}

	assert(xyzs.size() <= sim_event_ids.size());
	assert(xyzs.size() <= event_types.size());
	assert(sim_event_ids.size() == event_types.size());


	return true;
	
}


bool PSDPreSelectAlg::test_nav_in_current_buffer(){
	
	//JM::NavBuffer::Iterator it = nullptr;
	for(JM::NavBuffer::Iterator it = m_buf->begin() ; it != m_buf->end(); ++it){
		auto nav = (*it).get();
		if (not nav){
			LogError << "Failed to find the navigator. " << std::endl;
			return false;
		}
		else{
			LogInfo << " Navigator event id = " << nav->EventID()
					<< std::endl;
		}

    	auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
    	if (not simhdr) {
    	    LogError << "Failed to find the SimHeader. " << std::endl;
    	    return false;
    	}
    	
    	auto simevt = static_cast<JM::SimEvt*>(simhdr->event());
    	if (not simevt) {
    	    LogError << "Failed to find the SimEvent. " << std::endl;
    	    return false;
    	}
    	
   		LogInfo << "The SimEvent ID: " << simevt->getEventID() << std::endl;
		
		// Rec
		auto CVRechdr = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
		if (not CVRechdr){
			LogError << "Failed to find the SimHeader. " << std::endl;
			return false;
		}
		
		auto recevt = dynamic_cast<JM::CdVertexRecEvt*>(CVRechdr->event());
		if (not recevt) {
			LogError << "Failed to find the rec event. " << std::endl;
			return false;
		}
		LogInfo << "The Reconstruct Event Vertices = " << recevt -> nVertices() 
				<< std::endl;

	}

	LogInfo << "current buf size " << m_buf->size()
			<< std::endl;
	return true;

}

 
bool PSDPreSelectAlg::test_shared_data(){
    // = prepare =
    auto nav = m_buf->curEvt();
    if (not nav) {
        LogError << "Failed to find the event navigator. " << std::endl;
        return false;
    }
	LogInfo << " current Navigator event id = " << nav->EventID()
			<< std::endl;


    auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
    if (not simhdr) {
        LogError << "Failed to find the SimHeader. " << std::endl;
        return false;
    }

    auto simevt = static_cast<JM::SimEvt*>(simhdr->event());
    if (not simevt) {
        LogError << "Failed to find the SimEvent. " << std::endl;
        return false;
    }

   	LogInfo << "The SimEvent ID: " << simevt->getEventID() << std::endl;

	//
    p::tuple shape = p::make_tuple(m_max_event_num);
    np::dtype dtype_int = np::dtype::get_builtin<int>();
	np::ndarray psd_tag_cache  = np::zeros(shape, dtype_int);


    // get them 
	SniperDataPtr<PyDataStore> pystore(*getRoot(), "DataStore");
    LogInfo << "Get the value from PyDataStore. " << std::endl;
    pystore->get("psd_tag", psd_tag_cache);

	int evt_id = simevt->getEventID();
	assert(evt_id <= m_max_event_num);

	 
 	//const char * tag_list = p::extract<const char *>(p::str(psd_tag_cache));

	char* tag_data = psd_tag_cache.get_data();
	int tag = tag_data[evt_id];
	LogInfo << "tag = " << tag
			<< std::endl;


	assert(tag == 0 || tag == 1 );
	return true;

}


bool PSDPreSelectAlg::execute() {

	
	std::cout<<" Begin of Trigger Event -->"<<std::endl;

	m_event_record->update_buffer_record();

	//update m_buf
    SniperDataPtr<JM::NavBuffer>  navBuf(*getRoot(), "/Event");
	if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();


	// look at all nav in this current buffer
	//test_nav_in_current_buffer();
	test_rec_event_helper();

	//assert(m_classifierNames.size() == m_classifiers.size());
	if (not (m_classifierNames.size() == m_classifiers.size())){
		LogError << "m_classifierNames.size is not same as m_classifiers.size()"<<std::endl;
		throw SniperException("Make sure you have load the classifier.");
	}

	// just use the tools to tag the trigger event.
	//
	ClassificationType type = ClassificationType::kUnknown;

	for(int idx = 0 ; idx < m_classifiers.size(); idx++ )
	{
		auto classifier = m_classifiers[idx];
		type = classifier->classify(m_buf);
	}
	
	if(type != ClassificationType::kUnknown){
		LogInfo << "Classify event to " << type 
				<< " trigger psdtask !! "<< std::endl;
		Incident::fire(*getParent(), "psdtask");
	}
	
	m_event_record->desc();
	return true;
}

bool PSDPreSelectAlg::finalize() {
    return true;
}
