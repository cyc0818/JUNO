/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-05 12:30
# Filename: ClusterInfoHeader.h
# Description: This EDM class contains informations about the Clusters in the 
event. While the hit informations are still stored in the CalibEvent, the indices
of the hits belonging to certain clusters in the hit distribution are stored in
this additional class.
=============================================================================*/


#ifndef ClusterInfoHeader_h
#define ClusterInfoHeader_h

#include "Event/HeaderObject.h"
#include "EDMUtil/SmartRef.h"
#include "Event/ClusterInfoEvt.h"

namespace JM
{
    class ClusterInfoHeader: public HeaderObject
    {
        private:
            /// The contained event
            JM::SmartRef m_event;
        public:
            /// Empty constructor
            ClusterInfoHeader();
            /// Empty destructor
            ~ClusterInfoHeader();

            /// Access function to obtain the event object
            EventObject* event() {
                return m_event.GetObject();
            }

            /// Set function for the event object
            void setEvent(JM::ClusterInfoEvt* value) {
                m_event = value;
            }

            /// Set event entry
            void setEventEntry(const std::string& eventName, Long64_t& value);
            /// Access function to obtain the event object
            EventObject* event(const std::string& eventName);
            /// true if the header contains an event
            bool hasEvent();

            ClassDef(ClusterInfoHeader,1)

    };
}

#endif
