#include <boost/python.hpp>
#include "GlobalRecOutAlg.h"

#include "SniperKernel/SniperLog.h"

#include "RootWriter/RootWriter.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavigator.h"
#include "Event/RecHeader.h"

DECLARE_ALGORITHM(GlobalRecOutAlg);

GlobalRecOutAlg::GlobalRecOutAlg(const std::string& name)
    : AlgBase(name) {

}

GlobalRecOutAlg::~GlobalRecOutAlg() {

}

bool GlobalRecOutAlg::initialize() {
    m_gbuf = GlobalBuffer::FromStream("GEvtStream");


    SniperPtr<IDataMemMgr> navBuf(*getParent(), "BufferMemMgr");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }

    m_buf = navBuf.data();

    return true;
}

bool GlobalRecOutAlg::execute() {

    void* evt = m_gbuf->pop_front();

    if (!evt) {
        LogWarn << "Got empty from global buffer. Stop the run. " << std::endl;
        getParent()->stop();
    }

    EvtNavMT* evtnavmt = static_cast<EvtNavMT*>(evt);

    // show the results
    JM::RecHeader* hdr = dynamic_cast<JM::RecHeader*>(evtnavmt->evtnav->getHeader("/Event/Rec"));
    if (hdr) {
        JM::CDRecEvent* cdevt = hdr->cdEvent();
        if (cdevt) {
            LogInfo << "Rec Vertex: ("
                    << cdevt->x() << ", "
                    << cdevt->y() << ", "
                    << cdevt->z() << ")" << std::endl;;
        }
    }

    JM::EvtNavigator* newnav = new JM::EvtNavigator();
    newnav->setTimeStamp(evtnavmt->evtnav->TimeStamp());
    newnav->copyHeader(evtnavmt->evtnav.get(), "/Event/Rec");
    m_buf->adopt(newnav, "/Event");

    // release the object
    delete evtnavmt;

    return true;
}

bool GlobalRecOutAlg::finalize() {
    return true;
}
