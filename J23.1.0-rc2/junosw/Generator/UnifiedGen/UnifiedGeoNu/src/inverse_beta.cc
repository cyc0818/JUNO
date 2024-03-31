//This is a generator program to throw angularly correlated positron and 
//neutron pairs based on the reactor neutrino energy spectrum and 
//the inverse-beta cross-section. I must give credit to C. Jilling, since
//this code is basically a hybrid of his Co60 generator and his 
//theta_13 sensitivity code ...
//Jianglai Liu 07/14/2006

#include <stdio.h>
#include <iostream>
//#include <CLHEP/config/CLHEP.h>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "CommonXSec/IBDXSec.hh"
#include "KRLGeoFlux.hh"
//#include "NuOscillations.hh" // for geo no such Oscillations
#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TF2.h>
#include <TRandom.h>

using namespace std;
using namespace CLHEP;

struct Params;
void ProcessArgs(int argc, char** argv, Params& params);
void Usage();

Double_t neutrino_spectrum(Double_t* x, Double_t* par)
{
  Double_t engInMeV = x[0];
  Double_t value;
  
  value = gGeoFlux->FluxGeo(engInMeV);
  return value;
}

Double_t inverse_beta_prob(Double_t* x, Double_t* par)
{
  Double_t engInMeV = x[0];
  Double_t cosThEL = x[1];
  // std::cout << "cosThEL: " << cosThEL 
  //          << " dsigma/dcostheta: " << gInverseBeta->DSigDCosTh(engInMeV, cosThEL) 
  //          << " Bool_t(par[0]): " << Bool_t(par[0])
  //          << std::endl;
  Bool_t oscOn = Bool_t(par[0]);
  Double_t prob;
  //do NOT need to multiply by the jacobian sin(theta) here!
    prob = gGeoFlux->FluxGeo(engInMeV)
        *gInverseBeta->DSigmaDcos(engInMeV, cosThEL);
           //* 0.54; // geo oscillations 0.54

  if(oscOn) {
    // std::cout << __func__ << ": oscOn. before: " << prob;
   // prob *= gNuOscillations->SurvivalProb(engInMeV);
    prob *= 0.54;// for geo neutrino
    // std::cout << " after: " << prob << std::endl;
  }
  return prob;
}
  
Double_t inverse_beta_flatProb(Double_t* x, Double_t* par)
{
  Double_t engInMeV = x[0];
  Double_t cosThEL = x[1];   
  Double_t prob; 
  // TODO Add DSigDCosThAt4MeV
  prob = 0.;
  if (0<=engInMeV and engInMeV <= 12.) {
    engInMeV = 4.0; // 4MeV
    prob = 2.23341e+19*gInverseBeta->DSigmaDcos(engInMeV, cosThEL);  
    // prob = 2.23341e+19*gInverseBeta->DSigDCosThAt4MeV(engInMeV, cosThEL);  
  }

  return prob; 
}

void GenerateInverseBetaPrimaries(TF2* invBetaProb, 
				  const double nu_angle_wrt_y_in_deg,
				  Hep3Vector& pnu,
				  Hep3Vector& p1, Hep3Vector& p2)
{
  //generate neutrino energy and outgoing electron cos angle
  Double_t aEngNu(0), aCosThEl(0);

NewRun:

  invBetaProb->GetRandom2(aEngNu, aCosThEl);

  //cout<<aEngNu<<" : "<<acos(aCosThEl)*360./M_2PI<<endl;
  
  //at this point, assume the incident nu momentum along z

  //calculate positron energy in MeV
  Double_t Ee = gInverseBeta->GetE_e(aEngNu, aCosThEl);

  //generate positron phi angle
  Double_t aPhiEl = gRandom->Uniform(0.,2.0*M_PI);
  Double_t kMe = electron_mass_c2/MeV;
  Double_t Pe;
  if(Ee*Ee-kMe*kMe>0){  
     Pe = sqrt(Ee*Ee-kMe*kMe);}
  else{goto NewRun;}


  p1.setRThetaPhi(Pe*MeV, acos(aCosThEl), aPhiEl);

  //now use the 3-momentum conservation to set neutron recoil 
  //momentum
  pnu.setRThetaPhi(aEngNu*MeV,0,0);
  p2 = pnu-p1;
  
  //now make the vector rotation. first rotate around x by 90 degrees
  //so that +z axis is now +y axis
  //pnu.rotateX(M_PI/2.);
  //p1.rotateX(M_PI/2.);
  //p2.rotateX(M_PI/2.);

  //To get specific theta phi
  Double_t rTheta;
  Double_t rPhi;
  rTheta = M_PI/6;
  rPhi = M_PI/3;
  //rTheta = acos(gRandom->Uniform(-1, 1));
  //rPhi = gRandom->Uniform(0.,2.0*M_PI);
  pnu.rotateY(rTheta);
  p1.rotateY(rTheta);
  p2.rotateY(rTheta);
  pnu.rotateZ(rPhi);
  p1.rotateZ(rPhi);
  p2.rotateZ(rPhi);

  
  //if nu_angle_wrt_y is non-zero, rotate again
  if(nu_angle_wrt_y_in_deg!=0) {
    pnu.rotateX(2*M_PI/360.*nu_angle_wrt_y_in_deg);
    p1.rotateX(2*M_PI/360.*nu_angle_wrt_y_in_deg);
    p2.rotateX(2*M_PI/360.*nu_angle_wrt_y_in_deg);  
  }
  return;
}

struct Params {

  Params() {
    rseed = 0;
    eplus_only = 0;
    neutron_only = 0;
    debug = 0;
    nu_angle_wrt_y_in_deg = 0;
    normal_hierarchy = true;
    oscillations = false;
    base_line_m = -1.e13;
    sin2_th12 = -1.e13;
    sin2_th13 = -1.e13;
    Dm2_21 = -1.e13;
    absDm2_31 = -1.e13;

    outFilename = NULL;
    nEvents = 1000000000; // a billion. Default to something big for piping 
    iGeo=1;
    flat = 0;
  }
  long rseed;
  unsigned int eplus_only;
  unsigned int neutron_only;
  unsigned int debug;
  double nu_angle_wrt_y_in_deg;
  bool normal_hierarchy;
  bool oscillations;
  double base_line_m;
  double sin2_th12;
  double sin2_th13;
  double Dm2_21;
  double absDm2_31;
  int iGeo;
  char* outFilename;
  unsigned long int nEvents; // a billion. Default to something big for piping 
  unsigned int flat;
  std::string flux; // geo neutrino flux
  std::string reactor; // world reactor file
};

int main(int argc, char** argv) {
  TApplication theApp("theApp",0,0);
  gROOT->SetStyle("Plain");

  Params params;
  ProcessArgs(argc, argv, params);


  if (params.flux.empty()) {
    const char* package_path = getenv("GEONUROOT");
    if (!package_path) {
        std::cerr << "Please setup GeoNu package first. " << std::endl;
        std::cerr << "Otherwise, please specify flux file path. " << std::endl;
    }
    std::string path = package_path;
    params.flux = path + "/data/Totdata";
  }
  if (params.reactor.empty()) {
    const char* package_path = getenv("GEONUROOT");
    if (!package_path) {
        std::cerr << "Please setup GeoNu package first. " << std::endl;
        std::cerr << "Otherwise, please specify flux file path. " << std::endl;
    }
    std::string path = package_path;
    params.reactor = path + "/data/WorldReactor.root";
  }


  gGeoFlux = new KRLGeoFlux(params.flux, params.reactor); // defaults to Petr's fit
//  gNuOscillations = new NuOscillations(params.normal_hierarchy); // geo no
  gInverseBeta = new IBDXSec();
  
  gGeoFlux->SetiGeocase(params.iGeo);  

/* // geo no
  if(params.base_line_m != -1.e13) gNuOscillations->SetBaseLine_m(params.base_line_m);
  if(params.sin2_th12 != -1.e13) gNuOscillations->SetSin2Th12(params.sin2_th12);
  if(params.sin2_th13 != -1.e13) gNuOscillations->SetSin2Th13(params.sin2_th13);
  if(params.Dm2_21 != -1.e13) gNuOscillations->SetDm2_21(params.Dm2_21);
  if(params.absDm2_31 != -1.e13) gNuOscillations->SetAbsDm2_31(params.absDm2_31);
*/
  //some dumb tests. Please do not remove
  if(params.debug) {
    TF1 *fflux = new TF1("engspec",neutrino_spectrum,
			 0.1, 10, 0);
    TF1 *ftotsig = new TF1("sigtot", gKRLSigmaTotal, 0.1, 10, 0);  
    
    TF1 *ftotprob = 0;
    if (params.flat == 1) {
        ftotprob = new TF1("totalprob", inverse_beta_flatProb, 0.1, 10, 0);
    } else {
        ftotprob = new TF1("totalprob", inverse_beta_prob, 0.1, 10, 1);
        if(params.oscillations) {
          std::cout << "enable oscillations. " << std::endl;
          ftotprob->SetParameter(0, 1.);
        } else {
          ftotprob->SetParameter(0, 0.);
        }
    }
    
    TCanvas *c1 = new TCanvas("c1","c1",400,600);
    c1->Divide(1,3); 
    c1->cd(1); ftotsig->SetNpx(1000); ftotsig->Draw();
    c1->cd(2); fflux->SetNpx(1000); fflux->Draw();
    c1->cd(3); 
    // draw extra normal 
    if (!params.flat && params.oscillations) {
      TF1* ftotprob_normal = new TF1("totalprob2", inverse_beta_prob, 0.1, 10, 1);
      ftotprob_normal->SetParameter(0, 0.);
      ftotprob_normal->SetNpx(1000);
      ftotprob_normal->SetLineStyle(2);
      ftotprob_normal->Draw();
      ftotprob->SetNpx(1000); ftotprob->Draw("SAME");
    } else {
      ftotprob->SetNpx(1000); ftotprob->Draw();
    }
  }
  //if(params.debug) theApp.Run();
  
  FILE* stream = stdout;
  if( params.outFilename!=NULL ) {
    stream = fopen(params.outFilename, "w");
    if( stream==NULL ) {
      printf("Please enetr a valid filename.\n");
      Usage();
      exit(0);
    }
  }

  //now create a TF2 object to calculate the reaction probability
  //given the neutrino energy and the inverse beta probability
  //x: eNu in MeV
  //y: cos(th_e)
  cout<<"Creating inverse-beta reaction PDF ..."<<endl;
  TF2 *funcInvBetaProb = 0;
  if (params.flat == 1) {
    funcInvBetaProb = new TF2("InvBeta",inverse_beta_flatProb,
                              0.01,15.0,-1.,1., 0); //"0" parameters
  } else {
    funcInvBetaProb = new TF2("InvBeta",inverse_beta_prob,
                              0.01,15.0,-1.,1., 1); //"1" parameter
    if(params.oscillations)
      funcInvBetaProb->SetParameter(0, 1.);
    else
      funcInvBetaProb->SetParameter(0, 0.);
  }
  //set the bins on x and y to help generate pseudo random numbers
  //according to the reaction probabilities
  funcInvBetaProb->SetNpx(1000);//energy
  funcInvBetaProb->SetNpy(1000);//costh

  //debugging histogram
  TH1F *hNeutrinoEng =0;
  TH1F *hPositronEng =0;
  if(params.debug) {
    hNeutrinoEng = new TH1F("hNeutrinoEng","Neutrino Energy Spectrum",
			    600, 0, 60); //MeV
    hPositronEng = new TH1F("hPositronEng","Position Energy Spectrum",
			    600, 0, 60); //MeV
  }
  
  gRandom->SetSeed(params.rseed);
  //start by printing some information to comment lines
  fprintf(stream, "# File generated by %s.\n", argv[0]);
  fprintf(stream, "# Random seed for generator = %ld.\n", params.rseed);
  unsigned int i;
  
  Hep3Vector pNeutrino; //incoming neutrino momentum
  Hep3Vector pPositron, pNeutron; // the e+ and n momentum vectors. Unit GeV/c
  for( i=0 ; i<params.nEvents ; i++ ) {
    //if(i%1000==0&&i>0) cout<<i<<" events processed"<<endl;
    GenerateInverseBetaPrimaries(funcInvBetaProb, params.nu_angle_wrt_y_in_deg, 
				 pNeutrino, 
				 pPositron, pNeutron);
    if(!params.eplus_only&&!params.neutron_only){//both neutron and positron
      fprintf(stream, "3\n"); //3 particles from this vertex
      //positron is -11, neutron is 2112, anti-nu_e is -12
      //neutrino is "informational particle, start with isthep>1!
      fprintf(stream, "2\t-12 0 0 %e %e %e %d\n", pNeutrino.x()/GeV, 
	      pNeutrino.y()/GeV, pNeutrino.z()/GeV,0);      
      fprintf(stream, "1\t-11 0 0 %e %e %e %e\n", pPositron.x()/GeV, 
	      pPositron.y()/GeV, pPositron.z()/GeV,electron_mass_c2/GeV);     
      fprintf(stream, "1\t2112 0 0 %e %e %e %e\n", pNeutron.x()/GeV, 
	      pNeutron.y()/GeV, pNeutron.z()/GeV, neutron_mass_c2/GeV);
    } else if(params.eplus_only) {
      fprintf(stream, "2\n"); //2 particle from this vertex, anti_v and e+
      fprintf(stream, "2\t-12 0 0 %e %e %e %d\n", pNeutrino.x()/GeV, 
	      pNeutrino.y()/GeV, pNeutrino.z()/GeV,0);
      fprintf(stream, "1\t-11 0 0 %e %e %e %e\n", pPositron.x()/GeV, 
	      pPositron.y()/GeV, pPositron.z()/GeV,electron_mass_c2/GeV); 

     }
       else if(params.neutron_only) {
       fprintf(stream, "2\n"); //2 particle from this vertex, anti_v and neutron
       fprintf(stream, "2\t-12 0 0 %e %e %e %d\n", pNeutrino.x()/GeV, 
	       pNeutrino.y()/GeV, pNeutrino.z()/GeV,0);
       fprintf(stream, "1\t2112 0 0 %e %e %e %e\n", pNeutron.x()/GeV, 
	       pNeutron.y()/GeV, pNeutron.z()/GeV, neutron_mass_c2/GeV);
    } else {
      printf("Unsupported switch option! Quit now!\n");
      exit(1);
    }


    //for debugging
    Double_t neutrinoEng = pNeutrino.mag()/MeV;
    Double_t positronMom = pPositron.mag()/MeV;
    Double_t positronEng = sqrt(positronMom*positronMom
				+electron_mass_c2/MeV*electron_mass_c2/MeV);
    
    if(params.debug) hNeutrinoEng->Fill(neutrinoEng);
    if(params.debug) hPositronEng->Fill(positronEng);
  }

  if(params.debug) {
    TCanvas *c2 = new TCanvas("c2","c2",600,400);
    hPositronEng->Draw();
    c2->Update();
    TCanvas *c3 = new TCanvas("c3","c3",600,400);
    hNeutrinoEng->Draw();
    c3->Update();
  }

  if(params.debug) theApp.Run();
  return 0;
}


void ProcessArgs(int argc, char** argv, Params& params) 
{
  int i;
  for( i=1 ; i<argc ; i++ ) {
    if( strcmp(argv[i], "-seed")==0 ) {
      i++;
      sscanf(argv[i], "%ld", &params.rseed);
    } else if( strcmp(argv[i], "-o")==0 ) {
      i++;
      params.outFilename = new char[strlen(argv[i]) +1];
      strcpy(params.outFilename, argv[i]);
    } else if( strcmp(argv[i], "-n")==0 ) { 
      i++;
      sscanf(argv[i], "%lud", &params.nEvents);
    } else if( strcmp(argv[i], "-angle")==0 ) {
      i++;
      sscanf(argv[i], "%lg", &params.nu_angle_wrt_y_in_deg);
    } else if( strcmp(argv[i], "-eplus_only")==0 ){
      params.eplus_only = 1;
    } else if( strcmp(argv[i], "-neutron_only")==0 ){
      params.neutron_only = 1;
    } else if( strcmp(argv[i], "-NH")==0 ) {
      params.normal_hierarchy = true;
      params.oscillations = true;
    } else if( strcmp(argv[i], "-IH")==0 ) {
      params.normal_hierarchy = false;
      params.oscillations = true;
    } else if( strcmp(argv[i], "-osc")==0 ) {
      params.normal_hierarchy = true;
      params.oscillations = true;
    } else if( strcmp(argv[i], "-base_line_m")==0) {
      i++;
      sscanf(argv[i], "%lf", &params.base_line_m);
      params.oscillations = true;
    } else if ( strcmp(argv[i], "-sin2_th12")==0) {
      i++;
      sscanf(argv[i], "%lf", &params.sin2_th12);
      params.oscillations = true;
    } else if ( strcmp(argv[i], "-sin2_th13")==0) {
      i++;
      sscanf(argv[i], "%lf", &params.sin2_th13);
      params.oscillations = true;
    } else if ( strcmp(argv[i], "-Dm2_21")==0) {
      i++;
      sscanf(argv[i], "%lf", &params.Dm2_21);
      params.oscillations = true;
    } else if ( strcmp(argv[i], "-absDm2_31")==0) {
      i++;
      sscanf(argv[i], "%lf", &params.absDm2_31);
      params.oscillations = true;
    } else if ( strcmp(argv[i], "-debug")==0 ) {
      params.debug = 1;
    } else if ( strcmp(argv[i], "-flat")==0 ) {
      params.flat = 1;
    }else if ( strcmp(argv[i], "-GeoUTh")==0 ) {
      i++;
      sscanf(argv[i],"%1d",&params.iGeo);
    }else if ( strcmp(argv[i], "-flux")==0 ) {
      i++;
      params.flux = argv[i];
    }else if ( strcmp(argv[i], "-reactor")==0 ) {
      i++;
      params.reactor = argv[i];
    }else if (strcmp(argv[i],"-h")==0) {//helper
      i++;
      Usage();
      exit(0);
    } else { //unsupported option
      Usage();
      exit(0);
    }
  }
}

void Usage() {
  printf("InverseBeta.exe [-seed seed] [-o outputfilename] [-n nevents] [-angle neutrino_angle_in_deg] [-eplus_only] [-neutron_only] [-osc] [-NH] [-IH] [-base_line_m] [-sin2_th12] [-sin2_th13] [-Dm2_21] [-absDm2_31] [-debug] [-flat] [-GeoUTh iGeo] [-flux file] [-reactor file] \n");
}

