#include "BeginEvtHdl.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperDataPtr.h"

BeginEvtHdl::BeginEvtHdl(Task *par, int stoptask_option)
    : IIncidentHandler(par), m_1stCall(true),
      m_par(par), m_stoptask_option(stoptask_option)
{
    SniperPtr<Task> subTask(m_par->getRoot(), "FilterTask"); // obtain Subtask
    if (subTask.invalid())
    {
        LogError << "cannot get the FilterTask." << std::endl;
        throw SniperException("FilterTask is invalid");
    }
    SniperPtr<DataExchangeSvc> svc(subTask.data(), "DataExchangeSvc");
    if (!svc.valid())
    {
        LogError << "failed to get service DataExchangeSvc" << std::endl;
        throw SniperException("DataExchangeSvc is invalid");
    }
    m_swap = svc.data();

    SniperDataPtr<AnalyNavBuffer> navBuf(m_par->getRoot(), "/Event");
    if (navBuf.invalid())
    {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        throw SniperException("AnalyNavBuffer is invalid");
    }
    m_nbuf = navBuf.data();
}

bool BeginEvtHdl::handle(Incident & /*incident*/)
{
    if (m_1stCall)
    {
    }

    // LogInfo<<"This is begin evt"<<std::endl;
    if (m_swap->filt())
    { // ask filter to filt IAD
        bool stat = m_nbuf->loadWindow(m_swap->getEvt());
        if (!stat)
            LogError << "Failed to load window" << std::endl;
        return true;
    }

    // End the task
    Task *whichtask = dynamic_cast<Task *>(m_par->getRoot());

    return whichtask->stop();
}
