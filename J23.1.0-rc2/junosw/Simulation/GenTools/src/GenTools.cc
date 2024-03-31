#include <boost/python.hpp>
#include "GenTools.h"
#include "GenTools/IGenTool.h"
#include "GenTools/GenEventBuffer.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"

#include "MCGlobalTimeSvc/IMCGlobalTimeSvc.h"

#include "TDatabasePDG.h"
#include "HepMC/GenEvent.h"

DECLARE_ALGORITHM(GenTools);

GenTools::GenTools(const std::string& name)
    : AlgBase(name), m_global_time_svc(0)
{
    declProp("GenToolNames", m_genToolNames);
    declProp("EvtID", m_evtid = 0);
    declProp("EnableGlobalTime", m_use_global_time=false);
    declProp("EnableSNTime", m_use_sn_time=false);
}

GenTools::~GenTools()
{

}

bool
GenTools::initialize()
{
    // = TDatabasePDG =
    update_pdg_database();
    // = Initialize the GenTools =
    for(std::vector<std::string>::iterator it=m_genToolNames.begin();
            it != m_genToolNames.end();
            ++it) {
        const std::string& cur_gen_name = *it;
        // create the tool by default
        IGenTool* tb = tool<IGenTool>(cur_gen_name);
        if (tb==NULL) {
            return false;
        }
        if (tb->configure()) {
            LogDebug << "create and configure tool " << cur_gen_name << std::endl;
            // append to m_genTools
            m_genTools.push_back(tb);
        } else {
            LogInfo << "configure tool \"" << cur_gen_name << "\" failed" << std::endl;
            return false;
        }
    }
    // get the global time svc if it is enabled
    if (m_use_global_time) {
        SniperPtr<IMCGlobalTimeSvc> mcgt(*dynamic_cast<Task*>(getParent()), "MCGlobalTimeSvc");
        if (mcgt.invalid()) {
            LogError << "Can't find MCGlobalTimeSvc. " << std::endl;
            return false;
        }
        m_global_time_svc = mcgt.data();
    }

    // check the evtMax set by user
    Task* curTask = dynamic_cast<Task*>(getParent());
    evtMax = curTask->evtMax();
    return true;


}

bool
GenTools::execute()
{
    // create GenEvent
    HepMC::GenEvent* event = nullptr;

    // mutate event
    // two cases here,
    // 1. load the event from previous event
    GenEventBuffer* genbuffer = GenEventBuffer::instance();
    GenEventPtr gep = nullptr;
    bool has_previous_event = genbuffer->pop_front(gep);
    if (has_previous_event && gep) {
        // copy the event
        // event = gep.get();
        event = gep;
        m_evtid = event->event_number();
        LogInfo << "load previous event" << std::endl;
        // event->print();
        // == update timestamp ==
        // First, get the time shift:
        // * event->T0
        // Then, timestamp is base time + shift
        // At end, substract the shift in GenEvent
        if (not m_use_global_time) {
            m_current_timestamp = TTimeStamp();
        } else if (m_use_sn_time and (not m_global_time_svc)) {
            LogError << "When enable SN time and load previous events, "
                     << "need to enable global time service." << std::endl;
            return false;
        } else if (m_global_time_svc) {
            bool isabsolute = false;
            substract_shift_time(*event, isabsolute);
        } else {
            LogError << "Global Time is enabled, however can't find the service." 
                     << std::endl;
            return false;
        }

    } else {
        event = new HepMC::GenEvent;
        event->set_event_number(m_evtid);
        // 2. create a new event
        for(std::vector<IGenTool*>::iterator it=m_genTools.begin();
                it != m_genTools.end();
                ++it) {
            if ((*it)->mutate(*event)) {

            } else {
                // mutate failed
                LogError << "Mutate Event Failed"
                         << std::endl;
                return false;
            }
        }
        // == update timestamp ==
        // Because this is a new event, just update the base time first,
        // then get the base time
        if (not m_use_global_time and not m_use_sn_time) {
            m_current_timestamp = TTimeStamp();
        } else if (m_use_sn_time) {
            // force use the absolute time of vertex.
            bool isabsolute = true;
            substract_shift_time(*event, isabsolute);
        } else if (m_global_time_svc) {
            bool st = m_global_time_svc->update_base_time();
            m_current_timestamp = m_global_time_svc->get_base_time();
            LogInfo << "current timestamp: '"
                    << m_current_timestamp
                    << std::endl;

            // no matter what evtmax is, we need to check the time range boundary
            // if it is enabled.
            if (!st) {
                return Incident::fire(*dynamic_cast<Task*>(getParent()), "StopRun");
            }
        } else {
            LogError << "Global Time is enabled, however can't find the service." 
                     << std::endl;
            return false;
        }
    }

    // debug level, print the primary event
    if (logLevel() <= 2) {
        event->print();
    }
    // increase the event number
    ++m_evtid;
    return register_data(event);
}

bool
GenTools::finalize()
{
    return true;
}

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/GenHeader.h"
#include "Event/GenEvt.h"

bool
GenTools::register_data(HepMC::GenEvent* event)
{
    auto nav = new JM::EvtNavigator();
    LogDebug << "time stamp: '" << m_current_timestamp << "'." << std::endl;
    nav->setTimeStamp(m_current_timestamp);

    SniperPtr<IDataMemMgr> mMgr(*dynamic_cast<Task*>(getParent()), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");

    auto gen_header = new JM::GenHeader;
    auto gen_event = new JM::GenEvt;
    gen_event->setEvent(event);

    gen_header->setEvent(gen_event);
    nav->addHeader("/Event/Gen", gen_header);
    return true;
}

void 
GenTools::update_pdg_database() {
    TDatabasePDG* db_pdg = TDatabasePDG::Instance();
    // == optical photon ==
    // please note, this value is defined in Simulation/DetSim/DetSimOptions
    // -> LSExpPhysicsList -> 20022
    db_pdg -> AddParticle(
                          "opticalphoton", // Name
                          "opticalphoton", // Title
                          0.0,             // mass
                          true,            // stable
                          0.0,             // decay width
                          0.0,             // charge
                          "GaugeBoson",    // Particle Class
                          20022            // Pdg Code
                            );


    // == alpha ==
    db_pdg -> AddParticle(
                          "alpha",
                          "alpha",
                          3.727417,
                          true,
                          0.0,
                          6,
                          "Ion",
                          1000020040
                          );
}

void
GenTools::substract_shift_time(HepMC::GenEvent& event, bool isabsolute) {
    // copy from dyb
    double dt = std::numeric_limits<double>::max();

    for (HepMC::GenEvent::vertex_const_iterator it = event.vertices_begin(); 
                                                it != event.vertices_end();
                                                ++it) {
        double t = (*it)->position().t();
        if (t < dt) dt = t;
    }

    double toffset = dt; // unit: ns

    // we should remove dt from all vertices
    for (HepMC::GenEvent::vertex_iterator it = event.vertices_begin(); 
                                          it != event.vertices_end();
                                          ++it) {
        HepMC::FourVector vtx = (*it)->position();
        double t = vtx.t() - toffset;
        vtx.setT(t);
        (*it)->set_position(vtx);
    }

    // add the shift time into global time
    // here, toffset's unit is ns
    static const int kNSPerS = 1000000000; // 1s = 10e9 ns
    int Sec = (int)(toffset/kNSPerS); 
    int NanoSec = (int)(toffset-Sec*1.0e9);
    if (isabsolute) {
        m_current_timestamp = TTimeStamp(Sec, NanoSec);
    } else {
        m_current_timestamp.Add(TTimeStamp(Sec, NanoSec));
    }
}
