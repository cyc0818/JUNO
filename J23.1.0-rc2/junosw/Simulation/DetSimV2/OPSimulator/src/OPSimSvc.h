#ifndef OPSimSvc_h
#define OPSimSvc_h

#include <string>

#include "SniperKernel/SvcBase.h"
#include "OPSimulator/IOPSimulator.h"
#include "OPSimulator/IOPSimSvc.h"

class OPSimSvc: public IOPSimSvc, public SvcBase {
public:

    OPSimSvc(const std::string& name);
    ~OPSimSvc();

    bool initialize();
    bool finalize();

    opsimulator::IOPSimulator* get_opsimulator();
    
private:
    opsimulator::IOPSimulator* m_opsimulator;

    std::string m_simulator_type;
};

#endif
