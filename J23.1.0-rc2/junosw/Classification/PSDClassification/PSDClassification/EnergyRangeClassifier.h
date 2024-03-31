/*
 * Description:
 *   classify the trigger events which is in energy range [ E_min, E_max]
 *  
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef EnergyRangeClassifier_h
#define EnergyRangeClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "EventRecordSvc/EventRecordSvc.h"


class EnergyRangeClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    EnergyRangeClassifier(const std::string& name);
    ~EnergyRangeClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buf);

private:
    ClassificationType classify(JM::NavBuffer* buf,JM::NavBuffer::Iterator it);

private:
	EventRecordSvc* m_event_record;

	double m_energy_range_max;
	double m_energy_range_min;
	bool is_in_energy_range(JM::EvtNavigator* nav);
	JM::NavBuffer* m_buf;

};

#endif
