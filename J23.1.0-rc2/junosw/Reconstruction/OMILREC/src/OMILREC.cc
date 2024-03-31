/********************************************
* OMILREC created by Wenjie Wu               
* wuwenjie@ihep.ac.cn
* 
* Developments:
* QMLE/QTMLE developed by Guihong Huang 
*   -- Using 3-D nPE map
*   -- Vertex & energy co-reconstrcution
*   -- Combined charge and time information
* 2020.5.26
* 
*   -- Enable Ge68 + Laser nPE map
*   -- Enable phi-correction nPE map
* 2020.11.12
*
*   -- Enable vertex dependent time pdfs 
* 2020.12.20
*
*   -- Enable Ge68 + Electron time pdfs
* 2021.4.26
*
* huanggh@ihep.ac.cn
*********************************************/

#include "OMILREC/OMILREC.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TString.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include <boost/filesystem.hpp>
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Geometry/PMTParamSvc.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "TAxis.h"
#include "TROOT.h"

#ifndef BUILD_ONLINE
#include "RootWriter/RootWriter.h"
#endif
namespace fs = boost::filesystem;

DECLARE_ALGORITHM(OMILREC);

static const double c = 2.99792458e8;
static const double ADCL = 1000.;

/* minimization configurations */
static const int maxiters = 1E4;
static const int maxfuncalls = 1E4;
static const double tolerance = 1.e-6;

static const double estep = 0.001; //MeV
static const double rstep = 10; // mm
static const double tangstep = 0.001; //rad
static const double pangstep = 0.001; //rad
static const double t0step = 0.1; // ns

static const double erange[2] = {0., 1.e6}; //MeV
static const double rrange[2] = {0., 17400.}; // mm
static const double tangrange[2] = {0., TMath::Pi()}; //rad
static const double pangrange[2] = {0., 2.*TMath::Pi()}; //rad
static const double t0range[2] = {-50., 50.}; //ns

static const double MapThetaStep = 180./1440.; //degree
static const double tRange = 600.; // ns
static const double dRange = 40.; // m
//static const int tBinN = 1200; //for ideal case
static const int tBinN = 400; // for tts case
static const int dBinN = 200; 

static const double GeEvis = 1.022; //MeV 
const double spmtTimeShift = 25.; //ns


//#define Ideal_Rcut
#define CLS_Rcut


const double tbinW = tRange/double(tBinN);
const double FullADCBinN =  ADCL/tbinW;
static const double DNProb  = 1./FullADCBinN;


OMILREC::OMILREC(const std::string& name)
:   AlgBase(name)
{
    m_iEvt = -1; 
    runID = 0;
    TruthLevel = 0;

    Use3DMap = true;
    enableUseEkMap = false;
    enableUsePhiMap = false;
    enableReadTruth = false;
    enableDebug = false;
    RecMode = "Real";
    enableUseTrueVertex = false;
    enableUseFixedVertex = false;
    enableUseTLHVertex = false;

    enableTimeInfo = false;
    enableDynTimeInfoOnly = false;
    enableDynInfoOnly = false;
    enableMcpInfoOnly = false;
    enableSPMTInfo = false;
    enableSPMTOnly = false;

    enableAddToyQ = false;
    enableAddToyTTS = false;
    enableAddToyDN = false;
    enableOccupancy = false;
    enableLTSPEs = false;
    enableQTimePdf = false;

    TotalLPMT = 17613;
    TotalSPMT = 25600;
    LPMTCalibEnergy = 1.022;
    SPMTCalibEnergy = 1.022;
    Ge68ES = 1.31787e+03;
    LaserES = 1.40440e+03;
    ElectronES = 1.31787e+03;
    PMT_R = 19.434;
    Ball_R = 19.246;
    LS_R = 17.7 + 0.12;
    pmt_r = 0.185;
    RfrIndxLS = 1.5;
    RfrIndxWR = 1.355;
    LFHSBinW = 5.;
    SFHSBinW = 10.;

    m_NPE = 0.;
    QCfactor = 1.4;
    m_UfrmScale = 0.;
    PedThres = 0.1;
    SignalWindowL = 280.;
    
    fix_x = 0.;
    fix_y = 0.;
    fix_z = 0.;

    TimePdfPath = "";
    
    declProp("runID", runID);
    declProp("TruthLevel", TruthLevel);
    declProp("enableReadTruth", enableReadTruth);
    declProp("enableDebug", enableDebug);
    declProp("RecMode", RecMode);
    declProp("enableUseTrueVertex", enableUseTrueVertex);
    declProp("enableUseFixedVertex", enableUseFixedVertex);
    declProp("enableUseTLHVertex", enableUseTLHVertex);
    declProp("enableTimeInfo",enableTimeInfo);
    declProp("enableDynTimeInfoOnly",enableDynTimeInfoOnly);
    declProp("enableDynInfoOnly",enableDynInfoOnly);
    declProp("enableMcpInfoOnly",enableMcpInfoOnly);
    declProp("enableSPMTInfo",enableSPMTInfo);
    declProp("enableSPMTOnly",enableSPMTOnly);
    declProp("enableOccupancy",enableOccupancy);
    declProp("enableAddToyQ", enableAddToyQ);
    declProp("enableAddToyTTS", enableAddToyTTS);
    declProp("enableAddToyDN", enableAddToyDN);
    declProp("enableLTSPEs", enableLTSPEs);
    declProp("enableQTimePdf", enableQTimePdf);
    declProp("enableUseEkMap", enableUseEkMap);
    declProp("enableUsePhiMap", enableUsePhiMap);

    declProp("RecMapPath", RecMapPath);
    declProp("TimePdfPath", TimePdfPath);
    declProp("SimFile", SimFile);
    declProp("TotalLPMT", TotalLPMT);
    declProp("LPMTCalibEnergy", LPMTCalibEnergy);
    declProp("SPMTCalibEnergy", SPMTCalibEnergy);
    declProp("SignalWindowL",SignalWindowL);
    declProp("RfrIndxLS",RfrIndxLS);
    declProp("RfrIndxWR",RfrIndxWR);
    declProp("PMT_R", PMT_R);
    declProp("Ball_R",Ball_R);
    declProp("LS_R", LS_R);
    declProp("pmt_r", pmt_r);
    declProp("fix_x", fix_x);
    declProp("fix_y", fix_y);
    declProp("fix_z", fix_z);


}


OMILREC::~OMILREC()
{
    /*if(TruthLevel == 2){
        delete m_lpmt_pulsetype;
        delete m_lpmt_id;
        delete m_lpmt_npe;
        delete m_lpmt_hittime;
        delete m_lpmt_amplitude;
        delete m_lpmt_tts;
        delete m_lpmt_timeoffset;
        delete m_lpmt_pulsehittime;

        delete m_spmt_tagid;
        delete m_spmt_pulsetype;
        delete m_spmt_id;
        delete m_spmt_npe;
        delete m_spmt_hittime;
        delete m_spmt_timewindow;
        delete m_spmt_amplitude;
        delete m_spmt_tts;
        delete m_spmt_pulsehittime;
        delete m_spmt_evthittime;
    }*/
}

bool OMILREC::initialize()
{
    LogInfo <<"In offline test: add spmt"<< std::endl;
    nPEMapPath        = RecMapPath + "/nPEMap";
    if(TimePdfPath=="") TimePdfPath = RecMapPath + "/TimePdf";
    PMTDataPath       = RecMapPath + "/CalibPMTPara";
    PMTChargeSpecPath = RecMapPath + "/ChargeSpec";
    std::cout<< TimePdfPath << std::endl;
    m_lpmt_fht = new std::vector<double>();
    m_spmt_fht = new std::vector<double>();
    if(TruthLevel == 2){
        m_lpmt_pulsetype = new std::vector<std::string>();
        m_lpmt_id = new std::vector<int>();
        m_lpmt_npe = new std::vector<int>();
        m_lpmt_hittime = new std::vector<double>();
        m_lpmt_amplitude = new std::vector<float>();
        m_lpmt_tts = new std::vector<float>();
        m_lpmt_timeoffset = new std::vector<float>();
        m_lpmt_pulsehittime = new std::vector<double>();

        m_spmt_tagid = new std::vector<std::string>();
        m_spmt_pulsetype = new std::vector<std::string>();
        m_spmt_id = new std::vector<int>();
        m_spmt_npe = new std::vector<int>();
        m_spmt_hittime = new std::vector<double>();
        m_spmt_timewindow = new std::vector<float>();
        m_spmt_amplitude = new std::vector<float>();
        m_spmt_tts = new std::vector<float>();
        m_spmt_pulsehittime = new std::vector<double>();
        m_spmt_evthittime = new std::vector<double>();
    }


    #ifndef BUILD_ONLINE
    SniperPtr<RootWriter> RWsvc(getParent(), "RootWriter");
    if (RWsvc.invalid()) {
        LogError << "Can't Locate RootWriter."
                 << std::endl;
        return false;
    }
    
    // Todo reduce output variable
    TRec = RWsvc->bookTree(*m_par, "USER_OUTPUT/TRec", "reconstruction data & true information");
    //final rec. vertex
    TRec->Branch("rec_x",&recx,"rec_x/F");
    TRec->Branch("rec_y",&recy,"rec_y/F");
    TRec->Branch("rec_z",&recz,"rec_z/F");
    //QMLE vertex
    /*TRec->Branch("recQx",&recQx,"recQx/F");
    TRec->Branch("recQy",&recQy,"recQy/F");
    TRec->Branch("recQz",&recQz,"recQz/F");
    //TMLE vertex
    TRec->Branch("recTx",&recTx,"recTx/F");
    TRec->Branch("recTy",&recTy,"recTy/F");
    TRec->Branch("recTz",&recTz,"recTz/F");
    //QTMLE vertex
    TRec->Branch("recQTx",&recQTx,"recQTx/F");
    TRec->Branch("recQTy",&recQTy,"recQTy/F");
    TRec->Branch("recQTz",&recQTz,"recQTz/F");
    TRec->Branch("recQTt0",&recQTt0,"recQTt0/F");
    //RecTimeLike vertex
    TRec->Branch("timeRecx",&timeRecx, "timeRecx/F");
    TRec->Branch("timeRecy",&timeRecy, "timeRecy/F");
    TRec->Branch("timeRecz",&timeRecz, "timeRecz/F");
    TRec->Branch("timeRecE",&timeRecE, "timeRecE/F");
    TRec->Branch("timeRect0",&timeRect0, "timeRect0/F");
    //rec. radial distance
    TRec->Branch("m_QR",&m_QR,"m_QR/F");
    TRec->Branch("m_TR",&m_TR,"m_TR/F");
    TRec->Branch("m_QTR",&m_QTR,"m_QTR/F");*/
    //rec. energy
    TRec->Branch("m_NPE",&m_NPE,"m_NPE/F"); //total charge
    TRec->Branch("m_NQE",&m_NQE,"m_NQE/F"); //total charge energy estimator
    TRec->Branch("m_QEn",&m_QEn,"m_QEn/F"); //QMLE energy 
    TRec->Branch("m_QTEn",&m_QTEn,"m_QTEn/F"); //QTMLE energy
    TRec->Branch("m_PESum",&m_PESum,"m_PESum/F"); //QMLE/QTMLE energy in pe units
    //rec. quality
    /*TRec->Branch("m_Qstat",&m_Qstat,"m_Qstat/F");
    TRec->Branch("m_Tstat",&m_Tstat,"m_Tstat/F");
    TRec->Branch("m_QTstat",&m_QTstat,"m_QTstat/F");
    TRec->Branch("m_QL",&m_QL,"m_QL/F");
    TRec->Branch("m_TL",&m_TL,"m_TL/F");
    TRec->Branch("m_QTL",&m_QTL,"m_QTL/F");
    TRec->Branch("m_QRerr",&m_QRerr,"m_QRerr/F");
    TRec->Branch("m_TRerr",&m_TRerr,"m_TRerr/F");
    TRec->Branch("m_QTRerr",&m_QTRerr,"m_QTRerr/F");*/
    //trigger time and event time 
    TRec->Branch("m_triggerT",&m_triggerT,"m_triggerT/D"); //elec. trigger time
    TRec->Branch("m_dTriggerT",&m_dTriggerT,"m_dTriggerT/D"); //trigger time distance of two event
    TRec->Branch("m_FadcEvtT",&m_FadcEvtT,"m_FadcEvtT/D"); //event time in the FADC window
    //rec. status
    /*TRec->Branch("m_Qtime",&m_Qtime,"m_Qtime/F");
    TRec->Branch("m_Ttime",&m_Ttime,"m_Ttime/F");
    TRec->Branch("m_QTtime",&m_QTtime,"m_QTtime/F");
    TRec->Branch("m_memory",&m_memory,"m_memory/F");*/
    // The first hit time of PMTs with elec simulation are sorted from PMTID 0 to the final one.
    TRec->Branch("LPMT_FHT", &m_lpmt_fht);
    TRec->Branch("SPMT_FHT", &m_spmt_fht);
    if(TruthLevel >= 1){

        //Physics truth
	    TRec->Branch("PDGID",			&m_pdgid,			"PDGID/I");
	    TRec->Branch("init_x",			&m_init_x,			"init_x/F");
	    TRec->Branch("init_y",			&m_init_y,			"init_y/F");
	    TRec->Branch("init_z",			&m_init_z,			"init_z/F");
	    TRec->Branch("init_r",			&m_init_r,			"init_r/F");
	    TRec->Branch("init_t",			&m_init_t,			"init_t/D");
	    TRec->Branch("edep",			&m_edep,			"edep/F");
	    TRec->Branch("edep_x",			&m_edep_x,			"edep_x/F");
	    TRec->Branch("edep_y",			&m_edep_y,			"edep_y/F");
	    TRec->Branch("edep_z",			&m_edep_z,			"edep_z/F");
	    TRec->Branch("edep_r",			&m_edep_r,			"edep_r/F");


        if(TruthLevel == 2){

            //LPMT truth
            TRec->Branch("LPMT_PulseType",      &m_lpmt_pulsetype);
            TRec->Branch("LPMT_PMTID",          &m_lpmt_id);
            TRec->Branch("LPMT_Npe",            &m_lpmt_npe);
            TRec->Branch("LPMT_HitTime",        &m_lpmt_hittime);
            TRec->Branch("LPMT_Amplitude",      &m_lpmt_amplitude);
            TRec->Branch("LPMT_TTS",            &m_lpmt_tts);
            TRec->Branch("LPMT_TimeOffset",     &m_lpmt_timeoffset);
            TRec->Branch("LPMT_PulseHitTime",   &m_lpmt_pulsehittime);

            //SPMT truth
            TRec->Branch("SPMT_TagID",          &m_spmt_tagid);
            TRec->Branch("SPMT_PulseType",      &m_spmt_pulsetype);
            TRec->Branch("SPMT_PMTID",          &m_spmt_id);
            TRec->Branch("SPMT_Npe",            &m_spmt_npe);
            TRec->Branch("SPMT_HitTime",        &m_spmt_hittime);
            TRec->Branch("SPMT_TimeWindow",     &m_spmt_timewindow);
            TRec->Branch("SPMT_Amplitude",      &m_spmt_amplitude);
            TRec->Branch("SPMT_TTS",            &m_spmt_tts);
            TRec->Branch("SPMT_PulseHitTime",   &m_spmt_pulsehittime);
            TRec->Branch("SPMT_EventHitTime",   &m_spmt_evthittime);

            // In the output file, the true infomation of PMT aren't sorted. Please check the PMTID for details to see which
            // PMT get hit and have the true QT information.
        }

    }


    /*TRec->Branch("Truex",&Truex,"Truex/F");
    TRec->Branch("Truey",&Truey,"Truey/F");
    TRec->Branch("Truez",&Truez,"Truez/F");
    TRec->Branch("TrueQx",&TrueQx,"TrueQx/F");
    TRec->Branch("TrueQy",&TrueQy,"TrueQy/F");
    TRec->Branch("TrueQz",&TrueQz,"TrueQz/F");
    TRec->Branch("TrueR",&TrueR,"TrueR/F");
    TRec->Branch("TrueQR",&TrueQR,"TrueQR/F");
    TRec->Branch("edep",&edep,"edep/F");*/
    #endif


    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom();
    TotalLPMT = m_cdGeom->findPmt20inchNum();
    TotalSPMT = m_cdGeom->findPmt3inchNum();

    LogInfo << "Total LPMT & SPMT: " << TotalLPMT << '\t' <<TotalSPMT<<std::endl;
    for(int i = 0; i<TotalLPMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_LPMT_pos.push_back(all_pmtCenter);
    }
    for(int i = 0; i<TotalSPMT; i++){
        unsigned int all_pmt_id = (unsigned int)(i+ZeroSpmtID);
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_SPMT_pos.push_back(all_pmtCenter);
    }
 
    m_pmtpara_tool = tool<IRecParaTool>("PMTParaTool");
    m_pmtpara_tool->init();
    m_pmtpara_tool->LoadPMTPara();
    
    m_pmtinput_tool = tool<IRecInputTool>("PMTInputTool");
    m_pmtinput_tool->init();

    //m_NPEUnifCTool = tool<NPEUnifCTool>("NPEUnifCTool");
    //m_NPEUnifCTool->configure();

    //PMTTable pmttable;
    //RecInfo recinfo;
    //TVector3 v3(10000., 10000., 10000.);
    //recinfo.energy = 1.;
    //recinfo.vtx = v3;
    //m_NPEUnifCTool->rec(pmttable, recinfo);
    //std::cout<< "energy:" << recinfo.energy << std::endl;

    // Record of final PMTPara 
    if(enableDebug) {
        TestFile = new TFile(Form("TestFile_%d.root", runID), "RECREATE");
        TH1D* hLGain = new TH1D("hLGain", "hLGain", 300, 0, 3);
        TH1D* hLQRes = new TH1D("hLQRes", "hLQRes", 100, 0, 1);
        TH1D* hLDNR  = new TH1D("hLDNR", "hLDNR", 120, 0, 120e3);
        TH1D* hLPDE  = new TH1D("hLPDE", "hLPDE", 2000, 0, 2);
        TH1D* hTOff = new TH1D("hTOff", "hTOff", 1000, -50, 50);
        for(int i=0;i<TotalLPMT;i++) {
            hLGain->Fill(m_pmtpara_tool->GetLGain(i));
            hLQRes->Fill(m_pmtpara_tool->GetLQRes(i));
            hLDNR->Fill(m_pmtpara_tool->GetLDNR(i));
            hLPDE->Fill(m_pmtpara_tool->GetLPDE(i));
            hTOff->Fill(m_pmtpara_tool->GetTimeOffset(i));
        }
        TestFile->cd();
        hLGain->Write();
        hLQRes->Write();
        hLDNR->Write();
        hLPDE->Write();
        hTOff->Write();  

        delete hLGain;
        delete hLQRes;
        delete hLDNR;
        delete hLPDE;
        delete hTOff;
    }
    //The default energy scale is defined by Ge-68
    nHESF = 1.;

    ////////////////////////
    // Load the Expectations
    // -- 3D nPEMap
    // -- Residual time pdfs
    // -- Charge response
    ////////////////////////
    Load_ExpectedPEQTime();


    fcn1 = new MyFCN1(this);
    recminuit = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
    

    if(enableReadTruth) { 
        LogDebug << "===> Reading simulation file " << std::endl;
        LogInfo  << SimFile << std::endl;
        sfile = TFile::Open(SimFile.c_str(), "READ");
        simevt = (TTree*)sfile->Get("evt");

        simevt->SetBranchAddress("edepX",&Truex);
        simevt->SetBranchAddress("edepY",&Truey);
        simevt->SetBranchAddress("edepZ",&Truez);

        simevt->SetBranchAddress("edep",&edep);

        prmtrkdep = (TTree*)sfile->Get("prmtrkdep");
        prmtrkdep->SetBranchAddress("QedepX",&TrueQx);
        prmtrkdep->SetBranchAddress("QedepY",&TrueQy);
        prmtrkdep->SetBranchAddress("QedepZ",&TrueQz);
    }

    // Todo optimize the QCfactor
    if(RecMode == "Toy" && !enableAddToyDN) QCfactor = 1.1;
    if(RecMode == "Real" || enableAddToyQ) enableQTimePdf = true;

    enableTimeInfo_user = enableTimeInfo;
    enableUseEkMap_user = enableUseEkMap;
    enableSPMTInfo_user = enableSPMTInfo;

    LogInfo  << objName()
        << "   initialized successfully"
        << std::endl; 

    return true; 

}


bool OMILREC::execute()
{
    ++m_iEvt;
    //////////////////
    // Read truth info
    if(enableReadTruth) {
        simevt->GetEntry(m_iEvt);
        prmtrkdep->GetEntry(m_iEvt);
    }
    

    LogDebug << "---------------------------------------" << std::endl;
    LogDebug << "Processing OMILREC algorithm to reconstruct energy" << std::endl;
    LogDebug << "Processing event " << m_iEvt << std::endl;
    if(enableOccupancy) LogDebug <<">>>> enableOccupancy!"<<std::endl;
  
    
    bool st = m_pmtinput_tool->LoadPMTInput(m_pmtpara_tool);
    if (!st) { // skip this event due to no input
        m_iEvt--;
        return true;
    }



    // Extract true information from EDM
	if(TruthLevel >= 1){

        JM::TrackElecTruthHeader* trk_elec_truth_hdr = JM::getHeaderObject<JM::TrackElecTruthHeader>(m_pmtinput_tool->Get_nav());
        if(not trk_elec_truth_hdr){
            LogWarn << "The TrackElecTruth (truths of physics event) does not exist in this event" << std::endl;
            m_iEvt--;
            return true;
        }
        if(not trk_elec_truth_hdr->hasEvent()){
            LogWarn << "The TrackElecTruth (truths of physics event) does not exist in this event" << std::endl;
            m_iEvt--;
            return true;
        }

        JM::TrackElecTruthEvt* trk_elec_truth_evt = trk_elec_truth_hdr->event();
        const std::vector<JM::TrackElecTruth>& vec_trk_elec_truth = trk_elec_truth_evt->truths();
        if(vec_trk_elec_truth.size() == 0){
            LogWarn << "The TrackElecTruth (truths of physics event) does not exist in this event" << std::endl;
            m_iEvt--;
            return true;
        }

        const JM::TrackElecTruth& trk_elec_truth = vec_trk_elec_truth.at(0); // For the purpose of training,
        m_pdgid  = trk_elec_truth.pdgid();                                   // normally the vector only include 1 element,
        m_init_x = trk_elec_truth.initX();                                   // which is the track of e+
        m_init_y = trk_elec_truth.initY();
        m_init_z = trk_elec_truth.initZ();
        m_init_t = trk_elec_truth.initT();
		m_edep   = trk_elec_truth.edep();
		m_edep_x = trk_elec_truth.edepX();
		m_edep_y = trk_elec_truth.edepY();
		m_edep_z = trk_elec_truth.edepZ();
		m_init_r = TMath::Sqrt(m_init_x*m_init_x+m_init_y*m_init_y+m_init_z*m_init_z);
		m_edep_r = TMath::Sqrt(m_edep_x*m_edep_x+m_edep_y*m_edep_y+m_edep_z*m_edep_z);



        if(TruthLevel == 2){

            m_lpmt_pulsetype->clear();
            m_lpmt_id->clear();
            m_lpmt_npe->clear();
            m_lpmt_hittime->clear();
            m_lpmt_amplitude->clear();
            m_lpmt_tts->clear();
            m_lpmt_timeoffset->clear();
            m_lpmt_pulsehittime->clear();

            m_spmt_tagid->clear();
            m_spmt_pulsetype->clear();
            m_spmt_id->clear();
            m_spmt_npe->clear();
            m_spmt_hittime->clear(); 
            m_spmt_timewindow->clear();
            m_spmt_amplitude->clear();
            m_spmt_tts->clear();
            m_spmt_pulsehittime->clear();
            m_spmt_evthittime->clear();


            // Each vector corresponds to a physics event, and each element in a vector corresponds to an electronic trigger
            // caused by this physics event. Therefore, you'll find n enteries for n physics events.
            JM::CdLpmtElecTruthHeader* lpmt_elec_truth_hdr = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(m_pmtinput_tool->Get_nav()); 
            if(lpmt_elec_truth_hdr and lpmt_elec_truth_hdr->hasEvent()){
                JM::CdLpmtElecTruthEvt* lpmt_elec_truth_evt = lpmt_elec_truth_hdr->event();
                const std::vector<JM::LpmtElecTruth>& vec_lpmt_elec_truth = lpmt_elec_truth_evt->truths();
                for(const JM::LpmtElecTruth& lpmt_elec_truth : vec_lpmt_elec_truth){
                    m_lpmt_pulsetype->push_back(lpmt_elec_truth.pulsetype());
                    m_lpmt_id->push_back(lpmt_elec_truth.pmtId());
                    m_lpmt_npe->push_back(lpmt_elec_truth.npe());
                    m_lpmt_hittime->push_back(lpmt_elec_truth.hitTime());
                    m_lpmt_amplitude->push_back(lpmt_elec_truth.amplitude());
                    m_lpmt_tts->push_back(lpmt_elec_truth.tts());
                    m_lpmt_timeoffset->push_back(lpmt_elec_truth.timeoffset());
                    m_lpmt_pulsehittime->push_back(lpmt_elec_truth.pulseHitTime().GetSeconds());
                }
            }
            else LogWarn << "The LpmtElecTruth does not exist in this event" << std::endl; 


            JM::CdSpmtElecTruthHeader* spmt_elec_truth_hdr = JM::getHeaderObject<JM::CdSpmtElecTruthHeader>(m_pmtinput_tool->Get_nav()); 
            if(spmt_elec_truth_hdr and spmt_elec_truth_hdr->hasEvent()){
                JM::CdSpmtElecTruthEvt* spmt_elec_truth_evt = spmt_elec_truth_hdr->event();
                const std::vector<JM::SpmtElecTruth>& vec_spmt_elec_truth = spmt_elec_truth_evt->truths();
                for(const JM::SpmtElecTruth& spmt_elec_truth : vec_spmt_elec_truth){
                    m_spmt_tagid->push_back(spmt_elec_truth.tagId());
                    m_spmt_pulsetype->push_back(spmt_elec_truth.pulsetype());
                    m_spmt_id->push_back(spmt_elec_truth.pmtId());
                    m_spmt_npe->push_back(spmt_elec_truth.npe());
                    m_spmt_hittime->push_back(spmt_elec_truth.hitTime());
                    m_spmt_timewindow->push_back(spmt_elec_truth.timeWindow());
                    m_spmt_amplitude->push_back(spmt_elec_truth.amplitude());
                    m_spmt_tts->push_back(spmt_elec_truth.tts());
                    m_spmt_pulsehittime->push_back(spmt_elec_truth.pulseHitTime().GetSeconds());
                    m_spmt_evthittime->push_back(spmt_elec_truth.evtHitTime().GetSeconds());
                }
            }
            else LogWarn << "The SpmtElecTruth does not exist in this event" << std::endl; 


        }

    }
    // Fill the vector of first hit time for output usage
    m_lpmt_fht->clear();
    m_lpmt_fht->resize(TotalLPMT);
    for(int pmtid = 0; pmtid<TotalLPMT; pmtid++){
        m_lpmt_fht->at(pmtid) = m_pmtinput_tool->Get_LHIT_Time(pmtid);
    }

    m_spmt_fht->clear();
    m_spmt_fht->resize(TotalSPMT);
    for(int pmtid = 0; pmtid<TotalSPMT; pmtid++){
        m_spmt_fht->at(pmtid) = m_pmtinput_tool->Get_SHIT_Time(pmtid);
    }


    // Read EvtTime
    TTimeStamp EvtTime = (m_pmtinput_tool->Get_nav())->TimeStamp();
    LogInfo << "evt time stamp: " << EvtTime.GetSec()*1.e9 + EvtTime.GetNanoSec() << std::endl;
    m_dTriggerT = 0.;
    EvtT =  EvtTime.GetSec()*1.e9 + EvtTime.GetNanoSec();
    EvtTArray.push_back(EvtT);
    if(m_iEvt>0) m_dTriggerT = EvtT - EvtTArray.at(m_iEvt-1);
    m_triggerT = EvtT;

    //int64_t m_cur = 20201220;
    //m_conddb_svc->setCurrent(m_cur);


    LTOF.clear();
    for(int i=0;i<TotalLPMT;i++) {
         LTOF.push_back(0.);
    }
    STOF.clear();
    for(int i=0;i<TotalSPMT;i++) {
         STOF.push_back(0.);
    }   

    TStopwatch QTtimer;
    QTtimer.Start();

    TStopwatch Qtimer;
    Qtimer.Start();

    // User can specify the enableUseFixedVertex when event vertex is known
    // For example, some ACU calibration events
    if(enableUseFixedVertex) {
        Truex = fix_x;
        Truey = fix_y;
        Truez = fix_z;
    }

    //*************************
    // Load RecEvent
    //*************************
    JM::CdVertexRecHeader* aDataHdr = NULL;
    JM::CdVertexRecEvt* aDataEvt = NULL;

    //Todo explain
    if(enableUseTLHVertex) {
        aDataHdr = JM::getHeaderObject<JM::CdVertexRecHeader>(m_pmtinput_tool->Get_nav());
        aDataEvt = aDataHdr->event();
        auto aData = aDataEvt->getVertex(0); // get the first vertex
        timeRecx = aData->x();
        timeRecy = aData->y();
        timeRecz = aData->z();
        timeRecE = aData->eprec();
        timeRect0 = aData->px();
        LogDebug  << "Done to Load RecEvent >>>"
                  << timeRecx<<'\t'<<timeRecy<<'\t'<<timeRecz<<'\t'<<timeRect0<<'\t'<<timeRecE<<'\t'<<std::endl;
    }
    if(!aDataHdr) {
        aDataHdr = new JM::CdVertexRecHeader;
        aDataEvt = new JM::CdVertexRecEvt;
    }


    double ChargePos[3] = {0., 0., 0.};
    GetChargeCenter(ChargePos);
    recQx = QCfactor*ChargePos[0];
    recQy = QCfactor*ChargePos[1];
    recQz = QCfactor*ChargePos[2]; 


    //****************************************
    // Set initial value for energy and vertex
    //****************************************
    double QR = sqrt(recQx*recQx + recQy*recQy + recQz*recQz);
    double QEvtTheta = TMath::Pi()/180./100.;
    double QEvtPhi = 2.*TMath::Pi()/180./100.;

    CoorTransform(recQx, recQy, recQz, QR, QEvtTheta, QEvtPhi);


    TrueQR = sqrt(TrueQx*TrueQx + TrueQy*TrueQy + TrueQz*TrueQz);
    TrueR = sqrt(Truex*Truex + Truey*Truey + Truez*Truez);
    double TrueEvtR = 0.;
    double TrueEvtTheta = 0.;
    double TrueEvtPhi = 0.;

    LogDebug<<"QR: "<< QR <<std::endl;
    
    if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex) {
        CoorTransform(Truex, Truey, Truez, TrueEvtR, TrueEvtTheta, TrueEvtPhi);
        recQx = Truex;
        recQy = Truey;
        recQz = Truez;
        if(enableUseTLHVertex) {
            recQx = timeRecx;
            recQy = timeRecy;
            recQz = timeRecz;
        }

    }
    ////////////
    // Remove DN
    if(RecMode=="Real") {
        if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex) RemoveDN(m_pmtinput_tool->Get_chhlist(), recQx, recQy, recQz, true);
        else RemoveDN(m_pmtinput_tool->Get_chhlist(), recQx, recQy, recQz, false);

        GetChargeCenter(ChargePos);
        recQx = QCfactor*ChargePos[0];
        recQy = QCfactor*ChargePos[1];
        recQz = QCfactor*ChargePos[2];
    }

    if(RecMode=="Toy" && enableAddToyDN) {
        if(enableUseTrueVertex||enableUseFixedVertex) RemoveToyDN(recQx, recQy, recQz, true);
        else RemoveToyDN(recQx, recQy, recQz, false);
        GetChargeCenter(ChargePos);
        recQx = QCfactor*ChargePos[0];
        recQy = QCfactor*ChargePos[1];
        recQz = QCfactor*ChargePos[2];
    }

    if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex) {
        recQx = Truex;
        recQy = Truey;
        recQz = Truez;
        if(enableUseTLHVertex) {
            recQx = timeRecx;
            recQy = timeRecy;
            recQz = timeRecz;
        }

    }

    CoorTransform(recQx, recQy, recQz, QR, QEvtTheta, QEvtPhi);
   

    // Todo add more time stamp 
    /////////////
    // Start QMLE 
    ROOT::Math::Functor recfunc1(*fcn1, 5);

    recminuit->SetFunction(recfunc1);
    recminuit->SetMaxFunctionCalls(maxfuncalls);
    recminuit->SetMaxIterations(maxiters);
    recminuit->SetTolerance(tolerance);

    Use3DMap = false;
    if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex||!enableTimeInfo_user||enableSPMTOnly) Use3DMap = true;
    enableUseEkMap = enableUseEkMap_user;
    enableQInfo = true;
    enableTimeInfo = false;
    if(QR>rrange[1]) QR = rrange[1]-1.;

    recminuit->SetLimitedVariable(0,"n", LPMTCalibEnergy, estep, erange[0], erange[1]);

    if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex) {
        recminuit->SetFixedVariable(1, "rPos", QR);
        recminuit->SetFixedVariable(2, "thetaPos", QEvtTheta);
        recminuit->SetFixedVariable(3, "phiPos", QEvtPhi);
        recminuit->SetFixedVariable(4, "t0", 0.);
    } else {
        recminuit->SetLimitedVariable(1, "rPos", 10000., rstep, rrange[0], rrange[1]);
        recminuit->SetFixedVariable(2, "thetaPos", QEvtTheta);
        recminuit->SetFixedVariable(3, "phiPos", QEvtPhi);
        recminuit->SetFixedVariable(4, "t0", 0.);
    }


    LogInfo<<"Start Minimization!"<<std::endl;
     
    int ief = recminuit->Minimize();

    if(enableSPMTOnly) m_NQE = m_NPE/m_UfrmScale;
    else m_NQE = m_NPE/m_UfrmScale;

    
    const double *xs = recminuit->X();
    const double *xerr = recminuit->Errors();

    double rec_QE = xs[0];
    double rec_QR = xs[1];
    double rec_QTheta = xs[2];
    double rec_QPhi = xs[3];
    double E_err = xerr[0];
    double R_err = xerr[1];

    recQx = rec_QR*TMath::Sin(rec_QTheta)*TMath::Cos(rec_QPhi);
    recQy = rec_QR*TMath::Sin(rec_QTheta)*TMath::Sin(rec_QPhi);
    recQz = rec_QR*TMath::Cos(rec_QTheta);

    recx = recQx;
    recy = recQy;
    recz = recQz;

    m_QEn = rec_QE;
    m_QR = rec_QR;
    m_Qstat = recminuit->CovMatrixStatus();
    m_QL = recminuit->MinValue();
    m_QRerr = xerr[1];
    m_PESum = m_QEn*Ge68ES;

    LogDebug << "The Qstat and MinValue is: " << m_Qstat << '\t' << m_QL <<std::endl;
    LogDebug << "The m_QEn & QR & Rerr is "<< rec_QE << '\t' << rec_QR << '\t' << m_QRerr << std::endl;

    Qtimer.Stop();
    m_Qtime = Qtimer.RealTime();

    if(enableUseTrueVertex||enableUseFixedVertex||enableUseTLHVertex||!enableTimeInfo_user||enableSPMTOnly) {

        ProcInfo_t info;
        gSystem->GetProcInfo(&info);
        m_memory = info.fMemResident/1024;

        Printf("Memory = %.0ld MB, MemVirtual = %.0ld MB", info.fMemResident/1024, info.fMemVirtual/1024);

        LogDebug<<"==========================================================="<<std::endl;
        LogDebug<<"The Reconstructed x is "<<Truex<<'\t'<<recx<<'\t'<<ChargePos[0]<<std::endl;
        LogDebug<<"The Reconstructed y is "<<Truey<<'\t'<<recy<<'\t'<<ChargePos[1]<<std::endl;
        LogDebug<<"The Reconstructed z is "<<Truez<<'\t'<<recz<<'\t'<<ChargePos[2]<<std::endl;
        LogDebug<<"The Reconstructed R is "<<TrueR<<'\t'<<rec_QR<<std::endl;
        LogDebug<<"The Reconstructed Theta is "<<TrueEvtTheta<<'\t'<<rec_QTheta<<std::endl;
        LogDebug<<"The Reconstructed Phi is "<<TrueEvtPhi<<'\t'<<rec_QPhi<<std::endl;

        LogDebug<<"The Reconstructed energy is "<<m_QEn<<" +- "<< E_err << '\t' << m_NQE<<std::endl;
        LogDebug<<"The Reconstructed CovStat and Eerr is "<<m_Qstat<<'\t'<<E_err<<std::endl;
        LogDebug<<"The Reconstruction Process Cost "<<m_Qtime<<std::endl;
        LogDebug<<"The Complete Reconstrution Process is Completed!"<<std::endl;
        LogDebug<<"==========================================================="<<std::endl;
 
        #ifndef BUILD_ONLINE
        TRec->Fill();
        #endif
        
        auto aData = new JM::RecVertex();
        //EvtEV
        aData->setPESum(m_NPE);
        aData->setEnergy(m_QEn);
        aData->setEprec(m_QEn);
        aData->setX(recx);
        aData->setY(recy);
        aData->setZ(recz);
        aData->setT0(m_FadcEvtT);

        //RecQuality
        aData->setPx(m_QL);
        aData->setChisq(m_QL);
        aData->setEnergyQuality(E_err);
        aData->setPositionQuality(R_err);


        aDataEvt->addVertex(aData);
        aDataHdr->setEvent(aDataEvt);
        JM::addHeaderObject(m_pmtinput_tool->Get_nav(), aDataHdr);

        Readout_PE.clear();
        Readout_hit_time.clear();
        recminuit->Clear();

        return true;
    }
    TStopwatch Ttimer;
    Ttimer.Start();

    // Start QTMLE
    // Remove the dark noise again after QMLE
    if(RecMode=="Real") RemoveDN(m_pmtinput_tool->Get_chhlist(), recQx, recQy, recQz, true);
    else {
        if(enableAddToyDN) {
            RemoveToyDN(recQx, recQy, recQz, true);
        } else {
            double m_x = recQx;
            double m_y = recQy;
            double m_z = recQz;
            for(int pmtid=0;pmtid<TotalLPMT;pmtid++) {   
                LTOF[pmtid] = CalLTOF(pmtid, m_x, m_y, m_z);
            }
        }
    }
    
    // Move the peak of the rsd time distribution to 100 ns
    //TH1D* hRsdTime = new TH1D("hRsdTime", "hRsdTime", 2400, -200, 1000); //for ideal case
    //TH1D* hRsdTimedum = new TH1D("hRsdTimedum", "hRsdTimedum", 800, -200, 1000); //for ideal case
    TH1D* hRsdTime = new TH1D("hRsdTime", "hRsdTime", 1200, -200, 1000); // for tts case
    TH1D* hRsdTimedum = new TH1D("hRsdTimedum", "hRsdTimedum", 300, -200, 1000); //for tts case
    hRsdTime->SetName(Form("QMLE_hRsdTime_%d", m_iEvt));
    hRsdTimedum->SetName(Form("QMLE_hRsdTimedum_%d", m_iEvt));
    for(int i=0;i<TotalLPMT;i++) {
        double hitt_temp = m_pmtinput_tool->Get_LHIT_Time(i);
        if(TMath::Abs(hitt_temp)>1e-8) {
            hRsdTime->Fill(hitt_temp - LTOF[i] - m_pmtpara_tool->GetTimeOffset(i));
            hRsdTimedum->Fill(hitt_temp - LTOF[i] - m_pmtpara_tool->GetTimeOffset(i));
        }
    }
    if(enableDebug) {
        TestFile->cd();
        hRsdTime->Write();
    }
    int HitMaxBinRec = hRsdTimedum->GetMaximumBin();
    double maxTime = hRsdTimedum->GetBinCenter(HitMaxBinRec);
    
    //for ideal case
    /* 
    hRsdTime->Fit("gaus", "Q0R", "", maxTime - 5., maxTime + 5.);
    TF1* func_temp = (TF1*)hRsdTime->GetFunction("gaus");
    maxTime = func_temp->GetMaximumX(); 
    */
    double t0Rec = maxTime - 100.;
    for(int i=0;i<TotalLPMT;i++) {
        double hitt_temp = m_pmtinput_tool->Get_LHIT_Time(i);
        if(hitt_temp!=0) m_pmtinput_tool->Set_LHIT_Time(i, hitt_temp - t0Rec);
    }
    for(int i=0;i<TotalSPMT;i++) {
        double hitt_temp = m_pmtinput_tool->Get_SHIT_Time(i);
        if(hitt_temp!=0) m_pmtinput_tool->Set_SHIT_Time(i, hitt_temp - t0Rec);
    }
    delete hRsdTime;
    delete hRsdTimedum;


    //////////////////////////////
    // reconstruction w/ time info
    recminuit->Clear();
    recminuit->SetFunction(recfunc1);
    recminuit->SetMaxFunctionCalls(maxfuncalls);
    recminuit->SetMaxIterations(maxiters);
    recminuit->SetTolerance(tolerance);

    enableQInfo = false;
    enableUseEkMap = false;
    enableTimeInfo = true;
    UseGeETimePdf = false;
    recminuit->SetFixedVariable(0, "n", m_QEn);
    recminuit->SetLimitedVariable(1, "rPos", rec_QR, rstep, rrange[0], rrange[1]);
    recminuit->SetLimitedVariable(2, "thetaPos", rec_QTheta, tangstep, tangrange[0], tangrange[1]);
    recminuit->SetLimitedVariable(3, "phiPos", rec_QPhi, pangstep, pangrange[0], pangrange[1]);
    recminuit->SetLimitedVariable(4, "t0", 0., t0step, t0range[0], t0range[1]);

    ief = recminuit->Minimize();
    xs = recminuit->X();
    xerr = recminuit->Errors();

    m_Tstat = recminuit->CovMatrixStatus();
    m_TL = recminuit->MinValue();
    m_TRerr = xerr[1];
    
    double rec_TR     = xs[1];
    double rec_TTheta = xs[2];
    double rec_TPhi   = xs[3];
           recTt0     = xs[4];

    m_TR  = rec_TR;
    recTx = rec_TR*TMath::Sin(rec_TTheta)*TMath::Cos(rec_TPhi);
    recTy = rec_TR*TMath::Sin(rec_TTheta)*TMath::Sin(rec_TPhi);
    recTz = rec_TR*TMath::Cos(rec_TTheta);

    LogDebug << "The Tstat and MinValue is: " << m_Tstat << '\t' << m_TL << std::endl;
    LogDebug << "The TR, Ttheta, Tphi and Tt0 is: " 
             << m_TR << '\t' << rec_TTheta << '\t' << rec_TPhi << '\t' << recTt0 <<std::endl;

    Ttimer.Stop();
    m_Ttime = Ttimer.RealTime();

    /////////////////////////////////////
    // reconstruction w/ charge&time info
    recminuit->Clear();
    recminuit->SetFunction(recfunc1);
    recminuit->SetMaxFunctionCalls(maxiters);
    recminuit->SetMaxIterations(maxfuncalls);
    recminuit->SetTolerance(tolerance);

    Use3DMap = false;
    enableQInfo = true;
    enableUseEkMap = enableUseEkMap_user;
    enableTimeInfo = true;
    UseGeETimePdf = false;
    recminuit->SetFixedVariable(0, "n", m_QEn);
    recminuit->SetLimitedVariable(1, "rPos", rec_QR, rstep, rrange[0], rrange[1]);
    recminuit->SetFixedVariable(2, "thetaPos", rec_TTheta); 
    recminuit->SetFixedVariable(3, "phiPos", rec_TPhi);
    recminuit->SetLimitedVariable(4, "t0", 0., t0step, t0range[0], t0range[1]);

    ief = recminuit->Minimize();
    xs = recminuit->X();
    xerr = recminuit->Errors();
    m_QTstat = recminuit->CovMatrixStatus();
    m_QTL = recminuit->MinValue();
    m_QTRerr = xerr[1];

    double rec_QTEn    = xs[0];
    double rec_QTR     = xs[1];
    double rec_QTTheta = xs[2];
    double rec_QTPhi   = xs[3];
           recQTt0     = xs[4];

    E_err = xerr[0];
    R_err = xerr[1];

    m_QTR = rec_QTR;
    m_QTEn = rec_QTEn;
    recQTx = rec_QTR*TMath::Sin(rec_QTTheta)*TMath::Cos(rec_QTPhi);
    recQTy = rec_QTR*TMath::Sin(rec_QTTheta)*TMath::Sin(rec_QTPhi);
    recQTz = rec_QTR*TMath::Cos(rec_QTTheta);

    recx = recQTx;
    recy = recQTy;
    recz = recQTz;

    if(enableSPMTOnly) m_NQE = m_NPE/m_UfrmScale;
    else m_NQE = m_NPE/m_UfrmScale;

    double QTL = Calculate_EVLikelihood(m_QTEn, rec_QTR, rec_QTTheta, rec_QTPhi, recQTt0);
    LogDebug << "The QTstat and MinValue is: " << m_QTstat << '\t'<< m_QTL <<std::endl;
    LogDebug << "The m_QTEn & QTR & Rerr & Likelihood is "<< m_QTEn << '\t' << rec_QTR <<  '\t' << m_QTRerr << '\t' << QTL <<std::endl;

    enableTimeInfo = false;
    double LtQ = Calculate_EVLikelihood(m_QTEn, rec_QTR, rec_QTTheta, rec_QTPhi, 0.);
    LogDebug << "The refined Likelihood is "<< LtQ <<std::endl;

    QTtimer.Stop();
    m_QTtime = QTtimer.RealTime();

    ProcInfo_t info;
    gSystem->GetProcInfo(&info);
    m_memory = info.fMemResident/1024;

    Printf("Memory = %.0ld MB, MemVirtual = %.0ld MB", info.fMemResident/1024, info.fMemVirtual/1024);

    LogDebug<<"==========================================================="<<std::endl;
    LogDebug<<"The Reconstructed tx, rx, qx is "<<Truex<<'\t'<<recx<<'\t'<<ChargePos[0]<<std::endl;
    LogDebug<<"The Reconstructed ty, ry, qy is "<<Truey<<'\t'<<recy<<'\t'<<ChargePos[1]<<std::endl;
    LogDebug<<"The Reconstructed tz, rz, qz is "<<Truez<<'\t'<<recz<<'\t'<<ChargePos[2]<<std::endl;
    LogDebug<<"The Reconstructed t0 is "<<recQTt0<<std::endl;
    LogDebug<<"The Reconstructed tr, tqedepr, Qr, Tr, QTr is "<<TrueR<<'\t'<<TrueQR<<'\t'<<rec_QR<<'\t' << m_TR << '\t' << rec_QTR <<std::endl;
    LogDebug<<"The Reconstructed TQnE, TQE, QE, NPE, rr is "<<m_QTEn<<'\t'<<m_QEn<<'\t'<<m_NPE<<'\t'<<rec_QTR<<std::endl;
    LogDebug<<"The Reconstructed energy is "<<m_QTEn<<" +- "<<E_err<<'\t'<<m_QTstat<<std::endl;
    LogDebug<<"The Reconstruction Process Cost "<<m_QTtime<<std::endl;
    LogDebug<<"The Complete Reconstrution Process is Completed!"<<std::endl;
    LogDebug<<"==========================================================="<<std::endl;


    ///////////////////////////////////////
    // Optimize energy with the best vertex
    recminuit->Clear();
    recminuit->SetFunction(recfunc1);
    recminuit->SetMaxFunctionCalls(maxiters);
    recminuit->SetMaxIterations(maxfuncalls);
    recminuit->SetTolerance(tolerance);
    recminuit->SetLimitedVariable(0,"n", m_QEn, estep, erange[0], erange[1]);

    Use3DMap = true;
    enableQInfo = true;
    enableUseEkMap = enableUseEkMap_user;
    enableTimeInfo = false;

    recminuit->SetFixedVariable(1, "rPos", rec_QTR);
    recminuit->SetFixedVariable(2, "thetaPos", rec_QTTheta);
    recminuit->SetFixedVariable(3, "phiPos", rec_QTPhi);
    recminuit->SetFixedVariable(4, "t0", recQTt0);

    ief = recminuit->Minimize();
    LogDebug << "The ief is: " << ief << std::endl;
    xs = recminuit->X();
    xerr = recminuit->Errors();
    E_err = xerr[0];

    rec_QTEn = xs[0];
    m_QTEn   = rec_QTEn; //update m_QTEn 
    m_PESum = m_QTEn*Ge68ES; //updatem_PESum
    LogDebug<<"The updated QTEn, rr is "<<m_QTEn<< '\t' << rec_QTR <<std::endl;

    // User output
    m_FadcEvtT = recQTt0 + t0Rec + 100.;
    #ifndef BUILD_ONLINE
    TRec->Fill();
    #endif
    
    // EDM output
    auto aData = new JM::RecVertex();
    //EvtEV
    aData->setPESum(m_NPE);
    aData->setEnergy(m_QTEn);
    aData->setEprec(m_QTEn);
    aData->setX(recx);
    aData->setY(recy);
    aData->setZ(recz);
    aData->setT0(m_FadcEvtT);

    //RecQuality
    aData->setPx(m_QL);
    aData->setPy(m_TL);
    aData->setChisq(m_QTL);
    aData->setEnergyQuality(E_err);
    aData->setPositionQuality(R_err);

    aDataEvt->addVertex(aData);
    aDataHdr->setEvent(aDataEvt);
    JM::addHeaderObject(m_pmtinput_tool->Get_nav(), aDataHdr);

    Readout_PE.clear();
    Readout_hit_time.clear();
    recminuit->Clear();


    return true; 

}


bool OMILREC::Load_ExpectedPEQTime() {
    LogDebug << "Load the 3-D nPEMap, the residual time pdfs and the charge response pdfs" << std::endl;
    bool UseTruthFile = false;
    if(RecMode=="Toy" && !enableAddToyQ && !enableAddToyTTS && !enableAddToyDN) {
        UseTruthFile = true;
    }
    std::string LnPEMapFileName = "/LnPEMapFile.root";
    std::string SnPEMapFileName = "/SnPEMapFile.root";
    if(UseTruthFile) {
        LnPEMapFileName = "/LnPEMapFile_Truth.root";
        SnPEMapFileName = "/SnPEMapFile_Truth.root";
    }
    //read Ge-68 to n-H energy scale factor
    std::ifstream ESFin;
    ESFin.open((nPEMapPath + "/nHEScaleFactor.txt").c_str(), std::ios::in);
    if(ESFin.is_open()==true){
        ESFin>>nHESF;
        ESFin.close();
    } else {
        LogError<< "Can't find nHEScaleFactor.txt!" << std::endl;
    }

    TFile* LnPEMapFile = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
    if(!LnPEMapFile) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
    for(int i=0;i<ThetaPMTNum;i++) {
        LMu2D[i] = (TH2F*)LnPEMapFile->Get(Form("hLMu2D_%d", i));
        LMu2D[i]->SetDirectory(nullptr);
        LMu1D[i] = (TH1D*)LMu2D[i]->ProjectionX(Form("hLMu1D_%d", i), 1, 19);
        LMu1D[i]->Scale(1./19.);
        LMu1D[i]->SetDirectory(nullptr);
    }
    LnPEMapFile->Close();
    delete LnPEMapFile;
    LogDebug<< "LMu2D[ThetaPMTNum-1]->Interpolate(rrange[1], Pi/2) " 
            << LMu2D[ThetaPMTNum-1]->Interpolate(179, TMath::Pi()/2.) << std::endl;

    if(enableUseEkMap) {
        LnPEMapFileName = "/LnPEMapFile_Ek.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_Ek.root";
        TFile* LnPEMapFile_Ek = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_Ek) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<ThetaPMTNum;i++) {
            LMu2D_Ek[i] = (TH2F*)LnPEMapFile_Ek->Get(Form("hLMu2D_%d", i));
            LMu2D_Ek[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_Ek->Close();
        delete LnPEMapFile_Ek;
        LogDebug<< "LMu2D_Ek[ThetaPMTNum-1]->Interpolate(rrange[1], Pi/2) " 
                << LMu2D_Ek[ThetaPMTNum-1]->Interpolate(179, TMath::Pi()/2.) << std::endl;
    }
    //Calculate the energy scale of Ge68 and Laser map
    Ge68ES = 0.;
    LaserES = 0.;
    for(int i = 0; i< TotalLPMT; i++){
        double pmt_pos_x = ALL_LPMT_pos.at(i).X();
        double pmt_pos_y = ALL_LPMT_pos.at(i).Y();
        double pmt_pos_z = ALL_LPMT_pos.at(i).Z();

        double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
        double cos_theta = pmt_pos_z/PMTR;
        double theta = TMath::ACos(cos_theta)*180/TMath::Pi();
    
        int GrMapIdA = theta/MapThetaStep;
        int GrMapIdB = GrMapIdA + 1;
        
        double m_average_PEA = LMu2D[GrMapIdA]->Interpolate(0., 0.);
        double m_average_PEB = LMu2D[GrMapIdB]->Interpolate(0., 0.);
        double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
        double Weight = ThetaAFrac/MapThetaStep;
        double m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;

        Ge68ES +=  m_average_PE;

        if(enableUseEkMap) {
            m_average_PEA = LMu2D_Ek[GrMapIdA]->Interpolate(0., 0.);
            m_average_PEB = LMu2D_Ek[GrMapIdB]->Interpolate(0., 0.);
            m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;
            LaserES +=  m_average_PE;
        }
    }
    ElectronES = Ge68ES;
    LogInfo<< "Ge68ES: " << Ge68ES << '\t' << "LaserES: " << LaserES << std::endl; 


    
    if(enableUsePhiMap) {
        LnPEMapFileName = "/LnPEMapFile_2d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_2d.root";
        TFile* LnPEMapFile_2d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_2d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_2d[i] = (TH2F*)LnPEMapFile_2d->Get(Form("hLMu2D_%d", i));
            LMu2D_2d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_2d->Close();
        delete LnPEMapFile_2d;
        
        LnPEMapFileName = "/LnPEMapFile_4d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_4d.root";
        TFile* LnPEMapFile_4d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_4d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_4d[i] = (TH2F*)LnPEMapFile_4d->Get(Form("hLMu2D_%d", i));
            LMu2D_4d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_4d->Close();
        delete LnPEMapFile_4d;

        LnPEMapFileName = "/LnPEMapFile_6d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_6d.root";
        TFile* LnPEMapFile_6d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_6d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_6d[i] = (TH2F*)LnPEMapFile_6d->Get(Form("hLMu2D_%d", i));
            LMu2D_6d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_6d->Close();
        delete LnPEMapFile_6d;

        LnPEMapFileName = "/LnPEMapFile_Ek_2d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_Ek_2d.root";
        TFile* LnPEMapFile_Ek_2d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_Ek_2d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_Ek_2d[i] = (TH2F*)LnPEMapFile_Ek_2d->Get(Form("hLMu2D_%d", i));
            LMu2D_Ek_2d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_Ek_2d->Close();
        delete LnPEMapFile_Ek_2d;

        LnPEMapFileName = "/LnPEMapFile_Ek_4d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_Ek_4d.root";
        TFile* LnPEMapFile_Ek_4d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_Ek_4d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_Ek_4d[i] = (TH2F*)LnPEMapFile_Ek_4d->Get(Form("hLMu2D_%d", i));
            LMu2D_Ek_4d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_Ek_4d->Close();
        delete LnPEMapFile_Ek_4d;

        LnPEMapFileName = "/LnPEMapFile_Ek_6d.root";
        if(UseTruthFile) LnPEMapFileName = "/LnPEMapFile_Truth_Ek_6d.root";
        TFile* LnPEMapFile_Ek_6d = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
        if(!LnPEMapFile_Ek_6d) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<CutThetaPMTI;i++) {
            LMu2D_Ek_6d[i] = (TH2F*)LnPEMapFile_Ek_6d->Get(Form("hLMu2D_%d", i));
            LMu2D_Ek_6d[i]->SetDirectory(nullptr);
        }
        LnPEMapFile_Ek_6d->Close();
        delete LnPEMapFile_Ek_6d;
    }

    if(enableSPMTInfo) {
        TFile* SnPEMapFile = new TFile((nPEMapPath + SnPEMapFileName).c_str(), "READ");
        if(!SnPEMapFile) LogInfo<< nPEMapPath + SnPEMapFileName  + "is not found!" << std::endl;
        for(int i=0;i<ThetaPMTNum;i++) {
            SMu2D[i] = (TH2F*)SnPEMapFile->Get(Form("hSMu2D_%d", i));
            SMu2D[i]->SetDirectory(nullptr);
        }
        SnPEMapFile->Close();
        delete SnPEMapFile;
        LogDebug<< "SMu2D[ThetaPMTNum-1]->Interpolate(rrange[1], Pi/2) " 
                << SMu2D[ThetaPMTNum-1]->Interpolate(179., TMath::Pi()/2.) << std::endl;

        double SPMTGe68ES = 0.;
        for(int i = 0; i< TotalSPMT; i++){
            double pmt_pos_x = ALL_SPMT_pos.at(i).X();
            double pmt_pos_y = ALL_SPMT_pos.at(i).Y();
            double pmt_pos_z = ALL_SPMT_pos.at(i).Z();

            double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
            double cos_theta = pmt_pos_z/PMTR;
            double theta = TMath::ACos(cos_theta)*180/TMath::Pi();

            int GrMapIdA = theta/MapThetaStep;
            int GrMapIdB = GrMapIdA + 1;

            double m_average_PEA = SMu2D[GrMapIdA]->Interpolate(0., 0.);
            double m_average_PEB = SMu2D[GrMapIdB]->Interpolate(0., 0.);
            double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
            double Weight = ThetaAFrac/MapThetaStep;
            double m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;

            SPMTGe68ES +=  m_average_PE;
        }
        LogInfo<< "SPMTGe68ES: " << SPMTGe68ES  << std::endl;
 
    }

    if(!enableUseTrueVertex && !enableUseFixedVertex && !enableUseTLHVertex && enableTimeInfo) {

        std::string TimePdfFileName = "/TimePdfFile.root";
        if(UseTruthFile) {
            TimePdfFileName = "/TimePdfFile_Truth.root";
        }

        TFile* TimePdfFile = TFile::Open((TimePdfPath + TimePdfFileName).c_str(), "READ");
        for(int rk=0;rk<TrNum;rk++) {
            std::cout<< "rk:"<< rk << '\t';
            //----------------------------- Ge68
            hDynGeProb[rk][0] = (TH2F*)TimePdfFile->Get(Form("hGeDynProb_%d", rk));
            hMcpGeProb[rk][0] = (TH2F*)TimePdfFile->Get(Form("hGeMcpProb_%d", rk));

            hDynGeIProb[rk] = (TH2F*)TimePdfFile->Get(Form("hGeDynIProb_%d", rk));
            hMcpGeIProb[rk] = (TH2F*)TimePdfFile->Get(Form("hGeMcpIProb_%d", rk));
            
            //2pe
              GeDyn_I_ft_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("GeDyn_I_ft_Ft_%d", rk));
             GeDyn_I_ft_Fdt[rk] = (TH1F*)TimePdfFile->Get(Form("GeDyn_I_ft_Fdt_%d", rk));
             GeDyn_I_fdt_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("GeDyn_I_fdt_Ft_%d", rk));

              GeMcp_I_ft_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("GeMcp_I_ft_Ft_%d", rk));
             GeMcp_I_ft_Fdt[rk] = (TH1F*)TimePdfFile->Get(Form("GeMcp_I_ft_Fdt_%d", rk));
             GeMcp_I_fdt_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("GeMcp_I_fdt_Ft_%d", rk));

            //----------------------------- Electron
            hDynLsProb[rk][0] = (TH2F*)TimePdfFile->Get(Form("hLDynProb_%d", rk));
            hMcpLsProb[rk][0] = (TH2F*)TimePdfFile->Get(Form("hLMcpProb_%d", rk));

            hDynLsIProb[rk] = (TH2F*)TimePdfFile->Get(Form("hLDynIProb_%d", rk));
            hMcpLsIProb[rk] = (TH2F*)TimePdfFile->Get(Form("hLMcpIProb_%d", rk));

            //2pe
              LsDyn_I_ft_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("LDyn_I_ft_Ft_%d", rk));
             LsDyn_I_ft_Fdt[rk] = (TH1F*)TimePdfFile->Get(Form("LDyn_I_ft_Fdt_%d", rk));
             LsDyn_I_fdt_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("LDyn_I_fdt_Ft_%d", rk));

              LsMcp_I_ft_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("LMcp_I_ft_Ft_%d", rk));
             LsMcp_I_ft_Fdt[rk] = (TH1F*)TimePdfFile->Get(Form("LMcp_I_ft_Fdt_%d", rk));
             LsMcp_I_fdt_Ft[rk] = (TH1F*)TimePdfFile->Get(Form("LMcp_I_fdt_Ft_%d", rk));

            //------------------------------ GeE
            //2pe
            Dyn_I_Geft_LFt[rk] = (TH1F*)TimePdfFile->Get(Form("Dyn_I_Geft_LFt_%d", rk));
            Dyn_I_Lft_GeFt[rk] = (TH1F*)TimePdfFile->Get(Form("Dyn_I_Lft_GeFt_%d", rk));

            Mcp_I_Geft_LFt[rk] = (TH1F*)TimePdfFile->Get(Form("Mcp_I_Geft_LFt_%d", rk));
            Mcp_I_Lft_GeFt[rk] = (TH1F*)TimePdfFile->Get(Form("Mcp_I_Lft_GeFt_%d", rk));
            
            for(int npe = 2;npe<TnPENum;npe++) {
                hDynGeProb[rk][npe] = new TH2F(Form("hDynGeProb_%d_%d", rk, npe), Form("hDynGeProb_%d_%d", rk, npe), dBinN, 0, dRange, tBinN, 0, tRange);
                hMcpGeProb[rk][npe] = new TH2F(Form("hMcpGeProb_%d_%d", rk, npe), Form("hMcpGeProb_%d_%d", rk, npe), dBinN, 0, dRange, tBinN, 0, tRange);

                hDynLsProb[rk][npe] = new TH2F(Form("hDynLsProb_%d_%d", rk, npe), Form("hDynLsProb_%d_%d", rk, npe), dBinN, 0, dRange, tBinN, 0, tRange);
                hMcpLsProb[rk][npe] = new TH2F(Form("hMcpLsProb_%d_%d", rk, npe), Form("hMcpLsProb_%d_%d", rk, npe), dBinN, 0, dRange, tBinN, 0, tRange);

                for(int checkBin = 1; checkBin <= dBinN; checkBin++) {
                    double SumDynGe = 0.;
                    double SumMcpGe = 0.;
                    double SumDynLs = 0.;
                    double SumMcpLs = 0.;
                    for(int i=1;i<=tBinN;i++) {
                        //------------------- Ge68
                        double ProbDynGe = hDynGeProb[rk][0]->GetBinContent(checkBin, i);
                        double IProbDynGe = hDynGeIProb[rk]->GetBinContent(checkBin, i);
                        double npeProbDynGe = ProbDynGe*TMath::Power(IProbDynGe, double(npe));
                        hDynGeProb[rk][npe]->SetBinContent(checkBin, i, npeProbDynGe);
                        SumDynGe += npeProbDynGe;

                        double ProbMcpGe = hMcpGeProb[rk][0]->GetBinContent(checkBin, i);
                        double IProbMcpGe = hMcpGeIProb[rk]->GetBinContent(checkBin, i);
                        double npeProbMcpGe = ProbMcpGe*TMath::Power(IProbMcpGe, double(npe));
                        hMcpGeProb[rk][npe]->SetBinContent(checkBin, i, npeProbMcpGe);
                        SumMcpGe += npeProbMcpGe;

                        //------------------- Electron
                        double ProbDynLs = hDynLsProb[rk][0]->GetBinContent(checkBin, i);
                        double IProbDynLs = hDynLsIProb[rk]->GetBinContent(checkBin, i);
                        double npeProbDynLs = ProbDynLs*TMath::Power(IProbDynLs, double(npe));
                        hDynLsProb[rk][npe]->SetBinContent(checkBin, i, npeProbDynLs);
                        SumDynLs += npeProbDynLs;

                        double ProbMcpLs = hMcpLsProb[rk][0]->GetBinContent(checkBin, i);
                        double IProbMcpLs = hMcpLsIProb[rk]->GetBinContent(checkBin, i);
                        double npeProbMcpLs = ProbMcpLs*TMath::Power(IProbMcpLs, double(npe));
                        hMcpLsProb[rk][npe]->SetBinContent(checkBin, i, npeProbMcpLs);
                        SumMcpLs += npeProbMcpLs;

                    }
                    if(SumDynGe<1e-8 || SumDynLs<1e-8) continue;
                    for(int i = 1; i<=tBinN; i++) {
                        hDynGeProb[rk][npe]->SetBinContent(checkBin, i, hDynGeProb[rk][npe]->GetBinContent(checkBin, i)/SumDynGe);
                        hMcpGeProb[rk][npe]->SetBinContent(checkBin, i, hMcpGeProb[rk][npe]->GetBinContent(checkBin, i)/SumMcpGe);

                        hDynLsProb[rk][npe]->SetBinContent(checkBin, i, hDynLsProb[rk][npe]->GetBinContent(checkBin, i)/SumDynLs);
                        hMcpLsProb[rk][npe]->SetBinContent(checkBin, i, hMcpLsProb[rk][npe]->GetBinContent(checkBin, i)/SumMcpLs);
                    } 
                }
            }
        
        }
        std::cout << std::endl;

    }
    

    
    //////////////////
    // nPE charge pdfs
    if(RecMode=="Real") {
        std::string QPdfFileName = "/NPEQpdf.root";
        std::string AvgQPdfFileName = "/AvgNPEQpdf.root";
        TFile* AvgQPdfFile = TFile::Open((PMTChargeSpecPath + AvgQPdfFileName).c_str(), "READ");
        //TFile* QPdfFile = TFile::Open((PMTChargeSpecPath + QPdfFileName).c_str(), "READ");
        //for(int i=0;i<TotalLPMT;i++) {
        //    if(!m_pmtpara_tool->GetIsDyn(i)) {
        //        for(int n=0;n<QPdfMaxPE;n++) {
        //            QPdfs[i][n] = (TGraph*)QPdfFile->Get(Form("channel%d_nPE%d_qpdf", i, n+1));
        //        }
        //    }
        //    else {
        //        for(int n=0;n<QPdfMaxPE;n++) QPdfs[i][n] = NULL;
        //    }
        //}
        //LogDebug<< "channel17611 QPdfMaxPE Qpdf check: " << QPdfs[17611][QPdfMaxPE-1]->Eval(QPdfMaxPE)<<std::endl;

        for(int i=0;i<AvgQPdfMaxPE;i++) {
            AvgQPdfs[i] = (TH1F*)AvgQPdfFile->Get(Form("nPE%d_mcp_qpdf", i+1));
        }
        LogDebug<< "QPdfMaxPE AvgQpdf check: " << AvgQPdfs[AvgQPdfMaxPE-1]->Interpolate(QPdfMaxPE)<<std::endl;
    }

    LogInfo<< "Done to load map/pdfs!" << std::endl;

    return true;
}



double OMILREC::Calculate_EVLikelihood(double E,
                                       double m_R,
                                       double m_Theta,
                                       double m_Phi,
                                       double t0)
{
    double m_x = m_R*TMath::Sin(m_Theta)*TMath::Cos(m_Phi);
    double m_y = m_R*TMath::Sin(m_Theta)*TMath::Sin(m_Phi);
    double m_z = m_R*TMath::Cos(m_Theta);

    double m_Likelihood = 0;

    if(m_R>rrange[1]-1.e-8) m_R = rrange[1]-1.;
    if(isnan(m_R)) m_R = 10000.;
    double RVar = TMath::Power(m_R, 3.)*180./TMath::Power(rrange[1], 3.);

    //phi-c map variables
    int PhiZone = 0;
    double PVar = m_Phi*180./TMath::Pi();
    if(PVar<6.) PVar = PVar;
    else if(PVar<354.) {
        PhiZone = int((PVar - 6.)/12.) + 1;
        double PhiCenter = double(PhiZone*12.);
        if(PVar >= PhiCenter) {
            PVar = PVar - PhiCenter;
        } else {
            PVar = PhiCenter - PVar;
        }
    } else {
        PVar = 360. - PVar;
    }

    //Ge68+Laser nPE map coefficients
    double L2E   = ElectronES/LaserES;
    if(enableUseEkMap) {
        if(E<=GeEvis) {
            rawEk = 0.;
            mapNorm = 1./GeEvis;
        } else {
            rawEk = E - GeEvis;
            mapNorm = 1./E;
        }
    }
    
    //Ge68+Electron time pdf coefficients
    double R3 = m_R*m_R*m_R/1.e9;

    double tw1 = GeEvis/E;
    if(tw1>1.-1.e-8) tw1 = 1.;
    //tw1 = 1.;
    double tw2 = 1. - tw1;

    //R-binning of time pdf
    #ifdef Ideal_Rcut
    int TIdA = int(R3/50.);
    if(TIdA>105) TIdA = 105;
    #endif

    #ifdef CLS_Rcut 
    int TIdA = 0;
    if(enableTimeInfo) {
    if(R3<1000.) TIdA = 0;
    else if(R3<2000.) TIdA = 1;
    else if(R3<2500.) TIdA = 2;
    else if(R3<3000.) TIdA = 3;
    else if(R3<3500.) TIdA = 4;
    else if(R3<3700.) TIdA = 5;
    else if(R3<3900.) TIdA = 6;
    else if(R3<3950.) TIdA = 7;
    else if(R3<4000.) TIdA = 8;
    else if(R3<4050.) TIdA = 9;
    else if(R3<4100.) TIdA = 10;
    else if(R3<4150.) TIdA = 11;
    else if(R3<4200.) TIdA = 12;
    else if(R3<4250.) TIdA = 13;
    else if(R3<4300.) TIdA = 14;
    else if(R3<4350.) TIdA = 15;
    else if(R3<4400.) TIdA = 16;
    else if(R3<4450.) TIdA = 17;
    else if(R3<4500.) TIdA = 18;
    else if(R3<4550.) TIdA = 19;
    else if(R3<4600.) TIdA = 20;
    else if(R3<4650.) TIdA = 21;
    else if(R3<4700.) TIdA = 22;
    else if(R3<4750.) TIdA = 23;
    else if(R3<4800.) TIdA = 24;
    else if(R3<4850.) TIdA = 25;
    else if(R3<4900.) TIdA = 26;
    else if(R3<4950.) TIdA = 26; //for toy/real case
    //else if(R3<4950.) TIdA = 27; //for ideal case
    else if(R3<5000.) TIdA = 28;
    else if(R3<5050.) TIdA = 29;
    else if(R3<5100.) TIdA = 30;
    else if(R3<5150.) TIdA = 31;
    else if(R3<5200.) TIdA = 32;
    else if(R3<5250.) TIdA = 33;
    else if(R3<5300.) TIdA = 34;
    else TIdA = 34;
    }
    #endif

    double m_average_PE = 0, m_average_PEA = 0, m_average_PEB = 0;
    double m_average_PEA1 = 0, m_average_PEB1 = 0;
    double m_average_PEA2 = 0, m_average_PEB2 = 0;

    m_NPE = 0.;
    m_UfrmScale = 0.;

    bool QInfoIsValid = false;
    if(enableQInfo) QInfoIsValid = true;

    TF1* GausFunc = new TF1("mGaus", "gaus");
    if(!enableSPMTOnly) {
        //large PMT part
        for(int i = 0; i< TotalLPMT; i++){
            bool isDyn = m_pmtpara_tool->GetIsDyn(i);
            if(isDyn && enableMcpInfoOnly) continue;
            if(!isDyn && enableDynInfoOnly) continue;

            double pmt_pos_x = ALL_LPMT_pos.at(i).X();
            double pmt_pos_y = ALL_LPMT_pos.at(i).Y();
            double pmt_pos_z = ALL_LPMT_pos.at(i).Z();

            double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
            double cos_theta = (pmt_pos_x*m_x + pmt_pos_y*m_y + pmt_pos_z*m_z)/m_R/PMTR;
            double theta = TMath::ACos(cos_theta)*180/TMath::Pi();
            if(isnan(theta)) theta = 90.0;

            double dstn = TMath::Sqrt((pmt_pos_x - m_x)*(pmt_pos_x - m_x) 
                                    + (pmt_pos_y - m_y)*(pmt_pos_y - m_y) 
                                    + (pmt_pos_z - m_z)*(pmt_pos_z - m_z))/1.e3;

            double VVar0 = dstn;

            if(Use3DMap) {
                int GrMapIdA = theta/MapThetaStep;
                int GrMapIdB = GrMapIdA + 1;

                // (2+1)D interpolation
                if(GrMapIdB<ThetaPMTNum) {
                    if(enableUseEkMap) {
                        if(m_R > 15500. && GrMapIdB<CutThetaPMTI && enableUsePhiMap) {
                            if(PVar<2.) {
                                m_average_PEA1 = (GeEvis*LMu2D[GrMapIdA]->Interpolate(RVar, m_Theta)    + rawEk*LMu2D_Ek[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB1 = (GeEvis*LMu2D[GrMapIdB]->Interpolate(RVar, m_Theta)    + rawEk*LMu2D_Ek[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEA2 = (GeEvis*LMu2D_2d[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_2d[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB2 = (GeEvis*LMu2D_2d[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_2d[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                double PhiFrac =  PVar - 0.;
                                double PhiWeight = PhiFrac/2.;
                                m_average_PEA = (1.-PhiWeight)*m_average_PEA1 + PhiWeight*m_average_PEA2;
                                m_average_PEB = (1.-PhiWeight)*m_average_PEB1 + PhiWeight*m_average_PEB2;
                            }
                            else if(PVar<4.) {
                                m_average_PEA1 = (GeEvis*LMu2D_2d[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_2d[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB1 = (GeEvis*LMu2D_2d[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_2d[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEA2 = (GeEvis*LMu2D_4d[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_4d[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB2 = (GeEvis*LMu2D_4d[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_4d[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                double PhiFrac =  PVar - 2.;
                                double PhiWeight = PhiFrac/2.;
                                m_average_PEA = (1.-PhiWeight)*m_average_PEA1 + PhiWeight*m_average_PEA2;
                                m_average_PEB = (1.-PhiWeight)*m_average_PEB1 + PhiWeight*m_average_PEB2;
                            } else {
                                m_average_PEA1 = (GeEvis*LMu2D_4d[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_4d[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB1 = (GeEvis*LMu2D_4d[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_4d[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEA2 = (GeEvis*LMu2D_6d[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_6d[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                m_average_PEB2 = (GeEvis*LMu2D_6d[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek_6d[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                                double PhiFrac =  PVar - 4.;
                                double PhiWeight = PhiFrac/2.;
                                m_average_PEA = (1.-PhiWeight)*m_average_PEA1 + PhiWeight*m_average_PEA2;
                                m_average_PEB = (1.-PhiWeight)*m_average_PEB1 + PhiWeight*m_average_PEB2;
                            }
                            if(m_R<15700.) {
                                double connectW = (m_R - 15500.)/200.;
                                m_average_PEA = m_average_PEA*connectW + (1.-connectW)*((GeEvis*LMu2D[GrMapIdA]->Interpolate(RVar, m_Theta) +
                                                                                          rawEk*LMu2D_Ek[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm);
                                m_average_PEB = m_average_PEB*connectW + (1.-connectW)*((GeEvis*LMu2D[GrMapIdB]->Interpolate(RVar, m_Theta) +
                                                                                          rawEk*LMu2D_Ek[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm);
                            }
                        } else {
                            m_average_PEA = (GeEvis*LMu2D[GrMapIdA]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek[GrMapIdA]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                            m_average_PEB = (GeEvis*LMu2D[GrMapIdB]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek[GrMapIdB]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                        } 

                    } else {
                        m_average_PEA = LMu2D[GrMapIdA]->Interpolate(RVar, m_Theta);
                        m_average_PEB = LMu2D[GrMapIdB]->Interpolate(RVar, m_Theta);
                    }
                    double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
                    double Weight = ThetaAFrac/MapThetaStep;
                    m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;
                } else {
                    if(enableUseEkMap) {
                        m_average_PE = (GeEvis*LMu2D[ThetaPMTNum-1]->Interpolate(RVar, m_Theta) + rawEk*LMu2D_Ek[ThetaPMTNum-1]->Interpolate(RVar, m_Theta)*L2E)*mapNorm;
                    } else {
                        m_average_PE = LMu2D[ThetaPMTNum-1]->Interpolate(RVar, m_Theta);
                    }
                }
            } else {
                
                int GrMapIdA = theta/MapThetaStep;
                int GrMapIdB = GrMapIdA + 1;

                // 1-D interpolation
                // Todo reduce the number of the bins of r
                if(GrMapIdB<ThetaPMTNum) {
                    m_average_PEA = LMu1D[GrMapIdA]->Interpolate(RVar);
                    m_average_PEB = LMu1D[GrMapIdB]->Interpolate(RVar);

                    double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
                    double Weight = ThetaAFrac/MapThetaStep;
                    m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;
                }
                else m_average_PE = LMu1D[ThetaPMTNum-1]->Interpolate(RVar);
                
                
            }
            double m_expected_PE = E*m_pmtpara_tool->GetLPDE(i)*m_average_PE/LPMTCalibEnergy/nHESF;

            double dmu = m_pmtpara_tool->GetLDNR(i)*SignalWindowL/1.e9;
            double fdmu = m_pmtpara_tool->GetLDNR(i)*ADCL/1.e9;
            if(RecMode=="Toy" && !enableAddToyDN) {dmu = 0.; fdmu = 0.;}

            double m_expected_LSPE = m_expected_PE;          
            double m_expected_FPE = m_expected_PE + fdmu;          
            m_expected_PE += dmu;

            double PMT_Hit = m_pmtinput_tool->Get_LHIT_Charge(i);
            int RecNpe =  int(m_pmtinput_tool->Get_LHIT_NPE(i));

            int krange = 1E3;
            double probPrcs = 1.e-8;
            double Pro_Qi = 1e-16;
            double Pro_Ti = 1e-16;
            double proTemp = 0., proTemp_last = 0.;

            bool timeInfoIsValid = false;
            if(enableTimeInfo&& !enableUseTrueVertex&& !enableUseFixedVertex&& !enableUseTLHVertex && RecNpe <= UsedTnPENum && RecNpe>=1) timeInfoIsValid=true;

            if(!isDyn && enableDynTimeInfoOnly) timeInfoIsValid=false;


            double LTOF_temp = 0.;
            if(timeInfoIsValid) LTOF_temp = CalLTOF(i, m_x, m_y, m_z);
           
            double Q1 = m_pmtpara_tool->GetLGain(i);
            double S1 = m_pmtpara_tool->GetLQRes(i);
            if(!enableOccupancy) {

                if(QInfoIsValid) {
                    if(RecMode=="Real" || enableAddToyQ) {
                        if( isDyn || !enableLTSPEs) {
                            if(PMT_Hit<PedThres) {
                                double Poisson0 = exp(-m_expected_PE);
                                double paraTemp[3] = {1.0/sqrt(2*TMath::Pi())/S1, Q1, S1};
                                GausFunc->SetParameters(paraTemp);
                                double probtemp = GausFunc->Integral(-2., PedThres);
                                Pro_Qi = Poisson0 + probtemp*Poisson0*m_expected_PE;
                            } 

                            else {
                                double PoissonProb = exp(-m_expected_PE);
                                for(int k = 1;k<krange;k++) {
                                    double kk = double(k);
                                    proTemp = TMath::Gaus(PMT_Hit, kk*Q1, sqrt(kk)*S1)/sqrt(2.*TMath::Pi()*kk*S1*S1);

                                    if(proTemp<0.0) {proTemp = probPrcs/10.;}
                                    if(proTemp<probPrcs && proTemp_last>probPrcs) break;
                                    proTemp_last = proTemp;
                                    
                                    PoissonProb = PoissonProb*m_expected_PE/kk;
                                    Pro_Qi += PoissonProb*proTemp;
                                }
                            }
                        }
                        else if(!isDyn && enableLTSPEs) {
                            if(PMT_Hit<PedThres) {
                                double Poisson0 = exp(-m_expected_PE);
                                double paraTemp[3] = {1.0/sqrt(2*TMath::Pi())/S1, Q1, S1};
                                GausFunc->SetParameters(paraTemp);
                                double probtemp = GausFunc->Integral(-2., PedThres);
                                Pro_Qi = Poisson0 + probtemp*Poisson0*m_expected_PE;
                            }
                            else {
                                double PoissonProb = exp(-m_expected_PE); 
                                for(int k = 1;k<krange;k++) {
                                    double kk = double(k);
                                    //if(k<=QPdfMaxPE) proTemp = QPdfs[i][k-1]->Eval(PMT_Hit);
                                    if(k<=AvgQPdfMaxPE) proTemp = AvgQPdfs[k-1]->Interpolate(PMT_Hit);
                                    else proTemp = TMath::Gaus(PMT_Hit, kk*Q1, sqrt(kk)*S1)/sqrt(2.*TMath::Pi()*kk*S1*S1);

                                    if(proTemp<0.0) {proTemp = probPrcs/10.;}
                                    if(proTemp<probPrcs && proTemp_last>probPrcs) break;
                                    proTemp_last = proTemp;

                                    PoissonProb = PoissonProb*m_expected_PE/kk;
                                    Pro_Qi += PoissonProb*proTemp;
                                }
                            }
                        }

                    }
                    else Pro_Qi = TMath::Poisson(PMT_Hit, m_expected_PE);
                }
                
                
                double tres = m_pmtinput_tool->Get_LHIT_Time(i) - LTOF_temp - t0 - m_pmtpara_tool->GetTimeOffset(i);
                if(tres < tRange && tres > 0. && timeInfoIsValid) {
                    double Poisson1 = exp(-m_expected_LSPE)*m_expected_LSPE;
                    double Poisson2 = Poisson1*m_expected_LSPE/2.;

                    double Poisson0_d = exp(-fdmu);
                    double Poisson1_d = Poisson0_d*fdmu;
                    double Poisson2_d = Poisson1_d*fdmu/2.;

                    if(enableQTimePdf) {
                        double pronpe[TnPENum] = {};

                        if(isDyn) {
                            if(UseGeETimePdf) {
                                double ProbDynGe = hDynGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbDynGe = hDynGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbDynLs = hDynLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbDynLs = hDynLsIProb[TIdA]->Interpolate(VVar0, tres);

                                double ProbDyn = tw1*ProbDynGe + tw2*ProbDynLs;
                                double IProbDyn = tw1*IProbDynGe + tw2*IProbDynLs;

                                double Ia = 2.*tw1*tw1*GeDyn_I_ft_Ft[TIdA]->Interpolate(VVar0) + tw1*tw2*Dyn_I_Geft_LFt[TIdA]->Interpolate(VVar0)
                                          + tw2*tw1*Dyn_I_Lft_GeFt[TIdA]->Interpolate(VVar0) + 2.*tw2*tw2*LsDyn_I_ft_Ft[TIdA]->Interpolate(VVar0);
                                double Ib = tw1*GeDyn_I_ft_Fdt[TIdA]->Interpolate(VVar0) + tw2*LsDyn_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = tw1*GeDyn_I_fdt_Ft[TIdA]->Interpolate(VVar0) + tw2*LsDyn_I_fdt_Ft[TIdA]->Interpolate(VVar0);

                                pronpe[0] = (m_expected_LSPE*ProbDyn + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);;
                                pronpe[1] = (Poisson2*Poisson0_d*(ProbDyn*IProbDyn + tw1*tw1*ProbDynGe*IProbDynGe + tw2*tw2*ProbDynLs*IProbDynLs) +
                                          Poisson1*Poisson1_d*ProbDyn*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbDyn)  /

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);

                                pronpe[2] = tw1*hDynGeProb[TIdA][2]->Interpolate(VVar0, tres) + tw2*hDynLsProb[TIdA][2]->Interpolate(VVar0, tres);
                                for(int kk=3;kk<TnPENum;kk++) {
                                     pronpe[kk] = tw1*hDynGeProb[TIdA][kk]->Interpolate(VVar0, tres) + tw2*hDynLsProb[TIdA][kk]->Interpolate(VVar0, tres);
                                }
                            } else {
                                double ProbDynGe = hDynGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbDynGe = hDynGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbDyn = ProbDynGe;
                                double IProbDyn = IProbDynGe;

                                double Ia = 2.*GeDyn_I_ft_Ft[TIdA]->Interpolate(VVar0); 
                                double Ib = GeDyn_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = GeDyn_I_fdt_Ft[TIdA]->Interpolate(VVar0);

                                pronpe[0] = (m_expected_LSPE*ProbDyn + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);;
                                pronpe[1] = (Poisson2*Poisson0_d*(ProbDyn*IProbDyn + ProbDynGe*IProbDynGe) +
                                          Poisson1*Poisson1_d*ProbDyn*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbDyn)  /

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);

                                pronpe[2] = hDynGeProb[TIdA][2]->Interpolate(VVar0, tres);
                                for(int kk=3;kk<TnPENum;kk++) {
                                    pronpe[kk] = hDynGeProb[TIdA][kk]->Interpolate(VVar0, tres);
                                }
                            }

                        } else {
                            if(UseGeETimePdf) {
                                double ProbMcpGe = hMcpGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbMcpGe = hMcpGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbMcpLs = hMcpLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbMcpLs = hMcpLsIProb[TIdA]->Interpolate(VVar0, tres);

                                double ProbMcp = tw1*ProbMcpGe + tw2*ProbMcpLs;
                                double IProbMcp = tw1*IProbMcpGe + tw2*IProbMcpLs;

                                double Ia = 2.*tw1*tw1*GeMcp_I_ft_Ft[TIdA]->Interpolate(VVar0) + tw1*tw2*Mcp_I_Geft_LFt[TIdA]->Interpolate(VVar0)
                                          + tw2*tw1*Mcp_I_Lft_GeFt[TIdA]->Interpolate(VVar0) + 2.*tw2*tw2*LsMcp_I_ft_Ft[TIdA]->Interpolate(VVar0);
                                double Ib = tw1*GeMcp_I_ft_Fdt[TIdA]->Interpolate(VVar0) + tw2*LsMcp_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = tw1*GeMcp_I_fdt_Ft[TIdA]->Interpolate(VVar0) + tw2*LsMcp_I_fdt_Ft[TIdA]->Interpolate(VVar0);


                                pronpe[0] = (m_expected_LSPE*ProbMcp + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);;
                                pronpe[1] = (Poisson2*Poisson0_d*(ProbMcp*IProbMcp + tw1*tw1*ProbMcpGe*IProbMcpGe + tw2*tw2*ProbMcpLs*IProbMcpLs) +
                                          Poisson1*Poisson1_d*ProbMcp*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbMcp)/

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);
                                pronpe[2] = tw1*hMcpGeProb[TIdA][2]->Interpolate(VVar0, tres) + tw2*hMcpLsProb[TIdA][2]->Interpolate(VVar0, tres);
                                for(int kk=3;kk<TnPENum;kk++) {
                                    pronpe[kk] = tw1*hMcpGeProb[TIdA][kk]->Interpolate(VVar0, tres) + tw2*hMcpLsProb[TIdA][kk]->Interpolate(VVar0, tres);
                                }
                            } else {
                                double ProbMcpGe = hMcpGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbMcpGe = hMcpGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbMcp = ProbMcpGe;
                                double IProbMcp = IProbMcpGe;

                                double Ia = 2.*GeMcp_I_ft_Ft[TIdA]->Interpolate(VVar0);
                                double Ib = GeMcp_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = GeMcp_I_fdt_Ft[TIdA]->Interpolate(VVar0);


                                pronpe[0] = (m_expected_LSPE*ProbMcp + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);;
                                pronpe[1] = (Poisson2*Poisson0_d*(ProbMcp*IProbMcp + ProbMcpGe*IProbMcpGe) +
                                          Poisson1*Poisson1_d*ProbMcp*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbMcp)/

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);

                                pronpe[2] = hMcpGeProb[TIdA][2]->Interpolate(VVar0, tres);
                                for(int kk=3;kk<TnPENum;kk++) {
                                    pronpe[kk] = hMcpGeProb[TIdA][kk]->Interpolate(VVar0, tres);
                                }
                            } 
                        }
                        double Poisson_F[9] = {};
                        Poisson_F[1] = exp(-m_expected_FPE)*m_expected_FPE;
                        double IPn = Poisson_F[1];
                        Pro_Ti = Poisson_F[1]*pronpe[0];
                        for(int npe=2;npe<=TnPENum;npe++) {
                            Poisson_F[npe] = Poisson_F[npe-1]*m_expected_FPE/double(npe);
                            IPn += Poisson_F[npe];
                            Pro_Ti += Poisson_F[npe]*pronpe[npe-1];
                        }
                        Pro_Ti /= IPn;
                        
                    } else {
                        if(isDyn) {
                            if(RecNpe == 1) {
                                double ProbDynGe = hDynGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double ProbDynLs = hDynLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double ProbDyn = tw1*ProbDynGe + tw2*ProbDynLs;

                                Pro_Ti = (m_expected_LSPE*ProbDyn + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);
                            } else if (RecNpe == 2) {
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbDynGe = hDynGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbDynGe = hDynGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double ProbDynLs = hDynLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbDynLs = hDynLsIProb[TIdA]->Interpolate(VVar0, tres);

                                double ProbDyn = tw1*ProbDynGe + tw2*ProbDynLs;
                                double IProbDyn = tw1*IProbDynGe + tw2*IProbDynLs;

                                double Ia = 2.*tw1*tw1*GeDyn_I_ft_Ft[TIdA]->Interpolate(VVar0) + tw1*tw2*Dyn_I_Geft_LFt[TIdA]->Interpolate(VVar0)
                                          + tw2*tw1*Dyn_I_Lft_GeFt[TIdA]->Interpolate(VVar0) + 2.*tw2*tw2*LsDyn_I_ft_Ft[TIdA]->Interpolate(VVar0);
                                double Ib = tw1*GeDyn_I_ft_Fdt[TIdA]->Interpolate(VVar0) + tw2*LsDyn_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = tw1*GeDyn_I_fdt_Ft[TIdA]->Interpolate(VVar0) + tw2*LsDyn_I_fdt_Ft[TIdA]->Interpolate(VVar0);

                                Pro_Ti = (Poisson2*Poisson0_d*(ProbDyn*IProbDyn + tw1*tw1*ProbDynGe*IProbDynGe + tw2*tw2*ProbDynLs*IProbDynLs) +
                                          Poisson1*Poisson1_d*ProbDyn*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbDyn)  /

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);
                            } else if (RecNpe >= 3) {
                                Pro_Ti = tw1*hDynGeProb[TIdA][RecNpe-1]->Interpolate(VVar0, tres) + tw2*hDynLsProb[TIdA][RecNpe-1]->Interpolate(VVar0, tres);
                            }
                        } else {
                            if(RecNpe == 1) {
                                double ProbMcpGe = hMcpGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double ProbMcpLs = hMcpLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double ProbMcp = tw1*ProbMcpGe + tw2*ProbMcpLs;

                                Pro_Ti = (m_expected_LSPE*ProbMcp + fdmu*DNProb)/(m_expected_LSPE + fdmu*double(tBinN)*DNProb);
                            } else if (RecNpe == 2) {
                                double IDNProb = 1. - tres*DNProb/tbinW;

                                double ProbMcpGe = hMcpGeProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbMcpGe = hMcpGeIProb[TIdA]->Interpolate(VVar0, tres);
                                double ProbMcpLs = hMcpLsProb[TIdA][0]->Interpolate(VVar0, tres);
                                double IProbMcpLs = hMcpLsIProb[TIdA]->Interpolate(VVar0, tres);

                                double ProbMcp = tw1*ProbMcpGe + tw2*ProbMcpLs;
                                double IProbMcp = tw1*IProbMcpGe + tw2*IProbMcpLs;

                                double Ia = 2.*tw1*tw1*GeMcp_I_ft_Ft[TIdA]->Interpolate(VVar0) + tw1*tw2*Mcp_I_Geft_LFt[TIdA]->Interpolate(VVar0)
                                          + tw2*tw1*Mcp_I_Lft_GeFt[TIdA]->Interpolate(VVar0) + 2.*tw2*tw2*LsMcp_I_ft_Ft[TIdA]->Interpolate(VVar0);
                                double Ib = tw1*GeMcp_I_ft_Fdt[TIdA]->Interpolate(VVar0) + tw2*LsMcp_I_ft_Fdt[TIdA]->Interpolate(VVar0);
                                double Ic = tw1*GeMcp_I_fdt_Ft[TIdA]->Interpolate(VVar0) + tw2*LsMcp_I_fdt_Ft[TIdA]->Interpolate(VVar0);

                                Pro_Ti = (Poisson2*Poisson0_d*(ProbMcp*IProbMcp + tw1*tw1*ProbMcpGe*IProbMcpGe + tw2*tw2*ProbMcpLs*IProbMcpLs) +
                                          Poisson1*Poisson1_d*ProbMcp*IDNProb   +
                                          Poisson1*Poisson1_d*DNProb*IProbMcp)  /

                                         (Poisson2*Poisson0_d*Ia +
                                          Poisson1*Poisson1_d*Ib +
                                          Poisson1*Poisson1_d*Ic);
                            } else if (RecNpe >= 3) {
                                Pro_Ti = tw1*hMcpGeProb[TIdA][RecNpe-1]->Interpolate(VVar0, tres) + tw2*hMcpLsProb[TIdA][RecNpe-1]->Interpolate(VVar0, tres);
                            }
                        }
                    }
                }

            } else {
                if(RecMode=="Real") {
                    double paraTemp[3] = {1.0/sqrt(2*TMath::Pi())/S1, Q1, S1};
                    GausFunc->SetParameters(paraTemp);
                    double probtemp = GausFunc->Integral(-2., PedThres);
                    Pro_Qi = exp(-m_expected_PE) + probtemp*TMath::Poisson(1, m_expected_PE);
                } 
                else Pro_Qi = exp(-m_expected_PE);
                if(PMT_Hit>1e-8) Pro_Qi = 1. - Pro_Qi;
            }
            if(QInfoIsValid) {
                if(Pro_Qi<1e-16) Pro_Qi = 1e-16;
                m_Likelihood = m_Likelihood - 2.*log(Pro_Qi);
            }
            if(timeInfoIsValid) {
                if(Pro_Ti<1e-6) Pro_Ti = 1e-6;
                if(Pro_Ti>1e-7) m_Likelihood = m_Likelihood - 2.*log(Pro_Ti);
            }
            
            m_NPE += (PMT_Hit/Q1 - dmu);
            m_UfrmScale += (m_average_PE*m_pmtpara_tool->GetLPDE(i));
        }

    }

    
    if(enableSPMTInfo) { 
        //small PMT part
        if(enableSPMTOnly) {
            m_NPE = 0.;
            m_UfrmScale = 0.;
            m_Likelihood = 0.;
        }   
        for(int i = 0; i< TotalSPMT; i++){
            double pmt_pos_x = ALL_SPMT_pos.at(i).X();
            double pmt_pos_y = ALL_SPMT_pos.at(i).Y();
            double pmt_pos_z = ALL_SPMT_pos.at(i).Z();

            double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
            double cos_theta = (pmt_pos_x*m_x + pmt_pos_y*m_y + pmt_pos_z*m_z)/m_R/PMTR;
            double theta = TMath::ACos(cos_theta)*180/TMath::Pi();
            if(isnan(theta)) theta = 90.0;

            int GrMapIdA = theta/MapThetaStep;
            int GrMapIdB = GrMapIdA + 1;

            if(GrMapIdB<ThetaPMTNum) {
                m_average_PEA = SMu2D[GrMapIdA]->Interpolate(RVar, m_Theta);
                m_average_PEB = SMu2D[GrMapIdB]->Interpolate(RVar, m_Theta);

                double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
                double Weight = ThetaAFrac/MapThetaStep;
                m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;
            }
            else m_average_PE = SMu2D[ThetaPMTNum-1]->Interpolate(RVar, m_Theta);
            if(m_average_PE<1.e-8) m_average_PE = 1.e-8;

            double m_expected_PE = E*(m_average_PE)*m_pmtpara_tool->GetSPDE(i)/SPMTCalibEnergy/nHESF;


            double PMT_Hit  = m_pmtinput_tool->Get_SHIT_Charge(i);
            double dmu = m_pmtpara_tool->GetSDNR(i)*double(ADCL)/1.e9;
            if(RecMode=="Toy" && !enableAddToyDN) dmu = 0.;
            m_expected_PE += dmu;

            double Pro_Qi = 1e-16;
            if(!enableOccupancy) {
                //if(RecMode=="Real") {
                //    Pro_Qi = TMath::Poisson(PMT_Hit, m_expected_PE);
                //} else Pro_Qi = TMath::Poisson(PMT_Hit, m_expected_PE);
                Pro_Qi = exp(-m_expected_PE) + 0.01*TMath::Poisson(1., m_expected_PE);
                if(PMT_Hit>0.2) Pro_Qi = 1. - Pro_Qi;
            } else {
                Pro_Qi = exp(-m_expected_PE) + 0.01*TMath::Poisson(1., m_expected_PE);
                if(PMT_Hit>0.2) Pro_Qi = 1. - Pro_Qi;
            }
            if(Pro_Qi<1e-16) Pro_Qi = 1e-16;
            m_Likelihood = m_Likelihood - 2.*log(Pro_Qi);

            m_NPE += (PMT_Hit - dmu);
            m_UfrmScale += (m_average_PE*m_pmtpara_tool->GetSPDE(i));
        }
    }
    //std::cout<<m_Likelihood<<std::endl;
    delete GausFunc;
    return m_Likelihood;
}




void OMILREC::RemoveToyDN(double m_x, double m_y, double m_z, bool IsSave)
{
    ////////////
    // Remove DN
    int FHS_hShift = -200; //In case the rsd time is negative
    TH1D* LFHSA = (TH1D*)gROOT->FindObject(Form("FirstHitTime1_%d", m_iEvt));
    delete LFHSA;
    TH1D* LFHSB = (TH1D*)gROOT->FindObject(Form("FirstHitTime2_%d", m_iEvt));
    delete LFHSB;
    LFHSA = new TH1D(Form("FirstHitTime1_%d", m_iEvt), Form("FirstHitTime1_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);
    LFHSB = new TH1D(Form("FirstHitTime2_%d", m_iEvt), Form("FirstHitTime2_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);
    
    TH1D* SFHSA = (TH1D*)gROOT->FindObject(Form("SFirstHitTime1_%d", m_iEvt));
    delete SFHSA;
    TH1D* SFHSB = (TH1D*)gROOT->FindObject(Form("SFirstHitTime2_%d", m_iEvt));
    delete SFHSB;
    SFHSA = new TH1D(Form("SFirstHitTime1_%d", m_iEvt), Form("SFirstHitTime1_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);
    SFHSB = new TH1D(Form("SFirstHitTime2_%d", m_iEvt), Form("SFirstHitTime2_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);

    int timeshift = 300; //Also in case the rsd time is negative
    for (int i=0;i<TotalLPMT;i++) {
        int pmtid = i;
        m_pmtinput_tool->Set_LHIT_Charge(i, 0.); 
        LTOF[pmtid] = CalLTOF(pmtid, m_x, m_y, m_z);
        double hitt_temp = m_pmtinput_tool->Get_LHIT_Time(pmtid);
        if(TMath::Abs(hitt_temp)>1e-8) {
            LFHSA->Fill(hitt_temp);
            LFHSB->Fill(hitt_temp + timeshift - LTOF[pmtid]);
        }
    }

    for (int i=0;i<TotalSPMT;i++) {
        int pmtid = i;
        m_pmtinput_tool->Set_SHIT_Charge(i, 0.);
        STOF[pmtid] = CalSTOF(pmtid, m_x, m_y, m_z);
        double hitt_temp = m_pmtinput_tool->Get_SHIT_Time(pmtid);
        if(TMath::Abs(hitt_temp)>1e-8) {
            SFHSA->Fill(hitt_temp); 
            SFHSB->Fill(hitt_temp + timeshift - STOF[pmtid]);
        }

    }

    if(IsSave && enableDebug) {
        TestFile->cd();
        LFHSA->Write();
        LFHSB->Write();
        SFHSA->Write();
        SFHSB->Write();
    }
    
    int HitMaxBin = LFHSB->GetMaximumBin();
    int LWindowA = 0, LWindowB = 0;
    double SignalThres = (m_pmtpara_tool->GetLDarkMu() * double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((m_pmtpara_tool->GetLDarkMu() * double(TotalLPMT))/(ADCL/LFHSBinW));
    for(int i = HitMaxBin;i>2; i--) {
        if(LFHSB->GetBinContent(i)+LFHSB->GetBinContent(i-1)+LFHSB->GetBinContent(i-2)<SignalThres*3.0) {
            LWindowA = LFHSBinW/2. + (double(i)-1.0)*LFHSBinW - 3.*LFHSBinW + FHS_hShift; break;
        }
        else continue;
    }
    LWindowB = SignalWindowL + LWindowA;

    HitMaxBin = SFHSB->GetMaximumBin();
    int SWindowA = 0, SWindowB = 0;
    SignalThres = (m_pmtpara_tool->GetSDarkMu() * double(TotalSPMT))/(ADCL/SFHSBinW)+sqrt((m_pmtpara_tool->GetSDarkMu() * double(TotalSPMT))/(ADCL/SFHSBinW));
    for(int i = HitMaxBin;i>2; i--) {
        if(SFHSB->GetBinContent(i)+SFHSB->GetBinContent(i-1)+SFHSB->GetBinContent(i-2)<SignalThres*3.0) {
            SWindowA = SFHSBinW/2. + (double(i)-1.0)*SFHSBinW - 2.*SFHSBinW + FHS_hShift;break;
        }
        else continue;
    }
    SWindowB = SignalWindowL + SWindowA;
    if(!enableSPMTOnly) {SWindowA = LWindowA; SWindowB = LWindowB;}

    double  TrueHitTime = 0.0;
    
    double TotalSelLNPE = 0, TotalSelSNPE = 0;
    for (int j=0;j<m_pmtinput_tool->Get_nPMT();j++) {    
        int pmtid = m_pmtinput_tool->Get_PMTID(j);
        double q = m_pmtinput_tool->Get_Buff_PEQ(j);
        if(pmtid>TotalLPMT) {
            if(enableSPMTInfo) {
                double nPE_Temp = m_pmtinput_tool->Get_SHIT_Charge(pmtid - ZeroSpmtID);
                TrueHitTime = m_pmtinput_tool->Get_hitTime_Toy(j) + timeshift - STOF[pmtid-ZeroSpmtID];
                if(TrueHitTime > SWindowA-1.0 && TrueHitTime < SWindowB+1.0)  {nPE_Temp += q; TotalSelSNPE += q;}
                m_pmtinput_tool->Set_SHIT_Charge(pmtid-ZeroSpmtID, nPE_Temp);
            }
        } else {
            double nPE_Temp = m_pmtinput_tool->Get_LHIT_Charge(pmtid);
            TrueHitTime = m_pmtinput_tool->Get_hitTime_Toy(j) + timeshift - LTOF[pmtid];
            if(TrueHitTime > LWindowA-1.0 && TrueHitTime < LWindowB+1.0) {nPE_Temp += q; TotalSelLNPE += q;}
            m_pmtinput_tool->Set_LHIT_Charge(pmtid, nPE_Temp);
        }
    }
    for(int j=0;j<m_pmtinput_tool->Get_nDNphotons();j++) {
        int pmtid = m_pmtinput_tool->Get_DNpmtID(j);
        double q = m_pmtinput_tool->Get_DNhitQ(j);

        if(pmtid>TotalLPMT) {
            if(enableSPMTInfo) {
                double nPE_Temp = m_pmtinput_tool->Get_SHIT_Charge(pmtid - ZeroSpmtID);
                TrueHitTime = m_pmtinput_tool->Get_DNhitTime(j) + timeshift - STOF[pmtid-ZeroSpmtID];
                if(TrueHitTime > SWindowA-1.0 && TrueHitTime < SWindowB+1.0) {nPE_Temp += q; TotalSelSNPE += q;}
                m_pmtinput_tool->Set_SHIT_Charge(pmtid-ZeroSpmtID, nPE_Temp);
            }
        } else {
            double nPE_Temp = m_pmtinput_tool->Get_LHIT_Charge(pmtid);
            TrueHitTime = m_pmtinput_tool->Get_DNhitTime(j) + timeshift - LTOF[pmtid];
            if(TrueHitTime > LWindowA-1.0 && TrueHitTime < LWindowB+1.0) {nPE_Temp += q; TotalSelLNPE += q;}
            m_pmtinput_tool->Set_LHIT_Charge(pmtid, nPE_Temp);
        }
    }

    LogDebug<<"Select NPE and LWindowA: "<<TotalSelLNPE<<'\t'<<LWindowA<<'\t'<<LWindowB<<std::endl;
    LogDebug<<"Select NPE and SWindowB: "<<TotalSelSNPE<<'\t'<<SWindowA<<'\t'<<SWindowB<<std::endl;

}




void OMILREC::RemoveDN(const std::list<JM::CalibPmtChannel*>& Chhlist, double m_x, double m_y, double m_z, bool IsSave)
{
    int FHS_hShift = -200; //In case the rsd time is negative
    TH1D* FHSA = (TH1D*)gROOT->FindObject(Form("FirstHitTime1_%d", m_iEvt));
    delete FHSA;
    TH1D* FHSB = (TH1D*)gROOT->FindObject(Form("FirstHitTime2_%d", m_iEvt));
    delete FHSB;
    FHSA = new TH1D(Form("FirstHitTime1_%d", m_iEvt), Form("FirstHitTime1_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);
    FHSB = new TH1D(Form("FirstHitTime2_%d", m_iEvt), Form("FirstHitTime2_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);

    TH1D* SFHSA = (TH1D*)gROOT->FindObject(Form("SFirstHitTime1_%d", m_iEvt));
    delete SFHSA;
    TH1D* SFHSB = (TH1D*)gROOT->FindObject(Form("SFirstHitTime2_%d", m_iEvt));
    delete SFHSB;
    SFHSA = new TH1D(Form("SFirstHitTime1_%d", m_iEvt), Form("SFirstHitTime1_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);
    SFHSB = new TH1D(Form("SFirstHitTime2_%d", m_iEvt), Form("SFirstHitTime2_%d_E_%.2f", m_iEvt, m_QEn), int(ADCL/LFHSBinW), FHS_hShift, ADCL + FHS_hShift);

    for(int i = 0; i<TotalLPMT; i++){
        m_pmtinput_tool->Set_LHIT_Charge(i, 0.);
        LTOF[i] = 0.;
    }
    for(int i = 0; i<TotalSPMT; i++){
        m_pmtinput_tool->Set_SHIT_Charge(i, 0.);
        STOF[i] = 0.;
    }

    std::list<JM::CalibPmtChannel*>::const_iterator chit = Chhlist.begin();
    while (chit!=Chhlist.end()) {
        const JM::CalibPmtChannel  *calib = *chit++;

        unsigned int pmtId = calib->pmtId();
        Identifier id = Identifier(pmtId);
        if (CdID::is20inch(id)) {
        
            double firstHitTime = calib->firstHitTime();
            int pmtid = CdID::module(id);
            LTOF[pmtid] = CalLTOF(pmtid, m_x, m_y, m_z);

            FHSA->Fill(firstHitTime);
            FHSB->Fill(firstHitTime - LTOF[pmtid] - m_pmtpara_tool->GetTimeOffset(pmtid));
        } 
        //else if (CdID::is3inch(id)) {
        //    double firstHitTime = calib->firstHitTime();
        //    int pmtid = CdID::module(id);
        //    pmtid -= TotalLPMT;
        //    STOF[pmtid] = CalSTOF(pmtid, m_x, m_y, m_z);

        //    SFHSA->Fill(firstHitTime);
        //    SFHSB->Fill(firstHitTime - STOF[pmtid] + spmtTimeShift);
        //}
            
    }
    if(IsSave && enableDebug) {
        TestFile->cd();
        FHSA->Write();
        FHSB->Write();
        SFHSA->Write();
        SFHSB->Write();
    }
    
    ///////////////////////
    // Dark noise reduction
    int HitMaxBin = FHSB->GetMaximumBin();
    //Todo futher check and optimize windowA
    double SignalThres = (m_pmtpara_tool->GetLDarkMu() * double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((m_pmtpara_tool->GetLDarkMu() * double(TotalLPMT))/(ADCL/LFHSBinW));
    for(int i = HitMaxBin;i>2; i--) {
        if(FHSB->GetBinContent(i)+FHSB->GetBinContent(i-1)+FHSB->GetBinContent(i-2)<3.0*SignalThres) {
            WindowA = LFHSBinW/2. + (double(i)-1.0)*LFHSBinW - 3.*LFHSBinW + FHS_hShift;break;
        }
        else continue;
    }
    WindowB = SignalWindowL + WindowA;
    LogDebug<<"============ The signal window is :"<<m_iEvt<<": "<< WindowA << " to " << WindowB << std::endl;


    // Recalculate the hit charge inside the signal window
    chit = Chhlist.begin();
    double nPE_Temp = 0.0;
    double TrueHitTime = 0.0;
    double pe_sum_select = 0.0;
    double spe_sum_select = 0.0;
    while (chit!=Chhlist.end()) {
        const JM::CalibPmtChannel  *calib = *chit++; nPE_Temp = 0.0;

        unsigned int pmtId = calib->pmtId();
        Identifier id = Identifier(pmtId);
        if (CdID::is20inch(id)) {
            int pmtid = CdID::module(id);
            Readout_hit_time = calib->time();
            Readout_PE = calib->charge();
            auto ChargeIter  = Readout_PE.begin(), ChargeEndIter = Readout_PE.end();
            auto HitTimeIter = Readout_hit_time.begin();

            while(ChargeIter!=ChargeEndIter) {
                TrueHitTime = (*HitTimeIter) - LTOF[pmtid] - m_pmtpara_tool->GetTimeOffset(pmtid);
                if(TrueHitTime < WindowA-1.0) {ChargeIter++; HitTimeIter++; continue;}
                else if(TrueHitTime < WindowB+1.0) {nPE_Temp += *ChargeIter; ChargeIter++; HitTimeIter++;}
                else break;
            }
            m_pmtinput_tool->Set_LHIT_Charge(pmtid, nPE_Temp);
            pe_sum_select += nPE_Temp;
        } else if (CdID::is3inch(id)) {
            //SPMT use full window
            //
            //int pmtid = CdID::module(id);
            //pmtid -= TotalLPMT;
            //Readout_hit_time = calib->time();
            //Readout_PE = calib->charge();
            //std::vector<double>::iterator ChargeIter   = Readout_PE.begin(), ChargeEndIter = Readout_PE.end();
            //std::vector<double>::iterator HitTimeIter   = Readout_hit_time.begin(), HitTimeEndIter = Readout_hit_time.end();

            ////todo add spmt timeoffset
            //while(ChargeIter!=ChargeEndIter) {
            //    TrueHitTime = (*HitTimeIter) - STOF[pmtid]  + spmtTimeShift; //SPMT use the same window as LPMT with spmtTimeShift
            //    if(TrueHitTime < WindowA-1.0) {ChargeIter++;HitTimeIter++;continue;}
            //    else if(TrueHitTime < WindowB+1.0) {nPE_Temp += *ChargeIter;ChargeIter++;HitTimeIter++;}
            //    else break;
            //}
            //SPMT use full window
            //m_pmtinput_tool->Set_SHIT_Charge(pmtid, calib->nPE());
            //spe_sum_select += calib->nPE();
        } else {
            continue;
        }  
    }
    LogDebug << "totalPE after RemoveDN:" << pe_sum_select << '\t' << spe_sum_select << std::endl;

}





bool OMILREC::GetChargeCenter(double ChargePos_[3])
{
     double Charge = 0;
     for(int i = 0; i< TotalLPMT; i++){
        double Q1 = m_pmtpara_tool->GetLGain(i);
        
        double pmt_pos_x = ALL_LPMT_pos.at(i).X();
        double pmt_pos_y = ALL_LPMT_pos.at(i).Y();
        double pmt_pos_z = ALL_LPMT_pos.at(i).Z();
 
        double hit_charge = m_pmtinput_tool->Get_LHIT_Charge(i); 
        Charge += hit_charge/Q1;
        ChargePos_[0] += pmt_pos_x*hit_charge/Q1;
        ChargePos_[1] += pmt_pos_y*hit_charge/Q1;
        ChargePos_[2] += pmt_pos_z*hit_charge/Q1;
     }

    if(enableSPMTInfo) {
        if(enableSPMTOnly) {
            Charge = 0.;
            ChargePos_[0] = 0.;
            ChargePos_[1] = 0.;
            ChargePos_[2] = 0.;
        }
        for(int i = 0; i< TotalSPMT; i++){
            double hit_charge = m_pmtinput_tool->Get_SHIT_Charge(i);

            double pmt_pos_x = ALL_SPMT_pos.at(i).X();
            double pmt_pos_y = ALL_SPMT_pos.at(i).Y();
            double pmt_pos_z = ALL_SPMT_pos.at(i).Z();

            //LogInfo  << pmt_pos_x << '\t' << pmt_pos_y<< '\t' << pmt_pos_z<<std::endl;
            Charge += hit_charge;
            ChargePos_[0] += pmt_pos_x*hit_charge;
            ChargePos_[1] += pmt_pos_y*hit_charge;
            ChargePos_[2] += pmt_pos_z*hit_charge;
        }
    }

     ChargePos_[0] = ChargePos_[0]/Charge;
     ChargePos_[1] = ChargePos_[1]/Charge;
     ChargePos_[2] = ChargePos_[2]/Charge;
     return true;
}


double OMILREC::CalLTOF(int pmtid, double evtx, double evty, double evtz)
{
    double pmt_pos_x = ALL_LPMT_pos.at(pmtid).X();
    double pmt_pos_y = ALL_LPMT_pos.at(pmtid).Y();
    double pmt_pos_z = ALL_LPMT_pos.at(pmtid).Z();
    double EvtR = sqrt(evtx*evtx + evty*evty + evtz*evtz);
    double Rsp = sqrt( (pmt_pos_x-evtx)*(pmt_pos_x-evtx) + (pmt_pos_y-evty)*(pmt_pos_y-evty) + (pmt_pos_z-evtz)*(pmt_pos_z-evtz));
    double CosThetaVC = (Rsp*Rsp + PMT_R*PMT_R*1.e6 - EvtR*EvtR)/(Rsp*PMT_R*1.e3*2.);
    double LengthWater = 1.e3*PMT_R*CosThetaVC - 1.e3*sqrt(PMT_R*CosThetaVC*PMT_R*CosThetaVC - PMT_R*PMT_R + LS_R*LS_R);

    return RfrIndxLS*(Rsp - LengthWater)*1.e6/c + RfrIndxWR*LengthWater*1.e6/c;
}


double OMILREC::CalSTOF(int pmtid, double evtx, double evty, double evtz)
{
    double pmt_pos_x = ALL_SPMT_pos.at(pmtid).X();
    double pmt_pos_y = ALL_SPMT_pos.at(pmtid).Y();
    double pmt_pos_z = ALL_SPMT_pos.at(pmtid).Z();

    double EvtR = sqrt(evtx*evtx + evty*evty + evtz*evtz);
    double SPMT_R = PMT_R - 0.05;

    double Rsp = sqrt( (pmt_pos_x-evtx)*(pmt_pos_x-evtx) + (pmt_pos_y-evty)*(pmt_pos_y-evty) + (pmt_pos_z-evtz)*(pmt_pos_z-evtz));
    double CosThetaVC = (Rsp*Rsp + SPMT_R*SPMT_R*1.e6 - EvtR*EvtR)/(Rsp*SPMT_R*1.e3*2.);
    double LengthWater = 1.e3*SPMT_R*CosThetaVC - 1.e3*sqrt(SPMT_R*CosThetaVC*SPMT_R*CosThetaVC - SPMT_R*SPMT_R + LS_R*LS_R);

    return  RfrIndxLS*(Rsp - LengthWater)*1.e6/c + RfrIndxWR*LengthWater*1.e6/c;
}


void OMILREC::CoorTransform(const double evtx, const double evty, const double evtz, double& r, double& theta, double& phi)
{
    r = sqrt(evtx*evtx + evty*evty + evtz*evtz);

    if(r>1.e-16) theta = TMath::ACos(evtz/r);
    if(TMath::Abs(evtx)>1.e-16) {
        phi = TMath::ATan(evty/evtx);
        if(sgn(TMath::Sin(theta)*TMath::Cos(phi))!=sgn(evtx)) phi += TMath::Pi();
        if(phi<0.)  phi += 2.*TMath::Pi();
    }
}


double OMILREC::sgn(double x)
{
    if(x < -1.e-16) return -1;
    else if(x < 1.e-16) return 0;
    else return 1;

}



bool OMILREC::finalize()
{
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true; 
}
