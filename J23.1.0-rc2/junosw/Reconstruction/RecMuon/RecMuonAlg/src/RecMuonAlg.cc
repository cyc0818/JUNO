/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn, Diru Wu - wudr@ihep.ac.cn
# Last modified:	2022-1-17
# Filename:		RecMuonAlg.cc
# Description: 
=============================================================================*/

#include "RecMuonAlg.h"

#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtCalibHeader.h"
#include "Event/CdTrackRecHeader.h"
#include "Event/WpRecHeader.h"
#include "Event/TtRecHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "Identifier/WpID.h"
#include "Identifier/TtID.h"

#include <fstream>

DECLARE_ALGORITHM(RecMuonAlg); 

	RecMuonAlg::RecMuonAlg(const std::string& name)
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
	declProp("PmtTTTimeReso", m_sigmaPmtTT = 2); // The sigma is not true, a placeholder
	declProp("Use3inchPMT",m_flagUse3inch=true);
	declProp("Use20inchPMT",m_flagUse20inch=true);
	declProp("OutputPmtPos",m_flagOpPmtpos=false);
	declProp("ChosenDetectors", m_chosenDetectors = 1);
}
RecMuonAlg::~RecMuonAlg(){}


	bool 
RecMuonAlg::initialize()
{
	m_params.set("Pmt3inchTimeReso", m_sigmaPmt3inch); 
	m_params.set("Pmt20inchTimeReso", m_sigmaPmt20inch); 

	CdUsed = false;
	WpUsed = false;
	TtUsed = false;

	if (m_chosenDetectors == 1 ||
		m_chosenDetectors == 3 ||
		m_chosenDetectors == 5 ||
		m_chosenDetectors == 7)
		CdUsed = true;
	if (m_chosenDetectors == 2 ||
		m_chosenDetectors == 3 ||
		m_chosenDetectors == 6 ||
		m_chosenDetectors == 7)
		WpUsed = true;
	if (m_chosenDetectors == 4 ||
		m_chosenDetectors == 5 ||
		m_chosenDetectors == 6 ||
		m_chosenDetectors == 7)
		TtUsed = true;

	if(not iniBufSvc())return false; 
	if(not iniGeomSvc())return false; 
	if(not iniPmtPos())return false; 
	if(not iniRecTool()) return false; 

	LogInfo  << objName()
		<< "   initialized successfully"
		<< std::endl; 
	return true;
}

	bool 
RecMuonAlg::finalize()
{
	(dynamic_cast<ToolBase*>(m_recTool))->finalize(); 
	LogInfo  << objName()
		<< "   finalized successfully" 
		<< std::endl; 
	return true;
}

	bool 
RecMuonAlg::execute()
{
	LogInfo << "---------------------------------------" 
		<< std::endl; 
	LogInfo << "Processing event by RecMuonAlg : " 
		<< m_iEvt << std::endl; 

	if(not freshPmtData())return true; 

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
	//nav->addHeader("/Event/WpRecEvt", rh); 
	LogDebug  << "Done to write RecTrack event" << std::endl; 
	++m_iEvt; 

	return true;
}
bool
RecMuonAlg::iniRecTool(){

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
RecMuonAlg::iniBufSvc(){

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
RecMuonAlg::iniGeomSvc(){

	//Retrieve Geometry service
	SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc"); 
	if ( rgSvc.invalid()) {
		LogError << "Failed to get RecGeomSvc instance!" 
			<< std::endl;
		return false;
	}

	if (CdUsed) {
		m_cdGeom = rgSvc->getCdGeom();
		m_offsets.push_back(m_cdGeom->getPmtNum());
	}
	if (WpUsed) {
		m_wpGeom = rgSvc->getWpGeom();
		m_offsets.push_back(m_wpGeom->getPmtNum());
	}
	if (TtUsed) {
		m_ttGeom = rgSvc->getTtGeom();
		m_offsets.push_back(m_ttGeom->getChannelNum());
	}

	for (int i = 0; i < m_offsets.size(); i ++)
		LogDebug << m_offsets[i] << std::endl;

	LogDebug << "CdUsed: " << CdUsed << std::endl
			<< "WpUsed: " << WpUsed << std::endl
			<< "TtUsed: " << TtUsed << std::endl;

	return true;
}
bool 
RecMuonAlg::iniPmtPos(){

	for (int i = 0; i < m_offsets.size(); i ++)
		m_totPmtNum += m_offsets[i];
	m_pmtTable.reserve(m_totPmtNum); 
	m_pmtTable.resize(m_totPmtNum); 

	std::map<Identifier, PmtGeom*>::const_iterator TtIt;
	if (TtUsed)
		TtIt = m_ttGeom->cbegin();

	LogInfo << "Total Pmt num from GeomSvc : " 
		<< m_totPmtNum << std::endl; 


	for(unsigned int pid=0;pid<m_totPmtNum;++pid){
		Identifier Id;
		PmtGeom* pmt;
		if (pid < m_offsets[0]) {
			if (CdUsed) {
				Id = Identifier(CdID::id(pid, 0));
				pmt = m_cdGeom->getPmt(Id);
				m_pmtTable[pid].loc = 1;
			}
			else if (WpUsed) {
				Id = Identifier(WpID::id(pid, 0));
				pmt = m_wpGeom->getPmt(Id);
				m_pmtTable[pid].loc = 2;
			}
			else {
				Id = TtIt->first;
				pmt = TtIt->second;
				TtIt ++;
				m_pmtTable[pid].loc = 3;
			}
		}
		else if (m_offsets.size() > 1 && pid < m_offsets[1] + m_offsets[0]) {
			if (m_chosenDetectors == 3 ||
				m_chosenDetectors == 7) {
				Id = Identifier(WpID::id(pid - m_offsets[0], 0));
				pmt = m_wpGeom->getPmt(Id);
				m_pmtTable[pid].loc = 2;
			}
			else {
				Id = TtIt->first;
				pmt = TtIt->second;
				TtIt ++;
				m_pmtTable[pid].loc = 3;
			}
		}
		else if (m_offsets.size() == 3) {
			Id = TtIt->first;
			pmt = TtIt->second;
			TtIt ++;
			m_pmtTable[pid].loc = 3;
		}
		else {
			m_pmtTable[pid].type = _PMTNULL; 
			LogError  <<  "Pmt ["  <<  pid  
				<<  "] is out of range!"  
				<<  std::endl; 
			return false; 
		}

		if(!pmt){
			LogError << "Wrong Pmt Id" << Id << std::endl; 
			return false; 
		}

		TVector3 pmtCenter = pmt->getCenter(); 
		m_pmtTable[pid].pos = pmtCenter;

		if (m_pmtTable[pid].loc == 1 && CdID::is3inch(Id)) {
			m_pmtTable[pid].res = m_sigmaPmt3inch; 
			m_pmtTable[pid].type=_PMTINCH3; 
		}
		else if (m_pmtTable[pid].loc == 1 && CdID::is20inch(Id)) {
			m_pmtTable[pid].res = m_sigmaPmt20inch; 
			m_pmtTable[pid].type = _PMTINCH20; 
		}
		else if (m_pmtTable[pid].loc == 2 && WpID::is20inch(Id)) {
			m_pmtTable[pid].res = m_sigmaPmt20inch;
			m_pmtTable[pid].type = _PMTINCH20;
		}
		else if (m_pmtTable[pid].loc == 3) {
			m_pmtTable[pid].res = m_sigmaPmtTT;
			m_pmtTable[pid].type = _PMTTT;
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
	/*
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
	*/

	return true; 

}
bool 
RecMuonAlg::freshPmtData(){

	LogInfo << "Freshing PMT data" << std::endl;
	//reset values
	for (unsigned int pid = 0; pid < m_totPmtNum; ++pid)
	{
		m_pmtTable[pid].q=-1; 
		m_pmtTable[pid].fht=999999; 
		m_pmtTable[pid].used=false; 

	}

        // TODO: this part should be updated. In the design, 
        // the hits from different detectors will be put together.
        // however, the new scheme of EDM splits the calib into 
        // different detectors.
        // Tao Lin, 2022.03.09

	//read CalibHit data
	auto nav = m_buf->curEvt(); 
	if(not nav){
		LogError << "Can not retrieve the current navigator!!!" 
			<< std::endl; 
		return false; 
	}

        // CD-LPMT
	auto chcol_lpmt =
            JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav); 
	if(chcol_lpmt and chcol_lpmt->event()){
            const auto& chlist = 
		chcol_lpmt->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

        // CD-SPMT
	auto chcol_spmt =
            JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav); 
	if(chcol_spmt and chcol_spmt->event()){
            const auto& chlist = 
		chcol_spmt->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

        // WP
	auto chcol_wp =
            JM::getHeaderObject<JM::WpCalibHeader>(nav); 
	if(chcol_wp and chcol_wp->event()){
            const auto& chlist = 
		chcol_wp->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

	// Load TT Calib Data
	auto ttHdr = JM::getHeaderObject<JM::TtCalibHeader>(nav);
	if (ttHdr and ttHdr->event()) {
		auto ttEvt = dynamic_cast<JM::TtCalibEvt*>(ttHdr->event());
		const auto& ttcol = ttEvt->calibTtCol();
		freshPmtDataHelper(ttcol);

	}

	LogDebug << "Loading calibrated data done !" << std::endl; 
	return true; 
}

bool
RecMuonAlg::freshPmtDataHelper(const std::list<JM::CalibPmtChannel*>& chlist){
	int offset = 0;
	if (CdUsed)
		offset = m_offsets[0];

	auto chit = chlist.begin(); 

	while (chit!=chlist.end() && (CdUsed || WpUsed)) {

		auto calib = *chit++; 
		Identifier id = Identifier(calib->pmtId());
		Identifier::value_type value = id.getValue(); 
		unsigned pid;
		if ((value&0xFF000000)>>24 == 0x10 && CdUsed) { 
			pid = CdID::module(id);
			m_pmtTable[pid].loc = 1;
			m_pmtTable[pid].value = value;
			// current 0x10 CD, 0x20 WP, 0x30 TT
		}
		else if ((value & 0xFF000000) >> 24 == 0x20 && WpUsed) {
			pid = WpID::module(id) + offset;
			m_pmtTable[pid].loc = 2;
			m_pmtTable[pid].value = value;
		}
		else
			continue;
		double nPE = calib->nPE(); 
		float firstHitTime = calib->firstHitTime(); 

		if (pid>m_totPmtNum) {
			LogError << "Data/Geometry Mis-Match: PmtID ("<< pid<< ")> m_totPmtNum ("<< m_totPmtNum <<")" << std::endl;
			return false;
		}
		m_pmtTable[pid].q = nPE;
		m_pmtTable[pid].fht =firstHitTime;
		m_pmtTable[pid].hittime = calib->time();
		m_pmtTable[pid].hitq = calib->charge();

		if (m_pmtTable[pid].loc == 1) {
			if((m_flagUse3inch  &&  CdID::is3inch(id)) ||
					(m_flagUse20inch  &&  CdID::is20inch(id)) 
			  ) {
				m_pmtTable[pid].used = true; 
			}
		}
		else {
			if (m_flagUse20inch && WpID::is20inch(id)) {
				m_pmtTable[pid].used = true;
			}
		}


		/*
		   LogTest <<"PMT id"<<pid << "(" 
		   << m_pmtTable[pid].pos.x() << "," 
		   << m_pmtTable[pid].pos.y() << "," 
		   << m_pmtTable[pid].pos.z() << ")"
		   <<" ; nPE ="<<nPE<<" ;  firsthit ="<<firstHitTime<<std::endl;
		   */

	}

	return true;
}

bool
RecMuonAlg::freshPmtDataHelper(const std::list<JM::CalibTtChannel*>& ttcol){
	int offset = 0;
	if ((CdUsed || WpUsed) && !(CdUsed && WpUsed))
		offset = m_offsets[0];
	else if (CdUsed && WpUsed)
		offset = m_offsets[0] + m_offsets[1];
	else
		offset = 0;
	int pid = offset;
	auto ttit = ttcol.begin();

	while (ttit != ttcol.end() && TtUsed) {
		auto ttch = *ttit ++;
		Identifier id = TtID::id(ttch->pmtId());
		m_pmtTable[pid].value = id.getValue();
		m_pmtTable[pid].q = ttch->nPE();
		m_pmtTable[pid].fht = ttch->time();
		m_pmtTable[pid].used = true;
		pid ++;
		if (pid > m_totPmtNum) {
			LogError << "The number of calibTTChannel exceeds the size of pmt table."
				<< std::endl;
			return false;
		}
	}

	return true;
}
