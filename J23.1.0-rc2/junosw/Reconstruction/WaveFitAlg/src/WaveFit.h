#ifndef WaveFit_h
#define WaveFit_h

#include "MyTree.h"
#include <stdio.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"

#include "AnalysisTool.h"
#include "IWaveFitTool.h"

namespace JM{
    class SimHeader;
    class SimEvt;

    class CdWaveformEvt;
    class ElecWaveform;
    
    class CdLpmtCalibHeader;
    class CdLpmtCalibEvt;
    class CalibPmtChannel;
}

class WaveFitAlg: public AlgBase
{
    public:
        WaveFitAlg(const std::string& name);
        ~WaveFitAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        JM::CdWaveformEvt* load_elecsim_crate();
        void draw_wave(int chni, int eventspantime, float* waveform);
        bool save_calib_event();
        IWaveFitTool* load_wave_fit_tool();
    private:
	std::string m_output_file;
	 //TString m_output_file;
    private:
	int m_count;
        int m_PmtTotal;

    private:
        // PMTID -> PMT Header
        std::map<int, JM::CalibPmtChannel*> m_cache_first_hits;
    private:
        FadcTree* outTree;
    private:
        bool m_flag_draw_wave;
        std::string m_fit_tool_name;

        IWaveFitTool* fit_tool;
};
#endif
