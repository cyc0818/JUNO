/*
    @auther: Wei Jiang
    Alg2Tool by jiangw@ihep.ac.cn 2023.02.03
*/
#include "SimpleIntegralTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

DECLARE_TOOL(SimpleIntegralTool);

SimpleIntegralTool::SimpleIntegralTool(const std::string &name): ToolBase(name){
    m_maxhits = 32;
    m_totalLPMT = 17612;
    enableCalibCorr = true;

    m_amp_threshold  = {  2.9,   3.3};
    m_itg_threshold  = {0.084, 0.083};
    m_SPEratio       = { 1.31,  1.21};
    declProp("TotalPMT", m_totalLPMT);
    declProp("enableCalibCorr", enableCalibCorr);
    declProp("SimpleIntegral_amp_threshold", m_amp_threshold);
    declProp("SimpleIntegral_itg_threshold", m_itg_threshold);
    declProp("SimpleIntegral_SPEratio", m_SPEratio);
}

SimpleIntegralTool::~SimpleIntegralTool(){
}

bool SimpleIntegralTool::initialize(){
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
    m_toffset = calibsvc->getTimeOffset();

    // get hmmt pmt
    m_hmmtpmt = std::vector<int>(m_totalLPMT);
    for(int i=0;i<m_totalLPMT;i++) m_hmmtpmt[i] = int(pmtsvc->isHamamatsu(i));

    // size check
    if(m_SPEsum.size()%m_totalLPMT!=0 || m_hmmtpmt.size()%m_totalLPMT!=0 || m_toffset.size()%m_totalLPMT!=0){
        LogError << "Size check fail!" << std::endl;
        return false;
    }
    return true;
}

void SimpleIntegralTool::rec(QTInfo &qtinfo){
    if(qtinfo.b_getFHT){
        recFHT(qtinfo);
        qtinfo.b_getFHT = false;
        return;
    }

    const int isHmmt = m_hmmtpmt[qtinfo.pmtid];
    const double Qscale = m_SPEsum[qtinfo.pmtid]*m_SPEratio[isHmmt];
    const double amp_threshold = m_amp_threshold[isHmmt] * qtinfo.bsl_sigma;
    const double itg_threshold = m_itg_threshold[isHmmt] * 1500;

    auto &wf = qtinfo.wf;
    const int L = int(wf.size());
    
    int passT=0, startT=0, endT=0;
    double tmpAdcSum;
    for(int i=1;i<L;){
        tmpAdcSum = 0.;
        if(wf[i]>=amp_threshold&&wf[i-1]<=amp_threshold){
            passT = i;
            for(int j=passT;j>=0;j--) if(wf[j]<=0 || j==0){startT = j; break;}
            for(int j=passT;j<L;j++) if(wf[j]<=0 || j==L-1){endT = j; break;}
            for(int j=startT+1;j<endT-1;j++) tmpAdcSum += wf[j];
            if(tmpAdcSum>itg_threshold){
                qtinfo.charge.emplace_back(tmpAdcSum/Qscale);
                qtinfo.time.emplace_back(startT);
            }
            i = endT + 1;
        }
        else ++i;
    }
    if(qtinfo.time.size()==0) return;

    double ttq = 0;
    for(auto it=qtinfo.charge.begin();it!=qtinfo.charge.end();++it) ttq += *it;
    qtinfo.nPE = ttq;
    if(enableCalibCorr){
        for(auto itT=qtinfo.time.begin();itT!=qtinfo.time.end();++itT){
            *itT -= m_toffset[qtinfo.pmtid];
        }
    }

};

void SimpleIntegralTool::recFHT(QTInfo &qtinfo){
    const int isHmmt = m_hmmtpmt[qtinfo.pmtid];
    const double amp_threshold = m_amp_threshold[isHmmt] * qtinfo.bsl_sigma;
    const double itg_threshold = m_itg_threshold[isHmmt] * 1500;

    auto &wf = qtinfo.wf;
    const int L = int(wf.size());

    int passT=0, startT=0, endT=0;
    double tmpAdcSum;
    bool foundPulse = false;
    for(int i=1;i<L;){
        tmpAdcSum = 0.;
        if(wf[i]>=amp_threshold&&wf[i-1]<=amp_threshold){
            passT = i;
            for(int j=passT;j>=0;j--) if(wf[j]<=0 || j==0){startT = j; break;}
            for(int j=passT;j<L;j++) if(wf[j]<=0 || j==L-1){endT = j; break;}
            for(int j=startT+1;j<endT-1;j++) tmpAdcSum += wf[j];
            if(tmpAdcSum<itg_threshold) i = endT + 1;
            else{
                foundPulse = true;
                break;
            }
        }
        else ++i;
    }
    if(foundPulse){
        if(enableCalibCorr) qtinfo.firstHitTime = startT-m_toffset[qtinfo.pmtid];
        else qtinfo.firstHitTime = startT;
    }else qtinfo.firstHitTime = -100;
}

JM::CalibPmtChannel * SimpleIntegralTool::recChannel(int, std::vector<uint16_t>&){
    return nullptr;
}