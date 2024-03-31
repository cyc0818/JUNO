/*=============================================================================
#
# Author: Jilei Xu - xujl@ihep.ac.cn
# Last modified:	2016-11-13 01:39
# Filename:		RecWpMuonAlg.cc
# Description:  reference RecCdMuonAlg
=============================================================================*/
#include <boost/python.hpp>
#include "RecWpMuonAlg.h"

#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/WpCalibHeader.h"
#include "Event/WpRecHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/WpID.h"

#ifdef WITH_OEC
#include "EvtStore/EvtStoreSvc.h"
#include "Event/WpRecHeader.h"
#include "Event/TtRecHeader.h"
#include "Event/OecHeader.h"
#endif

#include <fstream>

DECLARE_ALGORITHM(RecWpMuonAlg); 

    RecWpMuonAlg::RecWpMuonAlg(const std::string& name)
    : AlgBase(name)
    , m_iEvt(1)
    , m_totPmtNum(0)
    , m_wpGeom(NULL)
    , m_buf(NULL)
      , m_recTool(NULL)
{
    declProp("RecTool", m_recToolName); 
    declProp("Pmt3inchTimeReso",  m_sigmaPmt3inch= 1); 
    declProp("Pmt20inchTimeReso",  m_sigmaPmt20inch= 8); 
    declProp("Use3inchPMT",m_flagUse3inch=false);
    declProp("Use20inchPMT",m_flagUse20inch=true);
    declProp("OutputPmtPos",m_flagOpPmtpos=false);
    declProp("recMode", s_mode="normal");
}
RecWpMuonAlg::~RecWpMuonAlg(){

#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        if (n_cluster!=NULL) delete n_cluster;
        if (dEdx!=NULL) delete dEdx;
    }
#endif

}


    bool 
RecWpMuonAlg::initialize()
{
    m_params.set("Pmt3inchTimeReso", m_sigmaPmt3inch); 
    m_params.set("Pmt20inchTimeReso", m_sigmaPmt20inch); 

    if(not iniBufSvc())return false; 
    if(not iniGeomSvc())return false; 
    if(not iniPmtPos())return false; 
    if(not iniRecTool()) return false; 

     //initialize using services for OEC
#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        TotaltrackL = 0;
        TotalPE = 0;
        ClusterNum = 0;
        n_cluster = new CriteriaItemValue;
        dEdx = new CriteriaItemValue;
        SniperPtr<EvtStoreSvc>  stsvc(getParent(), "EvtStoreSvc");
        if ( stsvc.invalid() ) {
            LogError << "cannot get the EvtStoreSvc." << std::endl;
            return false;
        }
        EvtStoreSvc *m_storeSvc = stsvc.data();
        if (!m_storeSvc->put("n_cluster",n_cluster)){
            LogError<<"error when put n_cluster!!!!!!"<<endl;
            return false;
        }
        if (!m_storeSvc->put("dEdx",dEdx)){
            LogError<<"error when put dEdx!!!!!!"<<endl;
            return false;
        }
    }
#endif

    LogInfo  << objName()
        << "   initialized successfully"
        << std::endl; 
    return true;
}

    bool 
RecWpMuonAlg::finalize()
{
    (dynamic_cast<ToolBase*>(m_recTool))->finalize(); 
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true;
}

bool 
RecWpMuonAlg::execute()
{
    LogInfo << "---------------------------------------" 
        << std::endl; 
    LogInfo << "Processing event by RecWpMuonAlg : " 
        << m_iEvt << std::endl; 

    if(not freshPmtData()) {
        return true; 
    }

    RecTrks* trks = new RecTrks();
    m_recTool->reconstruct(trks); 

    auto rh = new JM::WpRecHeader;
    auto evt = new JM::WpRecEvt();
    int n = trks->size();
    for (int i = 0; i < n; i ++) {
       TVector3 inci = trks->getStart(i);
       TVector3 Exit = trks->getEnd(i);
       double iTime = trks->getITime(i);
       double oTime = trks->getOTime(i);
       CLHEP::HepLorentzVector start(inci[0], inci[1], inci[2], iTime);
       CLHEP::HepLorentzVector end(Exit[0], Exit[1], Exit[2], oTime);
       auto rectrk = new JM::RecTrack(start, end);
       rectrk->setQuality(trks->getQuality(i));
       rectrk->setPESum(trks->getNPE(i));
       evt->addTrack(rectrk);
    }
    rh->setEvent(evt);

    auto nav = m_buf->curEvt(); 
    JM::addHeaderObject(nav, rh); 
    LogDebug  << "Done to write Rec Header" << std::endl; 
    ++m_iEvt; 

#ifdef WITH_OEC 
    if (!s_mode.compare("OEC")){

       auto tEventHeader = JM::getHeaderObject<JM::OecHeader>(nav);
       auto tEvent=dynamic_cast<JM::OecEvt*>(tEventHeader->event("JM::OecEvt"));
       MuID.clear();
       MuInX.clear();
       MuInY.clear();
       MuInZ.clear();
       MuOutX.clear();
       MuOutY.clear();
       MuOutZ.clear();
       for (int i = 0; i < n; i ++) {
           ClusterNum = trks->getClusterNum(i);
           TVector3 inci = trks->getStart(i);
           TVector3 Exit = trks->getEnd(i);
           TotaltrackL += (inci-Exit).Mag();
           TotalPE = trks->getNPE(i);
           MuID.push_back(i);
           MuInX.push_back(inci[0]);
           MuInY.push_back(inci[1]);
           MuInZ.push_back(inci[2]);
           MuOutX.push_back(Exit[0]);
           MuOutY.push_back(Exit[1]);
           MuOutZ.push_back(Exit[2]);
       }
       tEvent->setMuID(MuID);
       tEvent->setMuInX(MuInX);
       tEvent->setMuInY(MuInY);
       tEvent->setMuInZ(MuInZ);
       tEvent->setMuOutX(MuOutX);
       tEvent->setMuOutY(MuOutY);
       tEvent->setMuOutZ(MuOutZ);
       dEdx->reset();
       dEdx->setValue(TotalPE/(TotaltrackL*0.1));
       n_cluster->reset();
       n_cluster->setValue(ClusterNum);
    }
#endif

    return true;
}
bool
RecWpMuonAlg::iniRecTool(){

    m_recTool =  tool<IRecMuonTool>(m_recToolName); 
    if(not m_recTool){
        LogError << "Failed to retrieve reconstruction tool!!  "
             << "Check the tool name [\"" << m_recToolName << "\"]!!"
             << std::endl; 
        return false; 
    }
    if(not m_recTool->configure(&m_params, &m_pmtTable)) return false; 
    return true; 
}

bool 
RecWpMuonAlg::iniBufSvc(){

    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    return true; 
}
bool 
RecWpMuonAlg::iniGeomSvc(){

    //Retrieve Geometry service
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" 
            << std::endl;
        return false;
    }
    m_wpGeom = rgSvc->getWpGeom(); 
    return true; 
}
bool 
RecWpMuonAlg::iniPmtPos(){

    m_totPmtNum=m_wpGeom->getPmtNum(); 
    m_pmtTable.reserve(m_totPmtNum); 
    m_pmtTable.resize(m_totPmtNum); 
    LogDebug << "Total Pmt num from GeomSvc : " 
        << m_totPmtNum << std::endl; 
    for(unsigned int pid=0;pid<m_totPmtNum;++pid){
        Identifier Id = Identifier(WpID::id(pid, 0)); 
        PmtGeom *pmt = m_wpGeom->getPmt(Id); 
        if(!pmt){
            LogError << "Wrong Pmt Id" << Id << std::endl; 
            return false; 
        }
        TVector3 pmtCenter = pmt->getCenter(); 

        m_pmtTable[pid].pos = pmtCenter;
        if(WpID::is20inch(Id)) {
            m_pmtTable[pid].res = m_sigmaPmt20inch; 
            m_pmtTable[pid].type = _PMTINCH20; 
        }
        else {
            m_pmtTable[pid].type = _PMTNULL; 
            LogError  <<  "Pmt ["  <<  pid  
                <<  "] is neither 3 inch nor 20 inch!"  
                <<  std::endl; 
            return false; 
        }

    }

    //-----print out pmt positions  
    if(m_flagOpPmtpos){
        std::ofstream of("pmt_info.dat"); 
        for(unsigned int pid=0;pid<m_totPmtNum;++pid){
            of << pid << " : "
                << m_pmtTable[pid].pos.X() << "," 
                << m_pmtTable[pid].pos.Y() << "," 
                << m_pmtTable[pid].pos.Z() 
                << std::endl;
        }
    }

    return true; 

}
bool 
RecWpMuonAlg::freshPmtData(){
    //reset values
    for (unsigned int pid = 0; pid < m_totPmtNum; ++pid)
    {
        m_pmtTable[pid].q=-1; 
        m_pmtTable[pid].fht=999999; 
        m_pmtTable[pid].used=false; 

    }

    //read CalibHit data
    auto nav = m_buf->curEvt(); 
    if(not nav){
        LogError << "Can not retrieve the current navigator!!!" 
            << std::endl; 
        return false; 
    }
    auto chcol =
        JM::getHeaderObject<JM::WpCalibHeader>(nav); 
    if(not chcol){
        LogInfo << "Skip due to empty WpCalibHeader" 
                << std::endl; 
        return false; 
    }
    const auto& chhlist = 
        chcol->event()->calibPMTCol(); 
    auto chit = chhlist.begin(); 

    while (chit!=chhlist.end()) {

        auto calib = *chit++; 
        Identifier id = Identifier(calib->pmtId());
        Identifier::value_type value = id.getValue(); 
        Identifier::value_type vv = (value&0xFF000000)>>24;
        if (not ((value&0xFF000000)>>24 == 0x20)) { //current 0x10 CD, 0x20 WP, 0x30 TT
            continue;
        } else {
            //LogDebug << "WP PMT. " << id << std::endl;
        }
        double nPE = calib->nPE(); 
        float firstHitTime = calib->firstHitTime(); 

        unsigned pid = WpID::module(id); 

        assert(pid<m_totPmtNum);
        m_pmtTable[pid].q = nPE;
        m_pmtTable[pid].fht =firstHitTime;
        if(
                (m_flagUse20inch  &&  WpID::is20inch(id)) 
          ) {
            m_pmtTable[pid].used = true; 
        }


        LogTest <<"PMT id"<<pid << "(" 
            << m_pmtTable[pid].pos.x() << "," 
            << m_pmtTable[pid].pos.y() << "," 
            << m_pmtTable[pid].pos.z() << ")"
            <<" ; nPE ="<<nPE<<" ;  firsthit ="<<firstHitTime<<std::endl;

    }

    LogDebug << "Loading calibrated data done !" << std::endl; 
    return true; 
}
