#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

class SteeringTestAlg : public AlgBase
{
    public:
        SteeringTestAlg(const std::string& name);
        virtual ~SteeringTestAlg();

        virtual bool initialize() override;
        virtual bool execute() override;
        virtual bool finalize() override;

    private:
        JM::NavBuffer* m_buf;
};

DECLARE_ALGORITHM(SteeringTestAlg);

SteeringTestAlg::SteeringTestAlg(const std::string& name)
    : AlgBase(name),
      m_buf(nullptr)
{
}

SteeringTestAlg::~SteeringTestAlg()
{
}

bool SteeringTestAlg::initialize()
{
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true;
}

bool SteeringTestAlg::execute()
{
    JM::EvtNavigator* evt = m_buf->curEvt();

    std::string typestr;
    switch(evt->getDetectorType()) {
        case(JM::EvtNavigator::DetectorType::CD):
            if (evt->isPointLike()) {
                typestr = "CD_PointLike";
            }
            else if (evt->isTrackLike()) {
                typestr = "CD_TrackLike";
            }
            else {
                LogError << "Unknown event type: PointLike/TrackLike" << std::endl;
                return false;
            }
            break;
        case(JM::EvtNavigator::DetectorType::WP):
            typestr = "WP";
            break;
        case(JM::EvtNavigator::DetectorType::TT):
            typestr = "TT";
            break;
        default:
            LogError << "Unknown DetectorType" << std::endl;
            return false;
    }

    LogInfo << this->objName() << " is running for an event of " << typestr << std::endl;

    return true;
}

bool SteeringTestAlg::finalize()
{
    return true;
}
