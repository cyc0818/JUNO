#include "Recon_clipping.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include "TStyle.h"
#include "TMath.h"
#include "TDirectory.h"
#include "TMinuit.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TVector3.h"
#include <iterator>
#include <algorithm>

//#include "Correction.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "RecTools/PmtProp.h"
using namespace std;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void Fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){ //global fit function
   ((Recon*)gMinuit->GetObjectFit())->rec_fcn( npar, gin, f, par, iflag );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
 
Recon::Recon(){ //constructor
   //for execute only one time
   Init();
}

Recon::~Recon(){ //destructor
}

void Recon::reconstruct(){ //event by event reconstruct
   Event_Init();

   int Npar = 4;
   TMinuit *recMinuit = new TMinuit(Npar); //Instantiate Minuit for Npar parameters

   PMT_cut();cout<<"Eligible firedPmtN: "<<firedPmtN<<endl;
   Choose_pmt();
   First_guess();   //get initial value

   bool cut_stop2 = true;
   N_ThrowOut = 0;
   fitN = 0;

   do{ //throw out stop2
      OptPmtN =(int)OptPmtPe.size();//cout<<"OptPmtN:"<<OptPmtN<<endl;
      bool cut3 = OptPmtN>4;
      if( not cut3 ){
         cout<<"Warning!!! Eligible OptPmtN too little, is "<<OptPmtN<<endl;
         if( fitN ==0 ){
            fitN = 0; icstat = -3; status = 3;
         }
         if( fitN > 0 ){
            fitN = -2; icstat = -2; status = 3;
         }
         break;
      }

      fitN_stop1 = 0; //for IRLS
      Weight_OLS(OptPmtW,OptPmtN);

      vector<double> weight_unsorted(PMTSUM), last_weight_unsorted(PMTSUM);
      bool cut_stop1 = true;
      do{ //for IRLS stop1
         Fit_minuit(recMinuit);

         //===============good or bad===============//
         double danger = 1.-rec_lx*rec_lx-rec_my*rec_my;
         if( not ( danger > 0.) ){
            cout<<"Warning!"<<" lx: "<<rec_lx<<" my:  "<<rec_my<<" danger: "<<danger<<" Be carefull!!!"<<endl;
            flag_event = 0; break;
         }
         rec_nz = -TMath::Sqrt(1.-rec_lx*rec_lx-rec_my*rec_my); 
         rec_z = -(rec_lx*rec_x+rec_my*rec_y)/rec_nz;
         //===============good or bad===============//

         // OLS for Once Fit
         if( fitN==0 && fitN_stop1==0 ){
            rec_lx1 = rec_lx; rec_my1 = rec_my; rec_nz1 = rec_nz;
            rec_x1  = rec_x;  rec_y1  = rec_y;  rec_z1  = rec_z;

            //if(rec_nz1>COSTHETA){
      	   //   rec_lx1 *= rec_lx_last;
      	   //   rec_my1 *= rec_my_last;
            //}
            //rec_nz1 = -TMath::Sqrt(1.-rec_lx1*rec_lx1-rec_my1*rec_my1);
            rec_theta1 = 180.-TMath::ACos(rec_nz1)/TMath::Pi()*180.;  //0~90
            rec_phi1 = TMath::ATan2(rec_my1,rec_lx1)/TMath::Pi()*180.;  //-180~180
         }

         fitN_stop1++;

		   // check the fitN_stop1
		   bool b_fitN_stop1 = (fitN_stop1 > FIT_N_STOP1);
         bool b_weight_stop1 = false;

         if(!b_fitN_stop1){
            std::vector<double> v_weight = Weight_IRLS();
            unpack( weight_unsorted, v_weight ); //weight_unsorted: according to ID number, weights of 17612 PMTs

            double dweight = delta_weight( weight_unsorted, last_weight_unsorted );

            if( dweight < DIF_WEIGHT ){
               b_weight_stop1 = true;
            }
            else{
               last_weight_unsorted = weight_unsorted;
               OptPmtW = v_weight;
            }
         }
         if(b_fitN_stop1 || b_weight_stop1) cut_stop1 = false;
      }while(cut_stop1);
      if(flag_event==0) break;

      //Throw Out ~ check the fitN (the total FitN)
      fitN += fitN_stop1;
      bool b_fitN = (fitN < FIT_N);
      bool b_throwOut = (N_ThrowOut < THROWOUT_N);

      //v_residual
      int num = (int)v_residual.size();
      bool b_residual = v_residual[num-1]>T_RESIDUAL;
      if(b_fitN && b_throwOut && b_residual){
         Throw_out();
         N_ThrowOut++;
      }
      else{
         cut_stop2 = false;
      }
   }while(cut_stop2);
   delete recMinuit;

   rec_point.SetXYZ(rec_x,rec_y,rec_z);
   rec_dir.SetXYZ(rec_lx,rec_my,rec_nz);
   pool_center.SetXYZ(0,0,0);
   trackL = hitSphereAll(SphereRadius, pool_center, rec_point, rec_dir, X1, X2, Dis_center);
   cout<<"start:("<<X1.X()<<","<<X1.Y()<<","<<X1.Z()<<")  end:("<<X2.X()<<","<<X2.Y()<<","<<X2.Z()<<")  dir:("<<rec_dir.X()<<","<<rec_dir.Y()<<","<<rec_dir.Z()<<")"<<"  totalpe:"<<total_pe<<"  chi2byndf:"<<chi2<<endl;
}

void Recon::Init(){
   OptPmtTime.clear();
   OptPmtPe.clear();
   OptPmtPos.clear();
   OptPmtW.clear();
   OptPmtRes.clear();
   v_residual.clear();
   v_sign_residual.clear();

   OptPmtTime.reserve(17612);
   OptPmtPe.reserve(17612);
   OptPmtPos.reserve(17612);
   OptPmtW.reserve(17612);
   OptPmtRes.reserve(17612);
   v_residual.reserve(17612);
   v_sign_residual.reserve(17612);
}

void Recon::Event_Init(){
   //initialize 
   EventId = 0;
   Jentry = 0;
   
   //stat
   icstat = -1;
   fitN = -1;
   fitN_stop1 = -1; 

   //initial pmt pe and time info only and estimate the track para
   OptPmtTime.clear();
   OptPmtPe.clear();
   OptPmtPos.clear();
   OptPmtW.clear();
   OptPmtRes.clear();
   v_residual.clear();
   v_sign_residual.clear();

   //event flag
   flag_event = 1;
   status = 3;

   firedPmtN = 0;
   total_pe = 0.;
   OptPmtN = 0;
   pmtnum = (*m_ptable).size();

   /*if (m_corrmethod == "corr2dtad"){
      m_fhtcorrtool = new Correctiond2ad("corr2ad");
   }
   else if (m_corrmethod == "corr2dtqd"){
      m_fhtcorrtool = new Correctiond2qd("corr2qd");
   }
   else if (m_corrmethod == "corr1dtqdqd") {
      m_fhtcorrtool = new Correctiond1qdqd("corr1qdqd");
   }
   else if (m_corrmethod == "corr1dtdqdq") {
      m_fhtcorrtool = new Correctiond1dqdq("corr1qdqd");
   }
   else if (m_corrmethod == "corr1dtadad") {
      m_fhtcorrtool = new Correctiond1adad("corr1adad");
   }
   else if (m_corrmethod == "corr1dtdada") {
      m_fhtcorrtool = new Correctiond1dada("corr1dada");
   }
   else if (m_corrmethod == "corr1dtq") {
      m_fhtcorrtool = new Correctiond1q("corr1q");
   }
   else if (m_corrmethod == "nocorr") {
   }
   else{
      cout<<" Please set right fht correction method"<<endl;
   }

   //m_fhtcorrtool = dynamic_cast<IFhtCorrection*>(DLEFactory::instance()->create(m_corrmethod));
   if (m_fhtcorrtool == NULL){
      cout<< " Failed to create fht correction tool, make sure the tool name is right ["<< m_corrmethod<<"] "<<endl;
   }
   if(not m_fhtcorrtool->setCorrectionFileAndNumBinDist(m_corrfilename,m_nBinDist)){
      cout<< " Failed to set CorrectionFile And NumBinDist ["<< m_corrmethod<<"] "<<endl;
   }*/
}

void Recon::PMT_cut(){
   //Find_earliestFHT
   TH1F* fhts = new TH1F("", "", 1000, 0, 1000);
   for (unsigned int i = 0; i < pmtnum; i++){
      fhts->Fill((*m_ptable)[i].fht);
   }
   peakFHT = fhts->GetMaximumBin();
   earliestFHT = peakFHT;
   while (fhts->GetBinContent(earliestFHT) > 2){
      earliestFHT --;
   }
   earliestFHT -= 1;//cout<<"earliestFHT:"<<earliestFHT<<endl;

   //PMT_cut
   for(unsigned int i = 0; i < pmtnum; i++){
      if((*m_ptable)[i].fht < earliestFHT){
         (*m_ptable)[i].used = false;
      }
      if((*m_ptable)[i].fht-earliestFHT > Relativetime_cut){
         (*m_ptable)[i].used = false;
      }
      if((*m_ptable)[i].q < cutPE){
         (*m_ptable)[i].used = false;
      }
      if( not (*m_ptable)[i].used ) continue;
      if( (*m_ptable)[i].type != _PMTINCH20 ) continue;
      total_pe += (*m_ptable)[i].q;
      firedPmtN++;
   }
   if(firedPmtN == 0){
      for(unsigned int i = 0; i < pmtnum; i++){
         if((*m_ptable)[i].q < cutPE) continue;
         cout<<"earliestFHT:"<<earliestFHT<<'\t'<<"fht["<<i<<"]:"<<(*m_ptable)[i].fht<<'\t'<<"q["<<i<<"]:"<<(*m_ptable)[i].q<<endl;
      }
   }
}

//contain redundant pmts i.e. outliers
void Recon::Choose_pmt(){
   for(unsigned int i=0; i<pmtnum; i++){
      if( not (*m_ptable)[i].used ) continue;
      if( (*m_ptable)[i].type != _PMTINCH20 ) continue;

      OptPmtTime.push_back((*m_ptable)[i].fht);
      OptPmtPe.push_back((*m_ptable)[i].q);
      OptPmtPos.push_back((*m_ptable)[i].pos);
      OptPmtRes.push_back((*m_ptable)[i].res);
   }
}

void Recon::First_guess(){
   //Input : PMT_ID, PMTPe, PMT_Time
   //Output: ini_x,  ini_y,  ini_z, 
   //        ini_lx, ini_my, ini_nz;
   //Mu_V  : Px, Py, Pz 

   //choose pmt only for first guess
   std::vector<double> OptPmtTime_1stguess;
   std::vector<double> OptPmtPe_1stguess;
   std::vector<double> OptPmtW_1stguess;
   std::vector<TVector3> OptPmtPos_1stguess;
   
   for(int i=0; i<pmtnum; i++){
      if( not (*m_ptable)[i].used ) continue;
      if( (*m_ptable)[i].type != _PMTINCH20 ) continue;
      //bool bpmt = (*m_ptable)[i].fht>earliestFHT && (*m_ptable)[i].fht<FHTcut_FirstGuess && (*m_ptable)[i].q>Qcut_FirstGuess;
      //if(bpmt){
         OptPmtTime_1stguess.push_back((*m_ptable)[i].fht);
         OptPmtPe_1stguess.push_back((*m_ptable)[i].q);
         OptPmtPos_1stguess.push_back((*m_ptable)[i].pos);
         OptPmtW_1stguess.push_back(pow((*m_ptable)[i].q,Q_ALPHA));
      //}
   }

   double sum_x  =0., sum_y  =0., sum_z  =0.;
   double sum_px =0., sum_py =0., sum_pz =0.;
   double sum_denominator = 0.;
   int OptPmtN_1stguess = (int)OptPmtW_1stguess.size();
   
   for(int i=0; i<OptPmtN_1stguess; i++){
      for(int k=0; k<OptPmtN_1stguess; k++){
         if(i==k)continue;         
         double weight_pmt = OptPmtW_1stguess[i]*OptPmtW_1stguess[k]; 
         sum_denominator += weight_pmt*pow(OptPmtTime_1stguess[i]-OptPmtTime_1stguess[k],2);
         //vertex;
         double x_i = OptPmtPos_1stguess[i].X()*(OptPmtTime_1stguess[k]*OptPmtTime_1stguess[k]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         double x_k = OptPmtPos_1stguess[k].X()*(OptPmtTime_1stguess[i]*OptPmtTime_1stguess[i]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         sum_x += weight_pmt*(x_i+x_k);         
         double y_i = OptPmtPos_1stguess[i].Y()*(OptPmtTime_1stguess[k]*OptPmtTime_1stguess[k]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         double y_k = OptPmtPos_1stguess[k].Y()*(OptPmtTime_1stguess[i]*OptPmtTime_1stguess[i]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         sum_y += weight_pmt*(y_i+y_k);         
         double z_i = OptPmtPos_1stguess[i].Z()*(OptPmtTime_1stguess[k]*OptPmtTime_1stguess[k]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         double z_k = OptPmtPos_1stguess[k].Z()*(OptPmtTime_1stguess[i]*OptPmtTime_1stguess[i]-OptPmtTime_1stguess[i]*OptPmtTime_1stguess[k]);
         sum_z += weight_pmt*(z_i+z_k);        
         //direction         
         sum_px += weight_pmt*(OptPmtPos_1stguess[i].X()-OptPmtPos_1stguess[k].X())*(OptPmtTime_1stguess[i]-OptPmtTime_1stguess[k]);
         sum_py += weight_pmt*(OptPmtPos_1stguess[i].Y()-OptPmtPos_1stguess[k].Y())*(OptPmtTime_1stguess[i]-OptPmtTime_1stguess[k]);
         sum_pz += weight_pmt*(OptPmtPos_1stguess[i].Z()-OptPmtPos_1stguess[k].Z())*(OptPmtTime_1stguess[i]-OptPmtTime_1stguess[k]);
      }
   }

   //if(sum_denominator==0.) cout<<" ------ Warning! sum_denominator=0. !!! ------: firedPmtN is "<<firedPmtN<<endl;

   double ini_x_tmp = sum_x/sum_denominator;
   double ini_y_tmp = sum_y/sum_denominator;
   double ini_z_tmp = sum_z/sum_denominator;
   double Px = sum_px/sum_denominator;
   double Py = sum_py/sum_denominator;
   double Pz = sum_pz/sum_denominator;

   ini_v = TMath::Sqrt(Px*Px+Py*Py+Pz*Pz);
   ini_E = MASS/TMath::Sqrt(1-(ini_v*ini_v)/(V_c*V_c));
   ini_lx = Px/ini_v;
   ini_my = Py/ini_v; 
   ini_nz = -Pz/ini_v*TMath::Sign(1.,Pz/ini_v); 
   
   //if(ini_nz>COSTHETA){//trick for 90°~boundary(97°) 
   //   ini_lx *=rec_lx_last;
   //   ini_my *=rec_my_last;
   //}   
   //ini_nz = -TMath::Sqrt(1.-ini_lx*ini_lx-ini_my*ini_my);

   //reference point
   TVector3 gen_tmp(ini_x_tmp,ini_y_tmp,ini_z_tmp);// mu position vector at 0 moment
   TVector3 gen_dir(ini_lx,ini_my,ini_nz); if(gen_dir.Mag()) gen_dir.SetMag(1.);
   ini_x = ini_x_tmp;
   ini_y = ini_y_tmp;
   ini_z = ini_z_tmp;
   cout<<"start_ini:("<<ini_x<<", "<<ini_y<<", "<<ini_z<<")  dir_ini:("<<gen_dir.X()<<", "<<gen_dir.Y()<<", "<<gen_dir.Z()<<")"<<endl;

   //ini_theta = 180.-TMath::ACos(ini_nz)*180./TMath::Pi(); //latitude 
   //if(isnan(ini_theta)) cout<<"Warning! ini_theta : "<<ini_theta<<endl;
   //ini_phi = TMath::ATan2(ini_my,ini_lx)*180./TMath::Pi(); //longitude
   //double phi_tmp = direction_line.Phi()/TMath::Pi()*180.;
   //ini_difPhi = ini_phi - phi_tmp;
   //if(TMath::Abs(ini_difPhi)>180.) ini_difPhi -= TMath::Sign(360.,ini_difPhi);
}

//get the weight of each OptPmtW in terms of OptPmtN
void Recon::Weight_OLS(std::vector<double> &OptPmtW_Pe, int OptPmt_N){
   if( fitN_stop1 == 0 ){
      rec_point.SetXYZ(ini_x,ini_y,ini_z);
      rec_dir.SetXYZ(ini_lx,ini_my,ini_nz); if(rec_dir.Mag()) rec_dir.SetMag(1.);
   }
   else{
      rec_point.SetXYZ(rec_x,rec_y,rec_z);
      rec_dir.SetXYZ(rec_lx,rec_my,rec_nz); if(rec_dir.Mag()) rec_dir.SetMag(1.);
   }
   TVector3 X1_tmp, X2_tmp;
   trackL = hitSphereAll(SphereRadius, pool_center, rec_point, rec_dir, X1_tmp, X2_tmp, Dis_center);
   std::vector<double> OptPmtPe_OLS;

   for(int i=0; i<OptPmt_N; i++){
      TVector3 dir_max_nPMTs(OptPmtPos[i].X()-X1_tmp.X(), OptPmtPos[i].Y()-X1_tmp.Y(), OptPmtPos[i].Z()-X1_tmp.Z());
      TVector3 dir_min_nPMTs(OptPmtPos[i].X()-X2_tmp.X(), OptPmtPos[i].Y()-X2_tmp.Y(), OptPmtPos[i].Z()-X2_tmp.Z());

      //Judge water layer cherenkov angle
      double angle_max_nPMTs = rec_dir.Angle(dir_max_nPMTs);
      double angle_min_nPMTs = rec_dir.Angle(dir_min_nPMTs);
      if(X1_tmp.Mag()==0 || X2_tmp.Mag()==0){
         OptPmtPe_OLS.push_back(OptPmtPe[i]); continue;
      }

      //Judge LS occlusion
      double Dis2 = sqrt((OptPmtPos[i].X()-X1_tmp.X())*(OptPmtPos[i].X()-X1_tmp.X())+(OptPmtPos[i].Y()-X1_tmp.Y())*(OptPmtPos[i].Y()-X1_tmp.Y())+(OptPmtPos[i].Z()-X1_tmp.Z())*(OptPmtPos[i].Z()-X1_tmp.Z())); //Distance between incident point and fired PMT
      double Dis3 = ((OptPmtPos[i]-rec_point).Cross(rec_dir)).Mag()/rec_dir.Mag(); //Distance between fired PMT and MU track
      double Dis4 = sqrt(Dis2*Dis2-Dis3*Dis3);
      double Dis6 = Dis4-Dis3/tan(thetac);

      TVector3 dir_cherenkov_nPMTs;
      dir_cherenkov_nPMTs = OptPmtPos[i] - (Dis6*rec_dir.Unit() + X1_tmp);
      double Dis7 = ((pool_center-OptPmtPos[i]).Cross(dir_cherenkov_nPMTs)).Mag()/dir_cherenkov_nPMTs.Mag();

      if(angle_max_nPMTs>thetac || angle_min_nPMTs<thetac){
         OptPmtPe_OLS.push_back(0);
      } 
      else{
         if(Dis3>2250 && Dis7<17700) OptPmtPe_OLS.push_back(0);
         else{
            OptPmtPe_OLS.push_back(OptPmtPe[i]);
         }
      }

      if(angle_max_nPMTs>thetac || angle_min_nPMTs<thetac){
         if(Dis3>2250 && Dis7<17700) OptPmtPe_OLS.push_back(0);
         else{
            OptPmtPe_OLS.push_back(OptPmtPe[i]);
         }
      } 
      else{
         OptPmtPe_OLS.push_back(OptPmtPe[i]);
      }
   }

   OptPmtW_Pe.clear();
   double sum_charge = 0.;
   int WEIGHT = WEIGHT_IRLS;

   if(WEIGHT==0){
      sum_charge = 1.; cout<<"Warning! WEIGHT==0"<<endl;
   }
   else{	 
      for(int i=0; i<OptPmt_N; i++){
         sum_charge += pow(OptPmtPe_OLS[i],WEIGHT);
      }
   }

   for(int i=0; i<OptPmt_N; i++){
      OptPmtW_Pe.push_back( pow(OptPmtPe_OLS[i],WEIGHT) /sum_charge );
   }
}

void Recon::Fit_minuit(TMinuit *recMinuit){
   double ini_x0, ini_y0;
   double ini_lx0, ini_my0;

   //Initialize
   if( fitN_stop1 == 0 ){
      TVector3 gen_tmp(ini_x, ini_y, ini_z);
      TVector3 gen_dir(ini_lx,ini_my,ini_nz); if(gen_dir.Mag()) gen_dir.SetMag(1.);
      TVector3 ini_refer;
      Refer_point(ini_refer,gen_tmp,gen_dir); //Where muon intersects the sphere

      ini_x0  = ini_refer.X(); ini_y0  = ini_refer.Y();
      ini_lx0 = gen_dir.X();   ini_my0 = gen_dir.Y();
   }
   else{
      ini_x0  = rec_x;  ini_y0  = rec_y;
      ini_lx0 = rec_lx; ini_my0 = rec_my;

      //double ini_nz0 = -TMath::Sqrt(1.-rec_lx*rec_lx-rec_my*rec_my);
      //if(ini_nz0>COSTHETA && icstat==2){
	   //   ini_lx0 *=rec_lx;
	   //   ini_my0 *=rec_my;
      //}
   }

   bool b_minuit = true;
   int fitN_minuit = 0;
   double temp_x0  = ini_x0,  temp_y0  = ini_y0; 
   double temp_lx0 = ini_lx0, temp_my0 = ini_my0;
   do{
      if( fitN_minuit > 0 ){ //fitN_minuit for the results converged
         do{
            ini_x0 = gRandom->Uniform(temp_x0-3000, temp_x0+3000); 
            ini_y0 = gRandom->Uniform(temp_y0-3000, temp_y0+3000);
            ini_lx0 =gRandom->Uniform(temp_lx0-0.3, temp_lx0+0.3);
            ini_my0 =gRandom->Uniform(temp_my0-0.3, temp_my0+0.3);
         }while( 1.-ini_lx0*ini_lx0-ini_my0*ini_my0 <= 0. );

         //double ini_nz02 =-TMath::Sqrt(1.-ini_lx0*ini_lx0-ini_my0*ini_my0);
         //if( ini_nz02 >COSTHETA && status ==0 ){
         //   ini_lx0 *=ini_lx0;
         //   ini_my0 *=ini_my0;
         //}
      }

      recMinuit->mncler();//Resets the parameter list to UNDEFINED
      recMinuit->SetObjectFit(this);//data must be in 1 TObject
      recMinuit->SetFCN(Fcn);//Set fonction pointer
      int ierflg=0;//indicate the state of command below
      double arglist[2];//control output list

      //Initial parameter 
      recMinuit->mnparm(0,"x", ini_x0,  10,   ini_x0-3000, ini_x0+3000, ierflg);
      recMinuit->mnparm(1,"y", ini_y0,  10,   ini_y0-3000, ini_y0+3000, ierflg);
      recMinuit->mnparm(2,"lx",ini_lx0, 0.01, ini_lx0-0.2, ini_lx0+0.2, ierflg);
      recMinuit->mnparm(3,"my",ini_my0, 0.01, ini_my0-0.2, ini_my0+0.2, ierflg);

      //Set Print Level (-1: no output; 1: standard output) 
      recMinuit->SetPrintLevel(-1);

      recMinuit->mnexcm("SET NOW",arglist,1,ierflg);//No Warnings
      //Minimization strategy
      arglist[0] = 1;
      recMinuit->mnexcm("CALL FCN",arglist,1,ierflg);
      arglist[0] = 2; //(1 standard; 2 try to improve minimum (slower) )
      recMinuit->mnexcm("SET STR",arglist,1,ierflg);

      recMinuit->SetErrorDef(1);//set Error definition (1 for Chi square; 0.5 for negative log likelihood)
      double maxCalls = 10000;//Call Migrad with maxCalls iterations maximum
      double tolerance = 1.0e-3;
      arglist[0] = maxCalls;
      arglist[1] = tolerance;

      recMinuit->mnexcm("MIGRAD",arglist,2,ierflg);
      double ln0,edm,errdef;
      int nvpar,nparx;
      recMinuit->mnstat(ln0,edm,errdef,nvpar,nparx,icstat);//Get minimum log likelihood
      chi2 = ln0;
      
      if(recMinuit->fCstatu == "FAILED    "){
         status = 0;
      }
      if(recMinuit->fCstatu == "SUCCESSFUL"){
         status = 1;
      }
      if(recMinuit->fCstatu == "CONVERGED "){
         status = 2;
      }

      fitN_minuit++;
      bool b_fitN_minuit = (fitN_minuit >= FIT_N_MINUIT);
      bool b_status = status > 0 ;
      if( b_fitN_minuit || b_status ) b_minuit = false;
   }while(b_minuit);
   
   //Reconstruction parameter
   recMinuit->GetParameter(0,rec_x, minuit_x_err);
   recMinuit->GetParameter(1,rec_y, minuit_y_err);
   recMinuit->GetParameter(2,rec_lx,minuit_lx_err);
   recMinuit->GetParameter(3,rec_my,minuit_my_err);
   
   //trick for initial value to the boundary 90;
   //rec_lx_last = rec_lx;
   //rec_my_last = rec_my;
}

void Recon::rec_fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){ //for TMinuit
   //track parameter
   double x0 = par[0]; double y0 = par[1];
   double lx = par[2]; double my = par[3];

   double nz = -TMath::Sqrt(1.-lx*lx-my*my);
   double z0 = -(lx*x0+my*y0)/nz;//Refer_T0
   //double z0 = TMath::Sqrt(SphereRadius*SphereRadius - x0*x0 - y0*y0);

   TVector3 p_line(x0,y0,z0);
   TVector3 d_line(lx,my,nz);
   double Chisq = 0.;
   double t0 = Refer_T0(OptPmtPos,OptPmtTime,OptPmtW,p_line,d_line);
   //double t0 = earliestFHT;

   //Chi function
   for(int i=0; i<OptPmtN; ++i){
      TVector3 pick_pmt(OptPmtPos[i].X(),OptPmtPos[i].Y(),OptPmtPos[i].Z());
      double time = T_Residual(OptPmtTime[i],t0,pick_pmt,p_line,d_line); //cout<<"time:"<<time<<endl;
      //Chisq += OptPmtW[i]*time*time/OptPmtRes[i]/OptPmtRes[i];
      Chisq += OptPmtW[i]*time*time;

      //double q = Q_Residual();
      //q_chisq = ...;
      //chisq_total = Chisq + q_chisq;
   }
   f = Chisq;
   //cout<<"f: "<<f<<'\t'<<"point: "<<p_line(0)<<" "<<p_line(1)<<" "<<p_line(2)<<" "\
   //<<90-p_line.Theta()*180/TMath::Pi()<<" "<<p_line.Phi()/TMath::Pi()*180.<<'\t'\
   //<<"dir: "<<d_line(0)<<" "<<d_line(1)<<" "<<d_line(2)<<endl;
}

//calculate the weight(vweight) for the next fit
std::vector<double> Recon::Weight_IRLS(){
   std::vector<double> vweight;
   v_residual.clear(); //absolute value of time residual
   v_sign_residual.clear();

   Sort_TimeResidual(v_residual,v_sign_residual); //v_residual: time residual
   double mad = MAD_Residual(v_residual)*C_TUNING;

   std::vector<double> OptPmtWPe;
   Weight_OLS(OptPmtWPe,OptPmtN);

   //biweight
   for(int i=0; i<OptPmtN; i++){
      if( v_residual[i] > mad ){
         vweight.push_back(1e-8);
      }
      else{
         bool b_biweight1 = v_sign_residual[i]==-1 && v_residual[i]<LIMIT_LOW;
         bool b_biweight2 = v_sign_residual[i]==1  && v_residual[i]<LIMIT_HIGH;
         if(b_biweight1 || b_biweight2){
            double weight = OptPmtWPe[i];
            vweight.push_back(weight);
         }
         else{
            double weight = OptPmtWPe[i]*pow(1.-(v_residual[i]/mad)*(v_residual[i]/mad),2);
            vweight.push_back(weight);
         }
      }
   }
   return vweight;
}

//Process weight according to PMTID
void Recon::unpack( vector<double>& weight_unsorted, vector<double>const& weight ){ 
   weight_unsorted.assign( PMTSUM, 0 ); //weight_unsorted are assigned to 0

   for(int i=0; i<PMTSUM; ++i){
      weight_unsorted[i] = weight[i];
   }
}

double Recon::delta_weight( vector<double>& weight_unsorted, vector<double>& weight_unsorted_last ){
   double sum = 0.;
   double last_sum = 0.;
   
   for(int i=0; i<PMTSUM; ++i){
      sum += fabs( weight_unsorted_last[i]-weight_unsorted[i] );
      last_sum += weight_unsorted_last[i];
   }//cout<<"last_sum: "<<last_sum<<endl;

   if(last_sum==0.) return 1.;
   else return sum/last_sum;
}

void Recon::Throw_out(){
   int n_residual = (int)v_residual.size();
   int n_throw = 0;
   for(int i=0; i<n_residual; i++){
      if(v_residual[n_residual-1-i]>T_RESIDUAL){
         n_throw++;
         if(n_throw>THROWOUT_ONCE) break;
         OptPmtTime.pop_back();
         OptPmtPe.pop_back();
         OptPmtRes.pop_back();
         OptPmtPos.pop_back();
         OptPmtW.pop_back();
      }
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//求过原点平面上一点refer_point，两点refer_point~point_line构成向量与direction_line平行
void Recon::Refer_point(TVector3 &refer_point, TVector3 &point_line, TVector3 &direction_line){
   //point on line 
   double x0 = point_line.x(); 
   double y0 = point_line.y(); 
   double z0 = point_line.z();
   //line direction
   double lx = direction_line.x(); 
   double my = direction_line.y(); 
   double nz = direction_line.z();
   //plane through the origin：lx*x+my*y+nz*z=0
   double tmp = - (lx*x0+my*y0+nz*z0);
   //refer point
   refer_point.SetX(lx*tmp+x0);
   refer_point.SetY(my*tmp+y0);
   refer_point.SetZ(nz*tmp+z0);
}

double Recon::Di_line(TVector3 &point, TVector3 &point_line, TVector3 &direction_line){//Vector product, cross product
   //point out of line
   double x = point.x();
   double y = point.y();
   double z = point.z();
   //point on line 
   double x0 = point_line.x(); 
   double y0 = point_line.y(); 
   double z0 = point_line.z();
   //line direction
   double lx = direction_line.x(); 
   double my = direction_line.y(); 
   double nz = direction_line.z();
   
   double d1 =(my*(z-z0)-nz*(y-y0))*(my*(z-z0)-nz*(y-y0));
   double d2 =(nz*(x-x0)-lx*(z-z0))*(nz*(x-x0)-lx*(z-z0));
   double d3 =(lx*(y-y0)-my*(x-x0))*(lx*(y-y0)-my*(x-x0));

   double di =TMath::Sqrt(d1+d2+d3);
   return di;
}

double Recon::Li_line(TVector3 &point, TVector3 &point_line, TVector3 &direction_line){//Dot product
   //point out of line 
   double x = point.x(); 
   double y = point.y(); 
   double z = point.z();
   //point on line 
   double x0 = point_line.x(); 
   double y0 = point_line.y(); 
   double z0 = point_line.z();
   //line direction
   double lx = direction_line.x();
   double my = direction_line.y(); 
   double nz = direction_line.z();

   double li = (x-x0)*lx+(y-y0)*my+(z-z0)*nz;
   return li;
}

// ======== calculate the t0 reference time ========
double Recon::Refer_T0(std::vector<TVector3>& OptPmt_Pos, std::vector<double>& OptPmt_Time, std::vector<double>& OptPmt_W, TVector3 &point_line, TVector3 &direction_line){
   double t0 = 0., wt = 0.;
   int PmtNum = (int) OptPmt_Pos.size();
   if(PmtNum!=OptPmtN) cout<<"Warning!!! PmtNum!=OptPmtN"<<endl;
   
   for(int i=0; i<PmtNum; i++){
      TVector3 pick_pmt(OptPmt_Pos[i].X(),OptPmt_Pos[i].Y(),OptPmt_Pos[i].Z());

      double Li = Li_line(pick_pmt,point_line,direction_line);
      double Di = Di_line(pick_pmt,point_line,direction_line);
      double t_expec = ExpectedTime(Li,Di);

      t0+= OptPmt_W[i]*(OptPmt_Time[i]-t_expec);
      wt+= OptPmt_W[i];
   }
   t0/=wt;
   return t0;
}

double Recon::T_Residual(double OptPmt_Time, double t0, TVector3 point, TVector3 point_line, TVector3 direction_line){
   double Li = Li_line(point,point_line,direction_line);
   double Di = Di_line(point,point_line,direction_line);
   double t_theory = t0+ExpectedTime(Li,Di);

   double residual = OptPmt_Time-t_theory;
   return residual; 
}

//PMT receiving mu emitting Cherenkov photon at t0(weighted average of time residuals)
double Recon::ExpectedTime(double Li, double Di){
   double time = (Li+Di*TMath::Tan(thetac))/V_muon/1000.;
   return time;
}

#include <numeric>
//Sort in Time residual array according to the rec track, each IRLS this should be a new data
void Recon::Sort_TimeResidual(std::vector<double>& vi_residual, std::vector<int>& vi_sign_residual){

   //from small to big: OptPmtPe, OptPmtTime, OptPmtRes, OptPmtPos, vi_residual, vi_sign_residual
   double refer_x0 = rec_x;
   double refer_y0 = rec_y;
   double lx = rec_lx;
   double my = rec_my;

   double nz = -TMath::Sqrt(1.-lx*lx-my*my);
   double refer_z0 = -(lx*refer_x0+my*refer_y0)/nz; //Refer_T0
   //double refer_z0 = TMath::Sqrt(SphereRadius*SphereRadius - refer_x0*refer_x0 - refer_y0*refer_y0);

   TVector3 p_line(refer_x0,refer_y0,refer_z0);
   TVector3 d_line(lx,my,nz);
   double t0 = Refer_T0(OptPmtPos,OptPmtTime,OptPmtW,p_line,d_line);
   //double t0 = earliestFHT;

   //calculate di_plane and vi_residual and tag the sign of the time residual
   for(int i=0; i<OptPmtN; i++){
      TVector3 pmt_pos(OptPmtPos[i].X(),OptPmtPos[i].Y(),OptPmtPos[i].Z());
      double time = T_Residual(OptPmtTime[i],t0,pmt_pos,p_line,d_line);
      if(time>0.) vi_sign_residual.push_back(1);
      else if(time<0.) vi_sign_residual.push_back(-1);
      else cout<<"Warning! AMazing: Time Residual == 0 !"<<endl;

      vi_residual.push_back(fabs(time));
   }

   //sort bubble method
   double tpe, tt, tres, t_residual;
   int tid, t_sign;
   int Num = OptPmtN-1;
   TVector3 PMTPos;

   for(int j=0; j<Num; j++){
      for(int i=0; i<Num-j; i++){
         if(vi_residual[i]>vi_residual[i+1]){
            tpe        = OptPmtPe[i];
            tt         = OptPmtTime[i];
            tres       = OptPmtRes[i];
            PMTPos.SetXYZ(OptPmtPos[i].X(),OptPmtPos[i].Y(),OptPmtPos[i].Z());
            t_residual = vi_residual[i];
            t_sign     = vi_sign_residual[i];
            OptPmtPe[i]         = OptPmtPe[i+1];
            OptPmtTime[i]       = OptPmtTime[i+1];
            OptPmtRes[i]        = OptPmtRes[i+1];
            OptPmtPos[i]        = OptPmtPos[i+1];
            vi_residual[i]      = vi_residual[i+1];
            vi_sign_residual[i] = vi_sign_residual[i+1];   
            OptPmtPe[i+1]         = tpe;
            OptPmtRes[i+1]        = tres;
            OptPmtTime[i+1]       = tt;
            OptPmtPos[i+1].SetXYZ(PMTPos.X(),PMTPos.Y(),PMTPos.Z());
            vi_residual[i+1]      = t_residual;
            vi_sign_residual[i+1] = t_sign;
         }
      }
   }

   for(int i=0; i<Num; i++){
      //cout<<"After residual:  "<<vi_residual[i]<<"   "<<vi_sign_residual[i]<<endl;
      assert( vi_residual[i] <= vi_residual[i+1] );
   }
}

double Recon::MAD_Residual(std::vector<double>& vresidual){
   int Num = (int) vresidual.size();
   double median = 0.;

   if(Num%2==0) median = 0.5*(vresidual[Num/2-1]+vresidual[Num/2]);
   else median = vresidual[(Num-1)/2];
   return median;
}

double Recon::Second_Dvalue(double a1,double a2,double b1,double b2){
   // |a1 b1|
   // |a2 b2|
   double value = a1*b2-a2*b1;
   return value;
}

//For judge the rec state without know the true rec track
double Recon::Di_plane(double *parline, double *parpoint){
   //Ax+By+C=0
   double line_A = parline[0];
   double line_B = parline[1];
   double line_C = parline[2];

   double x0 = parpoint[0];
   double y0 = parpoint[1];

   double di = fabs(line_A*x0+line_B*y0+line_C)/TMath::Sqrt(line_A*line_A+line_B*line_B);
   return di;
}

double Recon::hitSphereAll(Double_t tarR, TVector3 &center, TVector3 &vpos, TVector3 &dir, TVector3 &X1, TVector3 &X2, Double_t &L){
   double R = tarR;
   double trackL = 0.;
   double x1=0., y1=0., z1=0., x2=0., y2=0., z2=0.;
   double x0 = vpos.X(), y0 = vpos.Y(), z0 = vpos.Z();
   double px = dir.X(), py =dir.Y(), pz = dir.Z();
   double xc = center.X(), yc = center.Y(), zc = center.Z();
   double a = px*px + py*py + pz*pz;
   double b = 2*(px*(x0-xc) + py*(y0-yc) + pz*(z0-zc));
   double c = (x0-xc)*(x0-xc) + (y0-yc)*(y0-yc) + (z0-zc)*(z0-zc) - R*R;
   double delta = b*b - 4*a*c;   //cout<<" delta: "<<delta<<endl;
   double k1=0, k2=0;
   //if(pz > 0){ cout<<"WARNING: muon up going!! pz >= 0: "<<pz<<endl; }
   //if(pz == 0){ cout<<"WARNING: muon flat going!! pz == 0: "<<pz<<endl;}
   if (delta < 0) {trackL = 0.; x1 = 0.; y1 = 0.; z1 = 0.; x2 = 0.; y2 = 0.; z2 = 0.; //cout<<" delta < 0"<<endl;
   }
   else if(delta == 0){//tangency with sphere
      k1 = -b/(2*a);
      x1 = px * k1 + x0;
      y1 = py * k1 + y0;
      z1 = pz * k1 + z0;
      x2 = x1; y2 = y1; z2 = z1;
      trackL = 0.;  //cout<<" delta = 0 "<<endl;
   }
   else{ //delta > 0
      k1 = (-b + sqrt(delta))/(2*a);
      k2 = (-b - sqrt(delta))/(2*a);
      z1 = pz * k1 + z0;
      z2 = pz * k2 + z0;
      if(z1<z2){ double kk = k1; k1 = k2; k2 = kk; }
      x1 = px * k1 + x0;
      y1 = py * k1 + y0;
      z1 = pz * k1 + z0;
      x2 = px * k2 + x0;
      y2 = py * k2 + y0;
      z2 = pz * k2 + z0;   
      trackL = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
      //L = sqrt( ((y0*pz-z0*py)*(y0*pz-z0*py)+(x0*pz-z0*px)*(x0*pz-z0*px)+(x0*py-y0*px)*(x0*py-y0*px)) / (px*px+py*py+pz*pz) );
      //cout<<" delta > 0 "<<endl;
      L = ((center-vpos).Cross(dir)).Mag()/dir.Mag(); //center distance to line
      //double tL = 2*sqrt(R*R-L*L);
      //cout<<" tL : "<<tL <<endl;
   }   
   X1.SetXYZ(x1,y1,z1);
   X2.SetXYZ(x2,y2,z2);
   //cout<<" track L: "<<trackL<<endl; 
   //cout<<" hit points: xyz1: ("<<x1<<", "<<y1<<", "<<z1<<")"<<endl; 
   //cout<<" hit points: xyz2: ("<<x2<<", "<<y2<<", "<<z2<<")"<<endl; 
   return trackL;
}