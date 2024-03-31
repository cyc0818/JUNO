#ifndef IAD_DUMMY_DATA_H
#define IAD_DUMMY_DATA_H

#include "IADFormat/IADDataId.h"

namespace IAD
{
    struct DummyData
    {
        float f_energy; // reconstructed energy of single event
        uint32_t u_sec;
        uint32_t u_nanoSec;
        int n_clusters;
        float f_vertexX; // reconstructed vertex of single event
        float f_vertexY;
        float f_vertexZ;
        int i_tag; // the tag of event //eg: CD_muonUncor_IBD
    };

    template <>
    class DataId<DummyData>
    {
    public:
        // Number to identify a specific type
        const static uint32_t id = 0xddaa0000;
    };
}

#endif