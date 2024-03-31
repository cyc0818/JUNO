#include "CorAnaAlg.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/AlgFactory.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/SimHeader.h"

DECLARE_ALGORITHM(CorAnaAlg);

CorAnaAlg::CorAnaAlg(const std::string& name)
    : AlgBase(name),
      m_iEvt(0),
      m_buf(0)
{
}

bool CorAnaAlg::initialize()
{
    LogDebug << "initializing" << std::endl;

    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true;
}

bool CorAnaAlg::execute()
{
    LogDebug << "executing: " << m_iEvt++
             << std::endl;

    JM::SimEvt* simevent = 0;
    JM::SimEvt* simevent_orig = 0;
    JM::CdVertexRecEvt* recevent = 0;
    JM::CdLpmtCalibEvt* calibevent = 0;
    JM::CdWaveformEvt* elecevent = 0;

    // Get the events of different stages

    auto nav = m_buf->curEvt();
    const auto& paths = nav->getPath();
    const auto& refs = nav->getRef();
    LogInfo << "Start to Explore SmartRef: " << std::endl;
    LogInfo << "Size of paths: " << paths.size() << std::endl;
    LogInfo << "Size of refs: " << refs.size() << std::endl;

    for (size_t i = 0; i < paths.size(); ++i) {
        LogInfo << "... -> ref: " << std::endl;
        const std::string& path = paths[i];
        JM::SmartRef* ref = refs[i];
        JM::EventObject* evtobj = ref->GetObject();

        LogInfo << " path: " << path
                << " ref->entry(): " << ref->entry()
                << " evtobj: " << evtobj;

        if (path=="/Event/Sim") {
            auto hdr = dynamic_cast<JM::SimHeader*>(evtobj);
            std::cout << " SimHeader: " << hdr;
        }
        std::cout << std::endl;
    }






    // Event After Split
    auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
    if (simheader) {
        simevent  = (JM::SimEvt*)simheader->event();
        LogInfo << "SimEvent Read in: " << simevent << std::endl;
        LogInfo << "SimEvent Track: " << simevent->getTracksVec().size() << std::endl;
        LogInfo << "SimEvent Hits: " << simevent->getCDHitsVec().size() << std::endl;
    }
    // Event Before Split
    auto simheader_orig = JM::getHeaderObject<JM::SimHeader>(nav, "/Event/SimOrig");
    if (simheader_orig) {
        simevent_orig  = (JM::SimEvt*)simheader_orig->event();
        LogInfo << "SimEvent (Orig) Read in: " << simevent_orig << std::endl;
        LogInfo << "SimEvent (Orig) Track: " << simevent_orig->getTracksVec().size() << std::endl;
        LogInfo << "SimEvent (Orig) Hits: " << simevent_orig->getCDHitsVec().size() << std::endl;
    }

    // A common case is: user get one recevent, then get corresponding calib/elec/sim events.
    auto recheader = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
    if (recheader) {
        recevent = recheader->event();
        LogInfo << "RecEvent Read in: " << recevent << std::endl;
    }

    auto calibheader = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
    if (calibheader) {
        calibevent = calibheader->event();
        LogInfo << "CalibEvent Read in: " << calibevent << std::endl;
    }

    auto elecheader = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if (elecheader) {
        elecevent = elecheader->event();
        LogInfo << "ElecEvent Read in: " << elecevent << std::endl;
    }
    // Do the correlation analysis with the events
    // ...

    if (simevent && recevent) {
        const auto& tracks = simevent->getTracksVec();
        LogInfo << "Compare vertex: " << std::endl;
        LogInfo << " sim tracks: " << std::endl; 
        for (auto it = tracks.begin();
             it != tracks.end(); ++it) {
            LogInfo << " -> ("
                    << (*it)->getQEdepX() << ","
                    << (*it)->getQEdepY() << ","
                    << (*it)->getQEdepZ() << ")"
                    << std::endl;
        }
        auto recvertex = recevent->getVertex(0); // get the first vertex
        LogInfo << " rec vertex: ("
                << recvertex->x() << ","
                << recvertex->y() << ","
                << recvertex->z() << ")"
                << std::endl;
    }

    return true;

}

bool CorAnaAlg::finalize()
{
    LogDebug << "finalizing" << std::endl;
    return true;
}

