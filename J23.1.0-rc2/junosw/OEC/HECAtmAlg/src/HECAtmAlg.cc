#include "SniperKernel/AlgFactory.h"
#include "Event/OecHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "TSystem.h"
#include "TMath.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath> 
#include <iomanip>
//#include "Event/ElecHeader.h"

#include "TFile.h"
#include "TTree.h"

#include "HECAtmAlg.h"
DECLARE_ALGORITHM(HECAtmAlg);

HECAtmAlg::HECAtmAlg(const std::string &name):AlgBase(name){
    declProp("dT", d_dt);
    declProp("Qbf", Qbf);
    declProp("thre_E", thre_E);
}

HECAtmAlg::~HECAtmAlg(){

}

bool HECAtmAlg::initialize(){
    LogInfo << "initializing HECAtmAlg..." << std::endl;
#ifdef PERFORMANCE_CHECK 
    // for resources test
    ProcInfo_t mem_info0;
    gSystem->GetProcInfo(&mem_info0);
    mem_resident[0] = mem_info0.fMemResident / 1024./1024.;
#endif
    //get navbuffer
    SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<std::endl;
        return false;
    }
    m_buf = navbuf.data();
    i_nEvt = 0;
    //get OECConfig service
    SniperPtr<OECTagSvc> tagsvc(getParent(),"OECTagSvc"); 
    if( tagsvc.invalid()) {
        LogError << "Unable to locate tagsvc" << std::endl;
        return false;
    }
    m_tagsvc = tagsvc.data();
    //get predefined tags
    std::string m_tagList=m_tagsvc->getpTag("AtmoFC");
    FCtag = strtoll(m_tagList.c_str(), NULL, 16);
    m_tagList = m_tagsvc->getpTag("CDAtmoPC");
    CdPCtag = strtoll(m_tagList.c_str(), NULL, 16);
    m_tagList = m_tagsvc->getpTag("WPAtmoPC");
    WpPCtag = strtoll(m_tagList.c_str(), NULL, 16);
    LogDebug << std::hex << FCtag << std::dec << std::endl;
    LogInfo << "Initializing tag list done." << std::endl;

    corEvts.reserve(MaxCorrEvtNum);

#ifdef PERFORMANCE_CHECK    
    //timer service
    SniperPtr<IJunoTimerSvc> junotimersvc(getRoot(), "JunoTimerSvc");
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("HECALGEVT/timerevt");

    // user data definitions
    SniperPtr<RootWriter> svc(getRoot(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
    }
    m_hecatmalg = svc->bookTree(*m_par, "USER_OUTPUT/hecatmalg", "simple output of qctr rec data");    
    m_hecatmalg->Branch("time_total",&time_total,"time_total/F");
    N_mem = 4;   
    m_hecatmalg->Branch("N_mem", &N_mem, "N_mem/I");   
    m_hecatmalg->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F"); 

    // for resources test
    ProcInfo_t mem_info1;
    gSystem->GetProcInfo(&mem_info1);
    mem_resident[1] = mem_info1.fMemResident / 1024./1024.;
#endif

    return true;
}

bool HECAtmAlg::finalize(){
    LogInfo << "finalizing HECAtmAlg..." << std::endl;

    return true;
}

bool HECAtmAlg::execute(){
#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info2;
    gSystem->GetProcInfo(&mem_info2);
    mem_resident[2] = mem_info2.fMemResident / 1024./1024.;
    m_timerevent->start();
#endif

    i_nEvt++;
    LogDebug << "The " << std::dec << i_nEvt << "th event" << std::endl;
    //get this event
    JM::EvtNavigator* tnav=m_buf->curEvt();
    JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tnav); 
    if(tHeaderOEC==NULL){
        LogDebug<<"OecHeader is NULL!"<<std::endl;
        return true;
    }   
    JM::OecEvt* m_tEvent = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& thistime = m_tEvent->getTime();
    LogDebug << "Trigger Time: " << std::fixed << std::setprecision(0) << (double)thistime*1e9 << " ns" << std::endl; 
    int tagst = m_tEvent->getTag();
    LogDebug << "Energy: " << std::fixed << std::setprecision(2) << m_tEvent->getEnergy() << std::endl;
    LogDebug << "pre_Tag: " << std::hex << tagst << std::dec << std::endl; 
    int with_WP = 0;
    int with_CD = 0;
    int tagv;
    corEvts.clear();

    // Time Window ( If Time sorting achieve, can add break. )
    // --------------- FC Identification
    if((!m_tEvent->getIDA().compare("CD")) && m_tEvent->getEnergy()>thre_E) {
        with_WP = 0;
        for (JM::NavBuffer::Iterator navit=m_buf->begin();navit!=m_buf->end();++navit){
            JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(navit->get());
            JM::OecEvt *aEventOEC = dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
            const TTimeStamp& afterTime=aEventOEC->getTime();
            double dt=(afterTime-thistime)*1e9;
            LogDebug << "dt: " << std::fixed << std::setprecision(0) << dt << std::endl;
            if(std::abs(dt)>d_dt) continue;
            tagv = aEventOEC->getTag();
            if(!aEventOEC->getIDA().compare("WP")) with_WP++;
            LogDebug << "with_WP: " << with_WP << std::endl;
        }
        if(with_WP==0) m_tEvent->addTag(FCtag);
    }
    // --------------- PC Identification
    else if(!m_tEvent->getIDA().compare("WP")) {
        LogDebug << "WP Trigger!." << std::endl;
        with_CD = 0;
        double qbf = m_tEvent->getQBF();
        for (JM::NavBuffer::Iterator navit=m_buf->begin();navit!=m_buf->end();++navit){
            JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(navit->get());
            JM::OecEvt *aEventOEC = dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
            const TTimeStamp& afterTime=aEventOEC->getTime();
            double dt=(afterTime-thistime)*1e9;
            if(std::abs(dt)>d_dt) continue;
            tagv = aEventOEC->getTag();
            if(!aEventOEC->getIDA().compare("CD")) {
                corEvts.push_back(aEventOEC);
                with_CD++;
                LogDebug << "CD correlated." << std::endl;
            }
        }
        LogDebug << "hec_qbf: " << std::fixed << std::setprecision(2) << qbf << std::endl;
        if(with_CD>0 && qbf>=0 && qbf<Qbf) {
            m_tEvent->addTag(WpPCtag);
            for(std::vector<JM::OecEvt*>::iterator it = corEvts.begin(); it != corEvts.end(); ++it){
                JM::OecEvt* hit = *it;
                if (hit->getEnergy()>thre_E) hit->addTag(CdPCtag);
                LogDebug<<"tag: "<<std::hex<<hit->getTag()<< std::dec<<std::endl;
                if((hit->getTag() | ~CdPCtag)==0xFFFFFFFF) LogDebug<<"CdPCtag has been added."<<endl;
                else LogDebug<<"CdPCtag add error!"<<endl;
            }
        }
    }
    LogDebug << "tag: " << std::hex << m_tEvent->getTag() << std::dec << std::endl;

#ifdef PERFORMANCE_CHECK    
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[3] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;  
    m_hecatmalg->Fill();
#endif  

    return true;
}
