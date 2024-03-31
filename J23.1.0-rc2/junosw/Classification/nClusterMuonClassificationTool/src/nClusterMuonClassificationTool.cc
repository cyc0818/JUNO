/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn, Diru Wu - wudr@ihep.ac.cn
# Last modified:	2022-1-17
# Filename:		nClusterMuonClassificationTool.cc
# Description: 
=============================================================================*/

#include "nClusterMuonClassificationTool.h"

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

DECLARE_ALGORITHM(nClusterMuonClassificationTool); 

nClusterMuonClassificationTool::nClusterMuonClassificationTool(const std::string& name)
	: ToolBase(name)
	, m_iEvt(1)
	, m_totPmtNum(0)
	, m_cdGeom(NULL)
{
	declProp("RecTool", m_recToolName); 
	declProp("Pmt3inchTimeReso",  m_sigmaPmt3inch= 1); 
	declProp("Pmt20inchTimeReso",  m_sigmaPmt20inch= 8); 
	declProp("PmtTTTimeReso", m_sigmaPmtTT = 2); // The sigma is not true, a placeholder
	declProp("Use3inchPMT",m_flagUse3inch=false);
	declProp("Use20inchPMT",m_flagUse20inch=true);
	declProp("OutputPmtPos",m_flagOpPmtpos=false);
	declProp("ChosenDetectors", m_chosenDetectors = 1);
	declProp("MaxPoints", maxpositions = 7);
	declProp("PECut", PMTThreshold_pe = 100);
	declProp("NpmtCut", PMTThreshold_n = 1);
	declProp("DistanceCut", AroundPePlus_DisCut = 5000);
}
nClusterMuonClassificationTool::~nClusterMuonClassificationTool(){}


bool nClusterMuonClassificationTool::configure()
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

	if(not iniGeomSvc())return false; 
	if(not iniPmtPos())return false; 

	LogInfo  << objName()
		<< "   initialized successfully"
		<< std::endl; 
	return true;
}

ClassificationType nClusterMuonClassificationTool::classify(JM::NavBuffer* buffer) {
/*
 * -1 = stop muon
 *  0 = clipping muon
 *  1 = single muon
 *  2 = double muon
 */

	if (not buffer) {
		LogInfo << "The buffer is NULL" << std::endl;
		return ClassificationType::kUnknown;
	}

	if (not freshPmtData(buffer)) {
		LogInfo << "freshPmtData error" << std::endl;
		return ClassificationType::kUnknown;
	}

	const PmtTable& ptab = m_pmtTable;
	unsigned int pmtnum  = ptab.size();
	const double DEG = 180. / TMath::Pi();
	double RADIUS_LS = 16500; //m, the best value
	double RADIUS_BALL = 19500; //m, pmt ring ball
	int AroundPePlus_PeCut = PMTThreshold_pe;//1st deal fit points, merge same points, for deal edge points
	int FitPointDiscard_PeCut = PMTThreshold_pe;  //>FitPointDiscard_PeCut
	double PointMerge_DisCut = 3000;     // <PointMerge_DisCut, merge, default is 3m
	int rec_pecut = PMTThreshold_pe;//for re-rec used
	double rec_discut = AroundPePlus_DisCut;//m, for re-rec used
	double trackL, pmtq, pmtfht;
	TVector3 vtmp, point, X1, X2, line1, line2, line3, dir, vpos;
	TVector3 center(0,0,0);
	TVector3 fitp(0,0,1);
	PmtPe.clear();
	OptPmtPe.clear();
	vpe_around.clear();
	PmtPos.clear();
	OptPmtPos.clear();
	vfitp.clear();
	rectracks.clear();
	qwpoints.clear();
	reqwpoints.clear();
	vrec_in.clear();
	vrec_out.clear();

	double totalpe = 0;
	for (unsigned int i = 0; i < pmtnum; ++ i) {
		if (ptab[i].used == 1) {
			if (ptab[i].type == _PMTINCH20) {
				pmtq = ptab[i].q;
				pmtfht = ptab[i].fht;
				vpos = ptab[i].pos;
				PmtPe.push_back(pmtq);
				PmtPos.push_back(vpos);
				if (pmtq > PMTThreshold_pe && pmtfht < 500) {
					OptPmtPe.push_back(pmtq);
					OptPmtPos.push_back(vpos);
				}
				totalpe += pmtq;
			}
		}
	}

	if (totalpe < 24000) {
		LogInfo << "Total PE is smaller than 24000" << std::endl;
		return ClassificationType::kUnknown;
	}

	if (int(OptPmtPe.size()) < PMTThreshold_n) {
		LogInfo << " Can't select enough pmts: " << OptPmtPe.size() << " < " << PMTThreshold_n << " Npmt cut." << std::endl;
		LogDebug << "Return kUnknown" << std::endl;
		return ClassificationType::kUnknown;
	}

	TSpectrum2* spec2 = new TSpectrum2(maxpositions);
	TH2F* h_all1 = new TH2F("h_all1", "", 100, -180, 180, 100, -90, 90); //all pmt pe that > pe threshold
	//project selected pmts on to theta phi plane
	for (unsigned int i = 0; i < OptPmtPe.size(); i ++) {
		double theta = OptPmtPos[i].Theta();
		double phi = OptPmtPos[i].Phi();
		double fillx = phi * DEG;
		double filly = -theta*DEG + 90;
		h_all1->Fill(fillx, filly, OptPmtPe[i]);
	}
	//TSpectrum fit
	LogDebug << " 1. fitting by TSpectrum2 ..." << std::endl;
	int nAllxy = spec2->Search(h_all1, 2, "noMarkov goff");
	Double_t *xpeaks = spec2->GetPositionX();
	Double_t *ypeaks = spec2->GetPositionY();
	double rectheta, recphi;
	for (int i = 0; i < nAllxy; i ++) {
		rectheta = - (ypeaks[i] - 90) / DEG; //unfolding theta
		recphi = xpeaks[i] / DEG;
		fitp.SetMagThetaPhi(RADIUS_BALL, rectheta, recphi);
		vfitp.push_back(fitp);
	}
	delete h_all1;
	delete spec2;

	for (unsigned int i = 0; i < vfitp.size(); i ++) {
		LogDebug << " Fit point vfitp xyz: " << i << " : (" << vfitp[i].X() << ", " << vfitp[i].Y() << ", " <<vfitp[i].Z() << ")" << std::endl;
	}

	if (nAllxy > 0 ) {
		//according fit points, see around all pe status 
		LogDebug << " 2. According fit points, see around pe status ..." << std::endl;
		int nFitp = nAllxy;
		int nerase = 0;
		for (int ip = 0; ip < nAllxy; ip ++) {
			double npe = 0;
			for (int i = 0; i < int(PmtPe.size()); i ++) {
				if (PmtPe[i] <= AroundPePlus_PeCut) continue;
				double dis = (PmtPos[i] - vfitp[ip]).Mag();
				if (dis > AroundPePlus_DisCut) continue;
				npe += PmtPe[i];
			}
			if (npe > FitPointDiscard_PeCut) {
				vpe_around.push_back(npe);
			}
			else {//if fit points pe is too low, discard it
				vfitp.erase(vfitp.begin() + ip - nerase);
				LogDebug << "  MARK: this events around pe is smaller than " << FitPointDiscard_PeCut << " pe, so discard it." << std::endl;
				nFitp --;
				nerase ++;
			}
		}
		//merge too near points   
		LogDebug << " 3. deal too close events, if 2 points <" << PointMerge_DisCut << " mm merge them" << std::endl;
		int vfitpL = vfitp.size();
		for (int ip = 0; ip < vfitpL - 1; ip ++) {
			for (int jp = ip + 1; jp < vfitpL; jp ++) {
				TVector3 vd = vfitp[ip] - vfitp[jp];
				if (vd.Mag() < PointMerge_DisCut) {
					LogDebug << "  WARNING: this 2 fit points is  too close: " << vd.Mag() << " mm, " << std::endl;
					double charge = vpe_around[ip] + vpe_around[jp];
					point = 1. * (vfitp[ip] * vpe_around[ip] + vfitp[jp] * vpe_around[jp]) * (1. / (vpe_around[ip] + vpe_around[jp]));
					vfitp.erase(vfitp.begin() + jp);//can not reverse with next line
					vfitp.erase(vfitp.begin() + ip);
					vfitp.push_back(point);
					vpe_around.erase(vpe_around.begin() + jp);
					vpe_around.erase(vpe_around.begin() + ip);
					vpe_around.push_back(charge);
					ip = 0;
					jp = 0;
					vfitpL = vfitp.size();
				}
			}
		}

		for (int i = 0; i < vfitp.size(); i ++) {
			LogDebug << "  After selection, vfitp left: " << vfitp.size() << " points, " << i << ", xyz(" << vfitp[i].X() << " " << vfitp[i].Y() << " " << vfitp[i].Z() << "), pe_around: " << vpe_around[i] << std::endl;
		}

		//rec fitting points by charge weighted ...
		nFitp = vfitp.size();
		LogDebug << "  fitting points left finally: " << nFitp << std::endl;
		LogDebug << " 4. rec fitting points by charge weighted ... " << std::endl;
		for (int j = 0; j < nFitp; j ++) {//here can not used vfitp.size() instead nFitp
			int maxpos = max_element(vpe_around.begin(), vpe_around.end()) - vpe_around.begin();
			vtmp = vfitp[maxpos];
			vpe_around.erase(vpe_around.begin() + maxpos);
			vfitp.erase(vfitp.begin() + maxpos);

			double rec_x_tmp = 0;
			double rec_y_tmp = 0;
			double rec_z_tmp = 0;
			double npe_tmp = 0;

			//rec in point
			for (unsigned int i = 0; i < PmtPe.size(); i ++) {
				if (PmtPe[i] <= rec_pecut) continue;
				double dis = (PmtPos[i] - vtmp).Mag();
				if (dis > rec_discut) continue;
				rec_x_tmp += PmtPos[i].X() * PmtPe[i];
				rec_y_tmp += PmtPos[i].Y() * PmtPe[i];
				rec_z_tmp += PmtPos[i].Z() * PmtPe[i];
				npe_tmp += PmtPe[i];
			}

			if (npe_tmp >0) {
				rec_x_tmp /= npe_tmp;
				rec_y_tmp /= npe_tmp;
				rec_z_tmp /= npe_tmp;
				point.SetXYZ(rec_x_tmp, rec_y_tmp, rec_z_tmp);
				qwpoints.push_back(point);
			}
		}

		//check repeat same rec points, since large distance cut
		if (qwpoints.size() >= 2) {
			for (int ic = 0; ic < int(qwpoints.size()) - 1; ic ++) {
				for (int id =ic + 1; id < int(qwpoints.size()); ) {
					X1 = qwpoints[ic];
					X2 = qwpoints[id];
					if ((X1 - X2).Mag() < 10)
						qwpoints.erase(qwpoints.begin() + id);
					else id ++;
				}
			}
		}
	}

	LogInfo << "Returning" << std::endl;

	int nLines = qwpoints.size() / 2;

	if (qwpoints.size() == 1) {
		LogInfo << "kMuonStop" << std::endl;
		return ClassificationType::kMuonStop;
	}
	else if (qwpoints.size() <= 4) {
		LogInfo << "kMuonSingle" << std::endl;
		return ClassificationType::kMuonSingle;
	}
	else {
		LogInfo << "kMuonBundles" << std::endl;
		return ClassificationType::kMuonBundles;
	}

	LogInfo << "Return kUnknown" << std::endl;
	return ClassificationType::kUnknown;
}

bool
nClusterMuonClassificationTool::iniGeomSvc(){

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
nClusterMuonClassificationTool::iniPmtPos(){

	for (int i = 0; i < m_offsets.size(); i ++)
		m_totPmtNum += m_offsets[i];
	m_pmtTable.reserve(m_totPmtNum); 
	m_pmtTable.resize(m_totPmtNum); 

	std::map<Identifier, PmtGeom*>::const_iterator TtIt;
	if (TtUsed)
		TtIt = m_ttGeom->cbegin();

	LogInfo << "Total Pmt num from GeomSvc : " 
		<< m_totPmtNum << std::endl; 


	for(unsigned int pid = 0; pid < m_totPmtNum; ++ pid){
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

		if (!pmt) {
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
nClusterMuonClassificationTool::freshPmtData(JM::NavBuffer* buffer){

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
	auto nav = buffer->curEvt();
	if(not nav){
		LogError << "Can not retrieve the current navigator!!!" 
			<< std::endl; 
		return false; 
	}

	LogInfo << "Loading lpmt info" << std::endl;
        // CD-LPMT
	auto chcol_lpmt =
            JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav); 
	if(chcol_lpmt and chcol_lpmt->event()){
            const auto& chlist = 
		chcol_lpmt->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

	LogInfo << "Loading spmt info" << std::endl;
        // CD-SPMT
	auto chcol_spmt =
            JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav); 
	if(chcol_spmt and chcol_spmt->event()){
            const auto& chlist = 
		chcol_spmt->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

	LogInfo << "Loading wp info" << std::endl;
        // WP
	auto chcol_wp =
            JM::getHeaderObject<JM::WpCalibHeader>(nav); 
	if(chcol_wp and chcol_wp->event()){
            const auto& chlist = 
		chcol_wp->event()->calibPMTCol(); 
            freshPmtDataHelper(chlist);
	}

	LogInfo << "Loading tt info" << std::endl;
	// Load TT Calib Data
	auto ttHdr = JM::getHeaderObject<JM::TtCalibHeader>(nav);
	if (ttHdr and ttHdr->event()) {
		auto ttEvt = dynamic_cast<JM::TtCalibEvt*>(ttHdr->event());
		const auto& ttcol = ttEvt->calibTtCol();
		freshPmtDataHelper(ttcol);
	}

	LogInfo << "Loading calibrated data done !" << std::endl; 
	return true; 
}

bool
nClusterMuonClassificationTool::freshPmtDataHelper(const std::list<JM::CalibPmtChannel*>& chlist){
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
nClusterMuonClassificationTool::freshPmtDataHelper(const std::list<JM::CalibTtChannel*>& ttcol){
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
