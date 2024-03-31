#ifndef CORBUILDALG_H
#define CORBUILDALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include <map>

class InputReviser;

class CorBuildAlg: public AlgBase
{
 public:
    CorBuildAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

 private :
    JM::NavBuffer* m_buf;
    std::map<std::string, std::string> m_taskmap;
    std::map<std::string, InputReviser*> m_incidentMap;
    std::map<std::string, int> m_task2random;
};

#endif
