//to do review

#ifndef OECWFRECTOOL_H
#define OECWFRECTOOL_H
#include "RecTools/IWFRecTool.h"
#include "SniperKernel/ToolBase.h"
#include <SniperKernel/AlgBase.h>
#include "Identifier/IDService.h"

class OECWFrecTool: public ToolBase, public IWFRecTool{
    public:
        OECWFrecTool(const std::string &);
        ~OECWFrecTool();

        bool initialize() override;
        void rec(QTInfo &) override;

        JM::CalibPmtChannel * recChannel(int, std::vector<uint16_t>&) override;

        void setFatherAlg(AlgBase* alg){m_fatherAlg = alg;}
        AlgBase* getFatherAlg(){return m_fatherAlg;}

    private:
        void subBSL_3TW(QTInfo &);

        int m_totalLPMT;
        bool WFOverTurn = false;
        bool enableSteering;
        bool m_sameMethod;
        std::string TTQ_method;
        std::string FHT_method;
        // std::string input_TTQ_method;
        // std::string input_FHT_method;
        
        std::string m_highEenergyWFTool;
        std::string m_lowEnergyWFTool;
        std::string m_roughNPETool;
        enum SubTools{
            highEenergyWFTool = 0,
            lowEnergyWFTool = 1,
            roughNPETool = 2
        };        
        bool createSubTool(const std::string toolName, SubTools locate);
        std::vector<IWFRecTool*> m_wfrecTools;
        // calib related
        std::vector<double> m_steering_cut;

        //loop val
        int m_nbsl;
        int m_bsl_length;
        std::vector<double> m_bsl_sums;

        std::vector<int> m_hmmtpmt;

        AlgBase* m_fatherAlg = nullptr;
        IDService* m_idServ = nullptr;
};
#endif