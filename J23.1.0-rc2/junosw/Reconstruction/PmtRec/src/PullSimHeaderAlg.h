#ifndef PullSimHeaderAlg_h
#define PullSimHeaderAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include <map>

namespace JM {
    class SimHeader;
    class SimEvt;
    class CdLpmtCalibHeader;
    class CdLpmtCalibEvt;
    class CalibPmtChannel;
}

class IRandomSvc;

class PullSimHeaderAlg: public AlgBase 
{
    public:
        PullSimHeaderAlg(const std::string& name);
        ~PullSimHeaderAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        bool load_sim_event();
        bool convert_to_calib();
        bool select_first_hit();

        int hit_or_not(int pmtid, int npe);
    private:
        JM::SimHeader* m_simheader;
        JM::SimEvt* m_simevent;
        JM::CdLpmtCalibHeader* m_calibhitcol;
    private:
        IRandomSvc* m_randomsvc;
    private:
        // PMTID -> PMT Header
        std::map<int, JM::CalibPmtChannel*> m_cache_first_hits;

        double m_qe_scale;
        bool m_flag_smear_qe;
        std::string m_smear_qe_file;

        // variables for hold Pmt Data
        int m_pmtId;
        double m_efficiency;

        std::map<int, double> m_pmt2efficiency;
};

#endif
