#include "MtUtilities/EventContext.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/AlgBase.h"

class FillGlobalBuffer : public AlgBase
{
public:
    FillGlobalBuffer(const std::string &name);
    virtual ~FillGlobalBuffer();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

private:
    JunoGlobalBuffer *m_gbuf;
    JM::NavBuffer *m_nbuf;
    std::string m_sname;
    std::vector<std::string> m_readAhead;
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(FillGlobalBuffer);

FillGlobalBuffer::FillGlobalBuffer(const std::string &name)
    : AlgBase(name),
      m_gbuf(nullptr),
      m_nbuf(nullptr)
{
    declProp("Stream", m_sname = "JunoGlobalStream");
    declProp("ReadAhead", m_readAhead);
}

FillGlobalBuffer::~FillGlobalBuffer()
{
}

bool FillGlobalBuffer::initialize()
{
    m_gbuf = JunoGlobalStream::GetBufferFrom(m_sname);

    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if (navBuf.invalid())
    {
        LogError << "Cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_nbuf = navBuf.data();

    return true;
}

bool FillGlobalBuffer::execute()
{
    auto& nav = *m_nbuf->current();

    for (auto &h : m_readAhead)
    {
        nav->getHeader(h);
    }

    m_gbuf->push_back(std::shared_ptr<EventContext>(new EventContext(nav)));

    return true;
}

bool FillGlobalBuffer::finalize()
{
    if (m_gbuf->status())
    {
        m_gbuf->push_back(std::shared_ptr<EventContext>(nullptr));
    }
    return true;
}