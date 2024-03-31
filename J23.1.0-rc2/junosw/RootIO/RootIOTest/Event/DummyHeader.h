#ifndef DummyHeader_h
#define DummyHeader_h

#include "Event/HeaderObject.h"
#include <string>

    class DummyHeader
    {
        private:
            /* 
             * Describe what's type of the current event.
             * + IBD
             * + U
             * + Th
             * + ...
             * or 
             * + Mixing
             */
            std::string m_evt_type; 

        public:
            DummyHeader();
            virtual ~DummyHeader();

        public:

            const std::string& getEventType() {
                return m_evt_type;
            }

            void setEventType(const std::string& evt_type) {
                m_evt_type = evt_type;
            }

        public:
            ClassDef(DummyHeader,3)

    };

#endif
