#ifndef RawHeader_h
#define RawHeader_h

#include "Event/HeaderObject.h"
#include "EDMUtil/SmartRef.h"
#include "Event/RawWaveform.h"
#include <string>

namespace JM
{
    class RawHeader : public HeaderObject
    {
        private :

            JM::SmartRef  m_waveform;

            std::string   m_evt_type;

            char*         m_addr;           // points to the Event_Fragment
            UInt_t        m_evtTag_type;

        public :

            RawHeader();
            virtual ~RawHeader();

            // implementation of base class interfaces
            void setEventEntry(const std::string& eventName, Long64_t& value);
            EventObject* event(const std::string& eventName);

            // set the Event_Fragment
            int setRawEvt(uint32_t size, char* addr);       // set size & address of Event_Fragment

            // get DAQ TimeStamp
            uint64_t getTimeStamp();

            // get/set RawWaveform
            JM::RawWaveform* rawWaveform() {
                return (JM::RawWaveform*)(m_waveform.GetObject());
            }

            void setRawWaveform(JM::RawWaveform* value) {
                m_waveform = value;
            }

            // get/set event type
            const std::string& getEventType() {
                return m_evt_type;
            }

            void setEventType(const std::string& evt_type) {
                m_evt_type = evt_type;
            }

        public :

            ClassDef(RawHeader, 1)

    };
}

#endif //RawHeader_h
