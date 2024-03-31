#include <boost/python.hpp>
#include "GlobalDetSimOutAlg.h"

#include "SniperKernel/SniperLog.h"

#include "RootWriter/RootWriter.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavigator.h"
#include "Event/SimHeader.h"


DECLARE_ALGORITHM(GlobalDetSimOutAlg);

GlobalDetSimOutAlg::GlobalDetSimOutAlg(const std::string& name)
    : AlgBase(name)
{

}

GlobalDetSimOutAlg::~GlobalDetSimOutAlg()
{

}

bool GlobalDetSimOutAlg::initialize()
{
    LogInfo << "Initializing the GlobalDetSimOutAlg..." << std::endl;
    m_gbuf = JunoGlobalStream::GetBufferFrom("GEvtStream");

    SniperPtr<RootWriter> rootwriter(*getParent(), "RootWriter");
    if (rootwriter.invalid()) {
        LogError << "Failed to find RootWriter." << std::endl;
        return false;
    }

    m_evttree = rootwriter->bookTree(*m_par, "SIMEVT/evt", "SimEvent");
    if (!m_evttree) {
        LogError << "Failed to create tree 'evt' using RootWriter. " << std::endl;
        return false;
    }

    m_evttree->Branch("evtID", &m_evtid, "evtID/I");
    m_evttree->Branch("nHits", &m_nHits, "nHits/I");
    m_evttree->Branch("totalPE", &m_totalPE, "totalPE/I");
    m_evttree->Branch("nPE", &m_nPE);
    m_evttree->Branch("hitTime", &m_hitTime);

    LogInfo << "Initialization of the GlobalDetSimOutAlg is done." << std::endl;

    return true;
}

bool GlobalDetSimOutAlg::execute()
{
    LogInfo << "Waiting to get evtnav from global buffer..." << std::endl;
    auto evt = m_gbuf->pop_front();
    // event* evt = static_cast<event*>(m_gbuf->pop_front());
    LogInfo << "Got evtnav: " << evt << " from global buffer..." << std::endl;

    if (evt!=nullptr) {

        // m_evtid = evt->eventid;
        // m_nHits = evt->nhits;
        // m_totalPE = evt->totalpe;
        // m_hitTime = evt->hittime;
        m_evttree->Fill();

        // delete evt;

        // put the evtnav into the normal output service
        SniperPtr<IDataMemMgr> mMgr(*getParent(), "BufferMemMgr");
        mMgr->adopt(evt->event, "/Event");
        
    } else { // no event, stop run
        LogInfo << "Got event " << evt << std::endl;
        LogInfo << "Stop the run" << std::endl;
        getParent()->stop();

    }

    return true;
}

bool GlobalDetSimOutAlg::finalize()
{

    return true;
}





