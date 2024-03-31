#include "EventRecordSvc/EventRecordSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/Task.h"

DECLARE_SERVICE(EventRecordSvc);

EventRecordSvc::EventRecordSvc(const std::string& name)
    : 
	SvcBase(name),
	m_buf(nullptr),
	m_pre_event_num(0),
	m_post_event_num(0)
{
    //declProp("Seed", m_init_seed);
}

EventRecordSvc::~EventRecordSvc()
{

}

void EventRecordSvc::initialize_buffer_record(JM::NavBuffer* buf){

	//auto it = buf->begin();
	std::vector<ClassificationType> types;
	types.push_back(ClassificationType::kUnknown);

	for(auto it = buf->begin(); it!=buf->end() ; it++){
		auto nav2type = std::make_pair((*it).get(), types);
		m_nav2tags_deq.push_back(nav2type);
	}
	assert(buf->size() == m_nav2tags_deq.size());
	
	m_pre_event_num = buf->current() - buf->begin();
	m_post_event_num = buf->end()- buf->current() - 1;
	assert((m_pre_event_num + m_post_event_num + 1) == buf->size());

	m_buf = buf;	

}

bool EventRecordSvc::update_buffer_record(){

	SniperDataPtr<JM::NavBuffer>  navBuf(*getRoot(), "/Event");
	if(navBuf.invalid()){
		LogError << "Can't find nav buffer" << std::endl;
		assert(0);
	}
	
	JM::NavBuffer* buf = navBuf.data();

	if(!m_nav2tags_deq.size()){
		LogInfo<<" Initialize the buffer Record at first"<<std::endl;
		initialize_buffer_record(buf);
		return true;
	}

	// pop the trigger event not in current buffer.
	int pre_event_num = buf->current() - buf->begin();
	int post_event_num = buf->end()- buf->current() - 1;
	assert((pre_event_num + post_event_num + 1) == buf->size());

	int pop_front_num = m_pre_event_num + 1 - pre_event_num;
	

	for(int i = 0 ; i < pop_front_num ; i++){
		m_nav2tags_deq.pop_front();
	}

	//load the new trigger event.
	int push_back_offset = m_post_event_num ;

	std::vector<ClassificationType> types;
	types.push_back(ClassificationType::kUnknown);
	for(auto it = (buf->current() + push_back_offset) ; it != buf->end(); it++){
		auto nav2type = std::make_pair((*it).get(), types);
		m_nav2tags_deq.push_back(nav2type);
	}

	//update the post and pre event number and buffer.
	m_pre_event_num = pre_event_num;
	m_post_event_num = post_event_num;
	m_buf = buf	;
	return true;

}

void EventRecordSvc::add_tag(JM::NavBuffer* buf , ClassificationType type){

	JM::NavBuffer::Iterator it = buf->current();
	add_tag(it , type);
}


void EventRecordSvc::add_tag(JM::NavBuffer::Iterator it , ClassificationType type){

	
	std::vector<ClassificationType>& types = get_types(it);
	//check if this type is already in types.
	if(has_tag(it,type)){
		LogInfo << " type : " << type << " is already in the types"<< std::endl;
		return;
	}

	//check if this is the firstly add one type.
	if((types.size()==1) and (types[0] == ClassificationType::kUnknown)){
		LogInfo << "Firstly add type to event!" << std::endl;
		types.pop_back();
	}

	int size_before = types.size();
	types.push_back(type);

	
	int size_after = get_types(it).size();
	assert(size_before == (size_after-1));

}

std::vector<ClassificationType>& EventRecordSvc::get_types(JM::NavBuffer::Iterator it){

	if(it == m_buf->end()){
		LogError<< " Iterator is invalid "<< std::endl;
		assert(0);
	}

	int offset = it - m_buf->begin();
	assert( (offset>=0 ) && (offset < m_buf->size()));

	std::pair<JM::EvtNavigator*, std::vector<ClassificationType>> & a_pair = m_nav2tags_deq[offset];
	return a_pair.second;
}

bool EventRecordSvc::has_tag(JM::NavBuffer* buf , ClassificationType type){

	JM::NavBuffer::Iterator it = buf->current();
	return has_tag( it ,type);
	
}

bool EventRecordSvc::has_tag(JM::NavBuffer::Iterator it, ClassificationType type){

	std::vector<ClassificationType>& types = get_types(it);
	auto result = std::find(types.begin(),types.end(),type);
	return result == types.end() ? false : true ; 
}

void EventRecordSvc::desc(){

	std::cout<<" m_pre_event_num = " << m_pre_event_num
			 <<" m_post_event_num = " << m_post_event_num
			 <<" buffer size = " << m_buf->size()
			 <<std::endl;

	std::cout<<" ======= the follow is tag info ========= "<< std::endl;
	for(int idx = 0 ; idx < m_nav2tags_deq.size() ; idx++ ){
		std::cout<<" idx = " << idx;
		auto nav = m_nav2tags_deq[idx].first;
		std::cout<<" Navigator = " << nav ;
		auto types = m_nav2tags_deq[idx].second;
	
		for(int jdx = 0 ; jdx < types.size(); jdx++){
			std::cout<< " "<< types[jdx] <<" ";
		}
		std::cout << std::endl;	
	}


}

/*
bool EventRecordSvc::has_tag(JM::EvtNavigator* nav, ClassificationType type){

	auto map_it = m_nav2tags.find(nav);
	if( map_it == m_nav2tags.end()){
		return false;
	}
	
	std::vector<ClassificationType> tags = map_it->second;

	auto v_it = std::find(tags.begin(), tags.end(), type);
	if ( v_it == tags.end()){
		return false;
	}
	
	return true;
} 
*/
bool
EventRecordSvc::initialize()
{

	clear_tags();	

	
    return true;
}

void 
EventRecordSvc::clear_tags(){
	
	m_nav2tags.clear();
	m_nav2tags_deq.clear();
	
}
bool
EventRecordSvc::finalize()
{
	clear_tags();
    return true;
}


