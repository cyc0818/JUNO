//to do review

#ifndef ADCSUMTOOL_H
#define ADCSUMTOOL_H
#include "RecTools/IWFRecTool.h"
#include "SniperKernel/ToolBase.h"
#include <memory>

class AdcsumTool: public ToolBase, public IWFRecTool{
    public:
        AdcsumTool(const std::string &);
        ~AdcsumTool();

        bool initialize() override;
        void rec(QTInfo &) override;
        JM::CalibPmtChannel * recChannel(int, std::vector<uint16_t>&) override;

    private:
        int m_totalLPMT;
        std::vector<int> m_hmmtpmt;
        // calib related
        std::vector<double> m_SPEsum;
        std::vector<double> m_SPEratio;

        //loop val
};
#endif