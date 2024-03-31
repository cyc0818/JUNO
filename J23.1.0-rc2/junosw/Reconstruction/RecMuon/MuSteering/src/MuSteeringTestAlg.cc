#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

class MuSteeringTestAlg : public AlgBase
{
    public:
        MuSteeringTestAlg(const std::string& name);
        virtual ~MuSteeringTestAlg();

        virtual bool initialize() override;
        virtual bool execute() override;
        virtual bool finalize() override;

    private:
        JM::NavBuffer* m_buf;
};

DECLARE_ALGORITHM(MuSteeringTestAlg);

MuSteeringTestAlg::MuSteeringTestAlg(const std::string& name)
    : AlgBase(name),
      m_buf(nullptr)
{
}

MuSteeringTestAlg::~MuSteeringTestAlg()
{
}

bool MuSteeringTestAlg::initialize()
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

bool MuSteeringTestAlg::execute()
{
    JM::EvtNavigator* evt = m_buf->curEvt();

    std::string typestr;
    switch(evt->getMuonType()) {
        case(JM::EvtNavigator::MuonType::single):
            typestr = "CD_MuSingleLike";
            break;
        case(JM::EvtNavigator::MuonType::bundle):
            typestr = "CD_MuBundleLike";
            break;
        default:
            LogError << "Unknown CD MuonType" << std::endl;
            return false;
    }

    LogInfo << this->objName() << " is running for an event of " << typestr << std::endl;

    return true;
}

bool MuSteeringTestAlg::finalize()
{
    return true;
}
