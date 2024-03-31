#include "EventIn.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootIOSvc/RootInputSvc.h"
#include "Event/TestHeaderA.h"
#include "Event/TestHeaderB.h"

DECLARE_ALGORITHM(EventInAlg);

EventInAlg::EventInAlg(const std::string& name)
    : AlgBase(name)
    , m_buf(0)
{
    m_iEvt = 0;
}

EventInAlg::~EventInAlg()
{
}

bool EventInAlg::initialize()
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

bool EventInAlg::execute()
{
    ++m_iEvt;
    // Get the current event from data buffer
    LogDebug << "reading: " << m_iEvt
             << "  buffer_size: " << m_buf->size()
             << "  cur: " << m_buf->curEvt()->TimeStamp()
             << std::endl;
    JM::TestHeaderA* headera = static_cast<JM::TestHeaderA*>(m_buf->curEvt()->getHeader("/Event/TestA"));
    if (headera) {
        std::cout << "headerA got. ID: "; 
        std::cout << headera->EventID() << std::endl;
        JM::ATestEventA* aeventa = headera->eventA();
        if (aeventa) {
            std::cout << "AeventA got. ID: " ;
            std::cout << aeventa->id() << std::endl;
        }
        JM::ATestEventB* aeventb = headera->eventB();
        if (aeventb) {
            std::cout << "AeventB got. ID: "; 
            std::cout << aeventb->id() << std::endl;
        }
    }
    JM::TestHeaderB* headerb = static_cast<JM::TestHeaderB*>(m_buf->curEvt()->getHeader("/Event/TestB"));
    if (headerb) {
        std::cout << "headerB got. ID: " ;
        std::cout << headerb->EventID() << std::endl;
        JM::BTestEventA* beventa = headerb->eventA();
        if (beventa) {
            std::cout << "BeventA got. ID: ";
            std::cout << beventa->id() << std::endl;
        }
        JM::BTestEventB* beventb = headerb->eventB();
        if (beventb) {
            std::cout << "BeventB got. ID: ";
            std::cout << beventb->id() << std::endl;
        }
    }
    return true;
}

bool EventInAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}
