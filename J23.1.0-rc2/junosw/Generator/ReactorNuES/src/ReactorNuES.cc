/**
 * This is a generator program to provid Electron momentum and direction 
 * of the final State in elastic scattering of reactor antineutrinos and 
 * electrons in Juno.
 * J.H. Huang 10/2021   huangjinhao@ihep.ac.cn
 * Details in JUNO Document 7644
 */
#include "TH1.h"
#include <TCanvas.h>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <TRandom3.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ReactorNeutrinoSpectrum.hh"
#include "TFile.h"        
#include "TTree.h"
#include <TROOT.h>
#include <TApplication.h>
#include "TMath.h"
#include <cmath>
#include "CLHEP/Units/SystemOfUnits.h"
//const double MeV = 1.0;
using namespace std;
using namespace CLHEP; 
void usage() {
/*		std::cerr<<		"*************************************"<<std::endl;
		std::cerr<<		"*************************************"<<std::endl;
		std::cerr<<		"*************************************"<<std::endl;
    	std::cerr<<"default setting: "<<std::endl;
		std::cerr<<	   "Event=100       ratio=0.65"<<std::endl;
		std::cerr<<		"fissionfraction:(U235:Pu239:Pu241:U238)=(0.6:0.3:0.05:0.05)"<<std::endl;
		std::cerr<<		"Normal mass ordering   output name: ReactorES.root "<<std::endl;
		std::cerr<<		"Fallot&HM spectrum with DYB bump fix"<<std::endl;
		std::cerr<<		"*************************************"<<std::endl;
		std::cerr<<		"*************************************"<<std::endl;
    std::cerr<<"Usage : ReaESGen.exe" <<std::endl;
	std::cerr <<" -n {Events Num} " <<std::endl;
    std::cerr <<" -r {ratio=YJ/(TS+YJ),range[0,1]} " <<std::endl;
	std::cerr <<" -b {1 for DYBbumpfix,0 for not use}" <<std::endl;
	std::cerr <<" -m {1 for NO, 0 for IO}" <<std::endl;
	std::cerr <<" -o {outputname}" <<std::endl;
	std::cerr <<" -l {Set 0~2MeV  spectrum,1 for Fallot,0 for Kopeikin}"	 <<std::endl;
	std::cerr <<" -ff {235} {238} {239} {241} "<<std::endl;
	std::cerr <<"-yff -tff {fissionfraction setting for YJ and TS, respectively}"<<std::endl;    
		std::cerr<<		"*************************************"<<std::endl;
		std::cerr<<		"*************************************"<<std::endl;
	if(option) {
		exit(1);
	};*/
 printf("ReactorNuES.exe [-seed seed] [-n nevents] [-o outputname] [-r ratio of two reactors][-debug 1][-ff fission fraction (4 input)][-lows 0-2spectrum][-Ybasel -Tbasel distance in meters][-mids 2-8spectrum][-sin2_th12][-sin2_th13][-Dm2(21 32)][-rng 1(reactor neutrino generator)]\n"); 
 exit(0);
}


void set_MassOrder(ReactorNeutrinoSpectrum &reactor,int massorder) {
    bool ok = reactor.SetMassOrder(massorder,true);
    if(!ok) {
		//std::cerr<<"!!!!!illegal -m  input !!!!!"<<std::endl;
		usage();
	}
} 

void set_FissionFraction(ReactorNeutrinoSpectrum &reactor,double val1, double val2, double val3, double val4) {
    bool ok = reactor.SetFissionFraction(val1, val2, val3, val4, true);
    //if(!ok)	std::cerr<<"!!!!!illegal -ff  input !!!!!"<<std::endl;
    if(!ok) usage();
}

void set_LowESpectrum(ReactorNeutrinoSpectrum &reactor,int val) {
    bool ok = reactor.SetLowESpectrum(val,true);
    //if(!ok)	std::cerr<<"!!!!!illegal -l  input !!!!!"<<std::endl;
    if(!ok) usage();
} 

int main(int argc, char *argv[]){
	TApplication theApp("theApp",0,0);
	//default setting
	int nEvent = 10000;
	ReactorNeutrinoSpectrum TSgen;
	ReactorNeutrinoSpectrum YJgen;
	bool ReactorES=true;
	bool Debug = false;
	char * outputName;
	outputName = NULL;
	long seed=0; 
	double ratio = 2.9*6. / (4.6*2.+2.9*6.);//  YJ/(TS+YJ);
    if(argc==1) ;	
	else {
		for(int i=1;i<argc;i=i+2){
			if( strcmp(argv[i],"-n")==0) {
				nEvent = atoi(argv[i+1]);
				if(nEvent<=0){
    				//std::cerr<<"!!!!!illegal -n  input !!!!!"<<std::endl;
					usage();	
				}
			}
			else if( strcmp(argv[i],"-o")==0){
				outputName  = argv[i+1]; 
			}
			else if( strcmp(argv[i],"-r")==0){
				ratio = atof(argv[i+1]);
				if(ratio<0||ratio>1) {
    				//std::cerr<<"!!!!!illegal -r  input !!!!!"<<std::endl;
					usage();	
				}
			}
			else if( strcmp(argv[i],"-seed")==0){
				seed = atoi(argv[i+1]);
				TSgen.SetSeed(seed+1);
				YJgen.SetSeed(seed+2);
			}
			else if( strcmp(argv[i],"-Yl")==0){
				YJgen.SetL(atof(argv[i+1]));
			}
			else if( strcmp(argv[i],"-Tl")==0){
                TSgen.SetL(atof(argv[i+1]));
            }
			else if( strcmp(argv[i],"-lows")==0){
                if(atoi(argv[i+1])!=0&&atoi(argv[i+1])!=1) usage();
				set_LowESpectrum(TSgen,atoi(argv[i+1]));
                set_LowESpectrum(YJgen,atoi(argv[i+1]));
            }
			else if( strcmp(argv[i],"-mids")==0){
				int val = atoi(argv[i+1]);
				if(val ==0||val==1||val==2){
					TSgen.SetMidESpectrum(val);	
					YJgen.SetMidESpectrum(val);	
				}
				else usage();
            } 
			else if( strcmp(argv[i],"-sin2_th12")==0){
				TSgen.Setsin2_th12(atof(argv[i+1]));
				YJgen.Setsin2_th12(atof(argv[i+1]));
            }
			else if( strcmp(argv[i],"-sin2_th13")==0){
                TSgen.Setsin2_th13(atof(argv[i+1]));
                YJgen.Setsin2_th13(atof(argv[i+1]));
            }
			else if( strcmp(argv[i],"-Dm2")==0){
                TSgen.SetDm2(atof(argv[i+1]),atof(argv[i+2]));
                YJgen.SetDm2(atof(argv[i+1]),atof(argv[i+2]));
				i++;
            }
        	else if( strcmp(argv[i],"-m")==0){
        	    set_MassOrder(TSgen,atoi(argv[i+1]));
        	    set_MassOrder(YJgen,atoi(argv[i+1]));
        	}
        	else if( strcmp(argv[i],"-ff")==0){
        	    set_FissionFraction(TSgen,atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]));
        	    set_FissionFraction(YJgen,atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]));
				i=i+3;
        	}
			else if( strcmp(argv[i],"-yff")==0){
        	    set_FissionFraction(YJgen,atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]));
        	    i=i+3;
        	}	
        	else if( strcmp(argv[i],"-tff")==0){
        	    set_FissionFraction(TSgen,atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]));
        	    i=i+3;
        	}
        	else if( strcmp(argv[i],"-rng")==0){
				if(atoi(argv[i+1])==1) ReactorES = false;
				else usage();
			}
			else if( strcmp(argv[i],"-debug")==0){                                                                                             
                if(atoi(argv[i+1])==1) Debug = true;
                else usage();
            }
			else{
				
    			//std::cerr<<"!!!!!read the usage, please only input the options you need to change!!!!!"<<std::endl;
				usage();
				return 0; 
 			}
		}
	}
	FILE * stream = stdout;
	if(outputName!=NULL){
		stream = fopen(outputName, "w");
		if(stream ==NULL){
			printf("Please enetr a valid filename.\n");
	    	usage();
    		exit(0);
    	}
	}

	double electron_kinetic_energy;
	double px,py,pz;
	double cos_theta;
//---------debug histogram
	TH1D* hpx=0;
	TH1D* hpy=0;
	TH1D* hpz=0;
	TH1D* hTe=0;
	TH1D* hEnu=0;
	//TCanvas *c0 = new TCanvas("c0","c0",850,600);
	if(Debug){
		if(ReactorES){
			hpx = new TH1D("hpx","px of recoiled electrons",100,-6,6);//MeV
			hpy = new TH1D("hpy","py of recoiled electrons",100,-6,6);//MeV
			hpz = new TH1D("hpz","pz of recoiled electrons",100,-6,6);//MeV
			hTe = new TH1D("hTe","T of recoiled electrons",200,0,10);//MeV
		}else hEnu = new TH1D("hEnu","Energy of Reactor antinuetrino",200,0,10);// MeV
	}
//------------------------------------------------debug---------------------------------
	//double Enu; 
	//TFile *output = TFile::Open("spectrumi.root","RECREATE");
    //TTree *tree = new TTree("ReactorES","ReactorES generator tree");
    //tree->Branch("Enu",&Enu,"Enu/D");
//-------------------------------------------
	//double pi = 3.1415926;
	//int YJEvents = int(nEvent * ratio);
	gRandom->SetSeed(seed);
	if(ReactorES){
		for(int i = 0; i<nEvent;i++){
			double val = gRandom->Uniform(0.,2.0*M_PI);
			double sin_phi=sin(val);
			double cos_phi=cos(val);
			double val1 = gRandom->Uniform(0,1);
			double sin_a,cos_a;
			if(val1<ratio) {
				YJgen.GeneratePrimaries(electron_kinetic_energy,cos_theta);
				sin_a= -0.059;
				cos_a= 0.998;
			}
			else {
				TSgen.GeneratePrimaries(electron_kinetic_energy,cos_theta);
				sin_a=-0.990;
				cos_a=-0.143;
			}
			double sin_theta = sqrt(1-cos_theta*cos_theta);
			px = cos_theta*sin_a - sin_theta*sin_phi*cos_a;
			py = cos_theta*cos_a + sin_theta*sin_phi*sin_a;
			pz = sin_theta*cos_phi;
			electron_kinetic_energy *= MeV;
			double emass = 0.510998928*MeV;
    		double electron_momentum = sqrt(pow(electron_kinetic_energy+emass,2)-emass*emass);
			//double Ee = electron_kinetic_energy+emass;
 			px *= electron_momentum;
			py *= electron_momentum;
			pz *= electron_momentum;
			fprintf(stream, "1\n");
			fprintf(stream, "1\t11 0 0 %e %e %e %f\n",px/GeV,py/GeV,pz/GeV,emass/GeV); 

			//------------for debug------------
			if(Debug){
				hpx->Fill(px/MeV);
				hpy->Fill(py/MeV);
				hpz->Fill(pz/MeV);
				hTe->Fill(electron_kinetic_energy/MeV);
			}
		}
		//------------draw debug hist------------
		if(Debug){
			TCanvas *c0 = new TCanvas("c0","c0",850,600);
			c0->Divide(2,2);
			c0->cd(1);
			hTe->Draw();
			c0->cd(2);
			hpx->Draw();
			c0->cd(3);
            hpy->Draw();
            c0->cd(4);
            hpz->Draw();
			hTe->SetXTitle("Te [MeV]");
			hpx->SetXTitle("px [MeV]");
			hpy->SetXTitle("py [MeV]");
			hpz->SetXTitle("pz [MeV]");
		}
	}else{
		TFile *output;
    	if(outputName==NULL) output = TFile::Open("reactorNu.root","RECREATE");
		else    output=TFile::Open(outputName,"RECREATE");                                                               
   		TTree *tree = new TTree("Reactor Neutrino","Reactor generator tree");
		double Enu; 
    	tree->Branch("Enu",&Enu,"Enu/D");
   		
		double a,b;
		YJgen.GeneratePrimaries(a,b);
		for(int i=0;i<nEvent;i++){
			Enu=YJgen.ShootEnergy();
			tree->Fill();		
			if(Debug){
				hEnu->Fill(Enu);
			}
		}
		if(Debug){
			TCanvas *c0 = new TCanvas("c0","c0",850,600);
			c0->cd();
			hEnu->Draw();
			hEnu->SetXTitle("antineu Energy [MeV]");
			c0->Update();
		} 
		tree->Write();
		output->Close();
	}	
	if(Debug) theApp.Run();
	return 0;
} 
