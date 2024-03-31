#include "testAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
DECLARE_ALGORITHM(testAlg);

testAlg::testAlg(const std::string& name)
    : AlgBase(name), m_count(0)
{ }
testAlg::~testAlg()
{ }
bool testAlg::initialize()
{ return true; }
bool testAlg::execute()
{
    ++m_count;
    return true;
}
bool testAlg::finalize()
{
    LogInfo<<m_count<<std::endl;
    return true;
}

