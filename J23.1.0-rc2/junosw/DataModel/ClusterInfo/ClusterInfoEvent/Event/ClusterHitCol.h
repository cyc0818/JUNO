/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-05 12:30
# Filename: ClusterHitCol.h
# Description: This class belonging to the ClusterInfoEvent stores the hit
indices for one cluster. In addition if one provides time inf
=============================================================================*/



#ifndef CLUSTERHITCOL_H
#define CLUSTERHITCOL_H

#include<vector>
#include<TObject.h>
#include<Rtypes.h>
#include"ClusterHitIdx.h"

namespace JM
{
    class ClusterHitCol : public TObject
    {
        private:
            /// To store the hit informations in this cluster
            std::vector<JM::ClusterHitIdx> m_clusterCol;
            /// The start time of the cluster for analysis
            double m_start;
            /// The end time of the cluster for analysis
            double m_end;
        public:
            ClusterHitCol();
            virtual ~ClusterHitCol();
            /// Add a hit with a channel and hit index to the cluster. The hit time is optionally needed, if one is interested in the easy access of cluster start, end and duration later
            void AddHit(UInt_t ChannelIdx, UInt_t HitIdx, double hittime = -1.e5);
            /// The number of hits in a cluster
            UInt_t getSize() const;
            /// Get the channel of a hit in this cluster
            UInt_t getChannel(UInt_t hitIdx) const;
            /// Get the index of the hit in the channel of this cluster
            UInt_t getHit(UInt_t hitIdx) const;
            /// if maintained: get the start time (in ns) of the cluster (value of 1e10 means not valid)
            double getStartTime() const;
            /// if maintained: get the end time (in ns) of the cluster (value of -1e10 means not valid)
            double getEndTime() const;
            /// if maintained: get the duration (in ns) of the cluster (value of -1 means not valid)
            double getLength() const;
            ClassDef(ClusterHitCol,1)
    };

}


#endif
