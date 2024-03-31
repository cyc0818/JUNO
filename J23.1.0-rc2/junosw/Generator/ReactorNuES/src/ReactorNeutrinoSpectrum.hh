#ifndef _GENERATORReactorNeutrino_HH
#define _GENERATORReactorNeutrino_HH
#include "CLHEP/Random/RandFlat.h"
#include <vector>
using std::vector;

class ReactorNeutrinoSpectrum {
public:

  //default constructor
  ReactorNeutrinoSpectrum();
  
  bool GeneratePrimaries(double &electron_kinetic_energy, double &electron_costheta);
  bool SetMassOrder(int val,bool); 
  void SetMassOrder(int val){ 
	    if(val==1) MassOrder = true;
	    else if(val==0) MassOrder = false;
  }
  void SetSeed(long val){Seed=val;}
  void SetL(double val){ L = val;}
  bool SetFissionFraction(double val1, double val2, double val3, double val4,bool);
  void SetFissionFraction(double val1, double val2, double val3, double val4){
    fFractionU235 = val1;
    fFractionU238 = val4;
    fFractionPu239 = val2;
    fFractionPu241 = val3;
  }

  bool SetLowESpectrum(int val, bool);
  void SetLowESpectrum(int val){
        if(val==1) LowESpectrum = true;
        else if(val==0) LowESpectrum = false;
  }
  void SetMidESpectrum(int val){MidESpectrum = val;}
  void Setsin2_th12(double val){sin2_th12=val;}
  void Setsin2_th13(double val){sin2_th13=val;}
  void SetDm2(double val1,double val2){
  	Dm2_21=val1;
	Dm2_32=val2;
	Dm2_31=val1+val2;
  }
  double ShootEnergy();
private:

  double survive(double );
  //Calculates the suivive probability
  double cross_antinue(double, double );
  double cross_antinux(double, double );
  void CumulativeDistribution (); //normalrize the spectrum, get the Cumulative distribution. 
  double Spectrum (double );
  long Seed;
/*  double fMinus(double , double );
  double fPlus(double , double );
  double fPlusMinus(double , double );
  double I_f (double ) ;
  double k_e (double ) ;
  double k_mu (double ) ;*/

  bool OldSigma; //methods to cross_nu_electron  
  bool LowESpectrum;// default true for fallot, false for kopeikin
  int MidESpectrum;//0 for DYB,1 for HM, 2 for Fallot
  bool is_read2_8;
  bool is_read8_10;
  bool is_read0_2 ;
  bool MassOrder; //true for normal;false for inverted
  bool is_cumulative;
  bool isFirstTime;
  bool is_normalcross;
  double L;//distance in meters
  double emass;
  int fbin;
  double step;//binwidth of fbin in cross sectrion
  int fNumberOfSteps;//step num in cumulative spectrum
  double fFractionU235;
  double fFractionU238;
  double fFractionPu239;
  double fFractionPu241;
  double UpEn;

  double sin2_th12;
  double Dm2_21; 
  double sin2_th13;
  double Dm2_32;
  double Dm2_31;
  vector<double>             fNuEnergy;
  vector<double>             fFlux;
  vector<double>             fProbability;//Cumulative distribution value
  vector<double>             fEnergyBin;
  double binWidth;    //binWidth of input spectrum
  double fNormalNue;
  double fNormalNux; 
  char *prefix;
};
#endif  
