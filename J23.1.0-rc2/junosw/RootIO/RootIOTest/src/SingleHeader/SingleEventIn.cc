#include "SingleEventIn.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootIOSvc/RootInputSvc.h"
#include "Event/PhyHeader.h"
#include "Event/PhyEvent.h"

DECLARE_ALGORITHM(SingleEventInAlg);

SingleEventInAlg::SingleEventInAlg(const std::string& name)
    : AlgBase(name)
    , m_buf(0)
{
    m_iEvt = 0;
}

SingleEventInAlg::~SingleEventInAlg()
{
}

bool SingleEventInAlg::initialize()
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

bool SingleEventInAlg::execute()
{ 
    ++m_iEvt;
    // Get the current event from data buffer
    LogDebug << "reading: " << m_iEvt
             << "  buffer_size: " << m_buf->size()
             << "  cur: " << m_buf->curEvt()->TimeStamp()
             << std::endl;
    JM::PhyHeader* header = static_cast<JM::PhyHeader*>(m_buf->curEvt()->getHeader("/Event/Phy"));
    if (header) {
        LogDebug << "header got. ID: " << header->EventID() << std::endl; 
        JM::PhyEvent* event = header->event();
        if (event) {
            LogDebug << "event got. Energy: " << event->energy() << std::endl;
        }
    }
    return true;
}

bool SingleEventInAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}
