#include "SimEventMerger.h"
#include "SniperKernel/SniperLog.h"
#include "Event/SimHeader.h"
#include "Event/SimPMTHit.h"
#include "EvtNavigator/EvtNavHelper.h"

SimEventMerger::SimEventMerger(IInputStream* concreateInputStream, const std::string& path)
    : m_concreateInputStream(concreateInputStream),
      m_path(path),
      m_currentEvent(0),
      m_cachedEvent(0),
      m_entry(-1)
{
}

SimEventMerger::~SimEventMerger()
{
}

bool SimEventMerger::initialize()
{
    return true;
}

bool SimEventMerger::finalize()
{
    return true;
}

bool SimEventMerger::next(int step, bool read UNUSED)
{
    if (step < 1) {
        LogError << "invalid step " << step << std::endl;
        return false;
    }

    // Read and burn. Costy!
    while (step != 1 ) {
        step = step - 1;
        m_entry = m_entry + 1;
        bool okay = this->readOneEvent();
        if (!okay) return false;
        delete m_currentEvent;
        m_currentEvent = 0;
    }

    m_entry = m_entry + 1;
    LogDebug << "TTTTTTTTTTTTTTTTTT Current Entry: " << m_entry << std::endl;
    bool okay = this->readOneEvent();
    return okay;
}

bool SimEventMerger::previous(int step UNUSED, bool read UNUSED)
{
    // Jumping back is not supported
    return false;
}

bool SimEventMerger::first(bool read)
{
    if (m_cachedEvent) {
        delete m_cachedEvent;
        m_cachedEvent = 0;
    }
    bool okay = m_concreateInputStream->first(read);
    if (!okay) return false;
    m_entry = 0;
    if (read) {
        m_cachedEvent = dynamic_cast<JM::EvtNavigator*>(m_concreateInputStream->get());
        if (!m_cachedEvent) return false;
        okay = this->readOneEvent();
    }
    return okay;
}

bool SimEventMerger::last(bool read UNUSED)
{
    // Not supported
    return false;
}

bool SimEventMerger::setEntry(int entry, bool read UNUSED) 
{
    // Jumping back is not supported
    if (entry < m_entry) return false;

    return this->next(entry - m_entry);
}

JM::EvtNavigator* SimEventMerger::get()
{
    return m_currentEvent;
}

bool SimEventMerger::getObj(TObject*& obj, const std::string& objName)
{
    return m_concreateInputStream->getObj(obj, objName);
}

long SimEventMerger::getEntry()
{
    return m_entry;
}

long SimEventMerger::getEntries()
{
    // This method needs to be improved. It could be implemented
    // in 2 different cases.
    // 1. If there is metadata information. Just get entries from metadata.
    //    TODO

    // 2. If there is no metadata, try to loop the input
    return getEntriesSlowV2();
}

std::string SimEventMerger::streamname()
{
    return m_concreateInputStream->streamname();
}

bool SimEventMerger::readOneEvent()
{
    if (m_entry < 0) return false;
     
    // If there's one event cached, it should belong to the this event. If not, read in next event as current event
    if (m_cachedEvent) {
        m_currentEvent = m_cachedEvent;
        m_cachedEvent = 0;
    }
    else {
        bool okay = m_concreateInputStream->next(1);
        if (!okay) return false;
        m_currentEvent = dynamic_cast<JM::EvtNavigator*>(m_concreateInputStream->get());
    }

    JM::SimHeader* header = dynamic_cast<JM::SimHeader*>(m_currentEvent->getHeader(m_path));
    if (!header) {
        LogError << "Failed to get SimHeader from EvtNavigator" << std::endl;
        return false;
    }
    JM::SimEvt* event = dynamic_cast<JM::SimEvt*>(header->event());
    if (!event) {
        LogError << "Failed to get SimEvt from SimHeader" << std::endl;
        return false;
    }
    
    int eventID = event->getEventID();
    int nLpmtHits = event->getLpmtHits(); // if the event is not in CD, this number will be 0.
                                          // if this value is 0, it should not be the split event.
    // Note: in Simulation/DetSimV2/AnalysisCode/src/DataModelWriterWithSplit.cc, only the first
    //       sub-event will be filled with nLpmtHits. 

    while (m_concreateInputStream->next(1)) {
        m_cachedEvent = dynamic_cast<JM::EvtNavigator*>(m_concreateInputStream->get());

        JM::SimHeader* next_header = dynamic_cast<JM::SimHeader*>(m_cachedEvent->getHeader(m_path));
        if (!next_header) {
            LogError << "Failed to get SimHeader from EvtNavigator" << std::endl;
            return false;
        }
        JM::SimEvt* next_event = dynamic_cast<JM::SimEvt*>(next_header->event());
        if (!next_event) {
            LogError << "Failed to get SimEvt from SimHeader" << std::endl;
            return false;
        }
        int next_eventID = next_event->getEventID();
        int next_nLpmtHits = next_event->getLpmtHits();

        // TODO: the selected variables should be unique. 
        // Note: In the previous implementation, the nLpmtHits in SimEvt is used. 
        //       However, there is one problem: if the muon only passes through WP, 
        //       the nLpmtHits is not set. 
        //       So we change it to the variable in header. 
        //       If it is negative, that means this is a sub-event.
        // bool isSameSubEvt = eventID == next_eventID && (nLpmtHits != 0) and (next_nLpmtHits == 0);
        bool isSameSubEvt = next_header->getCDLPMTtotalHits()<0;

        if (not isSameSubEvt) {
            LogDebug << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: "
                     << " isSameSubEvt (hdr): " << isSameSubEvt << " "
                     << " isSameSubEvt (evt): " << (eventID == next_eventID && (nLpmtHits != 0) and (next_nLpmtHits == 0))
                     << std::endl;
        }

        if (isSameSubEvt) {
            // Merge
            const std::vector<JM::SimPMTHit*>& cd_hits = next_event->getCDHitsVec();
            for (auto & hit : cd_hits) {
                JM::SimPMTHit* nhit = event->addCDHit();
                nhit->setPMTID(hit->getPMTID());
                nhit->setNPE(hit->getNPE());
                nhit->setHitTime(hit->getHitTime());
                nhit->setTimeWindow(hit->getTimeWindow());
                nhit->setTrackID(hit->getTrackID());
                nhit->setLocalTheta(hit->getLocalTheta());
                nhit->setLocalPhi(hit->getLocalPhi());
            }
            
            const std::vector<JM::SimPMTHit*>& wp_hits = next_event-> getWPHitsVec();
            for ( auto & hit : wp_hits){
                JM::SimPMTHit* nhit = event->addWPHit();
                nhit->setPMTID(hit->getPMTID());
                nhit->setNPE(hit->getNPE());
                nhit->setHitTime(hit->getHitTime());
                nhit->setTimeWindow(hit->getTimeWindow());
                nhit->setTrackID(hit->getTrackID());
                nhit->setLocalTheta(hit->getLocalTheta());
                nhit->setLocalPhi(hit->getLocalPhi());
              }


            delete m_cachedEvent;
            m_cachedEvent = 0;
        }
        else break;
    }

    LogDebug << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: "
             << " Sub-events are merged. "
             << std::endl;

    return true;
}

long SimEventMerger::getEntriesSlowV1()
{
    long nEvents = 0;

    struct EventSummaryInfo {
        int eventID;
        int nLpmtHits;

        EventSummaryInfo() {
            eventID = -1;
            nLpmtHits = -1;
        }

        void update(std::shared_ptr<JM::EvtNavigator> subevt) {
            auto hdr = JM::getHeaderObject<JM::SimHeader>(subevt.get());
            auto evt = dynamic_cast<JM::SimEvt*>(hdr->event());

            eventID = evt->getEventID();
            nLpmtHits = evt->getLpmtHits();
        }

        // the event ID should be same. 
        // but only the head owns the tracks/wp hits/tt hits
        bool is_same_as(std::shared_ptr<JM::EvtNavigator> subevt) {
            auto hdr = JM::getHeaderObject<JM::SimHeader>(subevt.get());
            auto evt = dynamic_cast<JM::SimEvt*>(hdr->event());

            return eventID == evt->getEventID()
                && (nLpmtHits != 0)           // the var in sub-event head should be non-zero.
                && (evt->getLpmtHits() == 0); // the var in sub-event body should be always 0.
        }
    } current_event_info;

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go to first event:" << std::endl;

    bool okay = m_concreateInputStream->first(read);

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go to first event - status:" << okay << std::endl;

    while (m_concreateInputStream->next(1)) {
        typedef JM::EvtNavigator JEN;
        std::shared_ptr<JEN> subevt(dynamic_cast<JEN*>(m_concreateInputStream->get()));

        if (current_event_info.eventID == -1) {
            // it is uninitialized
            current_event_info.update(subevt);
        } else if (current_event_info.is_same_as(subevt)) {
            // it is the sub-events (body)
            continue;
        } else {
            // it is the sub-events (head)
            current_event_info.update(subevt);
        }


        ++nEvents;
    }

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: total number of events:" << nEvents << std::endl;

    // now, reset the m_concreateInputStream
    okay = m_concreateInputStream->first(read);

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go back to first event again - status:" << okay << std::endl;

    return nEvents;
}

long SimEventMerger::getEntriesSlowV2()
{
    typedef JM::EvtNavigator JEN;

    long nEvents = 0;

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go to first event:" << std::endl;

    bool okay = m_concreateInputStream->first(read);

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go to first event - status:" << okay << std::endl;

    // assume the first event should be first sub-event
    if (okay) {
        std::shared_ptr<JEN> subevt(dynamic_cast<JEN*>(m_concreateInputStream->get()));
        auto hdr = JM::getHeaderObject<JM::SimHeader>(subevt.get());

        if (hdr->getCDLPMTtotalHits()<0) {
            LogError << "ERROR Found: hdr->getCDLPMTtotalHits()<0 for the first event " << std::endl;
            return nEvents;
        }
        ++nEvents;
    }


    while (m_concreateInputStream->next(1)) {
        std::shared_ptr<JEN> subevt(dynamic_cast<JEN*>(m_concreateInputStream->get()));
        auto hdr = JM::getHeaderObject<JM::SimHeader>(subevt.get());

        // the uninitialized value means it is a sub-event
        if (hdr->getCDLPMTtotalHits()<0) {
            continue;
        }

        ++nEvents;
    }

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: total number of events:" << nEvents << std::endl;

    // now, reset the m_concreateInputStream
    okay = m_concreateInputStream->first(read);

    LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTT: go back to first event again - status:" << okay << std::endl;


    return nEvents;
}
