#ifndef JunoTimerSvc_h
#define JunoTimerSvc_h

#include "SniperKernel/SvcBase.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include <map>

class JunoTimerSvc: public SvcBase, public IJunoTimerSvc
{
    public:
        JunoTimerSvc(const std::string& name);
        ~JunoTimerSvc();
    public:
        bool initialize();
        bool finalize();

    public:
        JunoTimerPtr get(const std::string& name);

    private:
        std::map<std::string, JunoTimerPtr> m_name2timer;
};

#endif
