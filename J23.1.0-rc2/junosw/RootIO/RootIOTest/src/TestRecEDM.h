#ifndef TestRecEDM_h
#define TestRecEDM_h

#include "SniperKernel/AlgBase.h"

class TestRecEDM: public AlgBase
{
public:
    TestRecEDM(const std::string& name);
    virtual ~TestRecEDM();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

private:
    bool test_write();

    bool test_read();

    int rw_mode; // 0: read; 1: write
};

#endif
