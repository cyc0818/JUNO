/*
 * Description:
 *   classify the trigger events which is in energy range [ E_min, E_max]
 *  
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef SingleClassifier_h
#define SingleClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "EventRecordSvc/EventRecordSvc.h"


class SingleClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    SingleClassifier(const std::string& name);
    ~SingleClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buf);
    ClassificationType classify(JM::NavBuffer* buf,JM::NavBuffer::Iterator it);

private:
	EventRecordSvc* m_event_record;

	JM::NavBuffer* m_buf;

};

#endif
