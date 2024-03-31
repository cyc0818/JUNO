//to do review

#ifndef DECONVOLUTIONTOOL_H
#define DECONVOLUTIONTOOL_H
#include "RecTools/IWFRecTool.h"
#include "SniperKernel/ToolBase.h"
#include "TVirtualFFT.h"
#include <mutex>
#include <memory>

class DeconvolutionTool: public ToolBase, public IWFRecTool{
    public:
        DeconvolutionTool(const std::string &);
        ~DeconvolutionTool();

        bool initialize() override;
        void rec(QTInfo &) override;
        JM::CalibPmtChannel * recChannel(int, std::vector<uint16_t>&) override;

        double getInterpolateValue(double, double, double);

    private:
        // specified

        int m_length;
        int m_totalLPMT;
        int m_maxhits;
        std::vector<int> m_hmmtpmt;
        
        // calib related
        bool enableCalibCorr;
        std::vector<double> m_toffset;
        std::vector<double> m_SPEratio;
        
        int m_calib_length;
        int m_calib_start_time;
        int m_dft_length;
        int m_maxProfileN;
        bool isLongerWave;
        double m_f_scale;

        std::vector<double> m_filter;
        std::vector<double> m_SPERE;
        std::vector<double> m_SPEIM;
        int N_filter;
        int N_freq;

        std::vector<double> m_amp_threshold;
        std::vector<int> m_width_threshold;
        std::vector<int> m_itg_width;

        //loop val
        static std::mutex m_mutex;
        std::unique_ptr<TVirtualFFT> fft_forward;
        std::unique_ptr<TVirtualFFT> fft_back;

        std::vector<int> HitBTime;
        std::vector<int> HitETime;
        std::vector<double> wfre;
        std::vector<double> wfim;
        std::vector<double> wfback;
        std::vector<int> adc_profile;
        std::vector<int> b_bsl;
};
#endif