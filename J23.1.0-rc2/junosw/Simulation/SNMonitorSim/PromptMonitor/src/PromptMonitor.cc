#include "PromptMonitor.h"

#include "SniperKernel/AlgFactory.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "TSystem.h"
DECLARE_ALGORITHM(PromptMonitor);

PromptMonitor::PromptMonitor(const std::string &name):AlgBase(name){
    d_T=1000;//ns
    m_muTime_tmp.clear();
    m_muTime.clear();
    m_snTime.clear();
    m_prnd = new TRandom3(0);

    m_vetoRate=3.6;//Hz
    m_vetoTime = 1.5e-3;//s 
    m_bkgRate = 0.37; //400/3600./24.;//Hz 
    //----------for event type-----------
    i_NpmtLcut=11500;
    i_NpmtHcut=29000;
    //-------for muon like event----------
    m_slideT_mu = 100e-3; //s
    m_freshT_mu = 1e-3;
    m_Nthr_mu   = 8; //9;
    //---------for sn like event-------
    m_slideT_sn = 80e-3;
    m_freshT_sn = 1e-3;
    m_Nthr_sn   = 4; //5;

    i_isTriggered=0;
    d_muTriggerTime=0;
    d_snTriggerTime=0;
}

PromptMonitor::~PromptMonitor(){
}

bool PromptMonitor::initialize(){
    //get the buffer
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = dynamic_cast<JM::NavBuffer*>(navBuf.data());

    //user output
    SniperPtr<RootWriter> rwsvc(getParent(), "RootWriter");
    if (rwsvc.invalid()){
        LogError<<"can not get the root writer service!"<<std::endl;
        return false;
    }
    ttrig=rwsvc->bookTree(*m_par, "ttrig", "trigger information");
    ttrig->Branch("isTriggered", &i_isTriggered, "isTriggered/I");
    ttrig->Branch("muTriggerTime", &d_muTriggerTime, "muTriggerTime/D");
    ttrig->Branch("snTriggerTime", &d_snTriggerTime, "snTriggerTime/D");
    tevt=rwsvc->bookTree(*m_par, "tevt", "event information");
    tevt->Branch("is_mulike", &b_ismulike, "is_mulike/O");
    tevt->Branch("is_snlike", &b_issnlike, "is_snlike/O");
    tevt->Branch("time", &d_time, "time/D");
    tevt->Branch("totFiredPMTs", &i_totFiredPMTs, "totFiredPMTs/I");
    tevt->Branch("Qedep",     &d_Qedep,            "Qedep/D");
    tevt->Branch("QedepX",    &d_QedepX,           "QedepX/D");
    tevt->Branch("QedepY",    &d_QedepY,           "QedepY/D");
    tevt->Branch("QedepZ",    &d_QedepZ,           "QedepZ/D");
    rwsvc->attach("SN_TRIGGER", ttrig);
    rwsvc->attach("SN_TRIGGER", tevt);

    return true;
}

bool PromptMonitor::finalize(){
    findTrigger();
    ttrig->Fill();
    return true;
}

bool PromptMonitor::execute(){
    initOutput();
    //monitor the memory usage
    ProcInfo_t info;
    gSystem->GetProcInfo(&info);
    float f_memory = info.fMemResident/1024;
    LogInfo<<"memory used is:"<<f_memory<<std::endl;
    //get the current calib event
    auto thisnav=m_buf->curEvt();
    auto headerCalib=JM::getHeaderObject<JM::CdLpmtCalibHeader>(thisnav);
    if (!headerCalib->hasEvent()){
        LogInfo<<"CalibEvent doesn't exist, skip it!"<<std::endl;
        return true;
    }

    //global trigger time, the same as that from elecfeecrate
    TTimeStamp &globalTriggerTime=thisnav->TimeStamp();
    double gtTime=globalTriggerTime.AsDouble();
    //double gtTime=1e9*globalTriggerTime.AsDouble();
    d_time=gtTime;
    LogInfo<<"global trigger time is:"<<gtTime<<std::endl;
    //categorize into muon-like and sn-like
    i_totFiredPMTs=getNFiredPMT();//getNFiredPMTbyTruth(gtTime);
    LogDebug<<"total fired pmts:"<<i_totFiredPMTs<<std::endl;
    if (i_totFiredPMTs<=0) return true;
    if (i_totFiredPMTs>i_NpmtHcut){//muon like
        m_muTime_tmp.push_back(gtTime);
        b_ismulike=true;
    }
    else if (i_totFiredPMTs>i_NpmtLcut){//sn like
        m_snTime.push_back(gtTime);
        b_issnlike=true;
    }

    //getEventUser();

    tevt->Fill();
    return true;
}

int PromptMonitor::getNFiredPMT(){//here NFiredPMT means total number of hits
    double tbeg=100;
    double tend=100+d_T;
    //first get the calibEvent
    auto thisnav=m_buf->curEvt();
    auto headerCalib=JM::getHeaderObject<JM::CdLpmtCalibHeader>(thisnav);
    if (!headerCalib->hasEvent()){
        LogInfo<<"CalibEvent doesn't exist, skip it!"<<std::endl;
        return -1;
    }
    auto eventCalib=headerCalib->event();
    const auto& cpc=eventCalib->calibPMTCol();
    //count the number of hits in time range tbeg to tend
    int Npulse=0;
    int Ntot=0;
    //std::vector<double> vtime;
    for (auto cit=cpc.begin();cit!=cpc.end();++cit){
        const auto& ctime=(*cit)->time();
        for (auto tit=ctime.begin();tit!=ctime.end();++tit){
            Ntot++;
            //vtime.push_back(*tit);
            if ((*tit)>tbeg && (*tit)<tend) Npulse++;
        }
    }
    //std::sort(vtime.begin(), vtime.end());
    //std::cout<<"Ntot:"<<Ntot<<"    Npulse:"<<Npulse<<"    earlist time:"<<vtime.at(0)<<std::endl;
    return Npulse;
}

int PromptMonitor::getNFiredPMTbyTruth(double gtTime){
    //first get hit time and pmt id of this event
    std::vector<double> m_hittime;
    std::vector<int> m_PMTID;
    auto thisnav=m_buf->curEvt();
    auto headerElecTruth=JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(thisnav);
    if (!headerElecTruth->hasEvent()){
        LogWarn<<"can not get fired pmt number of LPMT!"<<std::endl;
        return -1;
    }
    auto eventLpmtTruth=headerElecTruth->event();
    const auto& lpmttruth=eventLpmtTruth->truths();
    for (auto it=lpmttruth.begin();it!=lpmttruth.end();++it){
        const auto& lpmt = *it;
        double hittime = lpmt.pulseHitTime().GetSeconds(); 
        LogDebug<<"pmt hit time:"<<hittime<<std::endl;
        m_hittime.push_back(hittime);
        int pmtID = lpmt.pmtId();
        m_PMTID.push_back(pmtID);
    }

    //get the number of fired pmts in time range d_T relative to gtTime
    double itbeg=gtTime;
    double itend=gtTime+d_T/1e9;
    std::vector<double>::iterator ilow, iup;
    ilow = std::lower_bound(m_hittime.begin(), m_hittime.end(), itbeg);
    iup  = std::lower_bound(m_hittime.begin(), m_hittime.end(), itend);
    int indexlow = std::distance(m_hittime.begin(), ilow);
    int indexup  = std::distance(m_hittime.begin(), iup);

    int npmts = std::distance(ilow, iup);
    std::vector<int> ivecPMT(npmts);
    std::vector<int>::iterator itpmt;
    itpmt = std::unique_copy(m_PMTID.begin()+indexlow,m_PMTID.begin()+indexup,ivecPMT.begin());
    std::sort(ivecPMT.begin(),itpmt);
    itpmt = std::unique_copy(ivecPMT.begin(),itpmt,ivecPMT.begin(),uniqueFcn);
    ivecPMT.resize(std::distance(ivecPMT.begin(),itpmt));

    return ivecPMT.size();
}

void PromptMonitor::initOutput(){
    i_totFiredPMTs=0;
    b_ismulike=false;
    b_issnlike=false;
    d_Qedep=0;
    d_QedepX=0;
    d_QedepY=0;
    d_QedepZ=0;
}

void PromptMonitor::getEventUser(){
    //get qedep from trackelectruth
    auto thisnav=m_buf->curEvt();
    auto headerElecTruth=JM::getHeaderObject<JM::TrackElecTruthHeader>(thisnav);
    if (headerElecTruth==NULL){
        LogWarn<<"elec truth header is not found!"<<std::endl;
        d_Qedep=100;
        d_QedepX=0;
        d_QedepY=0;
        d_QedepZ=0;
        return;
    }
    if (!headerElecTruth->hasEvent()){
        LogWarn<<"can not get track truth!"<<std::endl;
        return;
    }
    auto eventTrackTruth=headerElecTruth->event();
    const auto& trktruth=eventTrackTruth->truths();

    if (trktruth.size()>=1){
        d_Qedep=0;
        for (auto it=trktruth.begin();it!=trktruth.end();++it){
            d_Qedep+=it->Qedep();
        }
        d_QedepX=trktruth.at(0).initX();
        d_QedepY=trktruth.at(0).initY();
        d_QedepZ=trktruth.at(0).initZ();
    }
    else{
        //for e+ sample, this case corresponds to after pulse
        d_Qedep=100;
        d_QedepX=0;
        d_QedepY=0;
        d_QedepZ=0;
    }
}

void PromptMonitor::findTrigger(){
    generateRndVeto(-60,60);//2 min
    generateRndBkg(-12*3600,12*3600);//1 day 

    double start_t = -0.5;
    double end_t   = 5;
    //monitor of mu-like events
    Long64_t nstep_mu = (end_t-start_t)/m_freshT_mu;
    for(Long64_t istep=0; istep<nstep_mu; istep++){
        double ibeg = start_t+m_freshT_mu*istep-m_slideT_mu;
        double iend = ibeg+m_slideT_mu;

        //mu-like candidate
        std::vector<double>::iterator ilow_mu, iup_mu;
        ilow_mu = std::lower_bound(m_muTime.begin(), m_muTime.end(), ibeg);
        iup_mu  = std::lower_bound(m_muTime.begin(), m_muTime.end(), iend);
        int Nmu = std::distance(ilow_mu, iup_mu);
        if(Nmu>m_Nthr_mu){
            LogInfo<< "Triggered by mu-like events !!!---[" << iend << " s]" <<std::endl;
            d_muTriggerTime = ibeg+m_slideT_mu;
            i_isTriggered = 2;
            break;
        }
    }
    
    //monitor of sn-like events
    Long64_t nstep_sn = (end_t-start_t)/m_freshT_sn;
    for(Long64_t istep=0; istep<nstep_sn; istep++){
        double ibeg = start_t+m_freshT_sn*istep-m_slideT_sn;
        double iend = ibeg+m_slideT_sn;

        //sn-like candidate
        std::vector<double>::iterator ilow_sn, iup_sn;
        ilow_sn = std::lower_bound(m_snTime.begin(), m_snTime.end(), ibeg);
        iup_sn  = std::lower_bound(m_snTime.begin(), m_snTime.end(), iend);
        int Nsn = std::distance(ilow_sn, iup_sn);
        if(Nsn>m_Nthr_sn){
            LogInfo<< "Triggered by sn-like events !!!---[" << iend << " s]" <<std::endl;
            d_snTriggerTime = ibeg+m_slideT_sn;
            if(i_isTriggered == 2) i_isTriggered = 3;
            else i_isTriggered = 1;

            break;
        }
    }
}

void PromptMonitor::generateRndVeto(double tbeg, double tend){
    LogInfo<< "generate muon events..." << std::endl;
    int nveto = (tend-tbeg)*m_vetoRate;
    for(int ievt=0; ievt<nveto; ievt++){
        double itime = m_prnd->Uniform(tbeg, tend);
        //std::cout << "bkg time " << itime << std::endl;
        //vec_veto.push_back(itime);
        m_muTime_tmp.push_back(itime);
    }
    std::sort(m_muTime_tmp.begin(), m_muTime_tmp.end());

    //remove muon events within veto time
    LogInfo<< "remove mu-like events within veto time..." << std::endl;
    int nevts = m_muTime_tmp.size();
    for(int ievt=0; ievt<nevts;){
        double itime = m_muTime_tmp[ievt];
        m_muTime.push_back(itime);
        if(ievt<nevts-1){
            for(int jevt=ievt+1; jevt<nevts;jevt++){
                if(m_muTime_tmp[jevt]>(itime+m_vetoTime)){
                    ievt = jevt;
                    break;
                }
                if(jevt==nevts-1)ievt = jevt;
            }
        }
        else{
            ievt++;
        }
    }
}

void PromptMonitor::generateRndBkg(double tbeg, double tend){
    LogInfo<< "generate background events..." << std::endl;
    int nbkgs = (tend-tbeg)*m_bkgRate;
    for(int ievt=0; ievt<nbkgs; ievt++){
        double itime = m_prnd->Uniform(tbeg, tend);
        m_snTime.push_back(itime);
    }
    std::sort(m_snTime.begin(), m_snTime.end());

    //remove sn-like events within veto time
    LogInfo<< "number of SN-like events: " << m_snTime.size() << std::endl;
    for(int ievt=0;ievt<int(m_muTime.size());ievt++){
        double itbeg = m_muTime[ievt];
        double itend = itbeg+m_vetoTime;

        std::vector<double>::iterator ilow_sn, iup_sn;
        ilow_sn = std::lower_bound(m_snTime.begin(), m_snTime.end(), itbeg);
        iup_sn  = std::lower_bound(m_snTime.begin(), m_snTime.end(), itend);
        m_snTime.erase(ilow_sn, iup_sn);
    }
    LogInfo<< "After muon veto: " << m_snTime.size() << std::endl;
}
