//to do review

#ifndef SIMPLEINTEGRALTOOL_H
#define SIMPLEINTEGRALTOOL_H
#include "RecTools/IWFRecTool.h"
#include "SniperKernel/ToolBase.h"
#include <memory>

class SimpleIntegralTool: public ToolBase, public IWFRecTool{
    public:
        SimpleIntegralTool(const std::string &);
        ~SimpleIntegralTool();

        bool initialize() override;
        void rec(QTInfo &) override;
        JM::CalibPmtChannel * recChannel(int, std::vector<uint16_t>&) override;

        void recFHT(QTInfo &);

    private:
        int m_totalLPMT;
        int m_maxhits;
        std::vector<int> m_hmmtpmt;

        // calib related
        bool enableCalibCorr;
        std::vector<double> m_toffset;
        std::vector<double> m_SPEsum;

        std::vector<double> m_SPEratio;
        std::vector<double> m_amp_threshold;
        std::vector<double> m_itg_threshold;

        //loop val
};
#endif