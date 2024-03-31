#include "BufferMemMgr.h"
#include "BeginEvtHdl.h"
#include "EndEvtHdl.h"
#include "FullStateNavBuf.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(BufferMemMgr);

BufferMemMgr::BufferMemMgr(const std::string& name)
    : SvcBase(name)
{
    declProp("TimeWindow", m_bufBounds);
    declProp("StopTaskOpt", m_stoptask_option=0);
}

BufferMemMgr::~BufferMemMgr()
{
}

bool BufferMemMgr::initialize()
{
    Task* par = dynamic_cast<Task*>(getParent());

    if ( m_bufBounds.size() < 2 ) {
        m_bufBounds.resize(2);
        m_bufBounds[0] = 0.0, m_bufBounds[1] = 0.0;
    }
    SniperPtr<DataMemSvc> mSvc(*par, "DataMemSvc");
    mSvc->regist("/Event",
            new FullStateNavBuf(m_bufBounds[0], m_bufBounds[1]));


    if ( par->find("InputSvc") != 0 ) {
        IIncidentHandler* bi = new BeginEvtHdl(par, m_stoptask_option);
        bi->regist("BeginEvent");
        m_icdts.push_back(bi);
    }

    if ( par->find("OutputSvc") != 0 ) {
        IIncidentHandler* ei = new EndEvtHdl(par);
        ei->regist("EndEvent");
        m_icdts.push_back(ei);
    }

    LogDebug << "Initialized Sucessfully." << std::endl;

    return true;
}

bool BufferMemMgr::finalize()
{
    std::list<IIncidentHandler*>::iterator it = m_icdts.end();
    while( it != m_icdts.begin() ) {
        delete *(--it);
    };

    return true;
}

bool BufferMemMgr::adopt(JM::EvtNavigator* nav, const std::string& path)
{
    return this->adopt(std::shared_ptr<JM::EvtNavigator>(nav), path);
}

bool BufferMemMgr::adopt(std::shared_ptr<JM::EvtNavigator> nav, const std::string& path)
{
    SniperDataPtr<FullStateNavBuf> navBuf(getParent(), path);
    if ( navBuf.valid() ) {
        return navBuf->adopt(nav);
    }

    LogError << "Cann't find NavBuffer @" << path << std::endl;
    return false;
}

bool BufferMemMgr::reset(const std::string& path, int entry)
{
    SniperDataPtr<FullStateNavBuf> navBuf(getParent(), path);
    if ( navBuf.valid() ) {
        return navBuf->reset(entry);
    }

    LogError << "Cann't find NavBuffer @" << path << std::endl;
    return false;
}
