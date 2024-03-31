/*
 * Description:
 *   A simple algorithm to create OecHeader/OecEvt in the current buffer.
 *   In MT OEC, it is executed at the beginning of each event at LEC.
 *
 * Author:
 *   Tao Lin <lintao AT ihep.ac.cn>
 */


#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/DataMemSvc.h"

#include "BufferMemMgr/IDataMemMgr.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/OecHeader.h"

class OECCreatorAlg: public AlgBase {

public:
    OECCreatorAlg(const std::string& name);
    ~OECCreatorAlg();

    bool initialize();
    bool execute();
    bool finalize();

private:
    JM::NavBuffer* m_buf;
};

OECCreatorAlg::OECCreatorAlg(const std::string& name)
    : AlgBase(name) {

}

OECCreatorAlg::~OECCreatorAlg() {

}

bool OECCreatorAlg::initialize() {

    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true;
}

bool OECCreatorAlg::execute() {
    auto curnav = m_buf->curEvt();

    // if header does not exist, create and register it
    JM::OecHeader* header = JM::getHeaderObject<JM::OecHeader>(curnav);
    if (!header){
        header = new JM::OecHeader();
        JM::addHeaderObject(curnav, header);
    }
    // if event does not exist, create and register it
    JM::OecEvt* event = dynamic_cast<JM::OecEvt*>(header->event("JM::OecEvt"));
    if (!event) {
        event = new JM::OecEvt();
        event->setTime(curnav->TimeStamp());
        header->setEvent(event);
    }

    return true;
}

bool OECCreatorAlg::finalize() {
    return true;
}

DECLARE_ALGORITHM(OECCreatorAlg);
