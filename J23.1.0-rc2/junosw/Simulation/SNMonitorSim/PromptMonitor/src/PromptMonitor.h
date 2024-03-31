#ifndef SNHWTRIGGER_H
#define SNHWTRIGGER_H

#include <vector>
#include "TRandom3.h"
#include "TTree.h"

#include "SniperKernel/AlgBase.h"
#include "RootWriter/RootWriter.h"
#include "EvtNavigator/NavBuffer.h"
class PromptMonitor: public AlgBase{
    public:
        PromptMonitor(const std::string &name);
        ~PromptMonitor();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        void initOutput();

        int getNFiredPMTbyTruth(double);
        static bool uniqueFcn(int i, int j){return (i==j);}
        int getNFiredPMT();

        void getEventUser();//for user output of event level

        void findTrigger();
        void generateRndVeto(double, double);
        void generateRndBkg(double, double);

    private:
        JM::NavBuffer *m_buf;

        //=============parameters==============
        double m_vetoRate;
        double m_vetoTime;
        double m_bkgRate;
        //----------for event type-----------
        int i_NpmtLcut;
        int i_NpmtHcut;
        double d_T;
        //---------for muon like event-------
        double m_freshT_mu;
        double m_slideT_mu;
        int m_Nthr_mu;
        //---------for sn like event-------
        double m_freshT_sn;
        double m_slideT_sn;
        int m_Nthr_sn;

        //=====================================
        std::vector<double> m_muTime_tmp;
        std::vector<double> m_muTime;
        std::vector<double> m_snTime;
        //for random number
        TRandom3* m_prnd;

        //=========output==========
        //trigger info
        TTree* ttrig;
        int i_isTriggered;//0: no trigger; 1:sn-trigger; 2 mu-trigger;3: both sn+mu-trigger
        double d_muTriggerTime;
        double d_snTriggerTime;
        //evt info
        TTree *tevt;
        int i_totFiredPMTs;
        bool b_ismulike;
        bool b_issnlike;
        double d_time;
        double d_Qedep;
        double d_QedepX;
        double d_QedepY;
        double d_QedepZ;
};
#endif
