/*
TopTracker Calib


Author: A.Meregaglia, C.Jollet (IPHC)
Changes to CalibEvent v2 => J. P. A. M. de André <jpandre@iphc.cnrs.fr>
Changes to read from ElecSim EDM v3 => Karaparambil Rajan Rebin
*/

#include "Event/TtElecHeader.h"
#include "Event/TtCalibHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

//#include "DetSimAlg/IDetElement.h"

#include "TTCalibAlg.h"
#include "SniperKernel/AlgFactory.h"


/////////////////////////////////////////////


DECLARE_ALGORITHM(TTCalibAlg);

TTCalibAlg::TTCalibAlg(const std::string& name)
	:AlgBase(name)
{
  // Set Default Values of Some Parameters

  m_evt_id = 0;

}

TTCalibAlg::~TTCalibAlg()
{

}

bool
TTCalibAlg::initialize()
{
  m_evt_id = 0;

// Get Data Navigator
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if( navBuf.invalid() )
  {
    LogError << "cannot get the NavBuffer @ /Event" << std::endl;
    return false;
  }
  m_buf = navBuf.data();

  LogInfo << objName() << " initialized successfully." << std::endl;

  //to be read from DB for real data conversion
  meangain=40;
  return true;
}

bool 
TTCalibAlg::execute()
{
  LogDebug << "---------------------------------------" << std::endl;
  LogDebug << "Processing event " << m_evt_id << std::endl;

  // Get Input Data
  // initialize

  auto nav = m_buf->curEvt();
  auto sh = JM::getHeaderObject<JM::TtElecHeader>(nav);
  if (not sh) {
    return true;
  }
  auto se  = dynamic_cast<JM::TtElecEvt*>(sh->event());
  if (not se) {
    return true;
  }
  // create a TTCalibEvent
  // JM::CalibHeader* cal_hdr = 0;

  // for Calib Header, we need to check it is existing or not.
  // especially integrated with other calib algorithms.
  bool isCalibHeaderNew = false;
  auto cal_hdr = JM::getHeaderObject<JM::TtCalibHeader>(nav);
  if (!cal_hdr) {
      LogDebug <<"creating a new TTCalibHeader "<<std::endl;
      cal_hdr = new JM::TtCalibHeader();
      JM::addHeaderObject(nav, cal_hdr);
      isCalibHeaderNew = true;
  }
  auto cal_event = dynamic_cast<JM::TtCalibEvt*>(cal_hdr->event());

  if (!cal_event) {
      LogDebug <<"creating a new TTCalibEvt "<<std::endl;
      cal_event = new JM::TtCalibEvt();
      cal_hdr->setEvent(cal_event);
  }
  
  const auto& tt_hitMap = se->channelData();


  //loop on hits from data model 
  for (auto ithit = tt_hitMap.begin();
       ithit != tt_hitMap.end(); ++ithit) 
    {
            
      int channelID=ithit->first;
      auto calibchannel=cal_event->addCalibTtChannel(channelID);

      auto& ttElecHit = *(ithit->second);

      if(ttElecHit.charge()[0] < 0){
        calibchannel->setNPE(NAN);
      }
      else{
        calibchannel->setNPE(ttElecHit.charge()[0]/meangain);
      }
      calibchannel->setTime(ttElecHit.time()[0]);

    }

  //cal_hdr->setEvent(cal_event);
  //if (isCalibHeaderNew) JM::addHeaderObject(nav, cal_hdr);
  
  m_evt_id ++;
  return true;
}



bool TTCalibAlg::finalize()
{
  LogInfo << objName() << " finalized successfully." << std::endl;
  return true;
}

