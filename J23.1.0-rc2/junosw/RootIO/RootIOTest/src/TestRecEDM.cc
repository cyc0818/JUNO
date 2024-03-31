#include "TestRecEDM.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "RootIOSvc/RootOutputSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdTrackRecHeader.h"
#include "Event/WpRecHeader.h"
#include "Event/TtRecHeader.h"
#include "TRandom.h"

DECLARE_ALGORITHM(TestRecEDM);

TestRecEDM::TestRecEDM(const std::string& name)
    : AlgBase(name)
    , rw_mode(1)
{
    declProp("RWMode", rw_mode);
}

TestRecEDM::~TestRecEDM()
{

}

bool
TestRecEDM::initialize()
{
    return true;
}

bool
TestRecEDM::execute()
{
    bool st = true;

    if (rw_mode == 1) {
        st = test_write();
    } else if (rw_mode == 0) {
        st = test_read();
    } else {
    
    }

    return st;
}

bool
TestRecEDM::finalize()
{
    return true;
}

bool
TestRecEDM::test_write()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        LogError << "Can't find the NavBuffer." << std::endl;
        return false;
    }

    JM::EvtNavigator* nav = new JM::EvtNavigator();

    SniperPtr<IDataMemMgr> mMgr(*getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");


    int sel = static_cast<int>(gRandom->Rndm()*4);
    // random 
    // * JM::RecEvent
    // * JM::CDTrackRecEvent
    // * JM::WPRecEvent
    // * JM::TTRecEvent
    // 
    if (sel == 0) {
        auto hdr = new JM::CdVertexRecHeader();
        auto evt = new JM::CdVertexRecEvt();
        hdr->setEvent(evt);
        nav->addHeader(hdr);
    } else if (sel == 1) {
        auto hdr = new JM::CdTrackRecHeader();
        auto evt = new JM::CdTrackRecEvt();

        int ntrk = static_cast<int>(1 + 4*gRandom->Rndm());
        for (int i = 0; i < ntrk; ++i) {
            CLHEP::HepLorentzVector start;
            CLHEP::HepLorentzVector end;
            JM::RecTrack* trk = new JM::RecTrack(start, end);

            evt->addTrack(trk);
        }

        hdr->setEvent(evt);
        nav->addHeader(hdr);
    } else if (sel == 2) {
        auto hdr = new JM::WpRecHeader();
        auto evt = new JM::WpRecEvt();
        hdr->setEvent(evt);
        nav->addHeader(hdr);
    } else if (sel == 3) {
        auto hdr = new JM::TtRecHeader();
        auto evt = new JM::TtRecEvt();
        hdr->setEvent(evt);
        nav->addHeader(hdr);

    } else {
        LogInfo << "Unknown sel idx: " << sel << std::endl;
    }


    return true;
}

bool
TestRecEDM::test_read()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        LogError << "Can't find the NavBuffer." << std::endl;
        return false;
    }

    auto nav = navBuf->curEvt();

    if (nav->getHeader("/Event/CdVertexRec")) {
        auto hdr = dynamic_cast<JM::CdVertexRecHeader*>(nav->getHeader("/Event/CdVertexRec"));
        if (hdr->hasEvent()) {
            LogDebug << "CD vertex event." << std::endl;
            auto evt = hdr->event();
            int vertices = evt->nVertices();
            LogDebug << "  contains " << vertices << " vertices." << std::endl;
        }
    } else if (nav->getHeader("/Event/CdTrackRec")) {
        auto hdr = dynamic_cast<JM::CdTrackRecHeader*>(nav->getHeader("/Event/CdTrackRec"));
        if (hdr->hasEvent()) {
            LogDebug << "CD track event." << std::endl;
            auto evt = hdr->event();
            int trks = evt->nTracks();
            LogDebug << "  contains " << trks << " tracks." << std::endl;
        }
    } else if (nav->getHeader("/Event/WpRec")) {
        LogDebug << "WP track event." << std::endl;
    } else if (nav->getHeader("/Event/TtRec")) {
        LogDebug << "TT track event." << std::endl;
    } else {
        LogError << "Can't get Rec Header from current event." << std::endl;
        return false;
    }

    return true;
}
