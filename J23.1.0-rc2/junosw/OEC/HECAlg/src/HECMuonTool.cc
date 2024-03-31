#include "HECAlg/HECMuonTool.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "TMath.h"

#include "TTree.h"
#include <string>
#include <vector>
#include <deque>
#include <TVector3.h>

DECLARE_TOOL(HECMuonTool);

HECMuonTool::HECMuonTool(const std::string &name):ToolBase(name){
  m_latestMuon=TTimeStamp(-100000, 0);
  declProp("maxN", i_maxN=2);
  declProp("dT", i_dT);
  declProp("TMuonWindow", d_MuonWindow=200);
}

HECMuonTool::~HECMuonTool(){
}

bool HECMuonTool::initialize(){
  LogInfo<<"initializing classification tool..."<<std::endl;
  //get the event buffer
  SniperDataPtr<JM::NavBuffer> navbuf(getRoot(), "/Event");
  if (navbuf.invalid()){
    LogError<<"can not get navbuffer!!!"<<std::endl;
    return false;
  }
  m_buf=navbuf.data();
  return true;
}

bool HECMuonTool::finalize(){
  return true;
}

bool HECMuonTool::isVetoed(JM::NavBuffer::Iterator navit){
  //Here judge if it's relatedWPorTT, true: not relatedWPorTT, false: relatedWPorTT
  //Get this OECEvent
  JM::OecHeader* tHeaderOEC = JM::getHeaderObject<JM::OecHeader>(navit->get());
  JM::OecEvt *tEventOEC = dynamic_cast<JM::OecEvt*>(tHeaderOEC->event("JM::OecEvt"));
  const TTimeStamp& ttime=tEventOEC->getTime();
  //Update the latest TTmuon or WPmuon
  if ((tEventOEC->isTTMuon() || tEventOEC->isWPMuon()) && m_latestMuon<ttime){
    m_latestMuon = ttime;
    return false;
  }

  //pre muon 
  double dt=(ttime-m_latestMuon)*1e9;
  if (dt<d_MuonWindow) return false;
  //post muon
  for (JM::NavBuffer::Iterator tmpit=navit;tmpit!=m_buf->end();++tmpit){
    JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tmpit->get());
    JM::OecEvt *aEventOEC=dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& afterTime=aEventOEC->getTime();
    dt=(afterTime-ttime)*1e9;
    if (dt>d_MuonWindow) break;
    if (aEventOEC->isTTMuon() || aEventOEC->isWPMuon()){
      return false;
    }
  }
  return true;
}

bool HECMuonTool::findCorrelation(JM::NavBuffer::Iterator navit, std::vector<JM::OecEvt*>& corEvts){
  if (corEvts.size()!=1){
    LogError<<"Current event is not pushed"<<std::endl;
    return false;
  }
  const TTimeStamp& thisTime=corEvts.at(0)->getTime();
  for (JM::NavBuffer::Iterator tmpit=++navit;tmpit!=m_buf->end();++tmpit){
    JM::OecHeader* aHeaderOEC = JM::getHeaderObject<JM::OecHeader>(tmpit->get());
    JM::OecEvt *aEventOEC = dynamic_cast<JM::OecEvt*>(aHeaderOEC->event("JM::OecEvt"));
    const TTimeStamp& afterTime=aEventOEC->getTime();
    double dt=(afterTime-thisTime)*1e9;
    if (dt>i_dT) break;
    if (!this->isVetoed(tmpit)) continue;
    corEvts.push_back(aEventOEC);
  }
  int totEvts = corEvts.size();
  if (totEvts>i_maxN){
    LogDebug<<"Multi tags:"<<totEvts<<std::endl;
    return false;
  }
  if (totEvts==1){
    LogDebug<<"Single tag:0"<<std::endl;
    return false;
  }
  return true;
}


bool HECMuonTool::calClassifyQuantity(const std::vector<JM::OecEvt*>& corEvents){
  int totEvts = corEvents.size();
  if (totEvts<2) return false;
  m_eventPair.clear();
  m_classQuantity.clear();

  classifyQuantity thecondition = classifyQuantity({"penergy", "denergy", "d2track", "deltaT"});
  JM::OecEvt *tEvent = corEvents.at(0);
  float penergy=tEvent->getEnergy();
  if(penergy > 20 ){
    TVector3 MuTrack;
    TVector3 EventPos;
    TVector3 cross_vector;
    double mag_MuTrack;
    double dist;
    for(int i = 0; i < tEvent->getMuID().size(); i++){
    MuTrack.SetXYZ(tEvent->getMuOutX()[i]-tEvent->getMuInX()[i],tEvent->getMuOutY()[i]-tEvent->getMuInY()[i],tEvent->getMuOutZ()[i]-tEvent->getMuInZ()[i]); //here not considering muon bundle, recontruction alg. return last muon track info.
    mag_MuTrack = MuTrack.Mag();
    LogDebug<<"This tag info., muon num = "<<tEvent->getMuID().size()<<"; penergy = "<<tEvent->getEnergy()<<"; tracklength ="<<mag_MuTrack<<std::endl;
    for (int ii=1;ii<totEvts;ii++){
      JM::OecEvt *aEvent = corEvents.at(ii);
      if(mag_MuTrack > 0) {
        EventPos.SetXYZ(aEvent->getVertexX()-tEvent->getMuInX()[i],aEvent->getVertexY()-tEvent->getMuInY()[i],aEvent->getVertexZ()-tEvent->getMuInZ()[i]);
        cross_vector = MuTrack.Cross(EventPos);
        dist = cross_vector.Mag()/mag_MuTrack;  // C to AB: d = (AB x AC)/|AB|
      }
      else {
        //double dx=tEvent->getVertexX()-aEvent->getVertexX();
        //double dy=tEvent->getVertexY()-aEvent->getVertexY();
        //double dz=tEvent->getVertexZ()-aEvent->getVertexZ();         
        //dist=TMath::Sqrt(dx*dx+dy*dy+dz*dz);
        dist = 0.1; //if muon track reconstruction fails, need to find neutron or michel in the whole LS volume.
      }
      float dT=1e9*(aEvent->getTime()-tEvent->getTime());
      float denergy=aEvent->getEnergy();
      thecondition.setValue("penergy", penergy);
      thecondition.setValue("denergy", denergy);
      thecondition.setValue("d2track", dist);
      thecondition.setValue("deltaT", dT);
      m_eventPair.push_back(std::make_pair(tEvent, aEvent));
      m_classQuantity.push_back(thecondition);
    }
  }
  }
  return true;
}

bool HECMuonTool::userFunc(JM::EvtNavigator* thisnav, const EventPair& thispair){
  return true;
}
