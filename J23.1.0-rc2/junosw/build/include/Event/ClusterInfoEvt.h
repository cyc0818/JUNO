/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-05 12:30
# Filename: ClusterInfoEvt.h
# Description: This EDM class contains informations about the Clusters in the 
event. While the hit informations are still stored in the CalibEvent, the indices
of the hits belonging to certain clusters in the hit distribution are stored in
this additional class.
=============================================================================*/



#ifndef ClusterInfoEvt_h
#define ClusterInfoEvt_h

#include "Event/EventObject.h"
#include "ClusterHitCol.h"
#include "ClusterHitIdx.h"
#include <vector>

namespace JM
{
    class ClusterInfoEvt: public EventObject
    {
        private:
            /// The collection of clusters
            std::vector< JM::ClusterHitCol* > m_clusterVec;
            /// The event id
            Int_t m_eventid;
        public:
            /// Constructor (eventid = 0)
            ClusterInfoEvt();
            /// Constructor with event id
            ClusterInfoEvt(Int_t evtid);
            /// Destructor
            virtual ~ClusterInfoEvt();

            /// add a new empty cluster
            void AddCluster();
            /// add a new hit to the cluster (the hit time is optionally needed to efficiently provide cluster duration information later)
            void AddHitToCluster(UInt_t ChannelIdx, UInt_t HitIdx, UInt_t ClusterIdx, double hittime=-1.e5);
            /// get a cluster
            JM::ClusterHitCol* getCluster(UInt_t ClusterIdx) const {return m_clusterVec.at(ClusterIdx);};
            /// get the number of clusters
            UInt_t getNCluster() {return m_clusterVec.size();};
            // == Event ID ==
            /// get the event id
            Int_t getEventID() { return m_eventid; }
            /// set the event id
            void setEventID(Int_t val) { m_eventid = val; }
            ClassDef(ClusterInfoEvt,1)
    };
}

#endif
