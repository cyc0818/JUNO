//to do review
#include "QCtrRecAlg.h"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "Identifier/CdID.h"
//#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include "Geometry/IPMTParamSvc.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "RecTools/IRecTool.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdVertexRecHeader.h"
#include "TF1.h"
#include "TH1.h"

#ifdef WITH_OEC
#include "EvtStore/EvtStoreSvc.h"
#include "Event/CdWaveformHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/WpWaveformHeader.h"
#include "Event/TtCalibHeader.h"
#include "Event/TtElecHeader.h"
#include "Event/OecHeader.h"
#endif

#include <mutex>

DECLARE_ALGORITHM(QCtrRecAlg);

QCtrRecAlg::QCtrRecAlg(const std::string &name): AlgBase(name){
    i_ithEvt = -1;
    i_totPMTs = 0;
    m_buf = NULL;

    declProp("recMode", s_mode="normal");
    declProp("recMethod", s_recMethod="default");
}

QCtrRecAlg::~QCtrRecAlg(){
    //delete the table
    for (auto* prop: m_pmtTable) {
        delete prop;
    }

#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        if (m_energy!=NULL) delete m_energy;
        if (m_detID!=NULL) delete m_detID;
        if (m_vtxR!=NULL) delete m_vtxR;
    }
#endif
}

bool QCtrRecAlg::initialize(){

    //get the buffer
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    if (!initPMTPars()) return false;
    if (!initRecTools()) return false;

    //initialize using services for OEC
#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        m_energy=new CriteriaItemValue;//NULL;
        m_detID=new CriteriaItemValue;//NULL;
        m_vtxR = new CriteriaItemValue;//NULL;
        SniperPtr<EvtStoreSvc>  stsvc(getParent(), "EvtStoreSvc");
        if ( stsvc.invalid() ) {
            LogError << "cannot get the EvtStoreSvc." << std::endl;
            return false;
        }
        EvtStoreSvc *m_storeSvc = stsvc.data();
        if (!m_storeSvc->put("energy",m_energy)){
            LogError<<"error when put energy!!!!!!"<<endl;
            return false;
        }
        if (!m_storeSvc->put("detectorID",m_detID)){
            LogError<<"error when put detectorID!!!!!!"<<endl;
            return false;
        }
        if (!m_storeSvc->put("R",m_vtxR)){
            LogError<<"error when put vtxR!!!!!!"<<endl;
            return false;
        }
    }
#endif

    m_pmtTable.reserve(i_totPMTs);
    m_hitPmtvector.reserve(i_totPMTs);

#ifdef PERFORMANCE_CHECK
    //timer service
    SniperPtr<IJunoTimerSvc> junotimersvc(getParent(), "JunoTimerSvc");
    m_junotimersvc = junotimersvc.data();
    m_timerevent=m_junotimersvc->get("QCTREVT/timerevt");

    // user data definitions
    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
    }
    m_qctr = svc->bookTree(*m_par, "USER_OUTPUT/qctr", "simple output of qctr rec data");
    m_qctr->Branch("time_total",&time_total,"time_total/F");
    N_mem = 2;
    m_qctr->Branch("N_mem", &N_mem, "N_mem/I");
    m_qctr->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F");
#endif
    return true;
}

bool QCtrRecAlg::execute(){
#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info2;
    gSystem->GetProcInfo(&mem_info2);
    mem_resident[0] = mem_info2.fMemResident / 1024./1024.;
    m_timerevent->start();
#endif
    i_ithEvt++;
    LogInfo << "---------------------------------------" << std::endl;
    LogInfo << "Processing event " << i_ithEvt << std::endl;

    //get event Header
    auto nav = m_buf->curEvt();

#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        //store the detector type
        m_detID->reset();
        //Get calib and waveform headers
        JM::CdWaveformHeader* header_cdwave = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
        JM::CdLpmtCalibHeader* header_cdcalib = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
        JM::WpWaveformHeader* header_wpwave = JM::getHeaderObject<JM::WpWaveformHeader>(nav);
        JM::WpCalibHeader* header_wpcalib = JM::getHeaderObject<JM::WpCalibHeader>(nav);
        JM::TtElecHeader* header_ttwave = JM::getHeaderObject<JM::TtElecHeader>(nav);
        JM::TtCalibHeader* header_ttcalib = JM::getHeaderObject<JM::TtCalibHeader>(nav);
        if (header_ttcalib!=NULL || header_ttwave!=NULL){
            unsigned int detid = 3;
            m_detID->setValue(detid);
            LogDebug<<"a TT event!"<<std::endl;
            return true;
        }
        if (header_wpcalib!=NULL || header_wpwave!=NULL){
            unsigned int detid = 2;
            m_detID->setValue(detid);
            LogDebug<<"a wp event!"<<std::endl;
            return true;
        }
        if (header_cdcalib!=NULL || header_cdwave!=NULL){
            unsigned int detid = 1;
            m_detID->setValue(detid);
        }
    }
#endif

    auto chcol =JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
    if (!chcol){
        LogWarn<<"Calib header not found! Skip this event!"<<std::endl;
        return true;
    }

    // //get detector type of this event
    // bool isTT=chcol->hasTTEvent();
    // bool isCDorWP=chcol->hasEvent();
    // if ((!isTT) && (!isCDorWP)){
    //     LogWarn<<"An event from unknow sub detector!Skip"<<std::endl;
    //     return true;
    // }

    //fill the PMTTable
    fillTable(chcol->event());

    //fill the RecInfo
    m_recInfo.evtID = i_ithEvt;
    m_recInfo.evttime = nav->TimeStamp();
    m_recInfo.vtx = TVector3(0, 0, 0);
    m_recInfo.energy = 0;
    for (std::string& toolname : m_toolNames) {
        LogDebug<<"Using Tool:"<<toolname<<std::endl;
        m_recTools[toolname]->rec(m_hitPmtvector, m_pmtTable, m_recInfo);
        if(m_recInfo.energy > 100.) break; //event steering using totalPE energy
    }

    store_normal();

#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        store_OEC();
    }
#endif

#ifdef PERFORMANCE_CHECK
    // for resources test
    ProcInfo_t mem_info3;
    gSystem->GetProcInfo(&mem_info3);
    mem_resident[1] = mem_info3.fMemResident / 1024./1024.;
    m_timerevent->stop();
    time_total=m_timerevent->elapsed()/1000.;
    LogInfo << "Time consumes: " << time_total << " s"<<endl;
    m_qctr->Fill();
#endif
    return true;
}

bool QCtrRecAlg::finalize(){
    return true;
}

// static std::mutex QCtrRecAlg_initPMTPars_mutex;

bool QCtrRecAlg::initPMTPars(){
    // std::lock_guard<std::mutex> lock(QCtrRecAlg_initPMTPars_mutex);

	//get the PMT positions and parameters
	SniperPtr<IPMTParamSvc> pmtsvc(getParent(), "PMTParamSvc");
    if (pmtsvc.invalid()) {
        LogError << "Failed to get service PMTParamSvc" << std::endl;
        return true;
    }
    IPMTParamSvc *m_pmtsvc = pmtsvc.data();


    SniperPtr<PMTCalibSvc> calSvc(*getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Can't Locate  PMTCalibSvc."
                 << std::endl;
        return false;
    }

    // Read PMT parameters from CalibSVC
    LogDebug<<"Loading PMTPara from SVC(database) ..."<<std::endl;
    LogDebug<<"LGain[0]:"<<calSvc->getGain().at(0)<<std::endl;
    LogDebug<<"LDNR[0]:"<<calSvc->getDarkRate().at(0)<<std::endl;
    LogDebug<<"LPDE[0]:"<< calSvc->getRelativeDE().at(0) <<std::endl;

    i_totPMTs = m_pmtsvc->get_NTotal_CD_LPMT();
    for (unsigned int ith=0;ith<i_totPMTs;ith++){
        PMTProp *thisPMT = new PMTProp(ith);
        PMTTYPE thistype = PMTTYPE::NONE;
        if (m_pmtsvc->isHamamatsu(ith)) thistype = PMTTYPE::LPMT_DYNODE;
        else if (m_pmtsvc->isNormalNNVT(ith)) thistype = PMTTYPE::LPMT_MCP;
        else if (m_pmtsvc->isHighQENNVT(ith)) thistype = PMTTYPE::LPMT_MCP_HIGHQE;
        else if (m_pmtsvc->isHZC(ith)) thistype = PMTTYPE::SPMT;
        thisPMT->type = thistype;
        thisPMT->pos= 19.18/19.434*TVector3(m_pmtsvc->getPMTX(ith), m_pmtsvc->getPMTY(ith), m_pmtsvc->getPMTZ(ith));
        thisPMT->dnrate = calSvc->getDarkRate().at(ith);
        thisPMT->rpde = calSvc->getRelativeDE().at(ith);
        thisPMT->gain = calSvc->getChargeSpec().at(ith)->GetMean();
        /*
        htemp->Fit("gaus", "RQ0", "", 0.5, 1.5);
        TF1* fitfunc = htemp->GetFunction("gaus");
        thisPMT->gain = fitfunc->GetParameter(1);
        if(!m_pmtsvc->isHamamatsu(ith)) thisPMT->gain = htemp->GetMean();;
        delete fitfunc;
        */
        m_pmtTable.push_back(thisPMT);
    }
    return true;
}

bool QCtrRecAlg::initRecTools(){
    IRecTool *tmptool = NULL;
    m_toolNames.insert(m_toolNames.end(), {"QSumTool"}); //For event steering using totalPE info
    //Specify the name of tools to be used
    if (!s_recMethod.compare("zy")){
        m_toolNames.insert(m_toolNames.end(), {"zyQCTool"});
    }
    else if (!s_recMethod.compare("yzy")){
        m_toolNames.insert(m_toolNames.end(), {"yzyQCTool"});
    }
    else if (!s_recMethod.compare("imb")){
      m_toolNames.insert(m_toolNames.end(), {"imbQCTool"});
    }
    else if (!s_recMethod.compare("ptf")){
        m_toolNames.insert(m_toolNames.end(), {"QcRecTool", "PtfRecTool"});
    }
    else if (!s_recMethod.compare("sqmle")){
      m_toolNames.insert(m_toolNames.end(), {"SQMLE"});
    }
    else{
        LogError<<"Unknown reconstruction method!"<<std::endl;
        return false;
    }
    if(s_recMethod.compare("sqmle")) {
        m_toolNames.push_back("QSumTool");
        m_toolNames.push_back("NPEUnifCTool");
    }

    //Get the tools
    for (std::string toolname : m_toolNames){
        tmptool = tool<IRecTool>(toolname);
        if (!tmptool){
            LogError<<"Can not find the tool:"<<toolname<<std::endl;
            return false;
        }
        m_recTools[toolname] = tmptool;
    }

    //configure the tools
    for (std::map<std::string, IRecTool*>::iterator it=m_recTools.begin();it!=m_recTools.end();++it){
        if (!it->second->configure()){
            LogError<<"Fail to configure the tool:"<<it->first<<std::endl;
            return false;
        }
    }
    return true;
}

void QCtrRecAlg::fillTable(JM::CdLpmtCalibEvt *thisevent){
    m_hitPmtvector.clear();

    for (auto* pmtprop: m_pmtTable) {
        pmtprop->isused = false;
    }

    const auto& chhlist = thisevent->calibPMTCol();
    for (auto it=chhlist.begin();it!=chhlist.end();++it){
        //get PMT id
        unsigned int pmtId = (*it)->pmtId();
        Identifier id = Identifier(pmtId);
        if (not CdID::is20inch(id)) {
            continue;
        }
        unsigned int thisid = CdID::module(id);

        auto* pmtprop = m_pmtTable[thisid];
        pmtprop->pmtcalib = *it;
        pmtprop->fht = (*it)->firstHitTime();
        pmtprop->totq = (*it)->sumCharge();
        pmtprop->isused = true;

        m_hitPmtvector.push_back(pmtprop);
    }
    for (auto* pmtprop: m_pmtTable) {
        if(!pmtprop->isused){
            pmtprop->set_unused();
        }
    }
}

void QCtrRecAlg::store_normal(){
    JM::CdVertexRecHeader *tRecHeader=NULL;
    JM::CdVertexRecEvt* tRecEvent=NULL;
    auto nav = m_buf->curEvt();
    if (!JM::getHeaderObject<JM::CdVertexRecHeader>(nav)){//header not found
        tRecHeader=new JM::CdVertexRecHeader();
        JM::addHeaderObject(nav, tRecHeader);
        tRecEvent=new JM::CdVertexRecEvt();
        tRecHeader->setEvent(tRecEvent);
    }
    else{
        tRecHeader=JM::getHeaderObject<JM::CdVertexRecHeader>(nav);
        if (tRecHeader->hasEvent()){
            tRecEvent=tRecHeader->event();
        }
        else{
            tRecEvent=new JM::CdVertexRecEvt();
            tRecHeader->setEvent(tRecEvent);
        }
    }

    //store energy and vertex
    auto tRecVertex = new JM::RecVertex();
    tRecVertex->setEnergy(m_recInfo.energy);
    tRecVertex->setX(m_recInfo.vtx.X());
    tRecVertex->setY(m_recInfo.vtx.Y());
    tRecVertex->setZ(m_recInfo.vtx.Z());
    tRecEvent->addVertex(tRecVertex);
    //if (!s_recMethod.compare("yzy")){
    //    // deposit energy with Non-linear correction for positron
    //    double m_eprec = m_non_li_positron->GetX(d_energy);
    //    tRecEvent->setEprec( m_eprec );
    //    //tRecEvent->setPESum( int(m_recPESum) );
    //    TVector3 recvtx(d_vtxX, d_vtxY, d_vtxZ);
    //    tRecEvent->setPx( d_energy* recvtx.x() / recvtx.Mag() ); // needs check
    //    tRecEvent->setPy( d_energy* recvtx.y() / recvtx.Mag() );
    //    tRecEvent->setPz( d_energy* recvtx.z() / recvtx.Mag() );
    //}
}

#ifdef WITH_OEC
void QCtrRecAlg::store_OEC(){
    m_energy->reset();
    m_energy->setValue(m_recInfo.energy);
    m_vtxR->reset();
    m_vtxR->setValue(m_recInfo.vtx.Mag());

    auto nav = m_buf->curEvt();

    if (!nav) {
        LogError << " No EvtNavigator found. " << std::endl;
        return;
    }

    JM::OecHeader* tEventHeader = JM::getHeaderObject<JM::OecHeader>(nav);

    if (!tEventHeader) {
        LogError << " No OecHeader found. " << std::endl;
        return;
    }

    auto tEvent=dynamic_cast<JM::OecEvt*>(tEventHeader->event("JM::OecEvt"));

    if (!tEvent) {
        LogError << " No OecEvt found. " << std::endl;
        return;
    }


    tEvent->setEnergy(m_recInfo.energy);
    tEvent->setVertexX(m_recInfo.vtx.X());
    tEvent->setVertexY(m_recInfo.vtx.Y());
    tEvent->setVertexZ(m_recInfo.vtx.Z());
}
#endif
