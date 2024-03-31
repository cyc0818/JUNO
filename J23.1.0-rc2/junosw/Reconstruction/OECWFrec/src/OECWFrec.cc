/*
    @author: Wei Jiang(IHEP)
    @E-mail: jiangw@ihep.ac.cn
    v0:
        Combined Deconvolution and SimpleIntegral for OEC waveform reconstruction @2022.11
*/

#ifndef OECWFrec_cc
#define OECWFrec_cc

#include "OECWFrec.h"
#include "OECWFrecTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Task.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include <TSystem.h>
using namespace std;

DECLARE_ALGORITHM(OECWFrec);

OECWFrec::OECWFrec(const string& name): AlgBase(name)
{
    m_subTools = {"SimpleIntegralTool", "DeconvolutionTool", "AdcsumTool", "OECWFrecTool"};
    declProp("SubTools", m_subTools);
    declProp("TotalPMT", m_totalLPMT);
}


OECWFrec::~OECWFrec(){
}


bool OECWFrec::initialize()
{
    LogInfo << "Here to initialize" <<std::endl;

    SniperPtr<PMTParamSvc> pmtParamSvc(getParent(), "PMTParamSvc");
    if (pmtParamSvc.invalid()) {
        LogError << "Failed to get PMTParamSvc instance!" << std::endl;
        return false;
    }
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
        return false;
    }

    oecwfrecTool = tool<OECWFrecTool>("OECWFrecTool");
    if(oecwfrecTool == nullptr){
        LogError << "Failed to get OECWFrecTool instance!" << std::endl;
        return false;
    }
    oecwfrecTool->setFatherAlg(this);
    if(!oecwfrecTool->initialize()){
        LogError<<"Failed to init OECWFrecTool"<<std::endl;
        return false;
    }

#ifdef PERFORMANCE_CHECK    
    //timer service
    SniperPtr<IJunoTimerSvc> junotimersvc(getParent(), "JunoTimerSvc");
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("CALIBEVT/timerevt");

    // user data definitions
    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
    }
    m_calib = svc->bookTree(*m_par, "USER_OUTPUT/calib", "simple output of calib rec data");    
    m_calib->Branch("time_total",&time_total,"time_total/F");
    N_mem = 2;   
    m_calib->Branch("N_mem", &N_mem, "N_mem/I");   
    m_calib->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F"); 
#endif         
    m_evt = 0;
    return true;
}


bool OECWFrec::execute()
{
#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info2;
    gSystem->GetProcInfo(&mem_info2);
    mem_resident[0] = mem_info2.fMemResident / 1024./1024.;
    m_timerevent->start();
#endif

    LogInfo << "start OEC simple waveform rec, now at evt: " << m_evt++ << endl;
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(),"/Event");
    if (navBuf.invalid()) {
        LogWarn << "can't find the buffer. " << std::endl;
        return false;
    }
    auto jstat = dynamic_cast<Task*>(getRoot())->Snoopy().state();
    if (jstat != Sniper::RunState::Running && jstat != Sniper::RunState::Ready) {
        LogWarn << "Stop running." << std::endl;
        return true;
    }
    auto nav = navBuf->curEvt();
    if (!nav) {
        LogWarn << "can't load event navigator." << std::endl;
        return dynamic_cast<Task*>(getRoot())->stop();
    }
    auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
    if (!eh) {
        LogWarn << "No waveform header is found. skip. " << std::endl;
        return true;
    }
    if (!eh->hasEvent()) return true;
    
    auto ee = (eh->event());
    const auto& feeChannels = ee->channelData();
    list<JM::CalibPmtChannel*> cpcl;//CalibPMTChannel list
    for(auto it=feeChannels.begin();it!=feeChannels.end();++it){
        if(it->first >= m_totalLPMT) continue;

        auto cpc = oecwfrecTool->recChannel(it->first, it->second->adc());
        if(cpc->size()>0) cpcl.emplace_back(cpc);
        else delete cpc;
    }

    auto ce = new JM::CdLpmtCalibEvt;
    ce->setCalibPMTCol(cpcl);
    auto ch = new JM::CdLpmtCalibHeader;
    ch->setEvent(ce);
    JM::addHeaderObject(nav, ch);
 
#ifdef PERFORMANCE_CHECK 
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[1] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_calib->Fill();
#endif
    LogDebug << "end OECWFrec" << endl;
    return true;
}

bool OECWFrec::finalize()
{
    LogInfo << "Finalize successfully! " << endl;
    return true;
}

#endif
