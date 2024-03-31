/*
 * Description:
 *   classify the after-pulse-like trigger events from the low energy sample
 *   based on interval time
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef AfterPulseLikeClassifier_h
#define AfterPulseLikeClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "EventRecordSvc/EventRecordSvc.h"
//#include "Event/CdVertexRecHeader.h"
//#include "Event/CdVertexRecEvt.h"


class AfterPulseLikeClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    AfterPulseLikeClassifier(const std::string& name);
    ~AfterPulseLikeClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buf);
private:
    ClassificationType classify(JM::NavBuffer* buf,JM::NavBuffer::Iterator it);

private:
	EventRecordSvc* m_event_record;

	double m_energy_ratio;
	double m_interval_time;
	//IClassificationTool* m_afterpulse_tool;
//private:
	//bool populate_data_model(JM::NavBuffer* buffer);
	//bool is_fast_neutron(JM::EvtNavigator* nav , JM::EvtNavigator*  post_nav);
	//bool is_afterpulse(JM::EvtNavigator* nav, JM::EvtNavigator* pre_nav);
	//bool is_neutron_capture(JM::EvtNavigator* nav, JM::EvtNavigator* pre_nav);


};

#endif
