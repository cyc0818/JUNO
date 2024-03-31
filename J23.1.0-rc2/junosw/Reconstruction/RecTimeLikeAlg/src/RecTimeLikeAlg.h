/*=============================================================================
#
# Author: Jingyu Luo - ljys1234@mail.ustc.edu.cn 
# modified: 2013-12-20 15:30
# Author: Ziyuan Li - liziyuan3@mail.sysu.edu.cn
# modified: 2019-12-12
#   Add Peak Time Fitter algorithm
#   Add ToyMC function for study
#   Add DarkNoise PDF
# modified: 2020-05-01
#   Add TMinuit to replace GridSearch
#   Solve DarkNoise issue at detector border
#   Add fos correction and bias correction
# Filename: RecTimeLikeAlg.cc
# Description: The algorithm includes the vertex reconstruction based on realtive
hit time likelihood, the visible energy reconstruction based on charge likelihood
and the non-linearity correction for positron
# Notification: the vertex reconstruction and visible energy reconstrucion got tuned,
The non-linearity need to be further considered.
=============================================================================*/
#ifndef RecTimeLikeAlg_h
#define RecTimeLikeAlg_h

#include <algorithm>

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "Geometry/RecGeomSvc.h"
#include <boost/filesystem.hpp>
#include <boost/python.hpp>

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "RootWriter/RootWriter.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "PMTCalibSvc/PMTCalibSvc.h"
#include "Math/Minimizer.h"
#include "Math/Functor.h"
#include "Math/Factory.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgBase.h"
#include "Geometry/RecGeomSvc.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/SimHeader.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
// #include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include "Geometry/IPMTParamSvc.h"
#include <string>
#include <vector>
#include <tuple>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TProfile2D.h"
#include "TProfile3D.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TString.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>
#include <CondObj/CommonCondObj.h>
#include <RecTools/IRecTool.h>
#include <thread>

using  std::string;
using namespace std;
class RecGeomSvc;
class CdGeom;
class IRecTool;
class PtfRecTool;
class QcRecTool;

struct PMTHit {
  Identifier pmtId;

  double hit = 0;
  double hitTime = __DBL_MAX__;
  double hitTimeTTS = 0;
  double darkRate = 0;

  // TODO: Can we merge the two ?
  bool hitCut = true;
  bool pmtHitCut = true;

  bool isDnHit = false;
  bool isUsed = true;
};

class RecTimeLikeAlg: public AlgBase 
{
	public:
		RecTimeLikeAlg(const std::string& name); 
		~RecTimeLikeAlg(); 

		bool initialize(); 
		bool execute(); 
		bool finalize(); 

		bool Book_Tree();
		bool Load_LikeFun();
		bool GridSearch(); 
		bool ChargeCenterRec();
		bool ChargeCenterFitter(double percent);
		bool ChargeCenterRecCorr();
		bool ApplyCorrection(double energy);
		void NpeCorrection();

		double HittimeGoodness(double, double, double,double);

		double CalculateTOF( double, double, double, int);
		double CalculateTOF_PTF( double, double, double, int);

		double Dist2PMT( double, double, double, int);

		//double Calculate_Energy_Likelihood(double, double, double, double,double);
		double Calculate_Energy_Likelihood(double, double, double, double);
		void Energy_Minimize(double x_fit, double y_fit, double z_fit, double& n_fit);
		void Energy_Correction(double x_fit, double y_fit, double z_fit, double n_fit, double& E_rec);

		//Add Peak Time Fitter algorithm
		bool PeakTimeFitter();
		TVector3 calDeltaR_Combine(double x,double y,double z);

		//Apply FOS time correction
		double FOS_correction(double npe, double tts);

		void GenerateDarkNoisePDF(TH1D *hist, double darkrate);
		void DarkNoiseFilter();

		void SwitchPMTType();
		void TuningNeff();

		//vector
		void VectorPush();
		void VectorClear();

		class MyFCN: public ROOT::Minuit2::FCNBase {
			public:
				MyFCN(RecTimeLikeAlg *alg){m_alg =alg;}
				double operator() (const std::vector<double>& x)const{
					return m_alg->Calculate_Energy_Likelihood(x[0],x[1],x[2],x[3]);
				}
				double operator() (const double * x)const{
					std::vector<double> p(x, x+4);
					return (*this)(p);
				}

				double Up() const {return 0.5;}
			private:
				RecTimeLikeAlg *m_alg;
		};

		//Use Minimizer for vertex reconstruction
		bool Vertex_Minimize();
		class VertexRecLikelihoodFCN: public ROOT::Minuit2::FCNBase {
			public:
				VertexRecLikelihoodFCN(RecTimeLikeAlg *alg){m_alg =alg;}
				double operator() (const std::vector<double>& x)const{
					return m_alg->HittimeGoodness(x[0],x[1],x[2],x[3]);
				}
				double operator() (const double * x)const{
					std::vector<double> p(x, x+4);
					return (*this)(p);
				}
				double Up() const {return 0.5;}

			private:
				RecTimeLikeAlg *m_alg;
		};

	private:

		//Time Likelihood function
		TFile* file_Time_nhit_mcp[5];
		TFile* file_Time_nhit_ham[5];

		TFile* file_eneuniformcorr;
		TH1D* h_correction; 

		TFile* file_PmtData;
		TTree* t_PmtData;

		TFile* file_vtxbiascorr;
		TProfile2D* p2_biascorr;

		TH1D* pdf_nhit_mcp[100];
		TH1D* pdf_nhit_ham[100];
		static constexpr int nbins_z = 10 ;
		std::vector<double> vec_zbinning;
		TH3F* pdf_z_r_dist2pmt_ham[10];
		TH3F* pdf_z_r_dist2pmt_cdf_ham[10];
		TH3F* pdf_z_r_dist2pmt_mcp[10];
		TH3F* pdf_z_r_dist2pmt_cdf_mcp[10];
		TH1F* pdf_dn_ham;
		TH1F* pdf_dn_cdf_ham;
		TH1F* pdf_dn_mcp;
		TH1F* pdf_dn_cdf_mcp;

		TFile *file_chargemap;
		TProfile3D* Qpdf_r3_z_dist2pmt;

		TGraph* gr_eneneff;

		int m_iEvt;
		JM::NavBuffer* m_buf; 

		int num_PMT;

    std::vector<PMTHit> PMT_HITS;

	  TH1D *h1_Hittime = nullptr;
	  TH1D* tmp_hit_time = nullptr;

    std::vector<double> TimeSpread;
    std::vector<double> DarkRate;


	  std::vector<double> v_res_time;

		int pmtId;
		double timeSpread;
		double darkRate;

		double tmp_ve_x;
		double tmp_ve_y;
		double tmp_ve_z;
		double tmp_t_zero;
		double sign_x;
		double sign_y;
		double sign_z;
		double sign_t;
		double m_ratio;
		double min_hit_time;

		double relaTime;

		//Charge and hit time information
		std::vector<double> Readout_PE;
		std::vector<double> Readout_hit_time; 
		std::vector<double>  PMT_TTS; 
		std::vector<int>  PMT_ID; 

		std::vector<std::tuple<double, double, double, double, double> > FirstHitTime_NPE_PMTPos;

		//PMTCalibSvc Information
		std::vector<double> pmtcal_RelativeDE;
		std::vector<double> pmtcal_Gain;
		std::vector<double> pmtcal_MeanGain;
		std::vector<double> pmtcal_TimeOffset;
		std::vector<double> pmtcal_DarkRate;
		std::vector<TH1D*>  pmtcal_ChargeSpec;

		std::vector<TVector3> ALL_PMT_pos;

		//Charge Center Reconstruction
		static TF1 *fun_ccscale;
		static std::mutex m_fun_ccscale_mtx;
		double ChaCenRec_x;
		double ChaCenRec_y;
		double ChaCenRec_z;
		double ChaCenRec_r;

		//Peak Time Fitter Reconstruction
		int m_FOSCorr;
		int	m_ptf_maxnpe;
		double m_PTFRec_x;
		double m_PTFRec_y;
		double m_PTFRec_z;
		double m_PTFRec_r;
		double m_PTFRec_t;
		
		//Temporary Residual Time Distribution
		TH1D* tmp_resTime;
		TH1D* tmp_resTime_fix;

		//center detector geometry
		//CdGeom*  m_cdGeom;

		//PMT Geom&Param Svc
		// Temporary modifications
		// IPMTSimParamSvc* m_pmtsimsvc;
    	IPMTParamSvc* m_pmtsvc;

		//Migrate to Minimizer
		ROOT::Math::Minimizer *recminimizer;
		MyFCN *fcn;

		//Use Minimizer for vertex reconstruction
		int m_Minimizer;
		ROOT::Math::Minimizer *vtxllminimizer;
		VertexRecLikelihoodFCN *vtxllfcn;

		string File_path; 
		int Total_num_PMT;
		double PMT_R ;
		double Ball_R;
		double LS_R;
		double pmt_r;
		double ChaCenRec_ratio;
		double m_Energy_Scale;
		int m_VtxCorr;
		int m_isElec;
		int m_FHS;

		//Performance Study
		TTree *m_ntuple;
		int m_Algorithm;
		int m_TTS;
		int m_DN;
		int m_DNPDF;
		int m_DNFilter;
		double m_TimeWindow;
		double m_DarkRate;
		double m_LightYield;
		int m_TimeOffset;
		int m_PmtType;
		int m_PMTRatio;
		bool m_isSaveMC;
		int m_InitVtx;
		int m_NpeSmear;
		int m_ToyElec;

		//fine tuning
		bool m_TuneNeff;
		double m_Neff;
		int m_tlh_maxnpe;

		double m_neff;
		bool flag_tts;
		bool flag_tts_hamamastu;
		bool flag_tts_mcp;
		bool flag_dn;

		//DarkNoiseFilter
		double m_start_time;
		double m_end_time;

		double m_effective_attenuation;

		//MC information
		double m_init_x; 
		double m_init_y;
		double m_init_z;
		double m_init_r;
		double m_init_t;
		double m_edep;
		double m_edep_x;
		double m_edep_y;
		double m_edep_z;
		double m_edep_r;

		//Fit Result
		double m_ccr_x;
		double m_ccr_y;
		double m_ccr_z;
		double m_rec_x;
		double m_rec_y;
		double m_rec_z;
		double m_rec_r;
		double m_rec_theta;
		double m_rec_phi;
		double m_rec_t;
		double m_like_vtx;
		double m_rec_n;
		double m_rec_E;
		double m_like_ene;
		double m_pe_sum;
		double m_time;
		double m_ene_time;
		double m_memory;

		CondDB::ICondDBSvc* m_conddb_svc;
		static std::mutex m_conddb_svc_mtx;
		CondObj::Common::CommonCondObj m_dynodepdf_path;
		CondObj::Common::CommonCondObj m_mcppdf_path;
};
#endif
