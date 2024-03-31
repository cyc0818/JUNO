#ifndef HADDALG_H
#define HADDALG_H

#include "SniperKernel/AlgBase.h"
#include <string>
#include <map>

class HaddAlg : public AlgBase {

    public:
        HaddAlg(const std::string& name);
        ~HaddAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        std::string m_output;
        std::string m_taskName;
        bool m_init;
};

#endif
