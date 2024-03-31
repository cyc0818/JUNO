/*
WpPMT Calib


Author: Haoqi Lu(IHEP)
*/
#include "Event/WpWaveformHeader.h"
#include "Event/WpCalibHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"  
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/CdID.h"
#include "Geometry/WpGeom.h"
#include "Geometry/GeoUtil.h"
#include "Identifier/WpID.h"
//#include "DetSimAlg/IDetElement.h"
#include <vector>

#include "WpPMTCalibAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "Geometry/PMT.h"

/////////////////////////////////////////////


DECLARE_ALGORITHM(WpPMTCalibAlg);

WpPMTCalibAlg::WpPMTCalibAlg(const std::string& name)
	:AlgBase(name)
{
  // Set Default Values of Some Parameters


}

WpPMTCalibAlg::~WpPMTCalibAlg()
{

}

bool
WpPMTCalibAlg::initialize()
{
  
  LogInfo << objName() << " initialized successfully." << std::endl;
  
  SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
  if (rgSvc.invalid()) {
    LogError << "Failed to get RecGeomSvc instance!" << std::endl;
    return false;
  }

  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if( navBuf.invalid() )
    {
      LogError << "cannot get the NavBuffer @ /Event" << std::endl;
      return false;
    }
  m_buf = navBuf.data();


  //to be read from DB for real data conversion
  Q_Spe = 1.0; 
  Q_factor =50*1.6*9.12;// assumed 50ohm resister and 1.6pc for 1E10^7 gain for one electron charge 1.6E-19 C. add sacle factor 
  StartIdWppmt=kOFFSET_WP_PMT;
  TotalWppmt=2400;

#ifndef BUILD_ONLINE
  SniperPtr<RootWriter> Writesvc(*getRoot(),"RootWriter");
  if (Writesvc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
	     << "enalbe it in your job option file."
	     << std::endl;
    return false;
  }
  m_calib = Writesvc->bookTree(*getParent(), "USER_OUTPUT/wppmtcalibevt", "simple output of wp pmt calibration data");
  m_calib->Branch("EvtId",&m_EvtId,"EvtId/I");
  m_calib->Branch("PMTID", &m_pmtId,"PMTID/I");
  m_calib->Branch("Charge", &m_charge);
  m_calib->Branch("Time", &m_time);
  m_calib->Branch("PMTPE", &m_totalpe, "TotalPE/F");
#endif

  EvtID=0;
  
  return true;
}

bool 
WpPMTCalibAlg::execute()
{
  
  // preparation of the user output
  m_charge.clear();
  m_time.clear();
  m_pmtId = 0;
  m_totalpe = 0;
  
  
  
  auto nav = m_buf->curEvt();
  if (!nav) {
    LogError <<"Failed to get event navigator "<<std::endl;
    return false;
  }
  LogInfo <<"Nav pointer: "<<nav<<std::endl;
  
  auto eh = JM::getHeaderObject<JM::WpWaveformHeader>(nav);
  if (not eh) {
    LogInfo << "WpWaveformHeader is not available in this event. " << std::endl;
    return true;
  }

  if (!eh->hasEvent()) {
    LogInfo << "WpElecEvent is not available in this event. " << std::endl;
    return true;
  }
  
  std::cout<<"WpPMTElecEvent "<<eh->hasEvent()<<std::endl;
  auto ee  = dynamic_cast<JM::WpWaveformEvt*>(eh->event());
  
  
  
  
  auto calibheader = JM::getHeaderObject<JM::WpCalibHeader>(nav);
  if(!calibheader) {
      LogInfo <<"Initialize WpCalibHeader "<<std::endl;
      calibheader = new JM::WpCalibHeader();
      JM::addHeaderObject(nav, calibheader);
  } else {
      LogWarn <<"WpCalibHeader already exists"<<std::endl;
  }
  

  auto calibevent = dynamic_cast<JM::WpCalibEvt*>(calibheader->event());
  if(!calibevent) {
      LogInfo <<"Initialize WpCalibEvt "<<std::endl;
      calibevent = new JM::WpCalibEvt();
      calibheader->setEvent(calibevent);
  } else {
      LogWarn <<"WpCalibEvt already exists"<<std::endl;
  }

  int pmtID=0;  
  
  float nPE = 0, firstHitTime = 0;
  std::vector<double> charge;
  std::vector<double> time;
  
  double temp_Charge(0);
  std::vector <float> ChargePerPMT[TotalWppmt];
  std::vector <float> TimePerPMT[TotalWppmt];
  
   const auto& waveforms = ee->channelData();
   //const map<int,JM::ElecFeeChannel> channelData = crate->channelData();
//  LogInfo << "Elec FeeCreate -channel.Size()="<<waveforms.size()<<"----------------- in this event. " << std::endl;
  double temp_charge=0;
  double temp_pedestal=0;
  double Q_calib =Q_Spe*Q_factor;
  double calib_pe;
  double hit_time;
  double max_amp;
  double max_amp_tdc;
  if(waveforms.size()>0) {
  for(auto it = waveforms.begin(); it != waveforms.end(); ++it) {
      int channelId = it->first;
      if(not PMT::IsWP(channelId)) continue;
      const auto& channel = *(it->second);

     if(channel.adc().size()>0){ 
 //LogInfo<<"EvtID="<<EvtID<<";channel Id="<<channelId<<" ;channel Id size="<<crate.channelData()[channelId].adc().size()<<std::endl;
       temp_charge=0;
       temp_pedestal=0;
       calib_pe=0;
       hit_time=-1;
       max_amp=0;
       max_amp_tdc =0;
       bool firstflag = false;
       int baseline =0;
       for(int i=0;i<1000;i++){
        if(channel.adc()[i]>max_amp){
          max_amp =channel.adc()[i];
          max_amp_tdc =channel.tdc()[i];
        }
       if(max_amp>TMath::Power(2,14)){ baseline = TMath::Power(2,14)+TMath::Power(2,12);}
       else {
         baseline =TMath::Power(2,12);  
        }
       // use the 50ns to calculate the pedestal
       if(i>0&&i<50){
        temp_pedestal =temp_pedestal +channel.adc()[i]-baseline;
      //  LogInfo << "Temp.ped="<<temp_pedestal<<"; channel adc="<<channel.adc()[i]<< std::endl;
       }
      }
    // LogInfo << "Test max. amp="<<max_amp<<"; max_amp_tdc="<<max_amp_tdc<< std::endl;
       double ped_amp = temp_pedestal/100.;
       double amp_diff = max_amp-ped_amp;
       int total_timeWidth =0; 
      for(int i=0;i<1000;i++){
    //  LogInfo << "Test Wp pmt info., channel Id "<<channelId<<"; adc="<<crate.channelData()[channelId].adc()[i] <<";tdc="<<crate.channelData()[channelId].tdc()[i]<< std::endl;
     // use 200ns for integral charge 
     if(i>=max_amp_tdc-20&&i<max_amp_tdc+180){
       total_timeWidth++;
       temp_charge=temp_charge +channel.adc()[i]-baseline;
       // LogInfo << "Temp.charge="<<temp_charge<<"; channel adc="<<channel.adc()[i]<< std::endl;
      }
      // LogInfo << "i "<<i<<"; channel="<<channelId<<"; ped amp="<<ped_amp<<"; diff="<<crate.channelData()[channelId].adc()[i]-ped_amp<<"; 0.1*ampdiff="<<amp_diff/10. <<"; firstflag="<<firstflag<< std::endl;
       if(i>max_amp_tdc-20&&i<max_amp_tdc&&channel.adc()[i]-ped_amp>amp_diff/10.&&firstflag==false){ 
         hit_time = i; 
         firstflag = true;
       }
     }
      double transF=1.;  
      if(max_amp>TMath::Power(2,14)) transF = 0.55/0.08;  //high gain and low gain different factor
      calib_pe = (temp_charge- temp_pedestal*(total_timeWidth/50.))/Q_calib*transF;
      //LogInfo<<"total temp_charge="<<temp_charge<<"; temp_ped="<<temp_pedestal<<"; total_timeWidth="<<total_timeWidth<<std::endl;
      if(calib_pe<0)
      { 
       calib_pe =0; 
     // LogInfo << "max_amp= "<<max_amp<<"; max_amp_tdc="<<max_amp_tdc<< std::endl;
     // LogInfo << "channel Id "<<channelId<<"; PMTid="<<channelId-StartIdWppmt<<";startId="<<StartIdWppmt<<"; temp_charge="<<temp_charge<<"; temp_pedestal=<<"<<temp_pedestal*5.<<"; calib_pe="<<calib_pe <<";tdc="<<hit_time<< std::endl;
       }
       ChargePerPMT[channelId-StartIdWppmt].push_back(calib_pe);
       TimePerPMT[channelId-StartIdWppmt].push_back(hit_time); 
      }// size>0
    } 
  }
  m_EvtId=EvtID;
  for(int i=0;i<TotalWppmt;i++)  
    {
      m_charge.clear();
      m_time.clear();
      m_pmtId = 0;
      m_totalpe = 0;
      nPE=0;
      firstHitTime=1e9;
    if(ChargePerPMT[i].size()==0) continue;
     // LogInfo <<"evt="<<EvtID<<"; PMTid="<<i<< ";Charge per PMT size ="<<ChargePerPMT[i].size()<< std::endl;
      m_pmtId=i+StartIdWppmt;
      for(int j=0;j<ChargePerPMT[i].size();j++)
	{
          //LogInfo <<"evt="<<EvtID<<"; PMTid="<<i<< ";Charge ="<<ChargePerPMT[i].at(j)<<";charge="<<ChargePerPMT[i][0]<< std::endl;
	  nPE=nPE+ChargePerPMT[i].at(j);
	  firstHitTime=std::min(TimePerPMT[i].at(j),firstHitTime);
	  m_charge.push_back(ChargePerPMT[i].at(j));
	  m_time.push_back(TimePerPMT[i].at(j));
	}
    //  LogInfo <<"evt="<<EvtID<<"; PMTid="<<i<< ";nPE ="<<nPE<<"; firsttime="<<firstHitTime<< std::endl;
      m_totalpe=nPE;
#ifndef BUILD_ONLINE
      m_calib->Fill();
#endif
      m_pmtId=i+StartIdWppmt;

      // convert copy number to identifier
      auto identifier = WpID::id(static_cast<unsigned int>(m_pmtId), 0);

      auto cpc = calibevent->addCalibPmtChannel(identifier); 
      cpc->setNPE(nPE);
      cpc->setFirstHitTime(firstHitTime+100); //+100 ns in order to have the same shift that after ElecSim
      cpc->setPmtId(identifier);
      cpc->setTime(TimePerPMT[i]);
      cpc->setCharge(ChargePerPMT[i]);
       
    }
  EvtID+=1;
  return true;
}



bool WpPMTCalibAlg::finalize()
{
  LogInfo << objName() << " finalized successfully." << std::endl;
  return true;
}

