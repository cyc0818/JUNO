#include "AbcBoard.h"

#include <iostream>
#include <math.h>
#include <climits>
#include "TRandom3.h"
#include "TF1.h"

AbcBoard::AbcBoard(const int num, SpmtElecConfigSvc* spmtSvc ) : boardNum(num), m_SpmtConfigSvc(spmtSvc) {}
AbcBoard::~AbcBoard(){}

void AbcBoard::initialize() {
    f2 = new TF1("f2","1 - 1. / (1 + exp((x-[0])/[1]))", 0, 150);
    func1 = new TF1("func1", "gaus(0)*(x<[4]) + pol0(3)*(x>=[4])",0,1000);
    gauss_func = new TF1("gauss_func","1/([1]*sqrt(2*TMath::Pi()))*exp(-0.5*pow((x-[0])/[1],2))",0,10);
    model_func = new TF1("model_func","[0]*exp(-0.5*((x-[1])/[2])**2)",-100,100);
}

void AbcBoard::delFunc() {
    delete f2;
    delete func1;
    delete gauss_func;
    delete model_func;
}

void AbcBoard::pushPulses(int channel, std::vector<Pulse*> pulses)
{
  if(pulsesMap.find(channel)==pulsesMap.end())
    pulsesMap.insert(std::make_pair(channel,pulses));
  else //append at the end
    for(int i=0; i<pulses.size(); i++) pulsesMap.find(channel)->second.push_back(pulses[i]);
}

void AbcBoard::AddTruth(JM::CdSpmtElecTruthEvt* spmtElecTruthEvent, std::vector<Pulse*>::iterator aPulse){
  if (!spmtElecTruthEvent) return; 
  Pulse a_Pulse = *(*aPulse);
  if(a_Pulse.truth)
  {
  	JM::SpmtElecTruth aTruth;
  	if((a_Pulse.truth)->m_entry_pointer) aTruth.setTagId((a_Pulse.truth)->m_entry_pointer->tag);
  
	if(a_Pulse.type == kNormalPulse) {aTruth.setPulsetype(a_Pulse.truth->m_entry_pointer->tag); }
	else if(a_Pulse.type == kDarkPulse) {aTruth.setPulsetype("DarkPulse"); }
	else if(a_Pulse.type == kAfterPulse) {aTruth.setPulsetype("AfterPulse"); }
	else if(a_Pulse.type == kDNAfterPulse) {aTruth.setPulsetype("DNAfterPulse");}
        else{
                	LogError << "Unknown pulse type!" << std::endl;
                	return;
            	    }
	aTruth.setPmtId(a_Pulse.pmtID);      
  	aTruth.setNpe(a_Pulse.truth->npe);          
  	aTruth.setHitTime(a_Pulse.truth->hitTime);      
  	aTruth.setTimeWindow(a_Pulse.truth->timeWindow);
  	aTruth.setAmplitude(a_Pulse.amplitude);
  	aTruth.setTTS(a_Pulse.truth->TTS);          
  	aTruth.setPulseHitTime(a_Pulse.pulseHitTime);
  	aTruth.setEvtHitTime(a_Pulse.evtTimeStamp);
  	spmtElecTruthEvent->AddTruth(aTruth);
  }
}


void AbcBoard::process( JM::CdSpmtElecEvt* spmtEvent, JM::CdSpmtElecTruthEvt* spmtElecTruthEvent)
{
  //if(pulsesMap.size()>0)  std::cout << " AbcBoard::process " << boardNum << " " << pulsesMap.begin()->first << std::endl;
  //if(pulsesMap.size()>0)  std::cout << " AbcBoard::process " << boardNum << " " << pulsesMap.begin()->first << std::endl;
  try{

    std::map< int, std::vector<Pulse*> >::iterator it;
    for( it = pulsesMap.begin(); it != pulsesMap.end(); ++it ){
      // double COARSE_TIME_STEP = m_SpmtConfigSvc->getDouble("COARSE_TIME_STEP");
      //int MAX_COARSE_TIME = m_SpmtConfigSvc->getInt("MAX_COARSE_TIME"); 

      int PC2ADC_THRES = m_SpmtConfigSvc->getInt("PC2ADC_THRES");      
      double PC2ADC_HG = m_SpmtConfigSvc->getDouble("PC2ADC_HG");     
      double PC2ADC_LG = m_SpmtConfigSvc->getDouble("PC2ADC_LG");     
      double Const_HG = m_SpmtConfigSvc->getDouble("Const_HG");      
      double Const_LG = m_SpmtConfigSvc->getDouble("Const_LG");

      double ChargeToAmp = 4.81;
      double ChargeAmpOffset=0.66;

      unsigned int channel_index=0;
      unsigned int channel_mask = 0xFF<<channel_index;  //128 from 0 to 127 (8  bits)
           
      unsigned int abc_index=8;  
      unsigned int abc_mask = 0xFF<<abc_index;  //200 from 0 to 199 (8 bits)
      
      //int channel = it->first;
      std::vector<Pulse*> pulses = it->second;
      std::vector<Pulse*>::iterator aPulse;
      JM::SpmtElecAbcBlock aBlock;

      std::vector<double> true_ti;
      true_ti.clear();
      std::vector<double> true_qi;
      true_qi.clear();
      std::vector<double> trig_qi;
      trig_qi.clear();
      std::vector<double> trig_ti;
      trig_ti.clear();
      std::vector<double> dds_ti;
      dds_ti.clear();

      // calculate block charge and time
         int CHH = 0;
      int channelNb=0;
      int ABCNb=0;
      bool WrongPM=false;

      // TRandom3* rnd = new TRandom3(0);
      TRandom* rnd = gRandom;
      float Prob_previousHit=0.006; //(600Hz times 10 microsec)   
      double threshold = 3.47/3.; //1/3 p.e.     

      double t0;
      int qi_temp;
      int icount(0);
      for( aPulse = pulses.begin(); aPulse != pulses.end(); ++aPulse ){
	/*
          if((*aPulse)->pulseHitTime > (*aPulse)->evtTimeStamp) {
              if (spmtElecTruthEvent) {
                  AddTruth(spmtElecTruthEvent, aPulse);
              }
          }
	*/
	//if((*aPulse)->pulseHitTime > (*aPulse)->evtTimeStamp || aPulse==pulses.end()-1)
	  {
	    AddTruth(spmtElecTruthEvent, aPulse);
	    double AbsTime=((*aPulse)->pulseHitTime-(*aPulse)->evtTimeStamp).GetNanoSec();
	    icount+=1;
	    if(icount==1)
	      {      
		float proba=rnd->Rndm();
		
		if(proba<Prob_previousHit) 
		  {
		    float previousTime=-rnd->Rndm()*10000.;
		    true_ti.push_back(previousTime); //previous time negative from previous trigger (test ping/pong availability)
		    true_qi.push_back(1); //1 p.e.
		  }
		t0=AbsTime;	
		qi_temp=0;
		CHH = m_SpmtConfigSvc->getChannel((*aPulse)->pmtID); 
		channelNb=((CHH&channel_mask)>>channel_index);
		ABCNb=((CHH&abc_mask)>>abc_index);
		//if(CHH==0&&aPulse->pmtID!=300000)
		  if(CHH==0&&(*aPulse)->pmtID!=313199) 
		  WrongPM=true;
	      }
	    
	    if((AbsTime-t0)>=0&&(AbsTime-t0)<=10.) qi_temp++; //clusterisation of signal in less than 10 ns

	    if((AbsTime-t0)>10)
	      {
		double random_charge=GetRandomCharge(qi_temp);
		double random_amplitude=ChargeToAmp*random_charge+ChargeAmpOffset;
		if(random_amplitude>threshold)
		  {
		    double estim_time=GetTime(random_amplitude,t0,threshold);
		    if(estim_time>0)
		      {
			true_ti.push_back(estim_time);
			true_qi.push_back(random_charge);
		      }
		  }
		t0=AbsTime;
		qi_temp=1;
	      }	    
	    if(aPulse==pulses.end()-1)
	      {
		double random_charge=GetRandomCharge(qi_temp);
		double random_amplitude=ChargeToAmp*random_charge+ChargeAmpOffset;
		if(random_amplitude>threshold)
		  {
		    double estim_time=GetTime(random_amplitude,t0,threshold);
		    if(estim_time>0)
		      {
			true_ti.push_back(estim_time);
			true_qi.push_back(random_charge);
		      }
		  }
	      }
	  }
      }

      if (true_ti.empty()) {
          LogInfo << "Skip due to true_ti is empty. " << std::endl;
          continue;
      }

      double virtualTime=true_ti.at(true_ti.size()-1)+100.;  //virtual hit 100 ns after the last one to force trigger
      true_ti.push_back(virtualTime);
      true_qi.push_back(0.);

      if(WrongPM==true) continue;
      Apply_Catiroc(50,true_ti,true_qi,trig_qi,trig_ti,dds_ti); //50 for RC value (baseline), but it could be 100.

      std::vector<double> final_ti;
      final_ti.clear();
      std::vector<double> final_qi;
      final_qi.clear();
      double tempCharge;
      for(int i=0;i<dds_ti.size();i++)
	{
	  final_ti.push_back(dds_ti.at(i));
	  tempCharge=1025;  //in case no registration of charge in ping/pong
	  for(int k=0;k<trig_ti.size();k++)
	    {
	      if(trig_ti.at(k)==dds_ti.at(i)) tempCharge=trig_qi.at(k);
	    }
	  final_qi.push_back(tempCharge);
	}
      
      
      for(int i=0;i<final_ti.size();i++) 
	{
	  double charge=final_qi.at(i); 

	  if(charge<PC2ADC_THRES){
	    aBlock.setGain(true);  //high gain
	    charge=charge*PC2ADC_HG+Const_HG;
	  }
	  else{
	    aBlock.setGain(false);// low gain
	    if(charge!=1025) 
	      {
		charge=charge*PC2ADC_LG+Const_LG;
		if(charge>1024) charge=1024;
	      }
	  }
	  //--------------------------------------
	  // channel number
	  aBlock.setChNb(channelNb%16);
	  aBlock.setAsicNb((channelNb/16)%8);
	  aBlock.setCardNb(ABCNb);
	  
	  // coarse time
	  aBlock.setCoarseTime(final_ti.at(i)); 
	  // event counter
	  aBlock.setEventCounter(pulses.size());// 12bits
	  // fine time
	  aBlock.setFineTime(0.);//set to 0 for now
	  // charge
	  aBlock.setCharge(charge);//10bits
	  spmtEvent->AddAbcBlock(aBlock);
	}
    }
  }
  catch(std::out_of_range& e){
    LogError << e.what() << std::endl;
  }

}

void AbcBoard::Apply_Catiroc(int RC, std::vector<double> ti,  std::vector<double> qi,  std::vector<double> &trig_qi,  std::vector<double> &trig_ti, std::vector<double> &dds_ti)
{
  //  ti is the time-ordered vector of all event hits + generated hits in the windows [tmin, tmax]
  
  double Treadout = 9.4e3; //9.4 us = 9400 ns
  double qhit = 1; //charge of each single hit we assume here that each pe is a hit;
  
  // TRandom3* rnd = new TRandom3(0);
  TRandom* rnd = gRandom;
  double charge_last_hit = 0;
  
  bool is_ping = 0;
  double t_last_ping = - 10e3; //at least 10 us 
  double t_last_hit = -10e3;
  int i_last_ping = -1;
  int i_last_hit = -1;
  double dt = 0;
  
  //decide if first seen hit is ping or pong
  if (rnd->Rndm()>0.5)
    {
      is_ping = 1;
    }
  else 
    {
      is_ping = 0;
    }
  
  
  double charge_correction = 0;  
  int t0 = -1;
  bool triggered = 1;
  
  //start scan over t_i
  for (int i=0; i<ti.size();++i)
    {
      qhit = qi.at(i);
      //check time order
      if (i>0 && ti.at(i-1)>0 && ti.at(i) > 0 && ti.at(i) < ti.at(i-1))
	{
	  std::cout << "ERROR : VECTOR Ti NOT ORDERED " <<std::endl;
	  exit(0);
	}
      
      //define initial conditions in the case of DN (negative times) 
       dt = ti.at(i) - t_last_hit;   
       if (ti.at(i) < -10e3) continue; //skip any event before 10 us : they are not used here
       if (ti.at(i) >= -Treadout && ti.at(i)<0)
	 {
	   //check triggering hits --> all consecutive hits after first one
	   triggered  = trigger_deadtime_func(dt);
	   
	   if (!triggered) continue;
	   
	   //triggered hits 
	   
	   t_last_hit = ti.at(i);
	   i_last_hit = i;
	   charge_last_hit = qhit;
			
	   if (t_last_hit - t_last_ping > Treadout)
	     {
	       //digitised
	       if (is_ping)
		 {
		   is_ping = 0; //here we should include undershoot in Sniper for future studies. not needed here!
		 }
	       else
		 {
		   is_ping = 1;
		   t_last_ping = t_last_hit;
		   i_last_ping = i;
		 }
	     } //end digitisation
	   
	 } //end consecutive hits in DN
       
       if (ti.at(i) < 0) continue; //no other info needed: skip rdn events 
       if (t0<0)
	 {
	   t0 = ti.at(i);
	 }
       	    
       //-----------------------------
       //now starting with physics hits
       //-----------------------------
       
       triggered  = trigger_deadtime_func(dt);
	    
       if (!triggered)
	 {
	   charge_correction = Apply_ChargeAcceptance(RC, dt)*qhit;
	   //std::cout<<"charge_correction "<<charge_correction<<" dt "<<dt<<" RC "<<RC<<std::endl;
	   charge_last_hit += charge_correction;
	 }
       else
	 {
	   //event triggered
	   if(qi.at(i)!=0) 
	     dds_ti.push_back(ti.at(i));  //push back dds but not the virtual one which has qi=0
	   //push last hit 
	   if (t_last_hit >= 0 && charge_last_hit > 0) 
	     {
	       trig_qi.push_back(charge_last_hit);
	       trig_ti.push_back(t_last_hit);
	     }
	   
	   //update hits
	   if (ti.at(i) - t_last_ping > Treadout)
	     {
	       t_last_hit = ti.at(i); //t_last only assigned for events in the CDS
	       i_last_hit = i;
	       charge_last_hit = qi.at(i);
	       
	       if (is_ping) 
		 {
		   is_ping = 0; //here we may need to include undershoot in the future
		 }
	       else
		 {
		   is_ping = 1;
		   t_last_ping = t_last_hit;
		   i_last_ping = i;
		 }
	     }
	   else
	     {
	       //Deadtime 9us --> check well this case 
	       //event is triggered but charge is not measured. we suppose this hit goes to DDS. 
	       
	       charge_last_hit = 0;
	       t_last_hit = ti.at(i);
	       i_last_hit = i;
	     } 
	 }//end triggered events
       
    }//end for i
}

bool AbcBoard::trigger_deadtime_func(double t)
{ 
  
  
  double tmax = 150;
  double par1 = 79.4514 ;
  double par2 =  4.27045 ;
  double val = 0;
  bool triggered = 1;
  double rand = 0;
  // TRandom3* rnd = new TRandom3(0);
  TRandom* rnd = gRandom;
  
  if (t>tmax)
    {
      return 1; //hit is triggered
    }
  
  f2->SetParameters(par1,par2);
  val = f2->Eval(t);
  //simpler implementation :
  
  //rand = rnd->Rndm(); //extract a double [0,1]
  //if (rand < val) triggered = 1;
  //else triggered = 0 ;
       
  
  //or to avoid generating rand all the times 
  if (val < 1e-3)
    {
      triggered = 0;
    }
  else if (val >= 1e-3 && val < 0.999)
    {
      //extract rand only if needed 	
      rand = rnd->Rndm(); 
      if (rand < val)
	{
	  triggered = 1;
	}
      else
	{
	  triggered = 0;
	}
    }
  else if (val >= 0.999 && val < 1.0001)
    {
      triggered = 1;
    }
  else
    {
      std::cout << "ERROR trigger_prob must be <= 1" << std::endl;
      exit(0);
    }
  
  return triggered;
}

double AbcBoard::Apply_ChargeAcceptance(int RCval, double t)
{
  double p0 = 0 ;
  double p1 = 0 ;
  double val = 0;
  
  if (t>1000)
    {
      return 1;
    }
  
  if (RCval == 50)
    {
      p0=20.58;
      p1=-3.274;
    }
  else if (RCval == 100)
    {
      p0=41.33;
      p1=-7.594;
    } 
  else
    {
      std::cout << "ERROR!! RC value can only be 50 or 100 " << std::endl;
      return 1;
    }
  
  double par_exp = (t - p0) / p1;
  val = 1.0/(1.0 + exp(-par_exp));
  
  if (val>1)
    {
      val=1;
    }
  return val; 
}
double AbcBoard::GetRandomCharge(int qi_tmp) {
  double R_Spe = 33.2; 	//%
  double Q_Spe = 0.48; 	//pC
  double random_charge = qi_tmp;
   
   double Q_Npe = qi_tmp*Q_Spe;
   double Sigma_Npe = R_Spe/(100.*sqrt(qi_tmp))*Q_Npe;
   
   gauss_func->SetParameter(0,Q_Npe);
   gauss_func->SetParameter(1,Sigma_Npe);
   
   random_charge = gauss_func->GetRandom();
   if (random_charge < 0) random_charge = 0; 
   
   return random_charge;
}

double AbcBoard::GetTime(double random_amplitude, double t0, double threshold) {
  /*
  double tau = 0; 	//ns
  double sigma=3.0;
  if(random_amplitude<20.82)
    sigma = -0.13*random_amplitude+5.76;
  double StartWaveform=1E-2;
  
  model_func->SetParameter(0,random_amplitude);
  model_func->SetParameter(1,tau);
  model_func->SetParameter(2,sigma);
  double time_start=model_func->GetX(StartWaveform,-100.,tau+sigma);
  double time_threshold=model_func->GetX(threshold,-100.,tau+sigma);
  return t0 + time_threshold-time_start; 
  */
  //parametrization for a threshold at 1/3 p.e.
  double p0=1.456;
  double p1=-0.0619;
  double p2=2.709;
  double p3=-1.012;
  double MesureSpread = 0.65;
  double DeltaTime=exp(p0+p1*random_amplitude)+exp(p2+p3*random_amplitude);
  gauss_func->SetParameter(0,DeltaTime);
  gauss_func->SetParameter(1,MesureSpread);
  DeltaTime = gauss_func->GetRandom();

   return t0+DeltaTime;
  //return t0;



}
