#ifndef PSDPreSelectAlg_hh
#define PSDPreSelectAlg_hh

#include <SniperKernel/AlgBase.h>

#include <EvtNavigator/NavBuffer.h>
#include <Event/SimHeader.h>


#include <boost/python/numpy.hpp>
#include "SniperKernel/ToolBase.h"

#include "ClassificationTool/IClassificationTool.h"
#include "EventRecordSvc/EventRecordSvc.h"

class PSDPreSelectAlg: public AlgBase {
public:
    PSDPreSelectAlg(const std::string& name);
    
    bool initialize();
    bool execute();
    bool finalize();

private:
    JM::NavBuffer* 	m_buf;
	int 			m_max_event_num;
	//ToolBase*       m_ncapture_tool;
	std::vector<IClassificationTool*> m_classifiers;
	std::vector<std::string> m_classifierNames;
	EventRecordSvc * m_event_record;
	

	//Task* m_psdtask;

private:
	bool init_psd_tag_cache();
	bool test_nav_in_current_buffer();
	bool test_rec_event_helper();
	bool init_classifiers();
	bool test_shared_data();
	bool init_record_svc();
};

#endif
