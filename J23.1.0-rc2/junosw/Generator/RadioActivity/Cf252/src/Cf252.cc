/// The style of Cf generators is standalone.
/// \author Jinchang Liu, Jilei Xu. 
/// liujinc@mail.ihep.ac.cn, Feb 24, 2008
///
/// Modified by Qing He, to support seed
/// qinghe@princeton.edu, 08 Jan., 2009

//copy from dayabay
//Modified by Zhang Feiyang

#include <iostream>
#include <fstream>
#include <vector>

#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Vector/TwoVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Random/Randomize.h>

#include <TROOT.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1.h>
#include <TF1.h>

#include "ParticleInformation.hh"

#include <cstdlib>

using namespace std;
using namespace CLHEP;

void Usage();
void ProcessArgs(int argc, char** argv, long* rseed, char** outfilename,
                 int* nEvents, char** whichpar, double *posx, 
                 double *posy, double *posz);

ParticleInformation ParInf;
void ParticleMomentum();
double *ParticlePosition();

TH1F  * capspec;
TH1F  * gammaspec;
TH1F  * gammaNN;

char* whichPar = NULL;
double Posx=0.,Posy=0.,Posz=0.;

int main(int argc, char** argv)
{ 
  long rseed = 1000000000;
  const int n=35;
  float x[n],y[n],z[n],px[n],py[n],pz[n],energy[n];
  int number,particleID[n];
  int EventType=4;
  
  char* outFilename = NULL;
  int nEvents = 0; 
  
  ProcessArgs(argc, argv, &rseed, &outFilename, &nEvents, &whichPar, &Posx, &Posy, &Posz);

  if(argc==1) {
    Usage();
    exit(0);
  }
  
  HepRandom::setTheSeed(rseed);
  //start by printing some information to comment lines
  cout << "# Ransom seed for generator = " << rseed << endl;
  cout << "# Events = " << nEvents << endl;

  TFile * specfile;
  TFile * gammafile;

  std::string fname;
  std::string fname1, fname2;
  const char* prefix = getenv("CF252ROOT");
  if (prefix != 0) {
    fname = prefix;
    fname1 = fname + "/data/Cfgamma.root";
    fname2 = fname + "/data/Cfneutron.root";
  }
  
  // get gamma data
  gammafile=TFile::Open(fname1.c_str());
  if(!(gammafile->IsOpen()))
    cout<<"Can not open gdcap_spec.root. Make sure it is in the current directory."<<endl;
  gammaspec = (TH1F*)gammafile ->Get("Cfgamma");
  gammaNN = (TH1F*)gammafile ->Get("CfgammaM");
  // get neutron data
  specfile = TFile::Open(fname2.c_str());
  if(!(specfile->IsOpen()))
    cout<<"Can not open rootfile. Make sure it is in the current directory."<<endl;
  capspec = (TH1F*)specfile ->Get("h1");
  // define t tree
  TFile* tfile_pointer =nullptr;
  TTree* t = nullptr;
  if (outFilename) {

      tfile_pointer = new TFile(Form("%s.root",outFilename),"recreate");
      t=new TTree("t","Cf source Generator");
      t->Branch("EventType",&EventType,"EventType/I");
      t->Branch("ParticleNumber", &number, "number/I");
      t->Branch("ParticleID",particleID,"particleID[number]/I");
      t->Branch("ParticleEnergy", energy, "energy[number]/F");
      t->Branch("ParticleMomentumDirection_x", px, "px[number]/F");
      t->Branch("ParticleMomentumDirection_y", py, "px[number]/F");
      t->Branch("ParticleMomentumDirection_z", pz, "pz[number]/F");
      t->Branch("ParticlePosition_x", x, "x[number]/F");
      t->Branch("ParticlePosition_y", y, "y[number]/F");
      t->Branch("ParticlePosition_z", z, "z[number]/F");
  }
  //Fill tree        

  FILE* stream = stdout;
  if( outFilename!=NULL ) {
    stream = fopen(outFilename, "w");
    if( stream==NULL ) {
      printf("Please enter a valid filename.\n");
      Usage();
      exit(0);
    }
  }
  for(Int_t i=0;i<nEvents;i++)
  { 
    ParticleMomentum();
    number=0;
    EventType=1;
    for(Int_t j=0;j<ParInf.GetNumber();j++)
    {
      particleID[number]=ParInf.GetPDG(j);
      x[number]=ParInf.GetX(j);
      y[number]=ParInf.GetY(j);
      z[number]=ParInf.GetZ(j);
      px[number]=ParInf.GetMomentum_x(j);
      py[number]=ParInf.GetMomentum_y(j);
      pz[number]=ParInf.GetMomentum_z(j);
      energy[number]=ParInf.GetEnergy(j);     
      number++;
      EventType=0;
    if(particleID[number]==2112){
    fprintf(stream, "1\n");
    fprintf(stream, "1\t%d 0 0 %e %e %e %e\n",particleID[number], px[number]/GeV, py[number]/GeV, pz[number]/GeV ,939.565378/GeV);}
    else if(particleID[number]==22){
    fprintf(stream, "1\n");
    fprintf(stream, "1\t%d 0 0 %e %e %e 0\n",particleID[number], px[number]/GeV, py[number]/GeV, pz[number]/GeV);}
    }
    if (t) t->Fill();
    ParInf.Resize();
    if(i%10000==0) cout<<"# event="<<i<<endl;
  }
  if (t) t->Write();
  if (tfile_pointer) tfile_pointer->Close();
  specfile->Close();
  gammafile->Close();
	
}

void ParticleMomentum()
{
  int Selected;
  double rndm,*pp;
  double gam_E,neu_E,cos_theta,sin_theta,phi,TWO_PI,p[3];
  int PDG,evtType;
  int neutronNum,gammaNum;
  TWO_PI=6.283185308;
  Selected=0;
  evtType=Selected;
  ParInf.SetEventType(evtType);
  pp=ParticlePosition();
  neutronNum=0;
  gammaNum=0;

  // neutron
//  if(strcmp(whichPar, "Neu")==0 || strcmp(whichPar, "Both")==0) 
  {  
    //    rndm=gRandom->Gaus(3.735,1.252996409);    
    rndm=RandGauss::shoot(3.735,1.252996409);    
    neutronNum= (int)(rndm+0.5);
    if(neutronNum<0)  neutronNum=0;
    if(neutronNum>10)  neutronNum=10; 
    for(Int_t i= 0;i<neutronNum;i++) {
      // get energy
      neu_E =capspec->GetRandom();  // MeV
      PDG=2112;

      // generate directions. phi and cos(theta) are chosen randomly.
      //phi = gRandom->Uniform()*TWO_PI;
      //cos_theta = gRandom->Uniform()*2.0 - 1.0;
      phi = RandFlat::shoot(0.,1.)*TWO_PI;
      cos_theta = RandFlat::shoot(0.,1.)*2.0 - 1.0;
      sin_theta = sqrt(1.0 - pow(cos_theta,2));
      
      p[0] = neu_E * sin_theta * sin(phi);
      p[1] = neu_E * sin_theta * cos(phi);
      p[2] = neu_E * cos_theta;
      ParInf.SetPosition(*pp,*(pp+1),*(pp+2));
      ParInf.SetMomentum(p[0],p[1],p[2]);
      ParInf.SetEnergy(neu_E);
      ParInf.SetPDG(PDG);
    }
  }
//
//  // gamma
//  if( strcmp(whichPar, "Gam")==0 || strcmp(whichPar, "Both")==0) 
  {
    gammaNum=(int)(gammaNN->GetRandom()+0.5);
    for(Int_t i= 0;i<gammaNum;i++)
      // get gamma energy
    { gam_E =gammaspec->GetRandom();  //MeV
    PDG=22;
    // generate directions. phi and cos(theta) are chosen randomly.       
    //    phi = gRandom->Uniform()*TWO_PI;
    //cos_theta = gRandom->Uniform()*2.0 - 1.0;
    phi = RandFlat::shoot(0.,1.)*TWO_PI;
    cos_theta = RandFlat::shoot(0.,1.)*2.0 - 1.0;
    sin_theta = sqrt(1.0 - pow(cos_theta,2)); 
    p[0] = gam_E * sin_theta * sin(phi);
    p[1] = gam_E * sin_theta * cos(phi);
    p[2] = gam_E * cos_theta;
    ParInf.SetPosition(*pp,*(pp+1),*(pp+2));
    ParInf.SetMomentum(p[0],p[1],p[2]);
    ParInf.SetEnergy(gam_E);
    ParInf.SetPDG(PDG);       
    }
  }
//  else if(!(strcmp(whichPar, "Neu")==0 )){
//    cout << " Cf252: *** ERROR *** Invalid whichPar! " << whichPar
//         << " selected. Valid site names are Neu, Gam, Both "
//         << endl;
//    assert(0); // this will end the job
//  }
  ParInf.SetNumber(neutronNum+gammaNum);
}

double *ParticlePosition()
{ 
  static double vg[3];
  vg[0]=Posx;
  vg[1]=Posy;
  vg[2]=Posz;
  return vg;
}

void ProcessArgs(int argc, char** argv, long* rseed, char** outfilename, 
                 int* nEvents, char** whichpar, double *posx, 
                 double *posy, double *posz) 
{
  int i;
  for( i=1 ; i<argc ; i++ ) 
  {
    if( strcmp(argv[i], "-seed")==0 ) {
      i++;
      sscanf(argv[i], "%ld", rseed);
    } 
    else if( strcmp(argv[i], "-o")==0 ) {
      i++;
      *outfilename = new char[strlen(argv[i]) +1];
      strcpy(*outfilename, argv[i]);
    } 
    else if( strcmp(argv[i], "-n")==0 ) {
      i++;
      sscanf(argv[i], "%d", nEvents);
    } 
    else if( strcmp(argv[i], "-w")==0 ) {
      i++;
      *whichpar = new char[strlen(argv[i]) + 1];
      strcpy(*whichpar, argv[i]);
    }
    else if ( strcmp(argv[i], "-p")==0) {
      i++;
      sscanf(argv[i], "%lf,%lf,%lf", posx, posy, posz);
    }
    else {
      Usage();
      exit(0);
    }
  }
}

void Usage()
{
  cout << " Cf252 [-seed seed] [-o outputfilename] [-n nEvents] "
       << " [-w whichPar (Neu, Gam, Both)] [-p posx, posy,posz (cm)] "
       << endl;
}

