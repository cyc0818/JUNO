//   Muon Reconstruction at Far Pool
//   use the information from the simulation result    
//   zhangyu96@ihep.ac.cn
#ifndef Recon_h
#define Recon_h 1
//Root
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include <iostream>
#include <fstream>
//other
#include "PoolConstant.h"
#include <vector>
#include <TObject.h>
#include "TMinuit.h"
#include "RecTools/PmtProp.h"
using std::vector;

class IFhtCorrection;
class Recon : public TObject{ // : public TObject of Root is inherit

public:
   Recon();
   //Recon(const char* outfile, TTree* input_tree);
   ~Recon();
   //public fuctions
   void reconstruct();
   void rec_fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);

private:
   unsigned int pmtnum;
   double peakFHT;
   double earliestFHT;

   //PMT position in the pool
   double PmtX[PMTSUM];
   double PmtY[PMTSUM];
   double PmtZ[PMTSUM];
 
   //for TMinuit fit because not all the fired pmt is suitable for reconstruction
   std::vector<double> OptPmtTime;//the time of picked pmt
   //std::vector<double> PmtPe;
   //std::vector<double> PmtFHT;
   std::vector<double> OptPmtPe;//the charge of picked pmt
   std::vector<double> OptPmtW;
   std::vector<double> OptPmtRes;
   std::vector<double> v_residual;//time residual;absolute value
   std::vector<int>    v_sign_residual;//time residual sign : plus : 1; minus -1;
   //std::vector<TVector3> PmtPos;
   std::vector<TVector3> OptPmtPos;
   //For the output reconstruction tree info
   TTree* rec_tree;//reconstruction tree
   int EventId;//the every event ID number 
   int Jentry; 
 
   //generator info for vertex or true info from geometry calculation
   double gen_x;// MuInitPos 
   double gen_y;
   double gen_z;
   double true_lx;//MuInitPx
   double true_my;
   double true_nz;
   double real_theta;// Mu real direction
   double real_phi;
   double real_disCenter;
 
   //for the initial value 
   double ini_x;//  first_guess initial position
   double ini_y;     
   double ini_z;//deduce
 
   double ini_lx;//-1,1      first_guess initial direction
   double ini_my;//-1,1      
   double ini_nz;//deduce
   double ini_theta;//deduce
   double ini_phi;//deduce
       
   double ini_dis;//first_guess distance from initial position to real track
   double ini_angle;
   double ini_midDeviation;
   double ini_difPhi; 
   double ini_disCenter;

   //mu energy guess from first guess
   double ini_E;//Mu energy 
   double ini_v;
 
   //once fit
   double rec_x1;    double rec_lx1;
   double rec_y1;    double rec_my1;
   
   double rec_z1;    double rec_nz1;
   double rec_theta1;//90~180
   double rec_phi1;//-180~180
   double rec_difPhi1;
 
   //from the TMinuit fit results
   double rec_lx;	double minuit_lx_err;
   double rec_my;	double minuit_my_err;
   double rec_x;	double minuit_x_err;
   double rec_y;	double minuit_y_err;
   
   //derivation deduce
   double rec_z;     double rec_nz;
   double rec_theta;//90~180
   double rec_phi;//-180~180
   
   //trick for equator 90°~boundary(97°) 
   double rec_lx_last;  double rec_my_last;
 
   //fitting parameters
   double chi2;//read out from TMinuit Chi2
   int fitN;//record the fit times for total TMinuit fits
   // -2 at last fitN failure
   // -1 no Fit cut1
   // 0  no Fit cut3
   int icstat;
   /* a status integer indicating how good is the covariance matrix
    * 0: not calculated at all
    * 1: diagonal approximation only, not accurate 
    * 2: Full matrix, but forced positive-definite
    * 3: Full accurate covariance matrix 
    * -1: no Fit for cut1
    * -3: no Fit for cut3
    * -2: at last Fit failure
    */
   
   int status;//STATUS
   //FAILED    0
   //SUCCESSFUL 1 //for migrad is converged
   //CONVERGED 2
   //Other(select failure) 3
   //FitN>0  10 
   //FitN<0 or FitN==0 Other status 3

   //IRLS :Stop
   int N_ThrowOut;//the times of the function Throw_out() stop2
   int fitN_stop1;
 
   int flag_event;
   //default flag_event == 1 good 
   //Be carefull flag_event == 0 bad
 
   //other criterion for the reconstruction 
   double rec_angle;//the angle between the reconstruct track and the real track
   double rec_midDeviation;//the minimum distance of two Line
   double rec_difPhi;//avoid period
   double rec_disCenter;

   //MC info
   int    firedPmtN;//Maximum number of fire PMTS per event
   int    firedPmtId[PMTSUM];//Each event fires PMTID 
 
   double firedPmtCharge[PMTSUM];//nPE collected by fire PMTS in each event
   double firedPmtTimeFirst[PMTSUM];//FHT of the PMT in each event
   double firedPmtX[PMTSUM], firedPmtY[PMTSUM], firedPmtZ[PMTSUM];

   double total_pe;//Each event produces PE
   double trackLengthInVetoWater;// the total trackLengthInWater for muon
   double trackLengthInCDWater;//total trackLengthInWater for muon in inner water pool
   double trackLengthInScint;
   double eLossInCDWater;
   double eLossInScint;
   double eLossInAcrylic;
 
   //Pick up MC info
   int    OptPmtN;//select Pmt Number
   int    OptPmtId[PMTSUM];
 
   double OptPmtCharge[PMTSUM];
   double OptPmtFirstTime[PMTSUM];
   double OptPmtWeight[PMTSUM];
  
   //rec info for Tree_fill
   double OptPmtX[PMTSUM];
   double OptPmtY[PMTSUM];
   double OptPmtZ[PMTSUM];  
      
/**********************************function***************************/
   void Init();
   void Event_Init();
   void PMT_cut();
   void Choose_pmt();
   void First_guess();
 
   void Weight_OLS(std::vector<double>& OptPmtW_Pe, int OptPmt_N);
   void Fit_minuit( TMinuit *recMinuit );//for TMinuit
   std::vector<double> Weight_IRLS();
 
   void Throw_out();
   void Tree_fill();
   
   void Sort_TimeResidual(std::vector<double>& vi_residual,std::vector<int>& vi_sign_residual);

//--------------------------------------------------------------------
   void Refer_point(TVector3 &refer_point,TVector3 &point_line, TVector3 &direction_line);      
   //muon expected time arrive at any position(PMT)
   double ExpectedTime(double Li, double Di);
   //calculation the distance of the point to the spacial line 
   double Di_line(TVector3 &point,TVector3 &point_line,TVector3 &direction_line);
   //calculation projection on the track Line
   double Li_line(TVector3 &point,TVector3 &point_line,TVector3 &direction_line);
   //calculate the determinant value of second order
   double Second_Dvalue(double a1,double a2,double b1,double b2);
   //Distance in the 2D plane
   double Di_plane(double *parline, double *parpoint);
   double hitSphereAll(double tarR, TVector3 &center, TVector3 &vpos, TVector3 &dir, TVector3 &X1, TVector3 &X2, double &L);

   double MAD_Residual(std::vector<double>& v_residual);
 
   //reference T0
   double Refer_T0(std::vector<TVector3>& OptPmt_Pos, std::vector<double>& OptPmt_Time, std::vector<double>& OptPmt_W, TVector3 &point_line, TVector3 &direction_line);

   //T_mc-T_theory
   double T_Residual(double OptPmt_Time, double t0, TVector3 point, TVector3 point_line, TVector3 direction_line);

   void unpack(vector<double>& weight_unsorted, vector<double>const& weight);
   double delta_weight(vector<double>& weight_unsorted, vector<double>& weight_unsorted2);
   void FindmaxPE_PMT(double &x, double &y, double &z);

private:
   PmtTable* m_ptable;
   TVector3 pool_center, rec_point, rec_dir, X1, X2;
   double trackL, Dis_center;
   IFhtCorrection* m_fhtcorrtool;

public:
   void set_pmt_num(PmtTable* ptab) { m_ptable = ptab; }
   
   void get_rec_num(TVector3& start, TVector3& end, double &totalpe, double &chi2byndf){ 
      start.SetXYZ(X1.X(),X1.Y(),X1.Z());
      end.SetXYZ(X2.X(),X2.Y(),X2.Z());
      totalpe = total_pe;
      chi2byndf = chi2;
   }

};
#endif
