/*=============================================================================
#
# Author: Philipp Kampmann - p.kampmann@fz-juelich.de
# Last modified: 2018-10-05 12:30
# Filename: ClusterHitIdx.h
# Description: This struct-like class is used by the ClusterInfoEvent to store
individual access indices to address hits in the CalibEvent. Therefore two
integers are needed which are stored as public variables
=============================================================================*/



#ifndef ClusterHitIdx_h
#define ClusterHitIdx_h

#include<utility>
#include<TObject.h>
#include<Rtypes.h>

namespace JM
{
    class ClusterHitIdx : public TObject
    {
        private:
            // no private variables used
        public:
            /// Empty constructor
            ClusterHitIdx(){};
            /// Empty destructor
            virtual ~ClusterHitIdx(){};
            /// The channel index of this hit
            UInt_t channel;
            /// The hit index in the given channel of this hit
            UInt_t hit;
           
            ClassDef(ClusterHitIdx,1)
    };
}

#endif
