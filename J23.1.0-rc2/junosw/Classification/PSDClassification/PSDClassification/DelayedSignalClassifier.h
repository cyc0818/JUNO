/*
 * Description:
 *   classify the neutron-capture-like trigger events from the neutron sample
 *   based on interval time
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef DelayedSignalClassifier_h
#define DelayedSignalClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "Event/SimHeader.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdVertexRecEvt.h"
#include "EventRecordSvc/EventRecordSvc.h"


class DelayedSignalClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    DelayedSignalClassifier(const std::string& name);
    ~DelayedSignalClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);
	ClassificationType classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it);

private:
	JM::NavBuffer* m_buf;
	JM::EvtNavigator* m_nav;
	JM::SimHeader* m_sim_hdr;
	JM::CdVertexRecHeader* m_cd_vertex_rec_hdr;
	JM::CdVertexRecEvt* m_cd_vertex_rec_evt;

	EventRecordSvc*      m_event_record;
private:
	bool populate_data_model(JM::NavBuffer* buffer);
	bool is_delayed_signal(JM::EvtNavigator* nav, JM::EvtNavigator* pre_nav);


};

#endif
