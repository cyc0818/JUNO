#ifndef EventKeeper_h
#define EventKeeper_h

/*
 * In electronics simulation, hits from different SimEvents will be mixed
 * and splited. It became complicated for user to get the correct relationship
 * between SimEvent and ElecEvent.
 * EventKeeper is used for such case. 
 *
 * About the data structure:
 *
     RECORD:
         +------+                  +------------------+
         | Elec | ---------------> | [U]sample_U.root |
         +------+        \         +------------------+
                          |
                          |        +------------------+
                          +------> | [K]sample_K.root |
                                   +------------------+

 * 
 * About the interaction
                                                          Waveform/  
      EventKeeper    EventMixing    Unpacking   PMTSim ... Readout
          |               |             |          | <------- |
          |               |             | <------- |          |
          |  save current | <---------- |          |          |
          | <------------ | record-hit  |          |          |
          | --------------------------> |          |          |
          |               |             |   add    |          |
          | <------------------------------------- | commit   |
          | <------------------------------------------------ |
          |               |             |          |          |

 *
 * During unpacking the event, we need to set the correlation between
 * hit and event.
 */
#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>
#include "EvtNavigator/EvtNavigator.h"
#include "Event/SimHeader.h"

class EventKeeper {
public:
    // Entry in File
    struct Entry {
        std::string tag;
        std::string filename;
        int entry;
        std::shared_ptr<JM::EvtNavigator> evtnav;
        JM::SimHeader* header; // cache the header, so we could associate event directly

        Entry();
    };

    // Record: keep correlation
    struct Record {
        int elecevent_id;
        // key: entry
        // val: count (how many hits/pulses refer to this event)
        std::map<Entry, int> simevents;

        Record();
        void reset();
    };

public:
    static EventKeeper& Instance();

public:
    void set_current_entry(const Entry& entry);
    const Entry& current_entry();
    const Record& current_record();

    // create final record
    bool add(const Entry& entry);
    bool rebuildGenEvent(JM::EvtNavigator* nav);
    bool rebuildSimEvent(JM::EvtNavigator* nav);
    bool commit();
public:
    EventKeeper();
private:
    void clear();
    
private:
    Entry m_entry; // used to sync between mixing and unpacking
    Record m_record;
};

#endif
