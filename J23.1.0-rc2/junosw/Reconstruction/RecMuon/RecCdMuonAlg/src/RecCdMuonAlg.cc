/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified:	2015-05-11 01:39
# Filename:		RecCdMuonAlg.cc
# Description: 
=============================================================================*/

#include "RecCdMuonAlg.h"

#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdTrackRecHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"

#ifdef WITH_OEC
#include "EvtStore/EvtStoreSvc.h"
#include "Event/WpRecHeader.h"
#include "Event/TtRecHeader.h"
#include "Event/OecHeader.h"
#endif

#include <fstream>

DECLARE_ALGORITHM(RecCdMuonAlg); 

    RecCdMuonAlg::RecCdMuonAlg(const std::string& name)
    : AlgBase(name)
    , m_iEvt(1)
    , m_totPmtNum(0)
    , m_cdGeom(NULL)
    , m_buf(NULL)
      , m_recTool(NULL)
{
    declProp("RecTool", m_recToolName); 
    declProp("Pmt3inchTimeReso",  m_sigmaPmt3inch= 1); 
    declProp("Pmt20inchTimeReso",  m_sigmaPmt20inch= 8); 
    declProp("Use3inchPMT",m_flagUse3inch=true);
    declProp("Use20inchPMT",m_flagUse20inch=true);
    declProp("OutputPmtPos",m_flagOpPmtpos=false);
    declProp("recMode", s_mode="normal");
}
RecCdMuonAlg::~RecCdMuonAlg(){

#ifdef WITH_OEC
    if (!s_mode.compare("OEC")){
        if (n_cluster!=NULL) delete n_cluster;
        if (dEdx!=NULL) delete dEdx;
    }
#endif

}


    bool 
RecCdMuonAlg::initialize()
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
RecCdMuonAlg::finalize()
{
    (dynamic_cast<ToolBase*>(m_recTool))->finalize(); 
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true;
}

    bool 
RecCdMuonAlg::execute()
{
    LogInfo << "---------------------------------------" 
        << std::endl; 
    LogInfo << "Processing event by RecCdMuonAlg : " 
        << m_iEvt << std::endl; 

    if(not freshPmtData()) {
        m_iEvt ++;
        return true;
    }

	  RecTrks* trks = new RecTrks();
    m_recTool->reconstruct(trks);

    auto rh = new JM::CdTrackRecHeader();
    auto evt = new JM::CdTrackRecEvt(); 
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
    LogDebug  << "Done to write RecTrack event" << std::endl;
    ++m_iEvt;

#ifdef WITH_OEC 
    if (!s_mode.compare("OEC")){

       //auto tEventHeader=dynamic_cast<JM::OECHeader*>(nav->getHeader("/Event/OEC"));
       //auto tEvent=dynamic_cast<JM::OECEvent*>(tEventHeader->event("JM::OECEvent"));
       JM::OecHeader* tEventHeader = JM::getHeaderObject<JM::OecHeader>(nav);
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
RecCdMuonAlg::iniRecTool(){

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
RecCdMuonAlg::iniBufSvc(){

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
RecCdMuonAlg::iniGeomSvc(){

    //Retrieve Geometry service
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" 
            << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom(); 
    return true; 
}
bool 
RecCdMuonAlg::iniPmtPos(){

    m_totPmtNum=m_cdGeom->getPmtNum(); 
    m_pmtTable.reserve(m_totPmtNum); 
    m_pmtTable.resize(m_totPmtNum); 
    LogDebug << "Total Pmt num from GeomSvc : " 
        << m_totPmtNum << std::endl; 
    for(unsigned int pid=0;pid<m_totPmtNum;++pid){
        Identifier Id = Identifier(CdID::id(pid, 0)); 
        PmtGeom *pmt = m_cdGeom->getPmt(Id); 
        if(!pmt){
            LogError << "Wrong Pmt Id" << Id << std::endl; 
            return false; 
        }
        TVector3 pmtCenter = pmt->getCenter(); 

        m_pmtTable[pid].pos = pmtCenter;
        if (CdID::is3inch(Id)) {
            m_pmtTable[pid].res = m_sigmaPmt3inch; 
            m_pmtTable[pid].type=_PMTINCH3; 
        }
        else if(CdID::is20inch(Id)) {
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
RecCdMuonAlg::freshPmtData(){

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

    bool isDataLoaded = false;

    // LPMT
    auto chcol =
        JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav); 
    if(chcol and chcol->event()){
        const auto& chlist = 
            chcol->event()->calibPMTCol(); 
        freshPmtDataHelper(chlist);
        isDataLoaded = true;
    }

    // SPMT
    auto chcol_spmt =
        JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav); 
    if(chcol_spmt and chcol_spmt->event()){
        const auto& chlist = 
            chcol_spmt->event()->calibPMTCol(); 
        freshPmtDataHelper(chlist);
        isDataLoaded = true;
    }


    LogDebug << "Loading calibrated data done !" << std::endl; 
    return isDataLoaded; 
}

bool 
RecCdMuonAlg::freshPmtDataHelper(const std::list<JM::CalibPmtChannel*>& chlist){
    auto chit = chlist.begin(); 

    int counter_lpmt = 0;
    int counter_spmt = 0;

    while (chit!=chlist.end()) {

        auto calib = *chit++; 
        Identifier id = Identifier(calib->pmtId());
        // skip if not CD.
        Identifier::value_type value = id.getValue(); 
        Identifier::value_type vv = (value&0xFF000000)>>24;
        if (not ((value&0xFF000000)>>24 == 0x10)) { 
            //current 0x10 CD, 0x20 WP, 0x30 TT
            continue;
        }
        double nPE = calib->nPE(); 

        unsigned pid = CdID::module(id);
	// Change ">=" in if to ">" because of the different number of PMTs in the new version. 2021/4/25 Diru Wu
        if (pid>m_totPmtNum) {
            LogError << "Data/Geometry Mis-Match: PmtID ("<< pid<< ")> m_totPmtNum ("<< m_totPmtNum <<")" << std::endl;
            return false;
        }
        m_pmtTable[pid].q = nPE;
        m_pmtTable[pid].hittime = calib->time();
        m_pmtTable[pid].hitq = calib->charge();
        hits.reset(new TH1F("", "", 1500, -250, 1250));
        hits->SetDirectory(0);
        for (int i = 0; i < m_pmtTable[pid].hittime.size(); i ++) {
            hits->Fill(m_pmtTable[pid].hittime[i], m_pmtTable[pid].hitq[i]);
        }
        int firstHitTime = hits->GetMaximumBin();
        firstHitTime -= 250;
        m_pmtTable[pid].fht = firstHitTime;
        if((m_flagUse3inch  &&  CdID::is3inch(id)) ||
                (m_flagUse20inch  &&  CdID::is20inch(id)) 
          ) {
            m_pmtTable[pid].used = true; 

            if (CdID::is3inch(id)) ++counter_spmt;
            if (CdID::is20inch(id)) ++counter_lpmt;

        }


        LogTest <<"PMT id"<<pid << "(" 
            << m_pmtTable[pid].pos.x() << "," 
            << m_pmtTable[pid].pos.y() << "," 
            << m_pmtTable[pid].pos.z() << ")"
            <<" ; nPE ="<<nPE<<" ;  firsthit ="<<firstHitTime<<std::endl;

    }

    LogInfo << "number of PMTs will be used (Lpmt/Spmt): "
            << counter_lpmt << "/" << counter_spmt << std::endl;

    return true;
}
