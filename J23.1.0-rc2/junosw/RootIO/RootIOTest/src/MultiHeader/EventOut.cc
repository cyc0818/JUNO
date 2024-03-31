#include "EventOut.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootIOSvc/RootOutputSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/TestHeaderA.h"
#include "Event/TestHeaderB.h"

DECLARE_ALGORITHM(EventOutAlg);

EventOutAlg::EventOutAlg(const std::string& name)
    : AlgBase(name)
    , m_buf(0)
{
    declProp("offset", m_offset=0);
    m_iEvt = 0;
}

EventOutAlg::~EventOutAlg()
{
}

bool EventOutAlg::initialize()
{
    // Get the data buffer
    SniperDataPtr<JM::NavBuffer>  navBuf(getScope(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();


    // Attach the geometry to output file
    // Create TGeoManager
    // Get the RootOutputSvc
    //SniperPtr<RootOutputSvc> ros(getScope(), "OutputSvc");
    //if ( ! ros.valid() ) {
    //    LogError << "Failed to get RootOutputSvc instance!" << std::endl;
    //    return false;
    //}
    // Attach TGeoManager to output stream
    //std::streambuf *backup; 
    //std::ofstream fout; 
    //fout.open("/dev/null"); 
    //backup = std::cout.rdbuf(); 
    //std::cout.rdbuf(fout.rdbuf()); 
    //TGeoManager* geom = TGeoManager::Import("geometry_test.gdml"); 
    //std::cout.rdbuf(backup);
    //geom->SetName("JunoGeom"); 

    //ros->attachObj("/Event/PhyEvent", geom);

    return true;

}

bool EventOutAlg::execute()
{
    ++m_iEvt;

    // Create Dummy EvtNavigator, set TimeStamp
    JM::EvtNavigator* nav = new JM::EvtNavigator();
    static TTimeStamp time(2014 + m_offset, 10, 18, 12, 15, 2, 111);
    time.Add(TTimeStamp(0, abs(m_r.Gaus(200000, 200000/5))));
    nav->setTimeStamp(time);

    // Put EvtNavigator into data buffer
    SniperPtr<IDataMemMgr> mMgr(getScope(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");

    if (m_iEvt % 2 == 0) {
        JM::TestHeaderA* headera = new JM::TestHeaderA();
        headera->setEventID(m_iEvt + 10000 * m_offset);
        nav->addHeader(headera);

        if (m_iEvt % 3 == 0) {
            JM::ATestEventA* aeventa = new JM::ATestEventA();
            aeventa->setId(m_iEvt + 10000 * m_offset);
            headera->setEventA(aeventa);
        }

        if (m_iEvt % 3 == 1) {
            JM::ATestEventB* aeventb = new JM::ATestEventB();
            aeventb->setId(m_iEvt + 10000 * m_offset);
            headera->setEventB(aeventb);
        }
    }

    if (m_iEvt % 2 == 1) {
        JM::TestHeaderB* headerb = new JM::TestHeaderB();
        headerb->setEventID(m_iEvt + 10000 * m_offset);
        nav->addHeader(headerb);

        if (m_iEvt % 3 == 0) {
            JM::BTestEventA* beventa = new JM::BTestEventA();
            beventa->setId(m_iEvt + 10000 * m_offset);
            headerb->setEventA(beventa);
        }

        if (m_iEvt % 3 == 1) {
            JM::BTestEventB* beventb = new JM::BTestEventB();
            beventb->setId(m_iEvt + 10000 * m_offset);
            headerb->setEventB(beventb);
        }
    }
    
    LogInfo << "executing: " << m_iEvt
             << "  buffer_size: " << m_buf->size()
             << std::endl;

    return true;
    // After the executing of all algorithm of the task, current event in data
    // buffer will be saved, if output streams are configured. 
}

bool EventOutAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}
