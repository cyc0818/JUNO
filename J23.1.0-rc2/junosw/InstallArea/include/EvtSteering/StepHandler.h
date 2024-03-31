#ifndef STEPHANDLER_H
#define STEPHANDLER_H
//#define HXDEBUG
#include "SniperKernel/AlgBase.h"

#ifdef HXDEBUG
#include "RootWriter/RootWriter.h"
#include "JunoTimer/JunoTimer.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "TTree.h"
#endif

class StepSequencer;
class StepDecision;
class StepHandler: public AlgBase{
    public:
        StepHandler(const std::string&);
        ~StepHandler();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        StepSequencer *m_sequencer;
        StepDecision *m_decision;
        std::vector<std::string> m_seqID;

#ifdef HXDEBUG
        //for time test
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent[5];
        TTree *m_LEC;
        float f_time[5];//time usage of the execute function
#endif
};
#endif
