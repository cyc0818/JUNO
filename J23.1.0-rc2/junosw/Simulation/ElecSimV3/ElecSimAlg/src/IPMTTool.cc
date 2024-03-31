#include "IPMTTool.h"

std::ostream& operator<<(std::ostream& out, const PMTType& value){
    const char* s = 0;
    switch(value) {
    case kPMT_CD_Lpmt: s = "kPMT_CD_Lpmt"; break;
    case kPMT_CD_Spmt: s = "kPMT_CD_Spmt"; break;
    case kPMT_CD: s = "kPMT_CD"; break;
    case kPMT_WP: s = "kPMT_WP"; break;
    case kPMT_TT: s = "kPMT_TT"; break;
    default: s = "kPMT_Unknown"; break;
    }

    return out << s;
}

