#ifndef MergeSimEventAlg_h
#define MergeSimEventAlg_h

#include "SniperKernel/AlgBase.h"
#include <vector>
#include <list>
namespace JM {
    class SimHeader;
    class SimEvt;
    class SimPMTHit;
}

class MergeSimEventAlg: public AlgBase {
    public:
        MergeSimEventAlg(const std::string& name);
        ~MergeSimEventAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        // for the output
        JM::SimHeader* m_simheader;
        JM::SimEvt* m_simevent;

        // this alg will zip the event in different tasks
        std::vector<std::string> m_tasks_name;

};

#endif
