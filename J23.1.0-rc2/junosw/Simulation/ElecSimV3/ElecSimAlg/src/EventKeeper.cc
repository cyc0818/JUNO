#include "EventKeeper.h"
#include "Event/SimHeader.h"
#include "Event/GenHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include <iostream>
#include <sstream>
#include <map>
#include "SniperKernel/SniperLog.h"

EventKeeper&
EventKeeper::Instance() {
    static EventKeeper s_keeper;
    return s_keeper;
}

EventKeeper::Entry::Entry() 
    : tag()
    , filename()
    , entry(-1)
    , header(0)
{

}

EventKeeper::Record::Record()
    : elecevent_id(-1)
{

}

void
EventKeeper::Record::reset() {
    simevents.clear();
    elecevent_id = -1;
}

EventKeeper::EventKeeper() {

}

void
EventKeeper::clear() {
    m_record.reset();

}

void
EventKeeper::set_current_entry(const EventKeeper::Entry& entry) {
    m_entry = entry;
}

const EventKeeper::Entry&
EventKeeper::current_entry() {
    return m_entry;
}

const EventKeeper::Record&
EventKeeper::current_record() {
    return m_record;
}


bool operator < (const EventKeeper::Entry &l, const EventKeeper::Entry &r) { 
    return (l.tag<r.tag) ||
        ((l.tag==r.tag)&&(l.filename<r.filename)) ||
        ((l.tag==r.tag)&&(l.filename==r.filename)&&(l.entry<r.entry));
}

bool operator==(const EventKeeper::Entry & lhs, const EventKeeper::Entry & rhs)
{
    return lhs.tag == rhs.tag && lhs.filename == rhs.filename && lhs.entry == rhs.entry;
}

bool
EventKeeper::add(const Entry& entry) {
    // only entry>=0 is valid
    if (entry.entry < 0) {
        return false;
    }
    // count the entry in current event.
    ++m_record.simevents[entry];
    return true;
}

bool
EventKeeper::commit() {

    // give summary:
    std::cout << "EventKeeper::commit summary: "
              << "total " << m_record.simevents.size() << " events mixing." << std::endl;
    for (std::map<Entry, int>::iterator it = m_record.simevents.begin();
         it != m_record.simevents.end(); ++it) {
        std::cout << "--> [" << it->first.tag << "] "
                  << " " << it->first.filename << ":" << it->first.entry
                  << " count: " << it->second
                  << std::endl;
    }

    clear();
    return true;
}

bool
EventKeeper::rebuildGenEvent(JM::EvtNavigator* nav) {
    // this should be invoked before commit
    // Create a new merged GenEvent based on the existing GenEvent
    // nav is a return value

    // get the genheader first
    auto gen_hdr = JM::getHeaderObject<JM::GenHeader>(nav);
    if (!gen_hdr) {
        LogError << "Failed to find the GenHeader. " << std::endl;
        return false;
    }

    auto gen_event = new JM::GenEvt();
    
    gen_hdr->setEvent(gen_event);

    auto mc_event = new HepMC::GenEvent();
    gen_event->setEvent(mc_event); // put the HepMC event into JUNO event

    for (std::map<Entry, int>::iterator it = m_record.simevents.begin();
         it != m_record.simevents.end(); ++it) {
        auto genhdr = JM::getHeaderObject<JM::GenHeader>(it->first.evtnav.get());

        LogInfo << "Merge GenHeader " << genhdr
                << " into merged GenEvent " << gen_event
                << std::endl;
        if (!genhdr) {
            LogWarn << "Empty GenHeader found. " << std::endl;
            continue;
        }

        auto genevt = dynamic_cast<JM::GenEvt*>(genhdr->event());
        if (!genevt) {
            LogWarn << "Empty GenEvt found. " << std::endl;
            continue;
        }

        auto hepmc_evt = genevt->getEvent();
        if (!hepmc_evt) {
            LogWarn << "Empty HepMC::GenEvent found. " << std::endl;
            continue;
        }

        // ====================================================
        // = copy the info from the hepmc_evt to merged event =
        // ====================================================

        // loop vertex
        HepMC::GenEvent::vertex_const_iterator
            iVtx = (*hepmc_evt).vertices_begin(),
            doneVtx = (*hepmc_evt).vertices_end();

        for (/*nop*/; doneVtx != iVtx; ++iVtx) {
            const HepMC::FourVector& v = (*iVtx)->position();

            auto newvertex = new HepMC::GenVertex(v);
            mc_event->set_signal_process_vertex(newvertex);

            LogInfo << "----> put new vertex: "
                    << v.x() << "," << v.y() << "," << v.z() << "," << v.t()
                    << std::endl;

            // loop particles
            HepMC::GenVertex::particles_out_const_iterator 
                iPart = (*iVtx)->particles_out_const_begin(),
                donePart = (*iVtx)->particles_out_const_end();
            for (/*nop*/; donePart != iPart; ++iPart) {
                const HepMC::FourVector& p = (*iPart)->momentum();

                LogInfo << "--------> put new particle: "
                        << (*iPart)->status() << " "
                        << (*iPart)->pdg_id() << " "
                        << "(" << p.px() << "," << p.py() << "," << p.pz() << ") "
                        << std::endl;

                auto newparticle = new HepMC::GenParticle(p,
                                                          (*iPart)->pdg_id(),
                                                          (*iPart)->status());
                newvertex->add_particle_out(newparticle);

            }
        }

    }

    return true;
}

bool
EventKeeper::rebuildSimEvent(JM::EvtNavigator* nav) {
    // this should be invoked before commit
    // Create a new merged SimEvent based on the existing SimEvent
    // nav is a return value

    // get the simheader first
    auto sim_hdr = JM::getHeaderObject<JM::SimHeader>(nav);
    if (!sim_hdr) {
        LogError << "Failed to find the SimHeader. " << std::endl;
        return false;
    }

    // Loop the entires, merge the track level information
    auto sim_evt = new JM::SimEvt();

    // Format: 
    //   Label:entry:ntrks:nhits
    std::string event_types;

    for (std::map<Entry, int>::iterator it = m_record.simevents.begin();
         it != m_record.simevents.end(); ++it) {
        LogInfo << "Merge SimHeader " << it->first.header
                << " into merged SimEvt " << sim_evt
                << std::endl;

        auto sh = it->first.header;
        if (!sh) {
            LogWarn << "Empty SimHeader found. " << std::endl;
            continue;
        }

        auto se = dynamic_cast<JM::SimEvt*>(sh->event());
        if (!sh) {
            LogWarn << "Empty SimEvent found. " << std::endl;
            continue;
        }

        const std::vector<JM::SimTrack*>&  tracks = se->getTracksVec();

        // construct the evt max in SimHeader
        std::stringstream ss;
        ss << it->first.tag << ":"
           << it->first.entry << ":"
           << tracks.size() << ":"
           << it->second
           << ";"; // end with ';'
        event_types += ss.str();

        // merge the tracks
        for( std::vector<JM::SimTrack*>::const_iterator it_track = tracks.begin();
             it_track != tracks.end();++it_track ) {
            auto track = *it_track;

            // copy the track info to the merged tracks
            auto newtrack = sim_evt->addTrack();
            newtrack->setPDGID   ( track->getPDGID()    );
            newtrack->setTrackID ( track->getTrackID()  );
            newtrack->setInitMass( track->getInitMass() );
            newtrack->setInitPx  ( track->getInitPx()   );
            newtrack->setInitPy  ( track->getInitPy()   );
            newtrack->setInitPz  ( track->getInitPz()   );
            newtrack->setInitX   ( track->getInitX()    );
            newtrack->setInitY   ( track->getInitY()    );
            newtrack->setInitZ   ( track->getInitZ()    );
            newtrack->setInitT   ( track->getInitT()    );

            newtrack->setExitPx  ( track->getExitPx()   );
            newtrack->setExitPy  ( track->getExitPy()   );
            newtrack->setExitPz  ( track->getExitPz()   );
            newtrack->setExitX   ( track->getExitX()    );
            newtrack->setExitY   ( track->getExitY()    );
            newtrack->setExitZ   ( track->getExitZ()    );
            newtrack->setExitT   ( track->getExitT()    );

            newtrack->setTrackLength(track->getTrackLength());

            newtrack->setEdep    ( track->getEdep()     );
            newtrack->setEdepX   ( track->getEdepX()    );
            newtrack->setEdepY   ( track->getEdepY()    );
            newtrack->setEdepZ   ( track->getEdepZ()    );

            newtrack->setQEdep   ( track->getQEdep()    );
            newtrack->setQEdepX  ( track->getQEdepX()   );
            newtrack->setQEdepY  ( track->getQEdepY()   );
            newtrack->setQEdepZ  ( track->getQEdepZ()   );

            newtrack->setEdepNotInLS(track->getEdepNotInLS());
        }
    }
    sim_hdr->setEventType(event_types);
    sim_hdr->setEvent(sim_evt);

    return true;
}
