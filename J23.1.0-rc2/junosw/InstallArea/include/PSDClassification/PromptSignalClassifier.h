/*
 * Description:
 *   Classify the prompt signal trigger events from the sample that one physical 
 *   event produce prompt-delayed correlated signal
 *   for example:
 *   	if you want to select the fast neutron signal from the neutron sample, you
 *   	can use this tool as the fast neutron siganl is prompt signal in neutron sample
 *
 * 	 The select alg is based on MC truth "m_event_type" in simheader. 
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef PromptSignalClassifier_h
#define PromptSignalClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "Event/SimHeader.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdVertexRecEvt.h"
#include "EventRecordSvc/EventRecordSvc.h"

//struct JM::RecEventHelper{};

class PromptSignalClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    PromptSignalClassifier(const std::string& name);
    ~PromptSignalClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buffer);
private:
    ClassificationType classify(JM::NavBuffer* buffer, JM::NavBuffer::Iterator it);

private:
	JM::NavBuffer* m_buf;
	JM::EvtNavigator* m_nav;
	JM::SimHeader* m_sim_hdr;
	JM::CdVertexRecHeader* m_cd_vertex_rec_hdr;
	JM::CdVertexRecEvt* m_cd_vertex_rec_evt;
	
	EventRecordSvc* m_event_record;
private:
	bool populate_data_model(JM::NavBuffer* buffer);
	bool is_prompt_signal(JM::EvtNavigator* nav , JM::EvtNavigator*  post_nav);


};

#endif
