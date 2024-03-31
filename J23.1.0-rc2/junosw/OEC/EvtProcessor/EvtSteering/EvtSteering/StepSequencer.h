#ifndef STEPSEQUENCER_H
#define STEPSEQUENCER_H
#include "SniperKernel/ToolBase.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "RootWriter/RootWriter.h"
#include "TTree.h"

class AlgBase;
class Sequence;
class StepSequencer: public ToolBase{
    public:
        StepSequencer(const std::string &name);
        ~StepSequencer();

        bool initialize();
        bool finalize();

        bool execSequence(const std::string &seq_id);
        std::vector<std::string> getSeqIDs();

    private:
        bool m_isEnabled;
        std::string m_cfg;

        std::map<std::string, Sequence*> m_seqs;

        //For testing the time of reconstruction algorithms
        JunoTimerPtr m_timer;
        double d_algtime;
        std::map<std::string, TTree*> m_timeTree;
};
#endif
