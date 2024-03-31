#include <boost/python.hpp>
#include "GlobalRecInputAlg.h"

#include "SniperKernel/SniperLog.h"

#include "EvtNavigator/EvtNavigator.h"
#include "Event/CalibHeader.h"

DECLARE_ALGORITHM(GlobalRecInputAlg);

GlobalRecInputAlg::GlobalRecInputAlg(const std::string& name)
    : AlgBase(name) {

}

GlobalRecInputAlg::~GlobalRecInputAlg() {

}

bool GlobalRecInputAlg::initialize() {
    m_gbuf = GlobalBuffer::FromStream("GEvtStream");


    //Event navigator
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true;
}

bool GlobalRecInputAlg::execute() {
    
    JM::EvtNavigator* evtnav = m_buf->curEvt();

    // pre access to force ROOTIO load necessary data
    JM::CalibHeader* hdr = dynamic_cast<JM::CalibHeader*>(evtnav->getHeader("/Event/Calib"));
    if (!hdr) {
        LogError << "Can't find CalibHeader." << std::endl;
        return false;
    }

    JM::CalibEvent* evt = dynamic_cast<JM::CalibEvent*>(hdr->event());
    if (!evt) {
        LogError << "Can't find CalibEvent." << std::endl;
        return false;
    }

    const std::list<JM::CalibPMTChannel*>& chhlist = evt->calibPMTCol();
    LogInfo << "size of calib PMT collection: " << chhlist.size() << std::endl;;

    // To avoid the object is released, copy the smart pointer
    m_gbuf->push_back(new EvtNavMT(*m_buf->current()));

    return true;
}

bool GlobalRecInputAlg::finalize() {
    return true;
}
