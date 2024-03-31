#include <boost/python.hpp>
#include "GlobalGenAlg.h"
#include "HepMC/GenEvent.h"

#include "SniperKernel/SniperLog.h"

#include "EvtNavigator/EvtNavigator.h"
#include "Event/SimHeader.h"

DECLARE_ALGORITHM(GlobalGenAlg);

GlobalGenAlg::GlobalGenAlg(const std::string& name)
    : AlgBase(name)
{

}

GlobalGenAlg::~GlobalGenAlg()
{

}

bool GlobalGenAlg::initialize()
{
    m_gbuf = GlobalBuffer::FromStream("GEvtStream");
    return true;
}

bool GlobalGenAlg::execute()
{
    static int evtcounter = 0;

    // produce one event
    JM::EvtNavigator* evtnav = new JM::EvtNavigator();
    TTimeStamp ts;
    evtnav->setTimeStamp(ts);
    LogInfo << "Produce an event navigator " << evtnav
            << " with timestamp" << ts << std::endl;

    JM::SimHeader* sim_header = new JM::SimHeader;
    JM::SimEvent* sim_event = new JM::SimEvent(evtcounter);
    sim_header->setEvent(sim_event);
    evtnav->addHeader("/Event/Sim", sim_header);

    // save into global buffer
    m_gbuf->push_back(evtnav);

    ++evtcounter;

    return true;
}

bool GlobalGenAlg::finalize()
{

    return true;
}

