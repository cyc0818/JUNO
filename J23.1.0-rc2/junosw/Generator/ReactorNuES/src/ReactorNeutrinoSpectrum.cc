/**
 * Please check the update of Oscillation parameters.
 * This generator is for Juno, and the Oscillation is based on it.
 * Solar neutrino generator imported from Borexino was referenced.
 * J.H. Huang 10/26  huangjinhao@ihep.ac.cn.
 * Details in JUNO Document 7644.
 */

#include "TF1.h"
#include "ReactorNeutrinoSpectrum.hh"
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TMath.h"
#include <TRandom3.h>
using std::ifstream;

ReactorNeutrinoSpectrum::ReactorNeutrinoSpectrum() {
    prefix = std::getenv("REACTORNUESROOT");  
    if(!prefix) {
        printf("Please set $REACTORNUESROOT to the location of the REACTORNUESROOT package root.\n");
		exit(0);
    }
  isFirstTime = true;
  is_read0_2 = false;
  is_read2_8 = false;
  is_read8_10 = false;
  is_cumulative = false;
  is_normalcross = false;
  LowESpectrum = true;
  fFractionU235 = 0.564;
  fFractionPu239 = 0.304;
  fFractionU238 = 0.076; 
  fFractionPu241 = 0.056;
  OldSigma = true;
  UpEn = 10;
  L = 5.25e4; //in meters        
  emass = 0.510998928; // MeV
  MassOrder = true;
  fbin=4000;
  step = (UpEn*1.05)/double(fbin);
  fNumberOfSteps=10000;
  fNormalNue=0;
  fNormalNux=0;
  Seed=0;
  MidESpectrum=0;

  sin2_th12 = 0.304;

    if(MassOrder){
        Dm2_21 = 7.42e-5;// in eV2 
        sin2_th13 = 0.0222;
        Dm2_31 = 2.517e-3;// Dm2 = m_3^2 - (m_2^2 + m_1^3)/2                                                                                                  
        Dm2_32 = Dm2_31-Dm2_21; 
    } else{
        Dm2_21 = 7.42e-5; 
        sin2_th13 = 0.0224;
        Dm2_32 = -2.498e-3;
        Dm2_31 = Dm2_32+Dm2_21; 
    }

}

bool ReactorNeutrinoSpectrum::GeneratePrimaries(double &electron_kinetic_energy, double &electron_costheta){ 
	if(isFirstTime) {
    	//cout<<"initialization"<<endl;
		CumulativeDistribution();
		gRandom->SetSeed(Seed);
		//cout<<"cum finished"<<endl;
        for(int k=0; k<fbin;k++){
            double norma_nue = 0; 
            double norma_nux = 0; 
            for(int i=0;i<fbin;i++){
                norma_nue += cross_antinue(step*i+step/2.,step*k+step/2.);
                norma_nux += cross_antinux(step*i+step/2.,step*k+step/2.);
            }
            if(fNormalNue < norma_nue) fNormalNue = norma_nue;
            if(fNormalNux < norma_nux) fNormalNux = norma_nux;//max neu for average energy e- with a specific energy of neutrino k
        }
        //cout<<"fNormalNue = "<<fNormalNue<<endl;
        //cout<<"fNormalNux = "<<fNormalNux<<endl;
		isFirstTime = false;
	}
	double EEnergy=0;
	double NuEnergy;
	bool isGenerated = false;
		
	while(!isGenerated){
		NuEnergy = ShootEnergy();
		double probx= 1-survive(NuEnergy);
			EEnergy=0;
		//find normal cross section
		if(gRandom->Uniform(0,1) < probx){ // antinux
			double sum = 0;
			double value = gRandom->Uniform(0,1);
			if(value<cross_antinux(0,NuEnergy)/fNormalNux){
			isGenerated = false;//kinetic_energy == 0
			break;
			}
			for(int i=0; i<fbin; i++){
				EEnergy = step*i+step/2.; 
				sum += cross_antinux(EEnergy,NuEnergy)/fNormalNux;
				if(cross_antinux(EEnergy,NuEnergy) == 0){
					isGenerated = false;
					break;
				}
				if(sum > value) {
					isGenerated = true;
					if(gRandom->Uniform(0,1)> fNormalNux/fNormalNue) isGenerated = false;
					break;
				}
			}
		}else{   //antinue
			double sum = 0;
            double value = gRandom->Uniform(0,1);
            if(value<cross_antinue(0,NuEnergy)/fNormalNue){
            isGenerated = false;//kinetic_energy == 0
            break;
            }
            for(int i=0; i<fbin; i++){
                EEnergy = step*i+step/2.; 
                sum += cross_antinue(EEnergy,NuEnergy)/fNormalNue;
                if(cross_antinue(EEnergy,NuEnergy) == 0){
                    isGenerated = false;
                    break;
                }
                if(sum > value) {
                    isGenerated = true;
                    break;
                }
            } 
		}
	}	


	electron_kinetic_energy = EEnergy;
	electron_costheta = (emass/NuEnergy+1.)*sqrt(EEnergy/(EEnergy+2.*emass));
	return true;
}

double ReactorNeutrinoSpectrum::survive(double E){
// data from http://www.nu-fit.org/?q=node/228
// Neutrino oscillations in matter was not contained 
    double cos2_th12 = 1. - sin2_th12;
    double sin2_2th12 = 4.*sin2_th12*cos2_th12;
    double cos2_th13 = 1. - sin2_th13;
    double sin2_2th13 = 4.*sin2_th13*cos2_th13;
    double cos4_th13 = pow(cos2_th13, 2.);  

	double t1 = sin2_2th12 * cos4_th13 * pow(sin(1.267 * Dm2_21 * L / E), 2.); //attention   units of E should be MeV with L in meters
	double t2 = sin2_2th13 * cos2_th12 * pow(sin(1.267 * Dm2_31 * L / E), 2.);
    double t3 = sin2_2th13 * sin2_th12 * pow(sin(1.267 * Dm2_32 * L / E), 2.);
    //double t2 = 0.5 * sin2_2th13 * (pow(sin(1.267 * Dm2_31 * L / E), 2)+pow(sin(1.267 * Dm2_32 * L / E), 2));
	//double t3 = 0.5 * cos_2th12 * sin2_2th13 * pow(sin(1.267 * Dm2_21 * L / E), 2)
	//		    * pow(sin(1.267 * (Dm2_31 + Dm2_32) * L / E), 2);
    double surProb = 1. - t1 - t2 - t3;
    return surProb;
}
//--------------Follow the SolNuGen-------------------
double ReactorNeutrinoSpectrum::cross_antinue(double E, double nuE){//(Energy e & Energy antinu)
	double sigma = 0;
	if(OldSigma){	
        double g1 = 0.23 ;
		double g2 = 0.73 ;
        if(E > 2.*nuE*nuE/(emass+2.*nuE)) return 0 ;
        else  sigma = g1*g1+g2*g2*pow(1.-E/nuE,2.) - g1*g2*E*emass/nuE/nuE ;
        if(sigma < 0) sigma = 0 ;
    }else{
		// method2
	}
	
    return sigma ;	
}

double ReactorNeutrinoSpectrum::cross_antinux(double E, double nuE){
	double sigma = 0;
	if(OldSigma){   
        double g1 = 0.23 ;
        double g2 = -0.27 ;
        if(E > 2.*nuE*nuE/(emass+2.*nuE)) return 0 ;
        else  sigma = g1*g1+g2*g2*pow(1.-E/nuE,2.) - g1*g2*E*emass/nuE/nuE ;
        if(sigma < 0) sigma = 0 ;
    }else{
        // method2
    }
                 
    return sigma ;  
}

void ReactorNeutrinoSpectrum::CumulativeDistribution( ){
	if(!is_cumulative){
		double sum=0;
		double fstep = UpEn/double(fNumberOfSteps);
		for(int i=0;i<fNumberOfSteps;i++){
			double bin = float(i)*fstep + fstep/2.;
			sum += Spectrum(bin)*fstep;
			fEnergyBin.push_back(bin);
			fProbability.push_back(sum);
		}
		for(int i = 0;i<fNumberOfSteps; i++){
			fProbability[i] = fProbability[i]/fProbability[fNumberOfSteps-1];
		}
		
	}
}    


double ReactorNeutrinoSpectrum::ShootEnergy( ){
	double val = gRandom->Uniform(0,1);
	for(int i=0; i< int(fProbability.size()); i++){
		if(fProbability[i]>=val){
			double deltaX = val - fProbability[i];
			double y = fEnergyBin[i] - fEnergyBin[i-1] ;
            double x = fProbability[i] - fProbability[i-1] ;
            return deltaX*y/x + fEnergyBin[i];
		}
	}
	return 0; 

}

double ReactorNeutrinoSpectrum::Spectrum(double E){
//units in /(fission*MeV)
	if(!is_read0_2){
		if(is_read2_8){
			//cerr<<"warning: Spectrum setting oder was wrong"<<endl; 
			throw -1;
		}
		if(LowESpectrum){
		//input fallot
        	double ene, prob235, prob239, prob241, prob238, ncapture,prob;
			is_read0_2 = true;
			std::string spcfilename =prefix ;
			spcfilename += "/data/";
        	spcfilename += "Fallotinput1.dat";
        	ifstream fil0_2(spcfilename.c_str());
        	if(!fil0_2.is_open()) {
        	   // cerr<<"file "<<spcfilename<<" does not exists."<<endl;
        	    throw -1;
        	}
        	for(int i=0;i<40;i++) {
        	    fil0_2 >> ene >> prob235>> prob239>> prob241>>prob238>>ncapture;
        	    //if(fil0_2.eof()) cerr<<"warning: Fallot Spectrum setting error"<<endl;
				prob = fFractionU235 * prob235 + fFractionU238 * prob238 +  fFractionPu239 * prob239 + fFractionPu241 * prob241+ncapture;
        	    fNuEnergy.push_back(ene);
				fFlux.push_back(prob);	
        	}
        	fil0_2.close();	
		}
		else{
		//input kopeikin
			double ene, prob;
            is_read0_2 = true;
			std::string spcfilename =prefix ;
            spcfilename += "/data/";
            spcfilename += "Kopeikininput.dat";
            ifstream fil0_2(spcfilename.c_str());
            if(!fil0_2.is_open()) {
               // cerr<<"file "<<spcfilename<<" does not exists."<<endl;
                throw -1;
            }
        	for(int i=0;i<40;i++) {
                fil0_2 >> ene >> prob;
        	 //   if(fil0_2.eof()) cerr<<"warning: kopeikin Spectrum setting error"<<endl;
        	    fNuEnergy.push_back(ene);
                fFlux.push_back(prob);  
            }
            fil0_2.close();
		}
	}
	
	if(!is_read2_8&&is_read0_2) {
        is_read2_8 = true;
		if(MidESpectrum==0){
			double ene,probtotal,prob235,probcom,prob241,prob238,prob;
			double f235=0.564;
			double f238=0.076;
			double f239=0.304;
			double f241=0.056;
			std::string spcfilename =prefix ;
            spcfilename += "/data/";
            spcfilename += "DayaBayinput.dat";
            ifstream fil2_8(spcfilename.c_str());
        	if(!fil2_8.is_open()) {
     		    //cerr<<"file "<<spcfilename<<" does not exists."<<endl;
				throw -1;
            }
			while(!fil2_8.eof()) {
                fil2_8 >> ene >> probtotal>> prob235>> probcom>>prob241>>prob238;
                if(fil2_8.eof()) break ;
                prob = probtotal+ (fFractionU235-f235) * prob235 + (fFractionU238-f238) * prob238 +  (fFractionPu239-f239) * probcom + (fFractionPu241-f241-0.183*(fFractionPu239-f239)) * prob241;
                fNuEnergy.push_back(ene);
                fFlux.push_back(prob);  
            }
            fil2_8.close();
		}else if(MidESpectrum == 1){                                                  
        	double ene, prob235, prob239, prob241, prob238, prob;
			//std::string spcfilename = prefix;
			std::string spcfilename =prefix ;
        	spcfilename += "/data/";
        	spcfilename += "HMspectrum.dat";
        	ifstream fil2_8(spcfilename.c_str());
        	if(!fil2_8.is_open()) {
     		    //cerr<<"file "<<spcfilename<<" does not exists."<<endl;
        	    throw -1;
        	}
        	while(!fil2_8.eof()) {
        	    fil2_8 >> ene >> prob235>> prob239>> prob241>>prob238;
        	    if(fil2_8.eof()) break ;
				prob =  fFractionU235 * prob235 + fFractionU238 * prob238 +  fFractionPu239 * prob239 + fFractionPu241 * prob241;
        	    fNuEnergy.push_back(ene);
				fFlux.push_back(prob);	
        	}
        	fil2_8.close();
		}else if(MidESpectrum==2){
			double ene, prob235, prob239, prob241, prob238, prob;
			std::string spcfilename =prefix ;
            spcfilename += "/data/";
            spcfilename += "Fallotinput2.dat";
            ifstream fil2_8(spcfilename.c_str());
            if(!fil2_8.is_open()) {
                //std::cerr<<"file "<<spcfilename<<" does not exists."<<std::endl;
                throw -1;
            }
            while(!fil2_8.eof()) {
                fil2_8 >> ene >> prob235>> prob239>> prob241>>prob238;
                if(fil2_8.eof()) break ;
                prob =  fFractionU235 * prob235 + fFractionU238 * prob238 +  fFractionPu239 * prob239 + fFractionPu241 * prob241;
                fNuEnergy.push_back(ene);
                fFlux.push_back(prob);  
            }
            fil2_8.close();	
		}
	}	
	if(!is_read8_10&&is_read2_8&&is_read0_2){
		is_read8_10=true;
		double ene, prob235, prob239, prob241, prob238, prob;
		std::string spcfilename =prefix ;
        spcfilename += "/data/";
        spcfilename += "Fallotinput3.dat";
        ifstream fil8_10(spcfilename.c_str());
        if(!fil8_10.is_open()) {
            //std::cerr<<"file "<<spcfilename<<" does not exists."<<std::endl;
            throw -1;
        }
        while(!fil8_10.eof()) {
            fil8_10 >> ene >> prob235>> prob239>> prob241>>prob238;
            if(fil8_10.eof()) break ;
            prob =  fFractionU235 * prob235 + fFractionU238 * prob238 +  fFractionPu239 * prob239 + fFractionPu241 * prob241;
            fNuEnergy.push_back(ene);
            fFlux.push_back(prob);  
        }
        fil8_10.close();
	}

    double binWidth = fNuEnergy[1] - fNuEnergy[0];//bin width
    int    k = int(E/binWidth) ;// position of bin have to change after given spectrum below 2.0
    return (fFlux[k+1]-fFlux[k])*(E-fNuEnergy[k])/(fNuEnergy[k+1]-fNuEnergy[k])+fFlux[k];
}


bool ReactorNeutrinoSpectrum::SetMassOrder(int val ,bool ){
    if (val==0||val==1){
        SetMassOrder( val);
        return true;
    }
    else return false;                                                                       
}

bool ReactorNeutrinoSpectrum::SetLowESpectrum(int val ,bool ){
    if (val==0||val==1){
        SetLowESpectrum( val);
        return true;
    }
    else return false;                                                                       
}

bool ReactorNeutrinoSpectrum::SetFissionFraction(double val1, double val2, double val3, double val4,bool  ){
	if(val1>=0&&val1<=1&&val2>=0&&val2<=1&&val3>=0&&val3<=1&&val4>=0&&val4<=1&&(val1+val2+val3+val4)==1){
		SetFissionFraction(val1,val2,val3,val4);
		return true;
	}
	else return false;
	
}
/*double SolarNeutrinoSpectrum::I_f(double E) {
    double x=sqrt(1+2*emass/E);
    return 1./6.*(1./3.+(3.-x*x)*(1./2.*x*log((x+1.)/(x-1.))-1.));
}
double SolarNeutrinoSpectrum::k_e(double E) {
    return 0.9786+0.0097*I_f(E);
}
double SolarNeutrinoSpectrum::k_mu(double E) {
    return 0.9965-0.00037*I_f(E);
}

double SolarNeutrinoSpectrum::fMinus(double z, double q){
    double E=z*q+emass;
    double l=sqrt(E*E-emass*emass);
    if(val!=val)
        return 0;
    else
        return val;
}

double SolarNeutrinoSpectrum::fPlus(double z, double q){
    double E=z*q+emass;
    double l=sqrt(E*E-emass*emass);
    double val= 1./(1.-z)/(1.-z)*(E/l*log((E+l)/emass)-1.)*((1-z)*(1-z)*(2*log(1-z-emass/(E+l))-log(1-z)-log(z)/2.-2./3.)-(z*z*log(z)+1-z)/2)-(1-z)*(1-z)/2.*(
log(1-z)*log(1-z)+l/E*(TMath::DiLog(1.-z)-log(z)*log(1-z)))+log(1-z)*(z*z/2.*log(z)+(1-z)/3.*(2*z-1./2.))-z*z/2.*TMath::DiLog(1-z)-z*(1-2*z)/3.*log(z)-z*(1-z)/6.-l/E/12.*(log(z)+(1-z)*(115-109*z)/6.);    if(val==val)
        return val;
    else
        return 0;
}

double SolarNeutrinoSpectrum::fPlusMinus(double z, double q){
    double E=z*q+emass;
    double l=sqrt(E*E-emass*emass);
    double val= (E/l*log((E+l)/emass)-1.)*2.*log(1.-z-emass/(E+l));
    if(val==val)
        return val;
    else
        return 0;
}
*/
