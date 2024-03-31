/*
SPMT Calib


Author: A.Meregaglia, C.Jollet (CENBG)
*/
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtCalibHeader.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"  
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"

#include "Geometry/RecGeomSvc.h"
#include "Geometry/PMT.h"
#include "Identifier/CdID.h"
//#include "DetSimAlg/IDetElement.h"
#include <vector>

#include "SPMTCalibAlg.h"
#include "SniperKernel/AlgFactory.h"


/////////////////////////////////////////////


DECLARE_ALGORITHM(SPMTCalibAlg);

SPMTCalibAlg::SPMTCalibAlg(const std::string& name)
	:AlgBase(name)
{
  // Set Default Values of Some Parameters


}

SPMTCalibAlg::~SPMTCalibAlg()
{

}

bool
SPMTCalibAlg::initialize()
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


  channel_index=0;
  channel_mask = 0xFF<<channel_index; 
  abc_index=8;  
  abc_mask = 0xFF<<abc_index;  
  //to be read from DB for real data conversion
  
  SniperPtr<SpmtElecConfigSvc> svc(*getParent(), "SpmtElecConfigSvc");
  if (svc.invalid()) {
    LogError << "can't find service SpmtElecConfigSvc" << std::endl;
    return false;
  }
  m_SpmtConfigSvc = svc.data();
  
  PC2ADC_THRES = svc->getInt("PC2ADC_THRES");      
  PC2ADC_HG = svc->getDouble("PC2ADC_HG");     
  PC2ADC_LG = svc->getDouble("PC2ADC_LG");     
  Const_HG = svc->getDouble("Const_HG");      
  Const_LG = svc->getDouble("Const_LG");
  Q_Spe = 0.48; 
  
  StartIdSpmt=kOFFSET_CD_SPMT;
  TotalSpmt=30000;

#ifndef BUILD_ONLINE
  SniperPtr<RootWriter> Writesvc(*getParent(),"RootWriter");
  if (Writesvc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
	     << "enalbe it in your job option file."
	     << std::endl;
    return false;
  }
  m_calib = Writesvc->bookTree(*m_par, "USER_OUTPUT/spmtcalibevt", "simple output of spmt calibration data");
  m_calib->Branch("EvtId",&m_EvtId,"EvtId/I");
  m_calib->Branch("Charge", &m_charge);
  m_calib->Branch("Time", &m_time);
  m_calib->Branch("PMTID", &m_pmtId,"PMTID/I");
  m_calib->Branch("TotalPE", &m_totalpe, "TotalPE/F");
#endif  

  EvtID=0;
  
  return true;
}

bool 
SPMTCalibAlg::execute()
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
  
  auto eh = JM::getHeaderObject<JM::CdSpmtElecHeader>(nav);
  if (not eh) {
    LogInfo << "Skip due to empty CdSpmtElecHeader. " << std::endl;
    return true;
  }

  std::cout<<"CdSpmtElecHeader "<<eh->hasEvent()<<std::endl;
  
  if (!eh->hasEvent()) {
    LogInfo << "CdSpmtElecHeader is not available in this event. " << std::endl;
    return true;
  }
  
  auto ee  = dynamic_cast<JM::CdSpmtElecEvt*>(eh->event());
  if (not ee) {
      LogInfo << "Skip due to empty CdSpmtElecEvt. " << std::endl;
      return true;
  }
  
  auto calibheader = JM::getHeaderObject<JM::CdSpmtCalibHeader>(nav);
  if(!calibheader) {
      LogInfo <<"creating a new CdSpmtCalibHeader "<<std::endl;
      calibheader = new JM::CdSpmtCalibHeader();
      JM::addHeaderObject(nav, calibheader);
  }
  

  auto calibevent = dynamic_cast<JM::CdSpmtCalibEvt*>(calibheader->event());
  if(!calibevent)
    {
      LogInfo <<"creating a new CdSpmtCalibEvt "<<std::endl;
      calibevent = new JM::CdSpmtCalibEvt();
      calibheader->setEvent(calibevent);
    }

  int pmtID=0;  
  
  float nPE = 0, firstHitTime = 0;
  std::vector<double> charge;
  std::vector<double> time;
  
  double temp_Charge(0);
  std::vector <float> ChargePerPMT[TotalSpmt];
  std::vector <float> TimePerPMT[TotalSpmt];

  const auto& blocks = ee->SpmtBlocks();
  for(auto itr=blocks.begin();
      itr !=blocks.end();++itr)
    {
      const auto& hit = *itr;
      unsigned int dec_channel= ((hit.ChNb()+hit.AsicNb()*16) << channel_index) & channel_mask;
      unsigned int dec_abc= (hit.CardNb() << abc_index) & abc_mask;
      
      pmtID=m_SpmtConfigSvc->getPMT(dec_abc|dec_channel);

      if(hit.Gain()==true)
	{
	  temp_Charge=((hit.Charge()-Const_HG)/PC2ADC_HG)/Q_Spe;
	}  
      else
	{
	  temp_Charge=((hit.Charge()-Const_LG)/PC2ADC_LG)/Q_Spe;
	}
      if(hit.Charge()==1025.) temp_Charge=0;
      ChargePerPMT[pmtID-StartIdSpmt].push_back(temp_Charge);
      TimePerPMT[pmtID-StartIdSpmt].push_back(hit.CoarseTime());

    }
  
  m_EvtId=EvtID;
  for(int i=0;i<TotalSpmt;i++)  
    {
      m_charge.clear();
      m_time.clear();
      m_pmtId = 0;
      m_totalpe = 0;
      nPE=0;
      firstHitTime=1e9;
      if(ChargePerPMT[i].size()==0) continue;
      m_pmtId=i+StartIdSpmt;
      for(int j=0;j<ChargePerPMT[i].size();j++)
	{
	  nPE+=ChargePerPMT[i].at(j);
	  firstHitTime=std::min(TimePerPMT[i].at(j),firstHitTime);
	  m_charge.push_back(ChargePerPMT[i].at(j));
	  m_time.push_back(TimePerPMT[i].at(j));
	}
      m_totalpe=nPE;
#ifndef BUILD_ONLINE
      m_calib->Fill();
#endif

      unsigned int detID = CdID::id(static_cast<unsigned int>(i+StartIdSpmt), 0);
           
      auto cpc = calibevent->addCalibPmtChannel(detID); 
            
      cpc->setNPE(nPE);
      cpc->setFirstHitTime(firstHitTime);
      cpc->setTime(TimePerPMT[i]);
      cpc->setCharge(ChargePerPMT[i]);
       
    }
  EvtID+=1;
  return true;
}



bool SPMTCalibAlg::finalize()
{
  LogInfo << objName() << " finalized successfully." << std::endl;
  return true;
}

