/// This is a simple program to generate the cosmic muons that go through the 
/// water Cherenkov detector in Daya Bay lab. The muon flux of the Daya Bay 
/// site is simulated via MUSIC by Daya Bay collaborator. We use the muon flux 
/// to generate the muon events in the water Cherenkov detector. To run this
/// program, the muon flux data file is needed.
/// 
/// update, change command: -s juno, dyb2 will not be used
///         the Dimension(50m*50m*55m) will cover all detectors. by Xu Jilei 2014/3/18
// Add the multiple muon events for JUNO simulation. // by Haoqi 2014.10.14

// rotate muon sample, let them match the detector coordinate, which Z up, x point to electronics room.
// Added single or multy muon option.   By Jilei. 2014/11/22
// Added detector volume selecting option, -v TT (muon only tracks through TT det), WP (muon tracks through TT + water pool det). 2014/11/27 Jilei
// Add an option "-s juno" to choose the new baseline (70m moved), "-s juno_original" to choose the previous juno baseline. 2017/5/22 Jilei
// Fly time correction for muon bundle and add one option of muon track number selection. E.g. "-ntrks 2" means only select double muon events. 2021/4/25 Diru Wu.
//Rotated muon phi angle 90deg anticlockwise (seeing from +z to -z), result in dphi from -56.7deg into 33.3deg, refering the new Coordinate system (JUNO-doc-4599) to match the real geometry direction. By Jilei, Dec. 16, 2021.
    
#include <iostream>
#include <fstream>
#include <cassert> 
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <TROOT.h>
#include <TRandom.h>
#include <vector>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <cstdlib>
#include <TChain.h>
#include <cmath>
#include "TVector3.h"
using namespace std;
using namespace CLHEP;

void Usage();

int judgement(double x, double y, double z, double px, double py, double pz) {
    int res = 0;
    TVector3 dir(px, py, pz);
    dir = dir.Unit();
    TVector2 dir2(dir.X(), dir.Y());
    TVector3 pos(x, y, z);
    TVector2 pos2(x, y);
    double dis2 = TMath::Sqrt(pos2.Mod2() - TMath::Power(pos2 * dir2, 2));
    if (dis2 < 21750) {
        double halfStr = TMath::Sqrt(21750 * 21750 - dis2 * dis2);
        TVector2 mid2 = pos2 - dir2 * (pos2 * dir2);
        TVector2 cro21 = mid2 - dir2 * halfStr;
        TVector2 cro22 = mid2 + dir2 * halfStr;
        double z1 = dir.Z() * (cro21.X() - pos.X()) / dir.X() + pos.Z();
        double z2 = dir.Z() * (cro22.X() - pos.X()) / dir.X() + pos.Z();
        if (!(z1 - 21750 > 0 && z2 - 21750 > 0 ||
            z1 + 21750 < 0 && z2 + 21750 < 0))
            res += 1;
    }
    double dis3 = TMath::Sqrt(pos.Mag2() - TMath::Power(pos * dir, 2));
    if (dis3 < 17700) {
        if (TMath::Sqrt(px * px + py * py + pz * pz) > 20)
            res += 10;
    }
    else {
        if (dis3 < 20000) {
            res += 100;
        }
    }
    return res;
}

void ProcessArgs(int argc, char** argv, long* rseed, char** outfilename,
                 int* nevents, char** whichsite, char** volume, char** rotation, int* ismult, 
                 char** music_dir, int* nTrks, int* wpo, int* stop, int* cdWaterBuffer);



const Double_t GetMuPlusMinusRatio(TH1F* hlookup, const Double_t momInGeV)
{
  Int_t binno = hlookup->FindBin(momInGeV);
  //return the measured value if within range. otherwise return 1.4
  if(binno>=1&&binno<=hlookup->GetNbinsX()) 
    return hlookup->GetBinContent(binno);
  else return 1.4;
}

//Double_t RatioFunc()
const Double_t GetMuPlusMinusRatio(const Double_t momInGeV)
{ 
  Double_t RESULT=0;
  Double_t par[2]={0.5505,0.6745};
  Double_t part1=par[0]/(1+(1.1*momInGeV+1)/115);
  Double_t part2=0.054*par[1]/(1+(1.1*momInGeV+1)/850);
  Double_t part3=(1-par[0])/(1+(1.1*momInGeV-1)/115);
  Double_t part4=0.054*(1-par[1])/(1+(1.1*momInGeV-1)/850);
  RESULT=(part1+part2)/(part3+part4);
  return RESULT;
}

const Int_t GetPIDFromMomentum(TH1F* hlookup, const Double_t momInGeV)
{
  Double_t ratio = GetMuPlusMinusRatio(hlookup, momInGeV);
  Double_t prob_mu_plus = ratio/(1.+ratio);
  Int_t number = gRandom->Binomial(1, prob_mu_plus);
  if(number==0) return 13; //mu-
  else return -13;//mu+
}

const Int_t GetPIDFromMomentum(const Double_t momInGeV)
{ 
  Double_t ratio = GetMuPlusMinusRatio(momInGeV);
  Double_t prob_mu_plus = ratio/(1.+ratio);
  Int_t number = gRandom->Binomial(1, prob_mu_plus);
  if(number==0) return 13; //mu-
  else return -13;//mu+
}

int main(int argc, char** argv)
{ 
  long rseed = 0;
  char* outFilename = NULL;
  int nEvents = 1000000000; 
  // a billion. Default to something big for piping.
  char* whichSite = NULL;
  char* volume = "Rock";
  char* musicDir = NULL;
  char* Rotation = "Yes";
  int nTrks = -1;
  int wpo = 0;
  int stop = 0;
  int cdWaterBuffer = 0;
  int isMult = 1;
  bool bFileExist = false;
  fstream IfFile;    
  ProcessArgs(argc, argv, &rseed, &outFilename, &nEvents, &whichSite, &volume, 
              &Rotation, &isMult, &musicDir, &nTrks, &wpo, &stop, &cdWaterBuffer);

  if(argc==1) {
    Usage();
    exit(0);
  }

  // if no filename is given, assuming it is piping to stdout 
  // and keep going. jianglai
//  if( outFilename == NULL)
//   {
//     cerr<<"Please enetr a valid filename."<<endl;
//     Usage();
//     return -1;
//   }

  if(musicDir==NULL) {//not specified at command line, then assume it is here
    const char* cmt_path = getenv("JUNOTOP");
    if (!cmt_path) {
      std::cerr << "Please setup JUNO environment first." << std::endl;
      exit(1);
    }
    std::string path_name = cmt_path;
    path_name += "/data/Generator/Muon/data";
    musicDir = new char[ path_name.size()+1 ];
    strcpy(musicDir, path_name.c_str());
    cout<<"# Assuming the music file is located under "<<musicDir<<endl;
  }
  
  FILE* stream = stdout;
  if( outFilename!=NULL ) {
    stream = fopen(outFilename, "w");
    if( stream==NULL ) {
      printf("Please enetr a valid filename.\n");
      Usage();
      exit(0);
    }
  }

  //Now get the mu+/mu- ratio histogram
  TH1F *mu_pm_ratio_lookup;
  TFile *rootfile;
  IfFile.open(Form("%s/mu_plus_minus_ratio.root",musicDir),ios::in);
  if(!IfFile){
    bFileExist=false;
  }
  else{
     bFileExist=true;
     rootfile = new TFile(Form("%s/mu_plus_minus_ratio.root",musicDir),
                     "read");
     mu_pm_ratio_lookup = (TH1F*)rootfile->Get("mu_plus_minus_ratio");
   }

  // get the right detector setting and the right muon flux file 
  Hep3Vector detectorDim;    //the water cherenkov detector dimension.
  Hep3Vector detectorCenter; // detector center.
  Double_t dphi = 0;             // detector angle.
  ifstream infile;              // the muon flux file.
  const char* infilename = NULL;
  Int_t Nmuon;              // the muon events in the flus file.
  
  Double_t Exp_x = 56.25*m; //refer JUNOEng-doc-5-v3
  Double_t Exp_y = 49*m;
  Double_t Exp_z = 27*m;
  Double_t TT_z = 8.4*m; 
  Double_t WP_dia = 43.5*m;
  Double_t WP_z = 43.5*m;
  Double_t Rock_thick = 3*m;
  Double_t ExpHallXgap = 6.25*m;
  Double_t Rock_x = Exp_x + 2.*Rock_thick + ExpHallXgap;
  Double_t Rock_y = Exp_y + 2.*Rock_thick;
  Double_t TopRock_z = Exp_z + Rock_thick;
  Double_t BtmRock_z = WP_z + Rock_thick;

  Double_t xDim = 0;
  Double_t yDim = 0; 
  Double_t zDim = 0; 
  // the origin (0,0,0) is always at the center of water pool
  Double_t zOffset = 0; // center of the volume

  if( strcmp(volume, "Rock")==0 ) {
    xDim = Rock_x;
    yDim = Rock_y;
    zDim = TopRock_z + BtmRock_z;
    zOffset = zDim/2. - WP_z/2. - Rock_thick;
  }
  else if( strcmp(volume, "TT")==0 ){
    xDim = Exp_x;
    yDim = Exp_y;
    zDim = TT_z;
    zOffset = WP_z/2. + TT_z/2.;
  }
  else if( strcmp(volume, "WP")==0 ){
   xDim = Exp_x; 
   yDim = Exp_y; 
   zDim = WP_z + TT_z; 
   zOffset = TT_z/2.;
  }
  else {
    cerr<<"muon_generator: *** ERROR *** Invalid volume " << volume
        << " selected. Valid volume names are Rock, TT, WP"<<endl;
    assert(0); // this will end the job
  }

  if( (!whichSite) or ( whichSite and strcmp(whichSite, "juno")==0 ) ) 
  {
    infile.open(Form("%s/mountain.juno",musicDir), ios_base::in);
    infilename = "mountain.juno";
    detectorDim = Hep3Vector(xDim, yDim, zDim);
    detectorCenter = Hep3Vector(0, 0, zOffset );
    if(Rotation!=NULL){
      if( strcmp(Rotation, "Yes")==0){
        dphi = -56.7*deg + 90*deg; //Changed into 33.3*deg as JUNO-doc-4599.
      }
    }
 
  }
  else if( whichSite and strcmp(whichSite, "juno_original")==0 ) 
  {
    infile.open(Form("%s/mountain.juno_original",musicDir), ios_base::in);
    infilename = "mountain.juno_original";
    detectorDim = Hep3Vector(xDim, yDim, zDim);
    detectorCenter = Hep3Vector(0, 0, zOffset );
    if(Rotation!=NULL){
      if( strcmp(Rotation, "Yes")==0){
        dphi = -56.7*deg;
      }
    }
 
  }
  else 
  {
    cerr<<"muon_generator: *** ERROR *** Invalid site." 
        << "Now the valid sites: juno or juno_original"<<endl;
    exit(1);
  }
  
  if ( infile.fail() )
  { cerr <<"muon_generator: ERROR Input file " << infilename << " in " << musicDir << " cannot be opened. does it exist?"<<endl;
    exit(1);
  }

  // read the flux file
  vector<Double_t> muonE, muonPhi, muonTheta; 
  Double_t flux_of_site = 0.00303433696;
  //temporary variable to hold data in flux file.
  Double_t t_energy, t_phi, t_theta, t_energy0, t_phi0,t_theta0;  

  // line 1 to line 6 are comments
  char tmp[255];
  for( int lineNo=0; lineNo<4; lineNo++)
  {
    infile.getline(tmp, 255);
  }
  // line 5 contains flux info of that site.
  infile.getline(tmp,255,':');
  infile>>flux_of_site;
  infile.getline(tmp,255);
  // line 6
  infile.getline(tmp,255);

    if( isMult == 0){
      for( Nmuon=0; ; Nmuon++)
      {
        if(infile.eof())
        {
          break;
        }
        infile>>t_energy>>t_theta>>t_phi>>t_energy0>>t_theta0>>t_phi0;
        muonE.push_back(t_energy);
        muonPhi.push_back(t_phi);
        muonTheta.push_back(t_theta);
      }
      Nmuon = Nmuon - 1;
      muonE.pop_back();
      muonPhi.pop_back();
      muonTheta.pop_back();
    }
  infile.close();

  //read the single and multiple muon information
  Int_t           evtMult;
  Double_t        thetag[7];   //[evtMult]
  Double_t        energy[7];   //[evtMult]
  Double_t        phig[7];   //[evtMult]
  Double_t        R[7];   //[evtMult]


    TFile *rootfile1;
    rootfile1 = new TFile(Form("%s/mugen.root",musicDir),
        "read");
    TChain fChain("mu_gen","tree for generator");
    fChain.Add(Form("%s/mugen.root",musicDir));
    fChain.SetBranchAddress("evtMult", &evtMult);
    fChain.SetBranchAddress("theta", thetag);
    fChain.SetBranchAddress("energy", energy);
    fChain.SetBranchAddress("phi", phig);
    fChain.SetBranchAddress("R", R);   
   if( isMult !=0 ){
    Nmuon = fChain.GetEntries();
  }

  cout<<"# Nmuon "<<Nmuon<<endl;

  gRandom->SetSeed(rseed);
  fprintf(stream, "# File generated by %s.\n", argv[0]);
  fprintf(stream, "# Ransom seed for generator = %ld.\n", rseed);
  fprintf(stream, "# Events number = %d.\n",nEvents);
  fprintf(stream, "# Water Cherenkov detector site and input file: %s, %s.\n",whichSite,infilename);
  fprintf(stream, "# The last 7 numbers in one line are:\n");
  fprintf(stream, "# momentum vector (GeV), mass (GeV), and position offset vector(mm).\n");
  
//   stream<<"# File generated by "<<argv[0]<<"."<<endl;
//   stream<<"# Ransom seed for generator = "<<rseed<<"."<<endl; 
//   stream<<"# Events number = "<<nEvents<<"."<<endl;
//   stream<<"# Water Cherenkov detector site: "<<whichSite<<"."<<endl;
//   stream<<"# The last 7 numbers in one line are"<<endl  
//      <<"# momentum vector (GeV), mass (GeV), and position vector(mm)."<<endl; 
  // the detector geometry 
  Double_t dimx, dimy, dimz; 
  Double_t xdet, ydet, zdet; 
  Double_t S_max;            // the maximum acceptance.
  Double_t S_x, S_y, S_z;    // surface area in x, y and z direction.
  Double_t pS_z, pS_x, pS_y; // projected area in x, y, z direction.
  
  dimx = detectorDim.x()/m; 
  dimy = detectorDim.y()/m; 
  dimz = detectorDim.z()/m;
  xdet = detectorCenter.x()/m; 
  ydet = detectorCenter.y()/m; 
  zdet = detectorCenter.z()/m; 
  cout<<"# The detector dimension is: "
      <<dimx<<"*m, "<<dimy<<"*m, "<<dimz<<"*m"<<endl;
  cout<<"# The detector center is: "
      <<xdet<<"*m, "<<ydet<<"*m, "<<zdet<<"*m"<<endl; 
  cout<<"# The detector angle is: "<<dphi<<endl;
  cout<<"# MAKE SURE the setting is consistent with Geant4 setting!!!"<<endl;

  S_z = dimx * dimy;
  S_x = dimy * dimz;
  S_y = dimx * dimz;
  S_max = sqrt(S_x*S_x + S_y*S_y + S_z*S_z);

  Double_t PI = 3.1415926;
  Hep3Vector momentum, position; // the muon momentum and position.
  Hep3Vector direction;          // the muon direction.
  Double_t pMuon;                // the muon momentum length.
  double muon_mass = 0.105658*GeV;
  Int_t ievent;                  // the ievents-th muon from the flux sample.
  Double_t weight = 1.;
  Double_t arandom;
  
  Int_t n_top = 0;  // how many muons generated on top.

  Int_t mubNum =0;
   Int_t pid; 
  double    mub_x[25]={0}, mub_y[25]={0},mub_z[25]={0};
  double    mub_px[25]={0}, mub_py[25] ={0}, mub_pz[25]={0};
  double mub_t[25] = {0};
 for( int i=0; i<nEvents; )
  {

    do // pick a muon
    { 
      // pick the ievents-th muon from the flux sample.
      ievent = (Int_t)(gRandom->Uniform()* (Nmuon-1));
      if( isMult != 0){
        fChain.GetEntry(ievent);
		if (evtMult != nTrks && nTrks != -1)
			continue;
      }
      // convert coordinates from MUSIC system to digimap system.
      double theta = 0.;
     double  phi = 0.;
     if( isMult == 0){
       theta = PI - PI *  muonTheta[ievent]/180;
       phi = PI/2 -PI * muonPhi[ievent]/180; 
     }
     else{
       theta = PI - PI * thetag[0]/180;
       phi = PI/2 -PI * phig[0]/180; 
     }
     if( phi < 0) 
       phi = phi + 2 * PI;

     // The WC detector may be rotated an angle to better shield neutrons, 
      // It is convenient to rotate muon direction instead of rotating detector
      //  in Geant4
      phi = phi + dphi;
      if(phi > 2 * PI)
        phi = phi - 2 * PI;
      direction.setX( sin(theta) * cos(phi));
      direction.setY( sin(theta) * sin(phi));
      direction.setZ( cos(theta) );
    
      // projected area of the WE surface to this muon.
      pS_z = S_z * abs(direction.z()); 
      pS_x = S_x * abs(direction.x()); 
      pS_y = S_y * abs(direction.y());

      weight = ( pS_z + pS_x + pS_y )/S_max; 
      arandom = gRandom->Uniform();
    }
    while ( arandom > weight ); // drop this muon.
     
    // calculate the muon position and momentum

    // muon hits the top panel.
    if( arandom < pS_z/S_max)
    {
      position.setX( detectorCenter.x() +
                   (gRandom->Uniform()-0.5) * detectorDim.x() ); 
      position.setY( detectorCenter.y() +
                   (gRandom->Uniform()-0.5) * detectorDim.y() ); 
      position.setZ( detectorCenter.z() + 0.5 * detectorDim.z() );

      n_top++;
    } 
    // muon hits the X panel.
    else if( arandom < (pS_z + pS_x)/S_max)
    {
      position.setY( detectorCenter.y() +
                   (gRandom->Uniform()-0.5) * detectorDim.y() ); 
      position.setZ( detectorCenter.z() +
                   (gRandom->Uniform()-0.5) * detectorDim.z() ); 
      if (direction.x() > 0)
        position.setX( detectorCenter.x() - 0.5 * detectorDim.x() );
      if (direction.x() < 0)
        position.setX( detectorCenter.x() + 0.5 * detectorDim.x() );
    } 
    // muon hits the Y panel.
    else if( arandom < weight)
    {
      position.setX( detectorCenter.x() +
                   (gRandom->Uniform()-0.5) * detectorDim.x() ); 
      position.setZ( detectorCenter.z() +
                   (gRandom->Uniform()-0.5) * detectorDim.z() ); 
      if (direction.y() > 0)
        position.setY( detectorCenter.y() - 0.5 * detectorDim.y() );
      if (direction.y() < 0)
        position.setY( detectorCenter.y() + 0.5 * detectorDim.y() );
    }

    if( isMult == 0){
      evtMult = 1;
    }
    else{
      fChain.GetEntry(ievent);
	  if (evtMult != nTrks && nTrks != -1)
		  continue;
    }
    for(int m=0;m<25;m++){
      mub_x[m]=0;
      mub_y[m]=0;
      mub_z[m]=0;
      mub_px[m]=0;
      mub_py[m]=0; 
      mub_pz[m]=0; 
      }
      mubNum =0;
  //  cout<<"evtMult= "<<evtMult<<endl;
    bool isSelect = false;
    for(int j =0;j<evtMult;j++) {
      if( isMult == 0){
        pMuon = sqrt((muonE[ievent]*GeV)*(muonE[ievent]*GeV)
            - (muon_mass)*(muon_mass));
      }
      else{
        if(energy[j]*GeV<=muon_mass) 
          continue; 
        else
          pMuon = sqrt((energy[j]*GeV)*(energy[j]*GeV)
              - (muon_mass)*(muon_mass));
      }
      momentum.setX(pMuon * direction.x());
      momentum.setY(pMuon * direction.y());
      momentum.setZ(pMuon * direction.z());
     // cout<<"pMuon ="<<pMuon<<" ; pMuon * direction.x() ="<<pMuon * direction.x()<<endl; 
     if(evtMult==1) {
        if(!bFileExist){
          pid = GetPIDFromMomentum(momentum.mag()/GeV);
         }
         else{
         pid = GetPIDFromMomentum(mu_pm_ratio_lookup, momentum.mag()/GeV);
         }
        // by wudr
        int res = judgement(position.x(), position.y(), position.z(), momentum.x(), momentum.y(), momentum.z());
        if (wpo && !(res % 10))
            continue;
        if (stop && (res % 100) < 10)
            continue;
        if (cdWaterBuffer && !(res / 100))
            continue;
        // by wudr
        fprintf(stream,"1\n");
        fprintf(stream,"1\t%d 0 0 %f %f %f %f 0 %f %f %f\n",
           pid, momentum.x()/GeV, momentum.y()/GeV, momentum.z()/GeV,
           muon_mass/GeV, position.x()/mm, position.y()/mm, position.z()/mm);
         isSelect = true;
      }
     else if (evtMult>1) {
      double x1 =position.x()/mm;
      double y1 =position.y()/mm;
      double z1 = position.z()/mm;
      double px =  momentum.x()/GeV;
      double py = momentum.y()/GeV;
      double pz = momentum.z()/GeV; 
      double theta1 = thetag[j];
      double phi1 = phig[j];
      double a = R[j]*1000;
      double b = R[j]*1000/cos(theta1*3.1415/180); 
      double thetaRand = gRandom->Uniform()*2*3.1415;
      double xe = a*cos(thetaRand);
      double ye = b*sin(thetaRand);
      double  xr = xe*cos(-(90-phi1)*3.1415/180)- ye*sin(-(90-phi1)*3.1415/180);
      double  yr = xe*sin(-(90-phi1)*3.1415/180)+ye*cos(-(90-phi1)*3.1415/180);
      double  xb = x1+xr;
      double  yb = y1+yr;
      double  zb = z1; 
      // find the muon inject point the surface
         // line function
         // 5 plane  
          double z_plane = detectorCenter.z() + 0.5*detectorDim.z();
          double x_plus = detectorCenter.x() + 0.5*detectorDim.x() ;
          double x_minus = detectorCenter.x() - 0.5*detectorDim.x();
          double y_plus =  detectorCenter.y() + 0.5*detectorDim.y();
          double y_minus = detectorCenter.y() - 0.5*detectorDim.y();
           int z_planeFlag = 0;
           int x_plusFlag =0;
           int x_minusFlag =0;
           int y_plusFlag =0;
           int y_minusFlag =0;
           double t,xf,yf,zf;
          //// Z+ plane
           t = (z_plane-zb)/pz;
           xf = xb+px*t;
           yf = yb+py*t;
           double tempx, tempy, tempz =-100;
            if(TMath::Abs(xf)<0.5*detectorDim.x()&&TMath::Abs(yf)<0.5*detectorDim.y()) {
                z_planeFlag = 1;
        //        cout<<"Z+ plane ; x ="<<xf<<" ; y ="<<yf<<" ; z_plane="<<z_plane<<endl;
                if(tempz<z_plane) {tempx =xf; tempy =yf; tempz = z_plane;} 
             }
           //x_plus
            t = (x_plus-xb)/px;
            zf = zb+pz*t;
            yf = yb+py*t;
            if(TMath::Abs(zf)<0.5*detectorDim.z()&&TMath::Abs(yf)<0.5*detectorDim.y()) {
                x_plusFlag = 1;
          //      cout<<"x_plus ; x_plus ="<<x_plus<<" ; y ="<<yf<<" ; z="<<zf<<endl;
                 if(tempz<zf) {tempx =x_plus; tempy =yf; tempz = zf;}
             }
          //x_minus 
            t = (x_minus-xb)/px;
            zf = zb+pz*t;
            yf = yb+py*t;
            if(TMath::Abs(zf)<0.5*detectorDim.z()&&TMath::Abs(yf)<0.5*detectorDim.y()) {
              x_minusFlag = 1;
            //  cout<<"x_minus ; x_minus ="<<x_minus<<" ; y ="<<yf<<" ; z="<<zf<<endl;
                if(tempz<zf) {tempx =x_minus; tempy =yf; tempz = zf;}
             }
            ///y_plus 
            t = (y_plus-yb)/py;
            zf = zb+pz*t;
            xf = xb+px*t;
           if(TMath::Abs(zf)<0.5*detectorDim.z()&&TMath::Abs(xf)<0.5*detectorDim.x()) {
           y_plusFlag = 1;
          // cout<<"y_plus ; x ="<<xf<<" ; y_plus ="<<y_plus<<" ; z="<<zf<<endl;
                if(tempz<zf) {tempx =xf; tempy =y_plus; tempz = zf;}
            }        
           //y_minus
           t = (y_minus-yb)/py;
           zf = zb+pz*t;
           xf = xb+px*t;
            if(TMath::Abs(zf)<0.5*detectorDim.z()&&TMath::Abs(xf)<0.5*detectorDim.x()) {
             y_minusFlag = 1;
            // cout<<"y_minus ; x ="<<xf<<" ; y_minus ="<<y_minus<<" ; z="<<zf<<endl;
               if(tempz<zf) {tempx =xf; tempy =y_minus; tempz = zf;}
            }
         if(tempz>-100) {
           mub_x[mubNum] = tempx;
           mub_y[mubNum] = tempy;
           mub_z[mubNum] = tempz;
           mub_px[mubNum] = px;
           mub_py[mubNum] = py;
           mub_pz[mubNum] = pz;
           mubNum++;
         }
       } // if Mult>1    
      } // for Mult
     if(mubNum > 0) {
        // by wudr
        int pWp = 0, stopped = 0, cdWater = 0;
        for (int k = 0; k < mubNum; k ++) {
            int res = judgement(mub_x[k], mub_y[k], mub_z[k], mub_px[k], mub_py[k], mub_pz[k]);
            if (res % 10)
                pWp ++;
            if (res % 100 >= 10)
                stopped ++;
            if (res / 100)
                cdWater ++;
        }
        if (wpo && !pWp)
            continue;
        if (stop && !stopped)
            continue;
        if (cdWaterBuffer && !cdWater)
            continue;
        // by wudr
        //Add time info
        if (nTrks != -1 && mubNum != nTrks)
            continue;
        double Vmuon = 299.792458;
        for (int k = 0; k < mubNum; k ++) {
            TVector3 oriPos(mub_x[k], mub_y[k], mub_z[k]);
            TVector3 oriDir(mub_px[k], mub_py[k], mub_pz[k]);
            oriDir.SetMag(1.);
            mub_t[k] = 0;
            bool breakFlag = false;
            for (int l = 0; l < mubNum; l ++) {
                if (l == k)
                    continue;
                TVector3 tmpPos(mub_x[l], mub_y[l], mub_z[l]);
                double disMu = (oriPos - tmpPos) * oriDir;
                if (disMu > 0) {
                    breakFlag = true;
                    break;
                }
                disMu = fabs(disMu);
                mub_t[l] = disMu / Vmuon;
            }
            if (!breakFlag)
                break;
        }
        fprintf(stream,"%d\n", mubNum);
         for(int k=0;k<mubNum;k++) { 
           if(!bFileExist){
           pid = GetPIDFromMomentum(momentum.mag()/GeV);
          }
          else{
          pid = GetPIDFromMomentum(mu_pm_ratio_lookup, momentum.mag()/GeV);
          }
          fprintf(stream,"1\t%d 0 0 %f %f %f %f %f %f %f %f\n",
          pid,  mub_px[k],  mub_py[k],  mub_pz[k],
          muon_mass/GeV, mub_t[k], mub_x[k], mub_y[k], mub_z[k]);
          isSelect = true;
      }    
    }//if mubNum>0

    if(isSelect)
     i++;
    //gSystem->Sleep(1000);
  } // for NEvent

  muonE.clear();
  muonPhi.clear();
  muonTheta.clear();

  {
    double ratio=double(nEvents)/double(n_top);
    fprintf(stream,"# Ntop=%d, Ntotal=%d, ratio=%f\n",n_top,nEvents,ratio);
    double top_area=detectorDim.x()*detectorDim.y()/m/m;
    double rate=flux_of_site*top_area*ratio;
    fprintf(stream,"# Site=%s, top_area=%f m^2, flux=%f #/(s.m^2)\n",whichSite,top_area,flux_of_site);
    fprintf(stream,"# Muon rate=%f #/s, Average time interval=%f s\n",rate,1/rate);    
  }

  return 0;
}

void ProcessArgs(int argc, char** argv, long *rseed,
                 char** outfilename, int* nevents, char** whichsite, char** volume,
                 char ** rotation, int* ismult, char** music_dir, int* nTrks, int* wpo, int* stop, int* cdWaterBuffer) 
{
  int i;
  for( i=1 ; i<argc ; i++ ) 
  {
    if( strcmp(argv[i], "-seed")==0 ) 
    {
      i++;
      sscanf(argv[i], "%ld", rseed);
    } 
    else if( strcmp(argv[i], "-o")==0 ) 
    {
      i++;
      *outfilename = new char[strlen(argv[i]) +1];
      strcpy(*outfilename, argv[i]);
    } 
    else if( strcmp(argv[i], "-n")==0 ) 
    {
      i++;
      sscanf(argv[i], "%d", nevents);
    } 
    else if( strcmp(argv[i], "-s")==0 )
    {
      i++;
      *whichsite = new char[strlen(argv[i]) + 1];
      strcpy(*whichsite, argv[i]);
    }
    else if( strcmp(argv[i], "-v")==0) {
      i++;
      *volume= new char[strlen(argv[i]) + 1];
      strcpy(*volume, argv[i]);
    }
    else if( strcmp(argv[i], "-r")==0)
    {
     i++;
     *rotation = new char[strlen(argv[i]) + 1];
     strcpy(*rotation, argv[i]);
    }
    else if( strcmp(argv[i], "-mult")==0)
    {
     i++;
      sscanf(argv[i], "%d", ismult);
    }
    else if( strcmp(argv[i], "-music_dir")==0){
      i++;
      *music_dir = new char[strlen(argv[i]) + 1];
      strcpy(*music_dir, argv[i]);
    }
    else if(strcmp(argv[i], "-ntrks") == 0) {
      i ++;
      sscanf(argv[i], "%d", nTrks);
    }
    else if (strcmp(argv[i], "-wpo") == 0) {
      *wpo = 1;
    }
    else if (strcmp(argv[i], "-stop") == 0) {
      *stop = 1;
    }
    else if (strcmp(argv[i], "-cdWater") == 0) {
      *cdWaterBuffer = 1;
    }
    else
    {
      Usage();
      exit(0);
    }
  }
}
 
void Usage() 
{
  cout<<"./Muon.exe [-seed seed] [-o outputfilename] [-n nevents]" 
      <<" [-s juno (juno, juno_original)] [-v volume (Rock, TT, WP)][-r rotating mountain(Yes, No)]"
      <<" [-mult single or real muon (0, 1)]"
      <<" [-ntrks events of n tracks selection]"
      <<" [-wpo only the muon events whose track pass through water pool]"
      <<" [-stop only the muon events whose energy is less than 20 GeV]"
      <<" [-cdWater only the muon events whose track pass through the CD water buffer]"
      <<" [-music_dir <path of music files relative to this dir>] "<<endl;
}
