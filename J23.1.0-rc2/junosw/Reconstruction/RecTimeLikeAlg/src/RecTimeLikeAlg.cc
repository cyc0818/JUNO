/*=============================================================================
#
# Author: Jingyu Luo - ljys1234@mail.ustc.edu.cn 
# modified: 2013-12-20 15:30
# Author: Ziyuan Li - liziyuan3@mail.sysu.edu.cn
# modified: 2019-12-12
#	Add Peak Time Fitter algorithm
#	Add ToyMC function for study
#	Add DarkNoise PDF
# modified: 2020-05-01
#   Add TMinuit to replace GridSearch
#   Solve DarkNoise issue at detector border
#	Add fos correction and bias correction
# modified: 2021-07-01
#	Add r-z-dist tres PDF and charge map
# Filename: RecTimeLikeAlg.cc
# Description: The algorithm includes the vertex reconstruction based on realtive
hit time likelihood, the visible energy reconstruction based on charge likelihood
and the non-linearity correction for positron
# Notification: the vertex reconstruction and visible energy reconstrucion got tuned,
The non-linearity need to be further considered.
=============================================================================*/
#include "RecTimeLikeAlg.h"

#include <boost/format.hpp>

namespace fs = boost::filesystem;

DECLARE_ALGORITHM(RecTimeLikeAlg); 

RecTimeLikeAlg::RecTimeLikeAlg(const std::string& name):AlgBase(name)
    , m_conddb_svc(nullptr)
{
	m_iEvt = -1; 

	Total_num_PMT = 17612; //new 17612
	PMT_R = 19.434;
	Ball_R = 19.18;
	LS_R = 17.7;
	pmt_r = 0.254;
	m_isElec = 0;
	m_isSaveMC = false;
	m_tlh_maxnpe = 50;
	m_ptf_maxnpe = 100;

    m_TimeWindow = 1250; // need to get from DAQ
	m_LightYield = 1000; // need to get from calibration

	m_VtxCorr = 0; //0:no corr; 1:corr with edep; 2:corr with rec_Energy
	m_PmtType = 0;

	m_neff = 1.54;
	flag_dn = true;

	m_effective_attenuation = 0.026;//0.0365

	declProp("TotalPMT", Total_num_PMT);
	declProp("PMT_R", PMT_R);
	declProp("Ball_R",Ball_R);
	declProp("LS_R", LS_R);
	declProp("pmt_r", pmt_r);
	declProp("File_path", File_path);
	//no:off; yes:elec simulation and waveform rec on
	declProp("Pdf_Value",m_isElec);
	//1:1D PDF; 2:3D PDF
	declProp("SwitchAlg", m_Algorithm);
	//save MC truth information (only work for ideal case)
	declProp("SaveMC", m_isSaveMC);
	//0:no corr; 1:bias correction
	declProp("VtxCorr",m_VtxCorr);
	//0:all; 1:Hamamastu PMT only; 2:MCP PMT only
	declProp("SwitchPMTType", m_PmtType);

	// initialize the pointer
	for (int i=0; i<5; i++){
		file_Time_nhit_mcp[i] = nullptr;
		file_Time_nhit_ham[i] = nullptr;
	}
	file_eneuniformcorr = nullptr;
	file_PmtData = nullptr;
	file_vtxbiascorr = nullptr;
	file_chargemap = nullptr;

	gr_eneneff = nullptr;
	fcn = nullptr;
	vtxllfcn = nullptr;

  h1_Hittime = new TH1D("h1_Hittime", "h1_Hittime; t_{i} [ns]", 100, -50, 1250);
  h1_Hittime->SetDirectory(nullptr);

  tmp_hit_time = new TH1D("tmp_hit_time","",5000,-20,2000);
  tmp_hit_time->SetDirectory(nullptr);

	v_res_time.resize(Total_num_PMT);
}

RecTimeLikeAlg::~RecTimeLikeAlg()
{
	for (int i=0; i<5; i++){
		delete file_Time_nhit_mcp[i];
		delete file_Time_nhit_ham[i];
	}
	delete file_eneuniformcorr;
	delete file_PmtData;

	delete file_vtxbiascorr;
	delete file_chargemap;
	delete gr_eneneff;
	delete fcn;
	delete vtxllfcn;


	delete h1_Hittime;
  delete tmp_hit_time;
	//delete fun_ccscale;
}

bool RecTimeLikeAlg::initialize()
{
	//Event navigator
	SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
	if ( navBuf.invalid() ) {
		LogError << "cannot get the NavBuffer @ /Event" << std::endl;
		return true;
	}
	m_buf = navBuf.data();

	//PMT Geom&Param service
	SniperPtr<IPMTParamSvc> pmtsvc(getParent(), "PMTParamSvc");
    if (pmtsvc.invalid()) {
        LogError << "Failed to get service PMTParamSvc" << std::endl;
        return true;
    }
    m_pmtsvc = pmtsvc.data();

	// Temporary modifications
	std::string m_db_path;
	if (getenv("JUNOTOP")) {
		std::string s = getenv("JUNOTOP");
		s += "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root";
		if (boost::filesystem::exists(s)) {
			m_db_path = s;
		}
	}

	if (getenv("WORKTOP")) {
		std::string s = getenv("WORKTOP");
		s += "/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root";
		if (boost::filesystem::exists(s)) {
			m_db_path = s;
		}
	}

	if (!m_db_path.empty() && boost::filesystem::exists(m_db_path)) {
        LogInfo << "Loading parameters from file: " << m_db_path << std::endl;
    } else {
        LogError << "Can't find PMT parameters file '" << m_db_path << "'." << std::endl;
        return false;
    }

	TFile *PMTSimParamFile = TFile::Open(m_db_path.c_str());
    TTree *PMTSimParamTree = (TTree*)PMTSimParamFile->Get("data");

	int _pmtId;
	double _timeSpread;
	double _darkRate;

  // Only enable used branch
  PMTSimParamTree->SetBranchStatus("*", false);

  PMTSimParamTree->SetBranchStatus("pmtID", true);
	PMTSimParamTree->SetBranchAddress("pmtID",&_pmtId);
  PMTSimParamTree->SetBranchStatus("DCR", true);
  PMTSimParamTree ->SetBranchAddress("DCR",&_darkRate);
  PMTSimParamTree->SetBranchStatus("TTS_SS", true);
  PMTSimParamTree ->SetBranchAddress("TTS_SS",&_timeSpread);

	//Cond Database
	SniperPtr<CondDB::ICondDBSvc> conddb(getParent(), "CondDBSvc");
    if (conddb.invalid()) {
        LogError << "Failed to get CondDBSvc!" << std::endl;
        LogError << "CondDB will not be used during reconstruction. " << std::endl;
	} else {
	    m_conddb_svc = conddb.data();
    }

	//Read Calibration Param
	SniperPtr<PMTCalibSvc> pmtcalSvc(getParent(), "PMTCalibSvc");
	if ( pmtcalSvc.invalid()) {
		LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
		return true;
	}
	pmtcal_RelativeDE = pmtcalSvc->getRelativeDE();
	pmtcal_Gain = pmtcalSvc->getGain();
	pmtcal_MeanGain = pmtcalSvc->getMeanGain();
	pmtcal_TimeOffset = pmtcalSvc->getTimeOffset();
	pmtcal_DarkRate = pmtcalSvc->getDarkRate();
	pmtcal_ChargeSpec = pmtcalSvc->getChargeSpec();

	for (int i=0; i<5; i++){
		LogDebug << "ipmt = " << i << ", pmtcal_RelativeDE = " << pmtcal_RelativeDE.at(i) << ", pmtcal_Gain = " << pmtcal_Gain.at(i) << ", pmtcal_MeanGain = " << pmtcal_MeanGain.at(i) << ", pmtcal_TimeOffset = " << pmtcal_TimeOffset.at(i) << ", pmtcal_DarkRate = " << pmtcal_DarkRate.at(i) << std::endl;
	}

	//Load the hit time likelihood function
	bool stLoad = Load_LikeFun();
  if (not stLoad) return false;

	//Book User Tree;
	Book_Tree();

	//Get the geometry service for all PMT;
	Total_num_PMT = m_pmtsvc->get_NTotal_CD_LPMT();
	LogDebug << "Total PMT: " << Total_num_PMT << std::endl;

	//Get PMT position
  ALL_PMT_pos.reserve(Total_num_PMT);
  TimeSpread.reserve(Total_num_PMT);
  DarkRate.reserve(Total_num_PMT);

	for(int i = 0; i<Total_num_PMT; i++){
		TVector3 all_pmtCenter(m_pmtsvc->getPMTX(i), m_pmtsvc->getPMTY(i), m_pmtsvc->getPMTZ(i));
		ALL_PMT_pos.push_back(Ball_R/PMT_R*all_pmtCenter);

		PMTSimParamTree->GetEntry(i);
		TimeSpread.push_back(_timeSpread * 2.354); //sigma to FWHM
		DarkRate.push_back(_darkRate * 1000); //kHz
	}

	//Use Minimizer for energy reconstruction
	fcn = new MyFCN(this);
	recminimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

	//Use Minimizer for vertex reconstruction
	vtxllfcn = new VertexRecLikelihoodFCN(this);
	vtxllminimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

  // PMTSimParamTree does not need deletion (https://root.cern/manual/object_ownership/)
	delete PMTSimParamFile;
	LogDebug << objName() << " initialized successfully" << std::endl;

	return true;
}

bool RecTimeLikeAlg::execute()
{
	TStopwatch timer;
	timer.Start();
	++m_iEvt;

	LogDebug << "---------------------------------------" << std::endl;
	LogDebug << "Processing event " << m_iEvt << std::endl;
	auto nav = m_buf->curEvt();

	//read Simulation data
	if ( m_isSaveMC ){
		auto simheader = JM::getHeaderObject<JM::SimHeader>(nav);
		auto se = (JM::SimEvt*)simheader->event();
		auto sim_trk = (JM::SimTrack*) se->findTrackByTrkID(0);
		m_init_x = sim_trk->getInitX();
		m_init_y = sim_trk->getInitY();
		m_init_z = sim_trk->getInitZ();
		m_init_t = sim_trk->getInitT();
		m_edep   = sim_trk->getEdep();
		m_edep_x = sim_trk->getEdepX();
		m_edep_y = sim_trk->getEdepY();
		m_edep_z = sim_trk->getEdepZ();

		m_init_r = TMath::Sqrt(m_init_x*m_init_x+m_init_y*m_init_y+m_init_z*m_init_z);
		m_edep_r = TMath::Sqrt(m_edep_x*m_edep_x+m_edep_y*m_edep_y+m_edep_z*m_edep_z);
	}

	//read CalibHit data
	auto chcol = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
	if (!chcol) {
		LogWarn << "The CalibHeader does not exist in this event. " << std::endl;
		return true;
	}
	if (!chcol->event()) {
		LogWarn << "The CalibEvent does not exist in this event. " << std::endl;
		return true;
	}

	const auto& chhlist = chcol->event()->calibPMTCol();

  PMT_HITS.clear();
  PMT_HITS.reserve(chhlist.size());

  for(const auto& calib : chhlist) {
		unsigned int cur_pmtId = calib->pmtId();
		Identifier id = Identifier(cur_pmtId);

		//only use 20inch pmt
		if (not CdID::is20inch(id)) {
			continue;
		}

    PMT_HITS.push_back(PMTHit {
        id, calib->nPE(), calib->firstHitTime(), TimeSpread[CdID::module(id)], DarkRate[CdID::module(id)]
    });
  }

	LogDebug << "Done to read CalibPMT" << std::endl;

	if ( m_isElec ){
		//Correct MCP long tail charge spectrum with elec simulation
		RecTimeLikeAlg::NpeCorrection();

		//Tag potential dark noise, to improve charge center accuracy
		RecTimeLikeAlg::DarkNoiseFilter();
	}

	//Hamamastu or MCP PMT only
  if ( m_PmtType ){
    RecTimeLikeAlg::SwitchPMTType();
  }

	RecTimeLikeAlg::VectorPush();


	min_hit_time = *min_element(Readout_hit_time.begin(),Readout_hit_time.end());
	LogDebug << "minimum hit time = " << min_hit_time << std::endl;

	num_PMT = PMT_HITS.size();
	LogDebug << "numer of Fired PMT = " << num_PMT << std::endl;   

	//Charge Center Reconstruction
	RecTimeLikeAlg::ChargeCenterRecCorr();

	//Peak Time Fitter to provide more precise initial vtx value to solve local minimum problem with dark noise
	RecTimeLikeAlg::PeakTimeFitter();

	//Use PTF vertex to provide first estimation of energy, in order to use energy dependent neff
	RecTimeLikeAlg::Energy_Minimize(m_PTFRec_x,m_PTFRec_y,m_PTFRec_z,m_rec_n);

	//Time Likelihood to reconstruct vertex
	RecTimeLikeAlg::Vertex_Minimize();

	timer.Stop();
	m_time = timer.RealTime();

	m_rec_x = tmp_ve_x;
	m_rec_y = tmp_ve_y;
	m_rec_z = tmp_ve_z;
	m_rec_t = tmp_t_zero;
	m_rec_r = TMath::Sqrt(m_rec_x*m_rec_x+m_rec_y*m_rec_y+m_rec_z*m_rec_z);

	TVector3 vec_r(m_rec_x, m_rec_y, m_rec_z);
	m_rec_theta = vec_r.Theta();
	m_rec_phi   = vec_r.Phi();

	//Energy Reconstruction
	timer.Start();
	RecTimeLikeAlg::Energy_Minimize(m_rec_x,m_rec_y,m_rec_z,m_rec_n);
	RecTimeLikeAlg::Energy_Correction(m_rec_x,m_rec_y,m_rec_z,m_rec_n,m_rec_E);
	timer.Stop();
	m_ene_time = timer.RealTime();

	if ( m_Algorithm == 1 ) { //bias correction for 1D PDF method
		if ( m_VtxCorr ){
			RecTimeLikeAlg::ApplyCorrection(m_rec_E);
		}
	}

	ProcInfo_t info;
	gSystem->GetProcInfo(&info);
	m_memory = info.fMemResident/1024;

	LogDebug<<"minimum hit time = "<<min_hit_time<<std::endl;
  LogDebug << (boost::format("Memory = %.0ld MB, MemVirtual = %.0ld MB") % (info.fMemResident/1024) % (info.fMemVirtual/1024)) << std::endl;
	LogDebug << (boost::format("ChaCenRec:\t x = %.2f \t y = %.2f \t z = %.2f \t r = %.2f \n") % ChaCenRec_x % ChaCenRec_y % ChaCenRec_z % ChaCenRec_r) << std::endl;
  LogDebug << (boost::format("PTFRec:\t\t x = %.2f \t y = %.2f \t z = %.2f \t r = %.2f \t t = %.2f \n") % m_PTFRec_x % m_PTFRec_y % m_PTFRec_z % m_PTFRec_r % m_PTFRec_t) << std::endl;
	LogDebug << (boost::format("TLHRec:\t\t x = %.2f \t y = %.2f \t z = %.2f \t r = %.2f \t t = %.2f \n") % m_rec_x % m_rec_y % m_rec_z % m_rec_r % m_rec_t) << std::endl;

	LogDebug<<"==========================================================="<<std::endl;
	LogDebug<<"The Timestamp is '" << nav->TimeStamp() << "'" << std::endl;
	LogDebug<<"The Reconstructed x is "<<m_rec_x<<std::endl;
	LogDebug<<"The Reconstructed y is "<<m_rec_y<<std::endl;
	LogDebug<<"The Reconstructed z is "<<m_rec_z<<std::endl;
	LogDebug<<"The Reconstructed r is "<<m_rec_r<<std::endl;
	LogDebug<<"The Reconstructed n is "<<m_rec_n<<std::endl;
	LogDebug<<"The Reconstructed To is "<<m_rec_t<<std::endl;
	LogDebug<<"The Reconstructed energy is "<<m_rec_E<<std::endl;
	LogDebug<<"The Reconstruction Process Cost "<<m_time<<std::endl;
    LogDebug<<"The Reconstruction Process for energy Cost "<<m_ene_time<<std::endl;
	LogDebug<<"The Complete Reconstrution Process is Completed!"<<std::endl;
	LogDebug<<"==========================================================="<<std::endl;

	//fill event data model
	auto aDataHdr = new JM::CdVertexRecHeader(); //unit: mm,  MeV, ...
	auto aDataEvt = new JM::CdVertexRecEvt();
    auto aData = new JM::RecVertex();
	aData->setX(m_rec_x); 
	aData->setY(m_rec_y); 
	aData->setZ(m_rec_z); 
	aData->setEnergy(m_rec_n);
	aData->setEprec(m_rec_E); 
	aData->setPESum(m_pe_sum); 
	aData->setT0(m_rec_t); 
	aData->setEnergyQuality(m_like_ene); 
	aData->setPositionQuality(m_like_vtx); 
    aDataEvt->addVertex(aData);
	aDataHdr->setEvent(aDataEvt);
    JM::addHeaderObject(nav, aDataHdr); 
	m_ntuple->Fill();

	RecTimeLikeAlg::VectorClear();

	return true; 
}

bool RecTimeLikeAlg::Load_LikeFun()
{  
	//FIXME:due to Q-T effect, with electronic simulation, large npe will bring strange behavior at border region
	if ( m_isElec ) m_tlh_maxnpe = 5;
	cout << "m_tlh_maxnpe = " << m_tlh_maxnpe << endl;

        // if user don't specify the File Path, stop here. 
        // Note: the data is moved to:
        //       data/Reconstruction/RecTimeLikeAlg/share/
        //       - elec
        //       - no-elec
        if (File_path.empty()) {
            LogError << "Cannot find the Data Directory. Please set in python script " << std::endl;
            return false;
        }


	fs::path s(File_path);

	static bool _declCondObj_done = false;//Make sure that declCondObj run once and only
    if (m_conddb_svc && !_declCondObj_done) {
		std::unique_lock<std::mutex> lock(m_conddb_svc_mtx);
    	if(!_declCondObj_done){
			_declCondObj_done = m_conddb_svc->declCondObj("Recon.RECTIMELIKEALG.DynodeTimePDF", m_dynodepdf_path);
			m_conddb_svc->declCondObj("Recon.RECTIMELIKEALG.McpTimePDF", m_mcppdf_path);
    		int m_cur = 20201219;
    		m_conddb_svc->setCurrent(m_cur);
		}
        //LogDebug << "CondDB path for DynodeTimePDF is " << m_dynodepdf_path.path() << std::endl;
        //LogDebug << "CondDB path for McpTimePDF is " << m_mcppdf_path.path() << std::endl;
	}

	//use pdf from CD center
	if (m_Algorithm == 1){
		file_Time_nhit_mcp[0] = new TFile(TString((s/"pdf_zli_mcp.root").string()));
		file_Time_nhit_ham[0] = new TFile(TString((s/"pdf_zli_hamamastu.root").string()));

		if(!file_Time_nhit_mcp[0]) {
			LogError  << "Failed to get Likelihood Function File!" << std::endl;
			return true;
		}
		else {
			LogDebug << "PDF : " << file_Time_nhit_mcp[0]->GetName() << std::endl;
			LogDebug << "PDF : " << file_Time_nhit_ham[0]->GetName() << std::endl;
		}

		for (int i=0; i<m_tlh_maxnpe; i++){
			pdf_nhit_mcp[i] = (TH1D*)file_Time_nhit_mcp[0]->Get(Form("pdf_%dhit_useFormular",i+1));
			pdf_nhit_ham[i] = (TH1D*)file_Time_nhit_ham[0]->Get(Form("pdf_%dhit_useFormular",i+1));
		}

		for (int i=0; i<m_tlh_maxnpe; i++){ // Scale to 1
			if (pdf_nhit_mcp[i]){
				pdf_nhit_mcp[i]->Scale(1./pdf_nhit_mcp[i]->Integral());
			}
		}
		for (int i=0; i<m_tlh_maxnpe; i++){ // Scale to 1
			if (pdf_nhit_ham[i]){
				pdf_nhit_ham[i]->Scale(1./pdf_nhit_ham[i]->Integral());
			}
		}

		//Update to DarkNoise PDF
		if ( m_isElec ){
			LogInfo << "Use DarkNoise PDF" << std::endl;
			double darkrate_ham = 15e3; //mean value
			double darkrate_mcp = 32e3;

			for (int i=0; i<m_tlh_maxnpe; i++){
				GenerateDarkNoisePDF(pdf_nhit_mcp[i], darkrate_mcp);
				GenerateDarkNoisePDF(pdf_nhit_ham[i], darkrate_ham);
			}
		}
	}
	//use z-r-dist pdf
	else if (m_Algorithm == 2){
		file_Time_nhit_mcp[0] = new TFile(TString((s/"pdf_zli_3d_mcp.root").string()));
		file_Time_nhit_ham[0] = new TFile(TString((s/"pdf_zli_3d_hamamastu.root").string()));

		if(!file_Time_nhit_mcp[0]) {
			LogError  << "Failed to get Likelihood Function File!" << std::endl;
			return true;
		}
		else {
			LogDebug << "PDF : " << file_Time_nhit_mcp[0]->GetName() << std::endl;
			LogDebug << "PDF : " << file_Time_nhit_ham[0]->GetName() << std::endl;
		}
		//nbins_z = 10;
		double zbinning[nbins_z+1] = {0, 8.21561, 10.351, 11.849, 13.0415, 14.0485, 14.9288, 15.7159, 16.4312, 17.0892, 17.7};
    vec_zbinning.reserve(nbins_z);
		for (int j=0; j<=nbins_z; j++){
			vec_zbinning.push_back(zbinning[j]);
		}
		for (int j=0; j<nbins_z; j++){
			pdf_z_r_dist2pmt_mcp[j] = (TH3F*)file_Time_nhit_mcp[0]->Get(Form("pdf_r_dist2pmt_tres_%d",j));
			pdf_z_r_dist2pmt_cdf_mcp[j] = (TH3F*)file_Time_nhit_mcp[0]->Get(Form("pdf_r_dist2pmt_tres_cdf_%d",j));

			pdf_z_r_dist2pmt_ham[j] = (TH3F*)file_Time_nhit_ham[0]->Get(Form("pdf_r_dist2pmt_tres_%d",j));
			pdf_z_r_dist2pmt_cdf_ham[j] = (TH3F*)file_Time_nhit_ham[0]->Get(Form("pdf_r_dist2pmt_tres_cdf_%d",j));
		}
		pdf_dn_ham = (TH1F*)pdf_z_r_dist2pmt_ham[0]->ProjectionZ("pdf_dn_ham",1,1,1,1);
		pdf_dn_mcp = (TH1F*)pdf_z_r_dist2pmt_mcp[0]->ProjectionZ("pdf_dn_mcp",1,1,1,1);
		for (int i=1; i<= pdf_dn_mcp->GetNbinsX(); i++){
			pdf_dn_mcp->SetBinContent(i, 1);
		}
		for (int i=1; i<= pdf_dn_ham->GetNbinsX(); i++){
			pdf_dn_ham->SetBinContent(i, 1);
		}
		pdf_dn_ham->Scale(1./pdf_dn_ham->Integral());
		pdf_dn_mcp->Scale(1./pdf_dn_mcp->Integral());
		pdf_dn_cdf_ham = (TH1F*)pdf_dn_ham->GetCumulative(0);
		pdf_dn_cdf_mcp = (TH1F*)pdf_dn_mcp->GetCumulative(0);
	}
	else {
		LogError  << "Algorithm is not correct!" << std::endl;
		return true;
	}

	file_chargemap = new TFile(TString((s/"chargemap.root").string()));
	Qpdf_r3_z_dist2pmt = (TProfile3D*)file_chargemap->Get("p3_r3_z_dist2edep_expnpe");
	LogDebug << "Charge PDF : " << file_chargemap->GetName() << std::endl;

	//energy dependent neff, to take into account energy dependent PDF
	double evis[5] = {1.022, 3.022, 5.022, 8.022, 10.022};
	double eneneff[5] = {1.543, 1.5416, 1.5396, 1.5393, 1.5390};
	gr_eneneff = new TGraph(5, evis, eneneff);

	//vtx bias correction
	file_vtxbiascorr = new TFile(TString((s/"biascorr.root").string()));
	p2_biascorr = (TProfile2D*)file_vtxbiascorr->Get("biascorr");

	//ene non-uniform correction
	file_eneuniformcorr = new TFile(TString((s/"E_calib_2MeV.root").string()));
	h_correction = (TH1D*)file_eneuniformcorr->Get("normal_h1_edepr3_RecN");

	{
		std::unique_lock<std::mutex> lock(m_fun_ccscale_mtx); //Make sure only one fun_ccscale is created 
		if(!fun_ccscale){
			fun_ccscale = new TF1("fun_ccscale", "expo(0)+expo(2)", 0, 1);
			fun_ccscale->SetParameters(-0.2955,-1.375,-0.4615,1.778);
		}
	}

	return true;
}

bool RecTimeLikeAlg::Book_Tree()
{
	SniperPtr<RootWriter> m_rw(getParent(), "RootWriter");
	if ( ! m_rw.valid() ) {
		LogError << "Failed to get RootWriter instance!" << std::endl;
		return true;
	}

	m_ntuple = m_rw->bookTree(*m_par, "USER_OUTPUT/data", "Vertex Reconstruction");

	//detector information
	m_ntuple->Branch("nTotalPMT",		&Total_num_PMT,		"nTotalPMT/I");

	//event information
	m_ntuple->Branch("nFiredPMT",		&num_PMT,			"nFiredPMT/I");
	m_ntuple->Branch("nTotalNPE",		&m_pe_sum,          "nTotalNPE/D");

	//charge center result
	m_ntuple->Branch("ccr_x",			&m_ccr_x,			"ccr_x/D");
	m_ntuple->Branch("ccr_y",			&m_ccr_y,			"ccr_y/D");
	m_ntuple->Branch("ccr_z",			&m_ccr_z,			"ccr_z/D");

	m_ntuple->Branch("ccrcorr_x",		&ChaCenRec_x,       "ccrcorr_x/D");
	m_ntuple->Branch("ccrcorr_y",		&ChaCenRec_y,       "ccrcorr_y/D");
	m_ntuple->Branch("ccrcorr_z",		&ChaCenRec_z,       "ccrcorr_z/D");

	//peak time fitter result
	m_ntuple->Branch("PTFRec_x",       	&m_PTFRec_x,		"PTFRec_x/D");
	m_ntuple->Branch("PTFRec_y",       	&m_PTFRec_y,		"PTFRec_y/D");
	m_ntuple->Branch("PTFRec_z",       	&m_PTFRec_z,		"PTFRec_z/D");
	m_ntuple->Branch("PTFRec_r",       	&m_PTFRec_r,		"PTFRec_r/D");
	m_ntuple->Branch("PTFRec_t",		&m_PTFRec_t,		"PTFRec_t/D");

	//vertex reconstruction result
	m_ntuple->Branch("rec_x",			&m_rec_x,			"rec_x/D");
	m_ntuple->Branch("rec_y",			&m_rec_y,			"rec_y/D");
	m_ntuple->Branch("rec_z",			&m_rec_z,			"rec_z/D");
	m_ntuple->Branch("rec_r",			&m_rec_r,			"rec_r/D");
	m_ntuple->Branch("rec_t",			&m_rec_t,			"rec_t/D");
	m_ntuple->Branch("rec_theta",		&m_rec_theta,		"rec_theta/D");
	m_ntuple->Branch("rec_phi",			&m_rec_phi,			"rec_phi/D");
	m_ntuple->Branch("likelihood",		&m_like_vtx,		"likelihood/D");

	//energy reconstruction result
	m_ntuple->Branch("rec_n",			&m_rec_n,           "rec_n/D");
	m_ntuple->Branch("rec_E",			&m_rec_E,           "rec_E/D");
	m_ntuple->Branch("recE_likelihood",	&m_like_ene,		"recE_likelihood/D");

	//cpu consumption
	m_ntuple->Branch("time",			&m_time,			"time/D");
	m_ntuple->Branch("recE_time",		&m_ene_time,        "recE_time/D");
	m_ntuple->Branch("memory",			&m_memory,			"memory/D");

	//MC truth information
	m_ntuple->Branch("init_x",			&m_init_x,			"init_x/D");
	m_ntuple->Branch("init_y",			&m_init_y,			"init_y/D");
	m_ntuple->Branch("init_z",			&m_init_z,			"init_z/D");
	m_ntuple->Branch("init_r",			&m_init_r,			"init_r/D");
	m_ntuple->Branch("init_t",			&m_init_t,			"init_t/D");
	m_ntuple->Branch("edep",			&m_edep,			"edep/D");
	m_ntuple->Branch("edep_x",			&m_edep_x,			"edep_x/D");
	m_ntuple->Branch("edep_y",			&m_edep_y,			"edep_y/D");
	m_ntuple->Branch("edep_z",			&m_edep_z,			"edep_z/D");
	m_ntuple->Branch("edep_r",			&m_edep_r,			"edep_r/D");

	return true;
}

void RecTimeLikeAlg::GenerateDarkNoisePDF(TH1D *hist, double darkrate)
{
	//Calculate DN fraction
	double start_time = -200.; // According to x_min and x_max of hist
	double end_time = 500.;
	double num_darknoise = Total_num_PMT * darkrate * m_TimeWindow * 1e-9; //FIXME::should be PMT dependence
	double num_firedpmt = m_LightYield; //FIXME::should be energy dependence
	double dnRatio = num_darknoise/(num_darknoise+num_firedpmt);
	double lsRatio = 1 - dnRatio;

	if (flag_dn){
		LogInfo << "Total_num_PMT = " << Total_num_PMT << ", darkrate = " << darkrate << ", m_TimeWindow = " << m_TimeWindow << std::endl;
		LogInfo << "num_darknoise = " << num_darknoise << ", dnRatio = " << dnRatio << ", lsRatio = " << lsRatio << endl;
		flag_dn = false;
	}

	int NbinsX = hist->GetNbinsX();
	TH1D *hist_dn = (TH1D*)hist->Clone("hist_dn");
	for (int ibin=1; ibin<=NbinsX; ibin++){
		hist_dn->SetBinContent(ibin, 1);
	}

	hist_dn->Scale(dnRatio * ((end_time - start_time)/m_TimeWindow) / hist_dn->Integral());
	hist->Scale(lsRatio/hist->Integral());

	//DN PDF = (1-frac) * LS_PDF + frac, normalized to 1
	for (int ibin=1; ibin<=NbinsX; ibin++){
		hist->SetBinContent(ibin, hist_dn->GetBinContent(ibin) + hist->GetBinContent(ibin));
	}

	hist->Scale(1./hist->Integral());

	delete hist_dn;
}

void RecTimeLikeAlg::NpeCorrection()
{
  for (auto & phit: PMT_HITS) {
		//correct peak gain to mean gain
    phit.hit /= pmtcal_MeanGain.at(CdID::module(phit.pmtId));
  }
	LogDebug << "Finish NPE correction for MCP long tail charge spectrum." << std::endl;
}

void RecTimeLikeAlg::DarkNoiseFilter()
{
  h1_Hittime->Reset();

  for(const auto & phit : PMT_HITS) {
    h1_Hittime->Fill(phit.hitTime);
  }

	double tmp_dn_mean = 0;
	double tmp_start_val = 700, tmp_end_val = 1000;
	int	   tmp_start_bin = h1_Hittime->FindBin(tmp_start_val);
	int    tmp_end_bin	 = h1_Hittime->FindBin(tmp_end_val);
	int	   tmp_nbins	 = tmp_end_bin - tmp_start_bin;
	for (int ibin = tmp_start_bin; ibin < tmp_end_bin; ibin++){
		tmp_dn_mean += h1_Hittime->GetBinContent(ibin);
	}
	tmp_dn_mean /= tmp_nbins;

	double tmp_start_threshold = 2 * tmp_dn_mean;
	double tmp_end_threshold   = 1 * tmp_dn_mean;
	bool flag_start_time = false, flag_end_time = false;
	for (int ibin = 1; ibin < h1_Hittime->GetNbinsX(); ibin++ ){
		double val1 = h1_Hittime->GetBinContent(ibin);
		double val2 = h1_Hittime->GetBinContent(ibin+1);
		double val3 = h1_Hittime->GetBinContent(ibin+2);
		if (!flag_start_time){
			if (val1 >= tmp_start_threshold && val2 >= tmp_start_threshold && val3 >= tmp_start_threshold){
				m_start_time = h1_Hittime->GetBinLowEdge(ibin);
				flag_start_time = true;
			}
		}
		if (flag_start_time && !flag_end_time){
			if (val1 < tmp_end_threshold && val2 < tmp_end_threshold){
				m_end_time = h1_Hittime->GetBinLowEdge(ibin);
				flag_end_time = true;
			}
		}

		if ( flag_start_time && flag_end_time){
			if (m_end_time < m_start_time) {
				LogError << "ERROR::End time < Start time" << std::endl;
			}
			break;
		}
	}

  for(auto & phit : PMT_HITS) {
    phit.isDnHit = !(phit.hitTime>= m_start_time && phit.hitTime <= m_end_time);
  }

	LogDebug << "Done to Filter Dark Noise" << std::endl;
	LogDebug << "Start time = " << m_start_time << ", End time = " << m_end_time << std::endl;
}

bool RecTimeLikeAlg::ChargeCenterRec()
{
	double x_sum = 0;
	double y_sum = 0;
	double z_sum = 0; 
	double PE_sum = 0;

  for(auto & phit : PMT_HITS) {
    if(phit.isDnHit || !phit.isUsed) continue;
    int _pmtId = CdID::module(phit.pmtId);
		x_sum = x_sum + ALL_PMT_pos.at(_pmtId).X() * phit.hit;
		y_sum = y_sum + ALL_PMT_pos.at(_pmtId).Y() * phit.hit;
		z_sum = z_sum + ALL_PMT_pos.at(_pmtId).Z() * phit.hit;
		PE_sum = PE_sum + phit.hit;
  }

	ChaCenRec_x = x_sum/PE_sum;
	ChaCenRec_y = y_sum/PE_sum;
	ChaCenRec_z = z_sum/PE_sum;

	m_ccr_x = ChaCenRec_x;
	m_ccr_y = ChaCenRec_y;
	m_ccr_z = ChaCenRec_z;

	return true;
}

//Use first 30% hit for Charge Center Rec
bool RecTimeLikeAlg::ChargeCenterFitter(double percent = 0.3)
{
	double x_sum = 0;
	double y_sum = 0;
	double z_sum = 0;
	double PE_sum = 0;

	sort(FirstHitTime_NPE_PMTPos.begin(), FirstHitTime_NPE_PMTPos.end());

	for(int i = 0; i< num_PMT; i++){
		if (PE_sum > m_pe_sum * percent) break;
		x_sum = x_sum + get<2>(FirstHitTime_NPE_PMTPos[i])*get<1>(FirstHitTime_NPE_PMTPos[i]);
		y_sum = y_sum + get<3>(FirstHitTime_NPE_PMTPos[i])*get<1>(FirstHitTime_NPE_PMTPos[i]);
		z_sum = z_sum + get<4>(FirstHitTime_NPE_PMTPos[i])*get<1>(FirstHitTime_NPE_PMTPos[i]);
		PE_sum = PE_sum + get<1>(FirstHitTime_NPE_PMTPos[i]);
	}
	ChaCenRec_x = x_sum/PE_sum;
	ChaCenRec_y = y_sum/PE_sum;
	ChaCenRec_z = z_sum/PE_sum;

	return true;
}

//Corrected Charge Center Rec
bool RecTimeLikeAlg::ChargeCenterRecCorr()
{
	RecTimeLikeAlg::ChargeCenterRec();

	ChaCenRec_r = sqrt(ChaCenRec_x*ChaCenRec_x+ChaCenRec_y*ChaCenRec_y+ChaCenRec_z*ChaCenRec_z);
	TVector3 vr_ChaCenRec;
	double ChaCenRec_theta;
	double ChaCenRec_phi;
	if (ChaCenRec_r <= 20000){
		if ( m_isElec ){
			ChaCenRec_r *= 1.45;
		}
		else { 
			ChaCenRec_r *= 1.25;
		}
		vr_ChaCenRec.SetXYZ(ChaCenRec_x,ChaCenRec_y,ChaCenRec_z);
		ChaCenRec_theta = vr_ChaCenRec.Theta();
		ChaCenRec_phi = vr_ChaCenRec.Phi();
	}
	else {
		RecTimeLikeAlg::ChargeCenterFitter(0.3);
		ChaCenRec_r = sqrt(ChaCenRec_x*ChaCenRec_x+ChaCenRec_y*ChaCenRec_y+ChaCenRec_z*ChaCenRec_z);
		vr_ChaCenRec.SetXYZ(ChaCenRec_x,ChaCenRec_y,ChaCenRec_z);
		ChaCenRec_theta = vr_ChaCenRec.Theta();
		ChaCenRec_phi = vr_ChaCenRec.Phi();
		ChaCenRec_r = (1.24042e+04-5.90834e+03*log((2.01546e+04-ChaCenRec_r)/1.70063e+03)); //10000 - 19500
		ChaCenRec_r *= 1.07143;
	}
	ChaCenRec_x = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Cos(ChaCenRec_phi);
	ChaCenRec_y = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Sin(ChaCenRec_phi);
	ChaCenRec_z = ChaCenRec_r * TMath::Cos(ChaCenRec_theta);

	return true;
}

bool RecTimeLikeAlg::PeakTimeFitter()
{
	double tmp_pos_x = ChaCenRec_x;
	double tmp_pos_y = ChaCenRec_y;
	double tmp_pos_z = ChaCenRec_z;

	TVector3 delta_r;

	bool flag = false;
	const int maxloop = 100;
	double limit_delta = 1;

	for (int i=0;i<maxloop;++i){
		double r = TMath::Sqrt(tmp_pos_x*tmp_pos_x+tmp_pos_y*tmp_pos_y+tmp_pos_z*tmp_pos_z);

		if (r>LS_R*1000){
			LogDebug << "invalid vertex. " << std::endl;
			if (!flag)
			{
				i = 0;
				flag = true;
				tmp_pos_x *= (LS_R*1000-10)/r ;
				tmp_pos_y *= (LS_R*1000-10)/r ;
				tmp_pos_z *= (LS_R*1000-10)/r ;
			}
			else
			{
				tmp_pos_x *= (LS_R*1000)/r ;
				tmp_pos_y *= (LS_R*1000)/r ;
				tmp_pos_z *= (LS_R*1000)/r ;
				break;
			}
		}

		delta_r = calDeltaR_Combine(tmp_pos_x,tmp_pos_y,tmp_pos_z);

		tmp_pos_x += delta_r.X();
		tmp_pos_y += delta_r.Y();
		tmp_pos_z += delta_r.Z();
		if (delta_r.Mag() < limit_delta){
			LogDebug << "iteration " << i << endl;
			break;
		}
		if (i == maxloop - 1){
			LogDebug << "iteration " << i << endl;
		}
	}
	LogDebug << "delta_r = " << delta_r.Mag() << std::endl;
	m_like_vtx = delta_r.Mag();

	m_PTFRec_x = tmp_pos_x;
	m_PTFRec_y = tmp_pos_y;
	m_PTFRec_z = tmp_pos_z;
    m_PTFRec_r = TMath::Sqrt(m_PTFRec_x*m_PTFRec_x+m_PTFRec_y*m_PTFRec_y+m_PTFRec_z*m_PTFRec_z);

	return true;
}

TVector3 RecTimeLikeAlg::calDeltaR_Combine(double x,double y,double z)
{
  tmp_hit_time->Reset();

	const int n_pe = m_ptf_maxnpe;

	// find the peak time of t_res
  std::fill(v_res_time.begin(), v_res_time.end(), 1e9);
  for(auto & phit : PMT_HITS) {
    int pe_num = int(phit.hit + 0.5);
    if((phit.isDnHit || !phit.isUsed) || pe_num > n_pe || pe_num < 1) continue;

		double timeflight = CalculateTOF_PTF(x,y,z,CdID::module(phit.pmtId));
		double tres = phit.hitTime-timeflight;

		//Apply FOS time correction
		tres = tres - FOS_correction(pe_num, phit.hitTimeTTS);

		tmp_hit_time -> Fill(tres);

		v_res_time[CdID::module(phit.pmtId)] = tres;
  }
	int max_bin_id_pe;
	double peak_time_pe;

	max_bin_id_pe = tmp_hit_time -> GetMaximumBin();
	peak_time_pe = tmp_hit_time -> GetBinCenter(max_bin_id_pe);

	double low = 10;
	double up = 5;
	// use t_res in [t_peak-low, t_peak+up]
	double mean_res_time = 0;
	double fired_pmt = 0;

  for(auto & phit : PMT_HITS) {
		int pe_num = int(phit.hit + 0.5);
		if (pe_num < 1 || pe_num > n_pe) continue;

		if (v_res_time[CdID::module(phit.pmtId)]<peak_time_pe-low || v_res_time[CdID::module(phit.pmtId)]>peak_time_pe+up) continue;
		mean_res_time += v_res_time[CdID::module(phit.pmtId)];
    ++fired_pmt;
  }

	mean_res_time /= fired_pmt;

	tmp_t_zero = peak_time_pe;
	m_PTFRec_t = peak_time_pe;

	TVector3 delta_r(0,0,0);
	double delta_x = 0.0;
	double delta_y = 0.0;
	double delta_z = 0.0;

  for(auto & phit : PMT_HITS) {
		int pe_num = int(phit.hit + 0.5);
		if (pe_num < 1 || pe_num > n_pe) continue;

    auto _pmtId = CdID::module(phit.pmtId);
		double timeflight = phit.hitTime - v_res_time[_pmtId];

		if (v_res_time[_pmtId]<peak_time_pe-low || v_res_time[_pmtId]>peak_time_pe+up) continue;

		delta_x += (v_res_time[_pmtId]-mean_res_time)*(x-ALL_PMT_pos[_pmtId].X())/(timeflight*fired_pmt);
		delta_y += (v_res_time[_pmtId]-mean_res_time)*(y-ALL_PMT_pos[_pmtId].Y())/(timeflight*fired_pmt);
		delta_z += (v_res_time[_pmtId]-mean_res_time)*(z-ALL_PMT_pos[_pmtId].Z())/(timeflight*fired_pmt);

  }

	delta_r.SetX(delta_x);
	delta_r.SetY(delta_y);
	delta_r.SetZ(delta_z);

	return delta_r;
}

//Apply first-order-stastic time correction
double RecTimeLikeAlg::FOS_correction(double npe, double tts)
{
	double par_0ns[]  = {6.07993, 0.467088, -2.40442}; 
	double par_ham[]  = {9.61728, -0.311931, -4.82104}; 
	double par_mcp[]  = {29.367, -7.6345, -14.495}; 


	double *par;
	if ( !m_isElec ){
		par = par_0ns;
	}
	else {
		if (tts < 5){
			par = par_ham;
		}
		else {
			par = par_mcp;
		}
	}

	double shift = par[0]/TMath::Sqrt(npe)+par[1]+par[2]/npe;

	return shift;
}

bool RecTimeLikeAlg::Vertex_Minimize()
{  
	tmp_ve_x = m_PTFRec_x;
	tmp_ve_y = m_PTFRec_y;
	tmp_ve_z = m_PTFRec_z;
	tmp_t_zero = m_PTFRec_t;

	ROOT::Math::Functor vtxllf(*vtxllfcn, 4);
	vtxllminimizer->SetFunction(vtxllf);
	vtxllminimizer->SetMaxFunctionCalls(1e5);
	vtxllminimizer->SetMaxIterations(1e5);
	vtxllminimizer->SetTolerance(1e-4);
	vtxllminimizer->SetStrategy(2);
	vtxllminimizer->SetPrintLevel(1);

	vtxllminimizer->SetLimitedVariable(0, "T0", tmp_t_zero, 1e-3, tmp_t_zero-100, tmp_t_zero+100);
	vtxllminimizer->SetLimitedVariable(1, "x",  tmp_ve_x,	1e-3, -LS_R*1000, LS_R*1000);
	vtxllminimizer->SetLimitedVariable(2, "y",  tmp_ve_y,	1e-3, -LS_R*1000, LS_R*1000);
	vtxllminimizer->SetLimitedVariable(3, "z",  tmp_ve_z,	1e-3, -LS_R*1000, LS_R*1000);

	int goodness = vtxllminimizer->Minimize();
	std::cout << "Vertex_Minimize::Goodness = " << goodness << std::endl;

	const double *xs = vtxllminimizer->X();
	tmp_t_zero  = xs[0];
	tmp_ve_x    = xs[1];
	tmp_ve_y    = xs[2];
	tmp_ve_z    = xs[3];

	m_like_vtx = vtxllminimizer->MinValue();

	return true;
}

double RecTimeLikeAlg::HittimeGoodness(double T_zero,
		double Vert_x,
		double Vert_y,
		double Vert_z)
{
	double Goodness = 0; 
	double FiredPMT = 0;

	double r = sqrt(pow(Vert_x,2)+pow(Vert_y,2)+pow(Vert_z,2));
	double r3 = pow(r/1000,3);

  for(auto & phit : PMT_HITS) {
		double & pmt_npe = phit.hit;
    if(pmt_npe <= 0.5 || phit.isDnHit || !phit.isUsed) continue;
    int _pmtId = CdID::module(phit.pmtId);

		double timeflight = RecTimeLikeAlg::CalculateTOF(Vert_x, Vert_y, Vert_z, _pmtId);
		relaTime = phit.hitTime - timeflight - T_zero;

		int pe_num = int(pmt_npe + 0.5); // 1PE to m_tlh_maxnpe PDF
		if (pe_num > m_tlh_maxnpe) pe_num = m_tlh_maxnpe; // > m_tlh_maxnpe consider as m_tlh_maxnpe PE

		if ( ! (relaTime > -30 && relaTime < 300) ) continue; 

		double pdfValue = 0;
		if ( m_Algorithm == 1 ){
      // TODO: Pmt type is checked through tts. It must exist a better way
			if (phit.hitTimeTTS > 5.0 ){ // MCP PMT
				pdfValue = pdf_nhit_mcp[pe_num-1]->Interpolate(relaTime);
			}
			else { // Hamamastu PMT
				pdfValue = pdf_nhit_ham[pe_num-1]->Interpolate(relaTime);
			}
		}
		else if ( m_Algorithm == 2 ){
			for (int zbin=0; zbin<nbins_z; zbin++){
				if ( std::abs(Vert_z)/1000 >= vec_zbinning[zbin] && std::abs(Vert_z)/1000 < vec_zbinning[zbin+1] ){
					double dist = Dist2PMT(Vert_x, Vert_y, Vert_z, _pmtId);
					TH3F *th3, *th3_cdf;
					TH1F *pdf_dn, *pdf_dn_cdf;
					if ( phit.hitTimeTTS > 5.0 ){ // MCP PMT
						th3 = pdf_z_r_dist2pmt_mcp[zbin];
						th3_cdf = pdf_z_r_dist2pmt_cdf_mcp[zbin];
						pdf_dn = pdf_dn_mcp;
						pdf_dn_cdf = pdf_dn_cdf_mcp;
					}
					else { // Hamamastu PMT
						th3 = pdf_z_r_dist2pmt_ham[zbin];
						th3_cdf = pdf_z_r_dist2pmt_cdf_ham[zbin];
						pdf_dn = pdf_dn_ham;
						pdf_dn_cdf = pdf_dn_cdf_ham;
					}
					double delta = 1e-3;
					if ( th3->GetXaxis()->FindBin(r3) <= 1 ) r3 = th3->GetXaxis()->GetBinCenter(1) + delta;
					if ( th3->GetXaxis()->FindBin(r3) >= th3->GetNbinsX() ) r3 = th3->GetXaxis()->GetBinCenter(th3->GetNbinsX()) - delta;
					if ( th3->GetYaxis()->FindBin(dist) <= 1 ) dist = th3->GetYaxis()->GetBinCenter(1) + delta;
					if ( th3->GetYaxis()->FindBin(dist) >= th3->GetNbinsY() ) dist = th3->GetYaxis()->GetBinCenter(th3->GetNbinsY()) - delta;
					if ( th3->GetZaxis()->FindBin(relaTime) <= 1 ) relaTime = th3->GetZaxis()->GetBinCenter(1) + delta;
					if ( th3->GetZaxis()->FindBin(relaTime) >= th3->GetNbinsZ() ) relaTime = th3->GetZaxis()->GetBinCenter(th3->GetNbinsZ()) - delta;

					double start_time, end_time, prob_dn, prob_ls, cdf_dn, cdf_ls;
					double num_darknoise, num_TotalNPE, dnRatio, lsRatio;
					double prob, prob_cdf;
					start_time = th3->GetZaxis()->GetBinLowEdge(1);
					end_time   = th3->GetZaxis()->GetBinLowEdge(th3->GetNbinsZ()+1);
					num_darknoise = Total_num_PMT * DarkRate[_pmtId] * (end_time - start_time) * 1e-9;
					num_TotalNPE = m_pe_sum;
					dnRatio = num_darknoise/num_TotalNPE;
					lsRatio = 1-dnRatio;
					prob_dn = pdf_dn->Interpolate(relaTime);
					cdf_dn = pdf_dn_cdf->Interpolate(relaTime);
					prob_ls = th3->Interpolate(r3, dist, relaTime);
					cdf_ls  = th3_cdf->Interpolate(r3, dist, relaTime);
					prob = prob_dn * dnRatio + prob_ls * lsRatio;
					prob_cdf = cdf_dn * dnRatio + cdf_ls * lsRatio;
					pdfValue = pe_num*prob*pow(prob_cdf,pe_num-1);
				}
			}
		}

		if( pdfValue > 0 && !std::isnan(pdfValue) ){
			Goodness = Goodness-TMath::Log(pdfValue);
			FiredPMT++;
		}
	}

	if((Vert_x*Vert_x+Vert_y*Vert_y+Vert_z*Vert_z) >= LS_R*1000*LS_R*1000) Goodness *= 100;	

	return Goodness/FiredPMT;
}

bool RecTimeLikeAlg::GridSearch()
{  
	m_like_vtx=1000.0;
	//Begin with Charge Center Reconstruction;
	tmp_ve_x = ChaCenRec_x;
	tmp_ve_y = ChaCenRec_y;
	tmp_ve_z = ChaCenRec_z;
	tmp_t_zero = 0;

	for(int init_val = 0;init_val < 1;init_val++){
		//tag for grid search
		int tag_x = 4;
		int tag_y = 4;
		int tag_z = 4;
		int tag_t = 10;

		double delta = 1e10;

		double step_length = 1000;

		for(int iteration = 0; iteration < 100; iteration++){//100
			if(step_length<0.1) break;
			for(int bin_t = -1;bin_t<2;bin_t++){//-1,2
				for(int bin_x = -1; bin_x<2; bin_x++){//-1,2
					for(int bin_y = -1; bin_y<2; bin_y++){
						for(int bin_z = -1; bin_z<2; bin_z++){
							double tmp_x = tmp_ve_x+bin_x*step_length;
							double tmp_y = tmp_ve_y+bin_y*step_length;
							double tmp_z = tmp_ve_z+bin_z*step_length;
							if((tmp_x*tmp_x+tmp_y*tmp_y+tmp_z*tmp_z)< LS_R*1000*LS_R*1000){
								double d;
								d = HittimeGoodness(tmp_t_zero+bin_t*step_length/100.0,
										tmp_ve_x+bin_x*step_length,
										tmp_ve_y+bin_y*step_length,
										tmp_ve_z+bin_z*step_length);
								if(d<delta){
									tag_x = bin_x;
									tag_y = bin_y;
									tag_z = bin_z;
									tag_t = bin_t;
									delta = d;
								}
							}                
							else continue;
						}
					}
				}
			}
			tmp_ve_x = tmp_ve_x + static_cast<double>(tag_x*step_length);
			tmp_ve_y = tmp_ve_y + static_cast<double>(tag_y*step_length);
			tmp_ve_z = tmp_ve_z + static_cast<double>(tag_z*step_length);
			tmp_t_zero = tmp_t_zero + static_cast<double>(tag_t*step_length/100.0);

			if(0==tag_x&&0==tag_y&&0==tag_z&&0==tag_t){
				step_length = step_length/2;
			}
			tag_x=tag_y=tag_z=tag_t=0;
		}
		if(delta<m_like_vtx){
			sign_x = tmp_ve_x;
			sign_y = tmp_ve_y;
			sign_z = tmp_ve_z;
			sign_t = tmp_t_zero;
			m_like_vtx = delta;
			m_ratio  = 1.2+0.1*init_val;
		}
		//  LogDebug<<delta<<std::endl;
	}
	tmp_ve_x = sign_x;
	tmp_ve_y = sign_y;
	tmp_ve_z = sign_z;
	tmp_t_zero = sign_t;

	return true;
}

double RecTimeLikeAlg::CalculateTOF(double source_x,
		double source_y,
		double source_z,
		int ID)
{
	double pmt_pos_x, pmt_pos_y, pmt_pos_z;
  pmt_pos_x = ALL_PMT_pos.at(ID).X();
  pmt_pos_y = ALL_PMT_pos.at(ID).Y();
  pmt_pos_z = ALL_PMT_pos.at(ID).Z();

	double dx = (source_x - pmt_pos_x)/1000;
	double dy = (source_y - pmt_pos_y)/1000;
	double dz = (source_z - pmt_pos_z)/1000;

	double r0 = (TMath::Sqrt(source_x*source_x+source_y*source_y+source_z*source_z))/1000;
	double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

	double cos_theta = (Ball_R*Ball_R+dist*dist-r0*r0)/(2*Ball_R*dist);

	double theta = TMath::ACos(cos_theta);

	double dist_oil = Ball_R*cos_theta-TMath::Sqrt(LS_R*LS_R-Ball_R*Ball_R*TMath::Sin(theta)*TMath::Sin(theta));
	double lightspeed = 299792458/1e9;
	double m_eff_oil = 1.33;
	//optimized for TLH algorithm base on ToyMC

	RecTimeLikeAlg::TuningNeff();

	if (m_Algorithm == 1){ //1D PDF
		return (dist-dist_oil)*m_neff/lightspeed+dist_oil*m_eff_oil/lightspeed;//currently
	}
	else { //3D PDF
		return dist*m_neff/lightspeed;//currently
	}
}

double RecTimeLikeAlg::CalculateTOF_PTF(double source_x,
		double source_y,
		double source_z,
		int ID)
{
	double pmt_pos_x, pmt_pos_y, pmt_pos_z;
	pmt_pos_x = ALL_PMT_pos.at(ID).X();
	pmt_pos_y = ALL_PMT_pos.at(ID).Y();
	pmt_pos_z = ALL_PMT_pos.at(ID).Z();

	double dx = (source_x - pmt_pos_x)/1000;
	double dy = (source_y - pmt_pos_y)/1000;
	double dz = (source_z - pmt_pos_z)/1000;

	double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

	double lightspeed = 299792458/1e9;
	//optimized for PTF algorithm base on ToyMC
	m_neff = 1.538;
	if ( m_isElec ){
		m_neff = 1.542;
	}

	return dist*m_neff/lightspeed;//currently
}

void RecTimeLikeAlg::TuningNeff()
{
	if ( m_Algorithm == 1){ //1D PDF
		m_neff = 1.539;
		if ( m_isElec ){
			m_neff = 1.556;
			if (m_PmtType == 1){ //ham only
				m_neff = 1.54;
			}
			if (m_PmtType == 2){ //mcp only
				m_neff = 1.586;
			}
		}
	}
	else { //3D PDF
		m_neff = 1.54;
		if ( m_isElec ) {
			m_neff = gr_eneneff->Eval(m_rec_n);
			if (m_PmtType == 1){ //ham only
				m_neff = 1.53;
			}
			if (m_PmtType == 2){ //mcp only
				m_neff = 1.56;
			}
		}
	}
}

double RecTimeLikeAlg::Dist2PMT(double source_x,
		double source_y,
		double source_z,
		int ID)
{   
	double pmt_pos_x, pmt_pos_y, pmt_pos_z;
  pmt_pos_x = ALL_PMT_pos.at(ID).X();
  pmt_pos_y = ALL_PMT_pos.at(ID).Y();
  pmt_pos_z = ALL_PMT_pos.at(ID).Z();

	double dx = (source_x - pmt_pos_x)/1000;
	double dy = (source_y - pmt_pos_y)/1000;
	double dz = (source_z - pmt_pos_z)/1000;

	double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

	return dist * 1000; //mm
}

void RecTimeLikeAlg::Energy_Minimize(double _m_rec_x, double _m_rec_y, double _m_rec_z, double& _m_rec_n)
{

	ROOT::Math::Functor recfunc(*fcn, 4);
	recminimizer->SetFunction(recfunc);

	recminimizer->SetMaxFunctionCalls(1000000);
	recminimizer->SetMaxIterations(100000);
	recminimizer->SetTolerance(0.001);
	recminimizer->SetStrategy(2);
	recminimizer->SetPrintLevel(1);

	recminimizer->SetLimitedVariable(0, "n",  _m_rec_n, 1e-3, 0, 1000);
	recminimizer->SetFixedVariable(1, "xpos", _m_rec_x);
	recminimizer->SetFixedVariable(2, "ypos", _m_rec_y);
	recminimizer->SetFixedVariable(3, "zpos", _m_rec_z);

	int goodness = recminimizer->Minimize();
	std::cout << "Energy_Minimize::Goodness = " << goodness << std::endl;

	const double *xs = recminimizer->X();
	_m_rec_n = xs[0];
	m_rec_E = _m_rec_n;

	m_like_ene = recminimizer->MinValue();

	//delete tmp_resTime_fix;

	recminimizer->Clear();
}

double RecTimeLikeAlg::Calculate_Energy_Likelihood(double n0,
		double m_x,
		double m_y,
		double m_z)
{
	double m_Likelihood = 0;
	double r0 = (TMath::Sqrt(m_x*m_x+m_y*m_y+m_z*m_z));
	double r3 = pow(r0/1000,3);

  // Build a umap just for this alg
  std::unordered_map<int, PMTHit*> _hits;
  for(auto & phit : PMT_HITS) _hits[CdID::module(phit.pmtId)] = &phit;

	for(int _pmtId = 0; _pmtId< Total_num_PMT; ++_pmtId){
		double pmt_pos_x = ALL_PMT_pos.at(_pmtId).X();
		double pmt_pos_y = ALL_PMT_pos.at(_pmtId).Y();
		double pmt_pos_z = ALL_PMT_pos.at(_pmtId).Z();

		double dx = (m_x - pmt_pos_x);
		double dy = (m_y - pmt_pos_y);
		double dz = (m_z - pmt_pos_z);

		double dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);

		TVector3 vpip0(-1*dx,-1*dy,-1*dz);
		TVector3 vpio(pmt_pos_x,pmt_pos_y,pmt_pos_z);
		double vpip0_theta = vpio.Angle(vpip0);
		double cos_theta = TMath::Cos(vpip0_theta);

		double E_ScaleFac;
		double m_expected_PE = 0;
		//use direct light model to calculate exp_npe
		if ( m_Algorithm == 1 ){
			E_ScaleFac = 2869;
			m_expected_PE = cos_theta*n0*pmt_r*pmt_r/(4*dist*dist*1e-6)*TMath::Exp(-dist*1e-3*m_effective_attenuation)*E_ScaleFac;
		}
		//use r3-z-dist charge map to get exp_npe
		if ( m_Algorithm == 2 ){
			TProfile3D *th3 = Qpdf_r3_z_dist2pmt;
			double delta = 1e-3;
			double fabsZ = std::abs(m_z);
			if ( th3->GetXaxis()->FindBin(r3) <= 1 ) r3 = th3->GetXaxis()->GetBinCenter(1) + delta;
			if ( th3->GetXaxis()->FindBin(r3) >= th3->GetNbinsX() ) r3 = th3->GetXaxis()->GetBinCenter(th3->GetNbinsX()) - delta;
			if ( th3->GetYaxis()->FindBin(fabsZ) <= 1 ) fabsZ = th3->GetYaxis()->GetBinCenter(1) + delta;
			if ( th3->GetYaxis()->FindBin(fabsZ) >= th3->GetNbinsY() ) fabsZ = th3->GetYaxis()->GetBinCenter(th3->GetNbinsY()) - delta;
			if ( th3->GetZaxis()->FindBin(dist) <= 1 ) dist = th3->GetZaxis()->GetBinCenter(1) + delta;
			if ( th3->GetZaxis()->FindBin(dist) >= th3->GetNbinsZ() ) dist = th3->GetZaxis()->GetBinCenter(th3->GetNbinsZ()) - delta;
			E_ScaleFac = 0.2993;
			m_expected_PE = n0*th3->Interpolate(r3, fabsZ, dist)*E_ScaleFac;
		}

		if ( m_isElec ){
			double m_expected_DN = pmtcal_DarkRate.at(_pmtId) * 1e-9 * m_TimeWindow;
			m_expected_PE += m_expected_DN;
		}

		if ( m_expected_PE < 1e-9 ) m_expected_PE = 1e-9;

		if (_hits.find(_pmtId) != _hits.end()){
      auto & phit = *(_hits.at(_pmtId));
			m_Likelihood = m_Likelihood+(m_expected_PE - phit.hit)+log(phit.hit/m_expected_PE)*phit.hit;
		}
		else {
			m_Likelihood = m_Likelihood+m_expected_PE;
		}
	}

	return m_Likelihood/1000;
}

void RecTimeLikeAlg::Energy_Correction(double _m_rec_x, double _m_rec_y, double _m_rec_z, double _m_rec_n, double& _m_rec_E)
{
	double R = TMath::Sqrt(_m_rec_x*_m_rec_x+_m_rec_y*_m_rec_y+_m_rec_z*_m_rec_z)/1000;
	double R3 = pow(R,3); 

	//Non-linearity Correction for positron
	double non_li_parameter[5] = {0.0727379, 0.706707, 0.937729, 0.000561567, 0.632805};
	TF1 *f_non_li_positron = new TF1("pos","1.022*(([1]+[3]*(x-1.022))*(x-1.022)/(1+[0]*exp(-[2]*(x-1.022)))+[4])/[4]",0,1000);
	f_non_li_positron->SetParameters(non_li_parameter);

	_m_rec_E = _m_rec_n;

	//non-linearity correction
	_m_rec_E = f_non_li_positron->GetX(_m_rec_E);

	//energy non-uniformity correction
	//only direct light model has large non-uniformity effect
	if ( m_Algorithm == 1 ){
		_m_rec_E = _m_rec_E/h_correction->Interpolate(R3);
	}

	delete f_non_li_positron;
}

bool RecTimeLikeAlg::ApplyCorrection(double energy)
{
	double r3 = pow(m_rec_r/1000, 3);  
	double deltaR = p2_biascorr->Interpolate(r3, energy);
	m_rec_r -= deltaR;

	m_rec_x = m_rec_r * TMath::Sin(m_rec_theta) * TMath::Cos(m_rec_phi);
	m_rec_y = m_rec_r * TMath::Sin(m_rec_theta) * TMath::Sin(m_rec_phi);
	m_rec_z = m_rec_r * TMath::Cos(m_rec_theta);

	LogDebug << "Done to apply bias correction" << std::endl;

	return true;
}

void RecTimeLikeAlg::SwitchPMTType()
{
  for(auto & phit : PMT_HITS) {
    // TODO: PMT type identification is done by TTS. Probably better way of doing it
    if((m_PmtType == 1 && phit.hitTimeTTS > 5)
        || (m_PmtType == 2 && phit.hitTimeTTS < 5))
      phit.isUsed = false;
  }
  if(m_PmtType == 1) LogDebug << "Use Hamamastu PMT only for reconstruction. " << std::endl;
  if(m_PmtType == 2) LogDebug << "Use MCP PMT only for reconstruction. " << std::endl;
}

//Write vector
void RecTimeLikeAlg::VectorPush()
{
	m_pe_sum = 0;

  // TODO: Should be able to replace those vector by PMT_HITS
  Readout_PE.reserve(PMT_HITS.size());
  Readout_hit_time.reserve(PMT_HITS.size());
  PMT_TTS.reserve(PMT_HITS.size());
  PMT_ID.reserve(PMT_HITS.size());
  FirstHitTime_NPE_PMTPos.reserve(PMT_HITS.size());

  for(auto & phit : PMT_HITS) {
    Readout_PE.push_back(phit.hit);
    m_pe_sum += phit.hit;

    Readout_hit_time.push_back(phit.hitTime);
    PMT_TTS.push_back(phit.hitTimeTTS);

    PMT_ID.push_back(CdID::module(phit.pmtId));
    const auto & cur_pmt_pos = ALL_PMT_pos[CdID::module(phit.pmtId)];

    FirstHitTime_NPE_PMTPos.push_back(make_tuple(
          phit.hitTime,
          phit.hit,cur_pmt_pos.X(),cur_pmt_pos.Y(),cur_pmt_pos.Z()));
  }
}

//Clear vector
void RecTimeLikeAlg::VectorClear()
{
	Readout_PE.clear();
	Readout_hit_time.clear();
	PMT_ID.clear();
	PMT_TTS.clear();
	FirstHitTime_NPE_PMTPos.clear();
}

bool RecTimeLikeAlg::finalize()
{
	//(dynamic_cast<ToolBase*>(m_recTool))->finalize();
	LogInfo  << objName()
		<< "   finalized successfully" 
		<< std::endl; 

	return true; 
}

TF1* RecTimeLikeAlg::fun_ccscale = nullptr;
std::mutex RecTimeLikeAlg::m_fun_ccscale_mtx;

std::mutex RecTimeLikeAlg::m_conddb_svc_mtx;


