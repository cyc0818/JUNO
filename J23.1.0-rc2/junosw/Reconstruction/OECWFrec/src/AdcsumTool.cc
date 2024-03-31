/*
    @auther: Wei Jiang
    Alg2Tool by jiangw@ihep.ac.cn 2023.02.03
*/
#include "AdcsumTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Geometry/PMTParamSvc.h"

DECLARE_TOOL(AdcsumTool);

AdcsumTool::AdcsumTool(const std::string &name): ToolBase(name){
    m_totalLPMT = 17612;

    m_SPEratio       = { 1.31,  1.21};
    declProp("SimpleIntegral_SPEratio", m_SPEratio);
    declProp("TotalPMT", m_totalLPMT);
}

AdcsumTool::~AdcsumTool(){
}

bool AdcsumTool::initialize(){
    SniperPtr<PMTParamSvc> pmtParamSvc(getParent(), "PMTParamSvc");
    if (pmtParamSvc.invalid()) {
        LogError << "Failed to get PMTParamSvc instance!" << std::endl;
        return false;
    }
    auto pmtsvc = pmtParamSvc.data();
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
        return false;
    }
    auto calibsvc = calSvc.data();

    m_SPEsum  = calibsvc->getLPMTSPEAdcSum();

    // get hmmt pmt
    m_hmmtpmt = std::vector<int>(m_totalLPMT);
    for(int i=0;i<m_totalLPMT;i++) m_hmmtpmt[i] = int(pmtsvc->isHamamatsu(i));

    // size check
    if(m_SPEsum.size()%m_totalLPMT!=0 || m_hmmtpmt.size()%m_totalLPMT!=0){
        LogError << "Size check fail!" << std::endl;
        return false;
    }
 
    return true;
}

void AdcsumTool::rec(QTInfo &qtinfo){
    double adcSum = 0.;
    for(auto it=qtinfo.wf.begin();it!=qtinfo.wf.end();++it){
        adcSum += *it;
    }
    qtinfo.nPE = adcSum/m_SPEsum[qtinfo.pmtid]/m_SPEratio[m_hmmtpmt[qtinfo.pmtid]];
}

JM::CalibPmtChannel * AdcsumTool::recChannel(int, std::vector<uint16_t>&){
    return nullptr;
}