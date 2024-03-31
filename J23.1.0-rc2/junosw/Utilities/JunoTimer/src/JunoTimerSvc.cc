#include "JunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(JunoTimerSvc);

JunoTimerSvc::JunoTimerSvc(const std::string& name)
    : SvcBase(name)
{

}

JunoTimerSvc::~JunoTimerSvc()
{
}

bool JunoTimerSvc::initialize()
{
    return true;
}

bool JunoTimerSvc::finalize()
{
    return true;
}

JunoTimerPtr JunoTimerSvc::get(const std::string& name)
{
    std::map<std::string, JunoTimerPtr>::iterator it = m_name2timer.find(name);
    if (it != m_name2timer.end()) {
        return it->second;
    }
    // create the timer if not exists
    m_name2timer[name] = JunoTimerPtr(new JunoTimer(name));
    return m_name2timer[name];
}
