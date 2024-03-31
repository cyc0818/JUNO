#ifndef IG4Svc_h
#define IG4Svc_h

#include "G4Svc/G4SvcRunManager.h"

class IG4Svc {
    public:
        virtual ~IG4Svc() = 0;
        virtual G4SvcRunManager* getRunMgr()=0;
};

#endif
