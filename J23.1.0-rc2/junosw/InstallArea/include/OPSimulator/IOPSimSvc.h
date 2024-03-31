#ifndef IOPSimSvc_h
#define IOPSimSvc_h


#include "OPSimulator/IOPSimulator.h"

class IOPSimSvc {
public:

    virtual opsimulator::IOPSimulator* get_opsimulator() = 0;
    
};

#endif
