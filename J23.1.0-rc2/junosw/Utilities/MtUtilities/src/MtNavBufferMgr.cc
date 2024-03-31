#include "MtUtilities/EventContext.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "RootWriter/MtTTreeStore.h"
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/IIncidentHandler.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperException.h"

class MtNavBufferMgr : public SvcBase
{
public:
    MtNavBufferMgr(const std::string &name);
    virtual ~MtNavBufferMgr();

    virtual bool initialize() override;
    virtual bool finalize() override;

private:
    //nested classes: the BeginEvent and EndEvent handlers
    class MtNavBeginEvtHandler : public IIncidentHandler
    {
    public:
        MtNavBeginEvtHandler(ExecUnit *par, JunoGlobalBuffer* gbuf, JunoGlobalBuffer::Elem **pcur);
        virtual ~MtNavBeginEvtHandler() {}

        virtual bool handle(Incident &incident) override;

    private:
        JunoGlobalBuffer *m_gbuf;
        JunoGlobalBuffer::Elem **m_pcur;
        IDataMemMgr *m_mmgr;
    };
    class MtNavEndEvtHandler : public IIncidentHandler
    {
    public:
        MtNavEndEvtHandler(ExecUnit *par, JunoGlobalBuffer* gbuf, JunoGlobalBuffer::Elem **pcur);
        virtual ~MtNavEndEvtHandler() {}

        virtual bool handle(Incident &incident) override;

    private:
        JunoGlobalBuffer *m_gbuf;
        JunoGlobalBuffer::Elem **m_pcur;
        MtTTreeStore *m_ts;
    };

    //data members
    JunoGlobalBuffer::Elem* m_cur;
    MtNavBeginEvtHandler* m_bh;
    MtNavEndEvtHandler* m_eh;
    std::string m_sname;
};

/// the implementation of MtNavBufferMgr

#include "SniperKernel/SvcFactory.h"
DECLARE_SERVICE(MtNavBufferMgr);

MtNavBufferMgr::MtNavBufferMgr(const std::string &name)
    : SvcBase(name),
      m_cur(nullptr),
      m_bh(nullptr),
      m_eh(nullptr)
{
    declProp("Stream", m_sname = "JunoGlobalStream");
}

MtNavBufferMgr::~MtNavBufferMgr()
{
}

bool MtNavBufferMgr::initialize()
{
    SniperPtr<DataMemSvc> dSvc(m_par, "DataMemSvc");
    dSvc->regist("MtTTreeStore", new MtTTreeStore());

    auto gbuf = JunoGlobalStream::GetBufferFrom(m_sname);

    m_bh = new MtNavBeginEvtHandler(getParent(), gbuf, &m_cur);
    m_eh = new MtNavEndEvtHandler(getParent(), gbuf, &m_cur);

    return true;
}

bool MtNavBufferMgr::finalize()
{
    delete m_bh;
    delete m_eh;

    return true;
}

/// the implementation of MtNavBufferMgr::MtNavBeginEvtHandler

MtNavBufferMgr::MtNavBeginEvtHandler::MtNavBeginEvtHandler(ExecUnit *par, JunoGlobalBuffer* gbuf, JunoGlobalBuffer::Elem **pcur)
    : IIncidentHandler(par),
      m_gbuf(gbuf),
      m_pcur(pcur),
      m_mmgr(nullptr)
{
    SniperPtr<IDataMemMgr> mMgr(par, "BufferMemMgr");
    if (mMgr.invalid())
    {
        throw ContextMsgException("Cannot get the BufferMemMgr");
    }
    m_mmgr = mMgr.data();

    this->regist("BeginEvent");
}

bool MtNavBufferMgr::MtNavBeginEvtHandler::handle(Incident &/*incident*/)
{
    auto elem = m_gbuf->next();
    if (elem != nullptr)
    {
        *m_pcur = elem;
        m_mmgr->adopt(elem->dptr->event, "/Event");
        return true;
    }
    return m_domain.stop();
}

/// the implementation of MtNavBufferMgr::MtNavEndEvtHandler

MtNavBufferMgr::MtNavEndEvtHandler::MtNavEndEvtHandler(ExecUnit *par, JunoGlobalBuffer* gbuf, JunoGlobalBuffer::Elem **pcur)
    : IIncidentHandler(par),
      m_gbuf(gbuf),
      m_pcur(pcur)
{
    this->regist("EndEvent");
    SniperDataPtr<MtTTreeStore> ts(par, "MtTTreeStore");
    m_ts = ts.data();
}

bool MtNavBufferMgr::MtNavEndEvtHandler::handle(Incident &/*incident*/)
{
    auto &trees = m_ts->trees();
    (*m_pcur)->dptr->trees.swap(trees);
    m_gbuf->setDone(*m_pcur);
    return true;
}
