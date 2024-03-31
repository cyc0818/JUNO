#ifndef OMILREC_h 
#define OMILREC_h 

#include "TString.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgBase.h"
#include "Geometry/RecGeomSvc.h"
#include "Geometry/PMT.h"
#include "Identifier/Identifier.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TString.h"
#include "TProfile.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include <string>
#include <map>
#include <vector>
#include "TAxis.h"
#include "TRandom3.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "EvtNavigator/NavBuffer.h"

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "IRecTool.h"
#include "OMILREC/NPEUnifCTool.h"

using std::string;
class RecGeomSvc;
class CdGeom;
 
class TTree;


class OMILREC: public AlgBase 
{
    public:
        OMILREC(const std::string& name); 
        ~OMILREC(); 

        bool initialize(); 
        bool execute(); 
        bool finalize(); 
    
        bool Load_ExpectedPEQTime();
        double sgn(double x);
        double Calculate_EVLikelihood(double, double, double, double, double);
        double CalLTOF(int pmtid, double evtx, double evty, double evtz);
        double CalSTOF(int pmtid, double evtx, double evty, double evtz);
        void CoorTransform(const double evtx, const double evty, const double evtz, double& r, double& theta, double& phi);
        void RemoveDN(const std::list<JM::CalibPmtChannel*>& Chhlist, double m_x, double m_y, double m_z, bool IsSave);
        void RemoveToyDN(double m_x, double m_y, double m_z, bool IsSave);
        bool GetChargeCenter(double  ChargePos_[3]);     

        class MyFCN1: public ROOT::Minuit2::FCNBase {
            public:
                MyFCN1(OMILREC *alg) {m_alg = alg;}
                double operator() (const std::vector<double>& x) const {
                    return m_alg->Calculate_EVLikelihood(x[0],x[1],x[2],x[3],x[4]);
                }
                double operator() (const double * x)const{
                    std::vector<double> p(x, x+5);
                    return (*this)(p);
                }

                double Up() const {return 0.5;}
            private:
                OMILREC *m_alg;
        };

    private:
        static const int RNum = 29;
        static const int CalibNum = 240;
        static const int MaxLPmtNum = 20000;
        static const int MaxSPmtNum = 30000;
        static const int ZeroSpmtID = kOFFSET_CD_SPMT;

        static const int QPdfMaxPE = 3;

        static const int ThetaPMTNum = 1440;
        static const int CutThetaPMTI = 1440;
        static const int AvgQPdfMaxPE = 100;
        static const int TnPENum = 6;
        static const int UsedTnPENum = 6;
        static const int TrNum = 35;

        int m_iEvt;
        
        std::vector<double> LTOF;
        std::vector<double> STOF;

       //Charge and hit time information
        std::vector<float> Readout_PE;
        std::vector<float> Readout_hit_time; 
       
        std::vector<TVector3> ALL_LPMT_pos;
        std::vector<TVector3> ALL_SPMT_pos;

        //center detector geometry
        CdGeom*  m_cdGeom;

        //TFitterMinuit* recminuit;
        ROOT::Math::Minimizer* recminuit;
 
        //Calibration Data
        TFile* sfile;
        std::string RecMapPath;
        std::string nPEMapPath; 
        std::string TimePdfPath;
        std::string PMTDataPath;
        std::string PMTChargeSpecPath;
        string nPEMapPath_L;
        string nPEMapPath_S;

        MyFCN1* fcn1;

        //simulation file
        std::string SimFile;
        TTree* simevt;
        TTree* prmtrkdep;

        float Truex;
        float Truey;
        float Truez;
        float TrueQx;
        float TrueQy;
        float TrueQz;
        float TrueR;
        float TrueQR;

        float recQx;
        float recQy;
        float recQz;
        float recTx;
        float recTy;
        float recTz;
        float recTt0;
        float recQTx;
        float recQTy;
        float recQTz;
        float recQTt0;

        float smear_x;
        float smear_y;
        float smear_z;

        int    runID;
        int    TruthLevel;
        double QCfactor;
        double PedThres;
        double TotalLPMT;
        double TotalSPMT;
        double PMT_R ;
        double Ball_R;
        double LS_R;
        double pmt_r;
        double RfrIndxLS;
        double RfrIndxWR;
        double LPMTCalibEnergy;
        double SPMTCalibEnergy;
        double nHESF;
        double Ge68ES;
        double LaserES;
        double ElectronES;
        double m_UfrmScale;

        double WindowA;
        double WindowB;
        double LFHSBinW;
        double SFHSBinW;
        
        bool Use3DMap;
        bool UseGeETimePdf;
        bool enableUseEkMap;
        bool enableUsePhiMap;
        bool enableReadTruth;
        bool enableDebug;
        bool enableUseTrueVertex;
        bool enableTimeInfo;
        bool enableQInfo;
        bool enableDynTimeInfoOnly;
        bool enableDynInfoOnly;
        bool enableMcpInfoOnly;
        bool enableSPMTInfo;
        bool enableSPMTOnly;
        bool enablePMTParaTruth;
        bool enableUseFixedVertex;
        bool enableUseTLHVertex;
        bool enableElectronic;
        bool enableChargeResponse;
        bool enableOccupancy;
        bool enableAddToyQ;
        bool enableAddToyTTS;
        bool enableAddToyDN;
        bool enableTimeInfo_user;
        bool enableUseEkMap_user;
        bool enableSPMTInfo_user;
        bool enableQTimePdf;
        double SignalWindowL;
        std::string RecMode;

        TTree* TRec;
        Float_t recx;
        Float_t recy;
        Float_t recz;
        Float_t timeRecx;
        Float_t timeRecy;
        Float_t timeRecz;
        Float_t timeRecE;
        Float_t timeRect0;
        Float_t fix_x;
        Float_t fix_y;
        Float_t fix_z;
        Float_t edep;

        float EvtT;
        std::vector<float> EvtTArray;

        Float_t m_QEn;
        Float_t m_QTEn;
        Float_t m_NPE;
        Float_t m_NQE;
        Float_t m_PESum;
        Float_t m_QR;
        Float_t m_TR;
        Float_t m_QTR;
        Float_t m_Qstat;
        Float_t m_Tstat;
        Float_t m_QTstat;
        Float_t m_QL;
        Float_t m_TL;
        Float_t m_QTL;
        Float_t m_QRerr;
        Float_t m_TRerr;
        Float_t m_QTRerr;
        Float_t m_firedPMT;
        Float_t m_QTtime;
        Float_t m_Qtime;
        Float_t m_Ttime;
        Double_t m_triggerT;
        Double_t m_dTriggerT;
        Double_t m_FadcEvtT;
        Float_t m_nPhotons;
        Float_t m_memory;
        std::vector<double>* m_lpmt_fht;
        std::vector<double>* m_spmt_fht;
		
        // Physics truth
        int    m_pdgid;
		float  m_init_x; 
		float  m_init_y;
		float  m_init_z;
		float  m_init_r;
		double m_init_t;
		float  m_edep;
		float  m_edep_x;
		float  m_edep_y;
		float  m_edep_z;
		float  m_edep_r;

        // LPMT truth
        std::vector<std::string>* m_lpmt_pulsetype;
        std::vector<int>*    m_lpmt_id;
        std::vector<int>*    m_lpmt_npe;
        std::vector<double>* m_lpmt_hittime;
        std::vector<float>* m_lpmt_amplitude;
        std::vector<float>* m_lpmt_tts;
        std::vector<float>* m_lpmt_timeoffset;
        std::vector<double>* m_lpmt_pulsehittime;

        // SPMT truth
        std::vector<std::string>* m_spmt_tagid;
        std::vector<std::string>* m_spmt_pulsetype;
        std::vector<int>*    m_spmt_id;
        std::vector<int>*    m_spmt_npe;
        std::vector<double>* m_spmt_hittime;
        std::vector<float>* m_spmt_timewindow;
        std::vector<float>* m_spmt_amplitude;
        std::vector<float>* m_spmt_tts;
        std::vector<double>* m_spmt_pulsehittime;
        std::vector<double>* m_spmt_evthittime;
    
        TFile* TestFile;
        //TH1D* SmearCheckList[50];
        //
        ////////////////
        // TimeInfo
        //1pe
        TH2F* hDynGeProb[TrNum][TnPENum];
        TH2F* hMcpGeProb[TrNum][TnPENum];

        TH2F* hDynGeIProb[TrNum];
        TH2F* hMcpGeIProb[TrNum];
        //2pe
        TH1F*   GeDyn_I_ft_Ft[TrNum];
        TH1F*  GeDyn_I_ft_Fdt[TrNum];
        TH1F*  GeDyn_I_fdt_Ft[TrNum];

        TH1F*   GeMcp_I_ft_Ft[TrNum];
        TH1F*  GeMcp_I_ft_Fdt[TrNum];
        TH1F*  GeMcp_I_fdt_Ft[TrNum];
        

        //--------------------- Laser
        //1pe
        TH2F* hDynLsProb[TrNum][TnPENum];
        TH2F* hMcpLsProb[TrNum][TnPENum];

        TH2F* hDynLsIProb[TrNum];
        TH2F* hMcpLsIProb[TrNum];

        //2pe
        TH1F*   LsDyn_I_ft_Ft[TrNum];
        TH1F*  LsDyn_I_ft_Fdt[TrNum];
        TH1F*  LsDyn_I_fdt_Ft[TrNum];

        TH1F*   LsMcp_I_ft_Ft[TrNum];
        TH1F*  LsMcp_I_ft_Fdt[TrNum];
        TH1F*  LsMcp_I_fdt_Ft[TrNum];

        //--------------------- GeL
        TH1F*  Dyn_I_Geft_LFt[TrNum];
        TH1F*  Dyn_I_Lft_GeFt[TrNum];
        TH1F*  Mcp_I_Geft_LFt[TrNum];
        TH1F*  Mcp_I_Lft_GeFt[TrNum]; 


        TH2F* LMu2D[ThetaPMTNum];
        TH2F* LMu2D_Ek[ThetaPMTNum];
        TH2F* SMu2D[ThetaPMTNum];

        TH2F* LMu2D_2d[CutThetaPMTI];
        TH2F* LMu2D_4d[CutThetaPMTI];
        TH2F* LMu2D_6d[CutThetaPMTI];

        TH2F* LMu2D_Ek_2d[CutThetaPMTI];
        TH2F* LMu2D_Ek_4d[CutThetaPMTI];
        TH2F* LMu2D_Ek_6d[CutThetaPMTI];
        double rawEk;
        double mapNorm;

        TH1D* LMu1D[ThetaPMTNum];
        //TH1D* SMu1D[CutThetaPMTI];

        //////////////////
        // nPE charge pdfs
        bool enableLTSPEs;
        //std::map< int, std::verctor<TGraph*> > QPdfs;
        TGraph* QPdfs[MaxLPmtNum][10];
        TH1F* AvgQPdfs[AvgQPdfMaxPE];

        IRecParaTool* m_pmtpara_tool;
        IRecInputTool* m_pmtinput_tool;

        NPEUnifCTool* m_NPEUnifCTool;


};
#endif
