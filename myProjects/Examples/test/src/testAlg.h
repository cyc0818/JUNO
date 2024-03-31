#ifndef testAlg_h
#define testAlg_h

#include "SniperKernel/AlgBase.h"

class testAlg:public AlgBase {
    public:
        testAlg(const std::string& name);
        ~testAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int m_count;
};

#endif
