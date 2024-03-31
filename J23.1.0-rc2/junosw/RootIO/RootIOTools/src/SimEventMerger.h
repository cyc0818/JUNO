#ifndef SIM_EVENT_MERGER
#define SIM_EVENT_MERGER 0

/*
 * Description:
 *  Merge the splitted SimEvent data. 
 *  The entry means the logical event number after merging.
 *
 *  The real entry before merge: 
 *
 *       [1-0]  [1-1]  [1-2]  [1-3]  [2-0]  [2-1]  [2-2]
 *
 *  The logical entry after merge:
 *
 *       [1]                         [2]
 *
 *  In order to speed up the counting of number of events, 
 *  a trick is checking the CD hits in SimHeader. 
 *  Only the first sub-event is initialized, while the other
 *  sub-events are uninitialized.
 * 
 * Author:
 *   Teng Li
 *   Tao Lin
 */

#include "RootIOSvc/IInputStream.h"
#include "EvtNavigator/EvtNavigator.h"

#include <string>

class SimEventMerger : public IInputStream {

    public:
        SimEventMerger(IInputStream* concreateInputStream, const std::string& path);
        ~SimEventMerger();

        bool initialize();
        bool finalize();
        // Interfaces to handle the stream
        // Read the event if read is set to true
        bool next(int step = 1, bool read = true);
        bool previous(int step = 1, bool read = true);
        bool first(bool read = true);
        bool last(bool read = true);
        bool setEntry(int entry, bool read = true);
        JM::EvtNavigator* get();
        bool getObj(TObject*& obj, const std::string& objName);
        long getEntry();
        long getEntries();
        // get current stream name.
        std::string streamname();

    private:
        bool readOneEvent();

        long getEntriesSlowV1(); // based on SimEvt
        long getEntriesSlowV2(); // based on SimHeader

    private:
        IInputStream* m_concreateInputStream;
        std::string   m_path;
        JM::EvtNavigator* m_currentEvent;
        JM::EvtNavigator* m_cachedEvent;
        int  m_entry;
};

#endif


