/*
 * Description:
 *   classify the trigger events which is in which fidducial volume [ radius_min, radius_max]
 *  
 *
 * Author:
 *   Hu Yuxiang   email:huyuxiang@ihep.ac.cn
 */

#ifndef FiducialVolumeClassifier_h
#define FiducialVolumeClassifier_h

#include "ClassificationTool/IClassificationTool.h"
#include "SniperKernel/ToolBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "EventRecordSvc/EventRecordSvc.h"


class FiducialVolumeClassifier: public ToolBase,
                            public IClassificationTool 
{
public:
    FiducialVolumeClassifier(const std::string& name);
    ~FiducialVolumeClassifier();

    bool configure();
    ClassificationType classify(JM::NavBuffer* buf);
    ClassificationType classify(JM::NavBuffer* buf,JM::NavBuffer::Iterator it);

private:
	EventRecordSvc* m_event_record;

	double m_radius_max;
	double m_radius_min;
	bool is_in_fiducial_volume(JM::EvtNavigator* nav);
	JM::NavBuffer* m_buf;

};

#endif
