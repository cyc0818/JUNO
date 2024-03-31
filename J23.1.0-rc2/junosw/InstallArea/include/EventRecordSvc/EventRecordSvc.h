#ifndef EventRecordSvc_h
#define EventRecordSvc_h

#include "SniperKernel/SvcBase.h"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/NavBuffer.h"
#include "ClassificationTool/ClassificationType.h"
#include <vector>
#include <map>

class EventRecordSvc: public SvcBase
{
public:
    EventRecordSvc(const std::string& name);
    ~EventRecordSvc();

    bool initialize();
    bool finalize();
	
	void add_tag(JM::NavBuffer::Iterator it, ClassificationType type);
	void add_tag(JM::NavBuffer* buf , ClassificationType type);

	bool has_tag(JM::NavBuffer* buf , ClassificationType type);
	bool has_tag(JM::NavBuffer::Iterator it, ClassificationType type);
	void clear_tags();

	bool update_buffer_record();
	void desc();

private:
	std::map<JM::EvtNavigator* , std::vector<ClassificationType>> m_nav2tags;

	std::deque<std::pair<JM::EvtNavigator*, std::vector<ClassificationType> > > m_nav2tags_deq;

	std::vector<ClassificationType>& get_types(JM::NavBuffer::Iterator it);
	void initialize_buffer_record(JM::NavBuffer* buf);

	int m_pre_event_num;
	int m_post_event_num;
	JM::NavBuffer* m_buf;

};

#endif
