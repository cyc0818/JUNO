#ifndef RandomSvc_h
#define RandomSvc_h

#include "RandomSvc/IRandomSvc.h"
#include "SniperKernel/SvcBase.h"
#include <vector>

class IIncidentHandler;

class RandomSvc: public IRandomSvc, public SvcBase
{
public:
    RandomSvc(const std::string& name);
    ~RandomSvc();

    bool initialize();
    bool finalize();

    double random();

    long getSeed();
    void setSeed(long seed);
private:
    long m_init_seed;
    std::vector<IIncidentHandler*> m_icdts;

    std::string m_ss_input;

    std::vector<unsigned long> m_seed_status_in;
};

#endif
