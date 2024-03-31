#include "MultiEventIn.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootIOSvc/RootInputSvc.h"
#include "Event/TestHeaderA.h"
#include "Event/ATestEventA.h"
#include "Event/ATestEventB.h"

DECLARE_ALGORITHM(MultiEventInAlg);

MultiEventInAlg::MultiEventInAlg(const std::string& name)
    : AlgBase(name)
    , m_buf(0)
{
    m_iEvt = 0;
}

MultiEventInAlg::~MultiEventInAlg()
{
}

bool MultiEventInAlg::initialize()
{
    // Get the data buffer
    SniperDataPtr<JM::NavBuffer>  navBuf(getScope(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    // Get the TGeoManager in input file
    //TObject* geo = 0;
    // Get RootInputSvc
    //SniperPtr<RootInputSvc> ris(getScope(), "InputSvc");
    //if ( ! ris.valid() ) {
    //    LogError << "Failed to get RootInputSvc instance!" << std::endl;
    //    return false;
    //}
    //ris->getObj(geo, "JunoGeom", "/Event/PhyEvent");
    //if ( 0 != geo) {
    //    LogInfo << "Loaded " << geo->ClassName() << ": " << geo << std::endl;
    //}

    LogInfo << "Initialized successfully" << std::endl;

    return true;
}

bool MultiEventInAlg::execute()
{
    ++m_iEvt;
    // Get the current event from data buffer
    LogDebug << "reading: " << m_iEvt
             << "  buffer_size: " << m_buf->size()
             << "  cur: " << m_buf->curEvt()->TimeStamp()
             << std::endl;
    JM::TestHeaderA* header = static_cast<JM::TestHeaderA*>(m_buf->curEvt()->getHeader("/Event/TestA"));
    if (header) {
        LogDebug << "header got. ID: " << header->EventID() << std::endl;
        JM::ATestEventA* eventa = header->eventA();
        if (eventa) {
            LogDebug << "eventA got. ID: " << eventa->id() << std::endl;
        }
        JM::ATestEventB* eventb = header->eventB();
        if (eventb) {
            LogDebug << "eventB got. ID: " << eventb->id() << std::endl;
        }
    }
    return true;
}

bool MultiEventInAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}
