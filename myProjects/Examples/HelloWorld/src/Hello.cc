#include "Hello.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"

DECLARE_ALGORITHM(HelloAlg);

HelloAlg::HelloAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
    // Using declare property
    declProp("VarString", m_string);
    declProp("VectorInt", m_vector_int);
    declProp("MapStrInt", m_str_int);
}

HelloAlg::~HelloAlg()
{
}

bool
HelloAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;
    LogInfo << " MyString: " << m_string << std::endl;

    LogInfo << " MyVectorInt(" << m_vector_int.size() << "): " << std::endl;
    for (std::vector<int>::iterator i=m_vector_int.begin();
            i!=m_vector_int.end(); ++i) {
        LogInfo << (*i) << std::endl;
    }
    LogInfo << std::endl;

    LogInfo << " MyStrInt(" << m_str_int.size() << "): " << std::endl; 
    for (std::map<std::string, int>::iterator i=m_str_int.begin();
            i!=m_str_int.end(); ++i) {
        LogInfo << i->first << ":"
                << i->second
                << std::endl;
    }
    return true;
}

bool
HelloAlg::execute()
{
    ++m_count;
    LogInfo << "Hello world: count: " << m_count << std::endl;

    return true;
}

bool
HelloAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;

    return true;
}
