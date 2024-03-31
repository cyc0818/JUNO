#include "Trigger.h"

std::ostream& operator<<(std::ostream& out, const TriggerType& value){
    const char* s = 0;
    switch(value) {
    case kTriggerLpmt: s = "kTriggerLpmt"; break;
    case kTriggerLpmtStd: s = "kTriggerLpmtStd"; break;
    case kTriggerLpmtVFL: s = "kTriggerLpmtVFL"; break;
    case kTriggerSpmt: s = "kTriggerSpmt"; break;
    case kTriggerCD: s = "kTriggerCD"; break;
    case kTriggerWP: s = "kTriggerWP"; break;
    case kTriggerTT: s = "kTriggerTT"; break;
    default: s = "kTriggerUnknown"; break;
    }

    return out << s;
}

