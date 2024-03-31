#include "MtUtilities/EventContext.h"
#include "EvtNavigator/NavBuffer.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "SniperKernel/AlgBase.h"

class PruneGlobalBuffer : public AlgBase
{
public:
    PruneGlobalBuffer(const std::string &name);
    virtual ~PruneGlobalBuffer();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

private:
    JunoGlobalBuffer *m_gbuf;
    JM::NavBuffer *m_nbuf;
    IDataMemMgr *m_mmgr;
    std::string m_sname;
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(PruneGlobalBuffer);

PruneGlobalBuffer::PruneGlobalBuffer(const std::string &name)
    : AlgBase(name),
      m_gbuf(nullptr),
      m_nbuf(nullptr)
{
    declProp("Stream", m_sname = "JunoGlobalStream");
}

PruneGlobalBuffer::~PruneGlobalBuffer()
{
}

bool PruneGlobalBuffer::initialize()
{
    m_gbuf = JunoGlobalStream::GetBufferFrom(m_sname);

    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if (navBuf.invalid())
    {
        LogError << "Cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_nbuf = navBuf.data();

    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    if (mMgr.invalid())
    {
        LogError << "Cannot get the BufferMemMgr" << std::endl;
        return false;
    }
    m_mmgr = mMgr.data();

    return true;
}

bool PruneGlobalBuffer::execute()
{
    auto eventContext = m_gbuf->pop_front();

    if (eventContext != nullptr)
    {
        m_mmgr->adopt(eventContext->event, "/Event");

        for (auto tree : eventContext->trees)
        {
            tree->DoFillOne();
        }
    }
    else
    {
        getParent()->stop();
    }

    return true;
}

bool PruneGlobalBuffer::finalize()
{
    return true;
}