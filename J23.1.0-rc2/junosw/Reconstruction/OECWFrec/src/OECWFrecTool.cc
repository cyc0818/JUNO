/*
    @auther: Wei Jiang
    Alg2Tool by jiangw@ihep.ac.cn 2023.03.03
*/
#include "OECWFrecTool.h"
#include "RecTools/IWFRecTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolBase.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Geometry/PMTParamSvc.h"
#include <SniperKernel/Sniper.h>
#include <algorithm>
#include <cmath>
#include "SniperKernel/AlgBase.h"

DECLARE_TOOL(OECWFrecTool);

OECWFrecTool::OECWFrecTool(const std::string &name): ToolBase(name){
    m_totalLPMT = 17612;
    enableSteering = true;
    declProp("TotalPMT", m_totalLPMT);
    declProp("enableSteering", enableSteering);
    declProp("WFOverTurn", WFOverTurn);

    TTQ_method = "SimpleIntegral";
    FHT_method = "SimpleIntegral";
    declProp("TTQ_method", TTQ_method);
    declProp("FHT_method", FHT_method);
    // input_FHT_method = FHT_method;
    // input_TTQ_method = TTQ_method;

    m_nbsl = 3;
    m_bsl_length = 50;
    m_steering_cut   = {   26,    27};
    declProp("Steering_Cut", m_steering_cut);

    declProp("HighEnergyWFTool", m_highEenergyWFTool = "SimpleIntegralTool");
    declProp("LowEnergyWFTool", m_lowEnergyWFTool = "DeconvolutionTool");
    declProp("RoughNPETool", m_roughNPETool = "AdcsumTool");
    m_wfrecTools = std::vector<IWFRecTool*>(3, nullptr);
}

OECWFrecTool::~OECWFrecTool(){
    for(auto& p : m_wfrecTools){
        delete p;
    }
}

bool OECWFrecTool::initialize(){
    m_idServ = IDService::getIdServ();
    m_idServ -> init();

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
    
    createSubTool(m_highEenergyWFTool, SubTools::highEenergyWFTool);
    createSubTool(m_lowEnergyWFTool, SubTools::lowEnergyWFTool);
    createSubTool(m_roughNPETool, SubTools::roughNPETool);

    if(!enableSteering){
        LogError << "Tmp not fit to disableSteering" << std::endl;
        return false;
    }

    // get hmmt pmt
    m_hmmtpmt = std::vector<int>(m_totalLPMT);
    for(int i=0;i<m_totalLPMT;i++) m_hmmtpmt[i] = int(pmtsvc->isHamamatsu(i));

    // size check
    if(m_hmmtpmt.size()%m_totalLPMT!=0){
        LogError << "Size check fail!" << std::endl;
        return false;
    }

    m_bsl_sums.resize(m_nbsl);
    return true;
}

JM::CalibPmtChannel* OECWFrecTool::recChannel(int pmtid, std::vector<uint16_t>& adc_int){
    QTInfo qtinfo(pmtid, adc_int);
    if(WFOverTurn) for(auto it=qtinfo.wf.begin(); it!=qtinfo.wf.end(); it++) *it = -*it; 
    rec(qtinfo);

    JM::CalibPmtChannel *cpc = new JM::CalibPmtChannel;
    if(qtinfo.is_valid){
        cpc->setNPE(qtinfo.nPE);
        cpc->setFirstHitTime(qtinfo.firstHitTime);
        cpc->setCharge(qtinfo.charge);
        cpc->setTime(qtinfo.time);
        // unsigned int detID = CdID::id(static_cast<unsigned int>(pmtid), 0);
        // cpc->setPmtId(detID);
        cpc->setPmtId(m_idServ->copyNo2Id(pmtid));
    }
    return cpc;
}

void OECWFrecTool::rec(QTInfo &qtinfo){
    if(!qtinfo.is_valid) return;
    if(FHT_method!="Deconvolution") subBSL_3TW(qtinfo);

    if(enableSteering){
        // tmp use Adcsum as cut
        m_wfrecTools[SubTools::roughNPETool]->rec(qtinfo);

        if(qtinfo.nPE>m_steering_cut[m_hmmtpmt[qtinfo.pmtid]])
        m_wfrecTools[SubTools::highEenergyWFTool]->rec(qtinfo);
        else{
            qtinfo.b_getFHT = true;
            m_wfrecTools[SubTools::highEenergyWFTool]->rec(qtinfo);
            m_wfrecTools[SubTools::lowEnergyWFTool]->rec(qtinfo);
        }
    }else{
        if(m_sameMethod)
        m_wfrecTools[0]->rec(qtinfo);
        else{
            qtinfo.b_getFHT = true;
            m_wfrecTools[0]->rec(qtinfo);
            m_wfrecTools[1]->rec(qtinfo);
        }
    }

    if(qtinfo.nPE<-9){ qtinfo.is_valid = false; return;} // no pulse found
    if(qtinfo.nPE< 0) qtinfo.nPE = 0.;
    if(m_sameMethod) qtinfo.firstHitTime = qtinfo.time[0];
    if(qtinfo.firstHitTime<-99){
        if(qtinfo.time.empty()){ qtinfo.is_valid = false; return;}
        else qtinfo.firstHitTime = qtinfo.time[0];
    }

}

void OECWFrecTool::subBSL_3TW(QTInfo &qtinfo)
{
    // JUNO-doc-8875
    // const int m_nbsl = 3;
    std::fill(m_bsl_sums.begin(), m_bsl_sums.end(), 0);
    auto &wf = qtinfo.wf;
    for(int i=0;i<m_nbsl;i++) for(auto it=wf.begin()+i*m_bsl_length; it!=wf.begin()+(i+1)*m_bsl_length; ++it) m_bsl_sums[i] += *it;
    auto minbsl_pos = min_element(m_bsl_sums.begin(), m_bsl_sums.end());
    double baseline = (*minbsl_pos)/m_bsl_length;
    for(auto it=wf.begin();it!=wf.end();++it) *it -= baseline;

    double tmp_sigma = 0;
    int BSL_BTime = int(minbsl_pos-m_bsl_sums.begin()) * m_bsl_length;
    int BSL_ETime = BSL_BTime + m_bsl_length;
    for(int i=BSL_BTime;i<BSL_ETime;i++) tmp_sigma += wf[i]*wf[i];
    tmp_sigma /= double(BSL_ETime-BSL_BTime);
    qtinfo.bsl_sigma = sqrt(tmp_sigma);
    return;
}

bool OECWFrecTool::createSubTool(const std::string toolName, SubTools locate){
    AlgBase* parentAlg = getFatherAlg();
    if(parentAlg == nullptr){
        LogError<<"Can't find parent Alg"<<std::endl;
        return false;
    }
    
    auto subTool = parentAlg->tool<IWFRecTool>(toolName);
    if(subTool == nullptr){
        LogError<<"Can't find the subtool: " + toolName<<std::endl;
        return false;
    }
    auto toolJson = dynamic_cast<ToolBase*>(subTool)->json();
    auto pobj = Sniper::create(toolJson.str());
    if(pobj == nullptr){
        LogError<<"Failed to create subTool: " + toolName<<std::endl;
        return false;
    }
    pobj->setParent(getParent());
    auto wfTool = dynamic_cast<IWFRecTool*>(pobj);
    wfTool->initialize();

    m_wfrecTools[locate] = (dynamic_cast<IWFRecTool*>(wfTool));

    return true;
}