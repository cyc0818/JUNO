#ifndef TemplateFitAlg_h
#define TemplateFitAlg_h

#include "SniperKernel/AlgBase.h"
#include "TemplateFit.h"

class TFile;
namespace JM{
    class ElecWaveform;
    class CdWaveformEvt;

    class CalibPmtChannel;
}

class TemplateFitAlg: public AlgBase 
{
    public:

        TemplateFitAlg(const std::string& name);
        ~TemplateFitAlg();

        bool initialize();
        bool execute();
        bool finalize();

        struct ChannelParams {
            int channelID;
            TH1F* hist;

            ChannelParams() {
                hist = 0;
            }

            ~ChannelParams() {
                if (hist) {
                    delete hist;
                }
            }
        };
    private:
        bool fitOneChannel(const ChannelParams&);
        JM::CdWaveformEvt* load_elecsim_crate();
        void build_hist(const JM::ElecWaveform& channel, ChannelParams& params);
        bool save_calib_event();

    private:
        // PMTID -> PMT Header
        std::map<int, JM::CalibPmtChannel*> m_cache_first_hits;
    private:
        TemplateFit* m_tf;

        float chi2torlerance;
        int m_PmtTotal;
        // a dummy mode: only load pulse from root file
        std::string m_rootfile;
        TFile* m_file;
        
        // output related
        TTree* myt;
        int chId;
        int npulse;
        int sumnpe;
        float chi2;
        float  pe[20];
        float time[20];
        double firsthittime;

};

#endif
