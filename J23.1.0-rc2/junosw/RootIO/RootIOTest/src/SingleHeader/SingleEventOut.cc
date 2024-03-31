#include "SingleEventOut.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootIOSvc/RootOutputSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/PhyHeader.h"
#include "Event/PhyEvent.h"

DECLARE_ALGORITHM(SingleEventOutAlg);

SingleEventOutAlg::SingleEventOutAlg(const std::string& name)
    : AlgBase(name) 
    , m_buf(0)
{
    declProp("offset", m_offset=0);
    m_iEvt = 0;
}

SingleEventOutAlg::~SingleEventOutAlg()
{
}

bool SingleEventOutAlg::initialize()
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

bool SingleEventOutAlg::execute()
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

    // Create dummy event, set headers and events
    JM::PhyHeader* header = new JM::PhyHeader();
    header->setEventID(m_iEvt + 10000 * m_offset);
    // Put the header into EvtNavigator
    nav->addHeader(header);

    if (m_iEvt % 1 == 0) {
        JM::PhyEvent* event = new JM::PhyEvent();
        event->setEnergy(m_iEvt + 10000 * m_offset);
        event->setRawEvis(m_r.Gaus(8,2));
        event->setEnrec(m_r.Gaus(12,3));
        event->setEprec(m_r.Gaus(12,3));
        header->setEvent(event);
    }

    LogInfo << "executing: " << m_iEvt
             << "  buffer_size: " << m_buf->size()
             << std::endl;

    return true;
    // After the executing of all algorithm of the task, current event in data
    // buffer will be saved, if output streams are configured. 
}

bool SingleEventOutAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}

