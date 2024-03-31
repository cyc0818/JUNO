#pragma once
#include "TMath.h"
const int PMTSUM = 17612;

//Cherenkov degree and about the water constant
const double V_muon = 0.299792381;// m/ns (0.1056583755/209)
const double V_c = 0.299792458;// m/ns
const double MASS  = 105.6583755;//MeV mu
const double thetac=TMath::ACos(1./1.356);//1./n radian 1.356:PMT_QE is 390ns(3.2632*eV)
//const double thetac=TMath::ACos(1./1.485);//LS
const double SphereRadius = 20050.;
const TString m_corrfilename = "$RECMU_FHTCORR_DATA/fhtcorr.root";
const TString m_corrmethod = "corr2dtad";
const int m_nBinDist = 18;

//-----Identification of CD Water Buffer Muon------
const int cut_firedPmtN_lack = 15000;
const int cut_firedPmtN_plenty = 1000;
const int cut_nPE = 410000;

//------------------PMT_cut------------------------
const double Relativetime_cut = 150.;
const double cutPE = 3.;

//----------------First_guess----------------------
const int Q_ALPHA = 3;

//---------------------IRLS------------------------
//WEIGHT
const int WEIGHT_IRLS = 2; //optimal
const double DIF_WEIGHT = 0.02; //0.001
const double C_TUNING = 3.0;

//MINUIT
const int FIT_N_MINUIT = 5;//Fixed_value

//real interval is -2 ~ 7ns   
const double LIMIT_LOW = 1.5;   //default negative
const double LIMIT_HIGH = 7.0;  //default positive

const int FIT_N_STOP1 = 25;//Fixed_value
const int FIT_N = 50;//Fixed_value

//THROWOUT
const int THROWOUT_N = 3;
const int THROWOUT_ONCE = 10;//Fixed_value
const double T_RESIDUAL = 10.;//ns

//white 3 with TTS
//const double COSTHETA = -0.121869343405147490; //83°    180°-83°