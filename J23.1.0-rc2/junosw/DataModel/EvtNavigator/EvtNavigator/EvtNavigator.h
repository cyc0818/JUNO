#ifndef EVTNAVIGATOR_H
#define EVTNAVIGATOR_H

#include "TObject.h"
#include "TClass.h"
#include "TTimeStamp.h"
#include "EDMUtil/SmartRef.h"
#include "Event/HeaderObject.h"
#include <vector>
#include <string>
#include "Rtypes.h"
#include <cstdint>

// drop bits outside the range [R, L]
uint32_t get_bit_range(uint32_t b, size_t R, size_t L);
// set bits in the range [R, L]
void set_bit_range(uint32_t& tar, uint32_t val, size_t R, size_t L); 

namespace JM {

class EvtNavigator : public TObject {

    public:
    enum DetectorType {CD, WP, TT};
    enum MuonType {unknown, single, bundle, clipping};

    public:
        // Default constructor
        EvtNavigator();
        // Destructor
        ~EvtNavigator();
        // Copy constructor
        EvtNavigator(const EvtNavigator& nav);
        // Assignment operator
        EvtNavigator& operator=(const EvtNavigator& nav);
        // Add an event header to EvtNavigator
        void addHeader(const std::string& path, JM::HeaderObject* header);
        // If path is not given, use default one
        void addHeader(JM::HeaderObject* header);
        // Copy an event header from another EvtNavigator, and save it using the newPath(if not given, use the oldPath)
        bool copyHeader(JM::EvtNavigator* another, const std::string& oldPath, const std::string& newPath="");
        // Addjust the paths of EvtNavigator(called by Output service)
        void adjustPath(const std::vector<std::string>& paths);
        // Get the event header of a certain path
        JM::HeaderObject* getHeader(const std::string& path);

        // Get the event headers of a certain path (the headers in the same path.)
        std::vector<JM::HeaderObject*> getHeaders(const std::string& path);
        

        // Get the list of the paths holded by EvtNavigator
        std::vector<std::string>& getPath();
        const std::vector<std::string>& getPath() const;
        // Get the list of SmartRefs
        std::vector<JM::SmartRef*>& getRef();
        const std::vector<JM::SmartRef*>& getRef() const;
        // Set the entry of an event header, called by output system
        void setHeaderEntry(const std::string& path, int entry);
        // Set m_paths after EvtNavigator is loaded, called by input system
        void setPath(const std::vector<std::string>& paths);
        // Set whether a path will be written out
        void setWriteFlag(const std::string& path, bool write = false);
        // Set the write flag of a path to true
        bool writePath(const std::string& path);
        // Set the write flags of all paths to true
        void resetWriteFlag();
        // Get time stamp
        const TTimeStamp& TimeStamp() const;
        TTimeStamp& TimeStamp();
        // Set time stamp
        void setTimeStamp(const TTimeStamp& value);
        // Get event ID
        int EventID() const;
        // Set event ID
        void setEventID(int value);
        // Get run ID
        int RunID() const;
        // Set run ID
        void setRunID(int value);

        // Event type interfaces
        bool isPointLike() {return TESTBIT(m_eventType,3);}
        bool isTrackLike() {return !TESTBIT(m_eventType,3);}
        bool isMC() {return TESTBIT(m_eventType,0);}
        bool isData() {return !TESTBIT(m_eventType,0);}
        DetectorType getDetectorType() {return DetectorType(get_bit_range(m_eventType,1,2));}
        MuonType getMuonType() {return MuonType(get_bit_range(m_eventType,4,5));}

        void setPointLike() { SETBIT(m_eventType,3); }
        void setTrackLike() { CLRBIT(m_eventType,3); }
        void setMC() { SETBIT(m_eventType,0); }
        void setData() { CLRBIT(m_eventType,0); }
        void setDetectorType(DetectorType type) {set_bit_range(m_eventType, uint32_t(type), 1, 2);}
        void setMuonType(MuonType type) {set_bit_range(m_eventType, uint32_t(type), 4, 5);}
        
    
    private:
        // paths and write flags won't be saved
        std::vector<std::string>    m_paths;     //!
        std::vector<bool>           m_writeFlag; //!
        std::vector<JM::SmartRef*>  m_refs;
        TTimeStamp                  m_TimeStamp; // Time stamp of event
        int                         m_EventID;   // Event ID
        int                         m_RunID;     // Run ID

        /*
          0：mc/data
          1-2: detector type
          3: point/track like
          4-5: muon type
        */
        uint32_t                    m_eventType; // Event type

        // Initialize self, using another EvtNavigator
        void init(const EvtNavigator& nav);
        JM::SmartRef* getSmartRef(const std::string& path);

    ClassDef(EvtNavigator,4)

};

// Basic getters and setters of EvtNavigator

inline int EvtNavigator::EventID() const
{
    return m_EventID;
}

inline void EvtNavigator::setEventID(int value)
{
    m_EventID = value;
}

inline int EvtNavigator::RunID() const
{
    return m_RunID;
}

inline void EvtNavigator::setRunID(int value)
{
    m_RunID = value;
}

inline std::vector<std::string>& EvtNavigator::getPath()
{
    return m_paths;
}

inline const std::vector<std::string>& EvtNavigator::getPath() const
{
    return m_paths;
}

inline void EvtNavigator::setPath(const std::vector<std::string>& paths)
{
    m_paths = paths;
    if (m_paths.size() < m_refs.size()) {
        return;
    }
    // Add Nulls in the SmartRef vector, just to take positions
    size_t diff = m_paths.size() - m_refs.size();
    for (size_t i = 0; i<diff; ++i) {
        m_refs.push_back(new SmartRef);
    }
}

inline std::vector<JM::SmartRef*>& EvtNavigator::getRef()
{
    return m_refs;
}

inline const std::vector<SmartRef*>& EvtNavigator::getRef() const
{
    return m_refs;
}

inline const TTimeStamp& EvtNavigator::TimeStamp() const
{
    return m_TimeStamp;
}

inline TTimeStamp& EvtNavigator::TimeStamp()
{
    return m_TimeStamp;
}

inline void EvtNavigator::setTimeStamp(const TTimeStamp& value)
{
    m_TimeStamp = value;
}

} // namespace JM

#endif
