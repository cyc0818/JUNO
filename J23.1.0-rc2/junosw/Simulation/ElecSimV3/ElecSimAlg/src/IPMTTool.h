#ifndef IPMTTool_h
#define IPMTTool_h

#include "Context/TimeStamp.h"

#include "Pulse.h"
#include <iostream>

enum PMTType {
    kPMT_TypeUnknown = -1,
    kPMT_CD_Lpmt,
    kPMT_CD_Spmt,
    kPMT_CD,
    kPMT_WP,
    kPMT_TT
};
std::ostream& operator<<(std::ostream& out, const PMTType& value);


// we don't need to creae an additional Hit object,
// just give all variables:
//   - pmtid
//   - hit time
//   - weight

struct IPMTTool {
    virtual Pulse generate(int pmtid, TimeStamp hittime, double weight, bool iftruths) = 0;
};

#endif
