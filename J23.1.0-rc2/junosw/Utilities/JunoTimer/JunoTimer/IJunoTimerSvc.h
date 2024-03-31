#ifndef IJunoTimerSvc_h
#define IJunoTimerSvc_h

/* This class is migrated from IBesTimer in Boss.
 */ 
#include <memory>

class JunoTimer;
typedef std::shared_ptr<JunoTimer> JunoTimerPtr;

class IJunoTimerSvc {
    public:
        virtual JunoTimerPtr get(const std::string& name)=0;
        virtual ~IJunoTimerSvc(){}
};

#endif
