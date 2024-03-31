#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SniperLog.h"
#include "IADUtil/WorkerInputStream.h"
#include "IADSvc/DataExchangeSvc.h"
#include "SniperKernel/Incident.h"
#include "IADFormat/IndexDict.h"
#include "EvtNavigator/NavBuffer.h"

class AnalyTopAlg : public AlgBase
{
public:
    AnalyTopAlg(const std::string &name);
    virtual ~AnalyTopAlg();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

private:
    WorkerInputStream *m_inputStream;
    int m_count = 0;
    JM::NavBuffer *m_buf;
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(AnalyTopAlg);

AnalyTopAlg::AnalyTopAlg(const std::string &name)
    : AlgBase(name)
{
}

AnalyTopAlg::~AnalyTopAlg()
{
}

bool AnalyTopAlg::initialize()
{
    // get the event buffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid())
    {
        LogError << "can not get navbuffer!!!" << std::endl;
        return false;
    }
    m_buf = navbuf.data();
    return true;
}

bool AnalyTopAlg::execute()
{
    JM::EvtNavigator *nav = m_buf->curEvt(); // get the current event
    LogInfo << "execute once" << std::endl;
    //.....Your analysis program
    return true;
}

bool AnalyTopAlg::finalize()
{

    return true;
}