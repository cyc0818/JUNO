#include "PSDTools/TestPSDTool.h"

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"
#include "TROOT.h"
#include "EvtNavigator/EvtNavHelper.h"

//#include "string.h"

using namespace std;
DECLARE_TOOL(TestPSDTool);

TestPSDTool::TestPSDTool(const std::string &name): ToolBase(name){
    declProp("AlignMethod", method_align);
    d_psdVar = 0;
}

TestPSDTool::~TestPSDTool(){
}

bool TestPSDTool::initialize(){
    LogInfo<<"Initializing TestPSDTool..."<<std::endl;
    //get the rootwriter
    SniperPtr<RootWriter> rwsvc(getParent(),"RootWriter");
    if ( rwsvc.invalid() ) {
        LogError << "cannot get the rootwriter service." << std::endl;
        return false;
    }

    //Store the pre-processed events
    gROOT->ProcessLine("#include <vector>");
    m_userTree=rwsvc->bookTree(*m_par,"USER_OUTPUT/evt", "Preprocess Output");
    m_userTree->Branch("PSDVar", &d_psdVar, "PSDVar/D");
    m_userTree->Branch("Time", &Time);
    m_userTree->Branch("Charge", &Charge);
    m_userTree->Branch("isHam", &isHam);
    m_userTree->Branch("PMTID", &PMTID);

    rwsvc->attach("USER_OUTPUT", m_userTree);

    //Store the PSDTools result, which may be implemented in data model in the future
    m_psdTree=rwsvc->bookTree(*m_par,"PSD_OUTPUT/PSD", "Prediction Output");
    m_psdTree->Branch("psdVal", &m_psdEvent.psdVal, "psdVal/D");
    m_psdTree->Branch("evtType", &m_psdEvent.evtType, "psdVal/I");
    rwsvc->attach("PSD_OUTPUT", m_psdTree);

//    SniperPtr<IPSDInputSvc> m_psdInput(getParent(), "PSDInputSvc");
    m_psdInput = dynamic_cast<IPSDInputSvc*>(getParent()->find("PSDInputSvc"));
    return true;
}

bool TestPSDTool::finalize(){
    LogInfo<<"Finalizing TestPSDTool..."<<std::endl;
    return true;
}

bool TestPSDTool::preProcess( JM::EvtNavigator *nav){
    LogDebug<<"pre processing an event..."<<std::endl;
    if (!m_psdInput->extractHitInfo(nav,method_align)) return false;
    Time = m_psdInput->getHitTime();
    Charge = m_psdInput->getHitCharge();
    isHam = m_psdInput->getHitIsHama();
    PMTID = m_psdInput->getHitPMTID();

    // Extract  Raw Waveform from ElecSim
    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    // only use large pmts
    if (eh!=NULL) {
        if (!m_psdInput->extractHitsWaveform(nav)) return false;
        auto& v2d_waveforms = m_psdInput->getHitsWaveform();
        //for(int i=0;i<v2d_waveforms[1].size();i++) cout << " " <<v2d_waveforms[0][i]<< " ";
        //cout<<endl;
    }
    else
        LogWarn << "Cannot find elecsim input, so we skip getting waveform!" << endl;

    d_psdVar = 100;
    if (!b_usePredict) m_userTree->Fill();
    return true;
}

double TestPSDTool::CalPSDVal(bool only_fill_TTree){
    if (not only_fill_TTree)
    {
        m_psdEvent.psdVal = 0.3;
        m_psdEvent.evtType = EVTTYPE::Electron;
    }
    else
        m_psdEvent.psdVal = -50;

    m_psdTree->Fill();
    return m_psdEvent.psdVal;
}

