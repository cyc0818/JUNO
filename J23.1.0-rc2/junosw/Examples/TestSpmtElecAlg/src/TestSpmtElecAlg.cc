#include "TestSpmtElecAlg.h"

#include <sstream>

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"

DECLARE_ALGORITHM(TestSpmtElecAlg);

TestSpmtElecAlg::TestSpmtElecAlg(const std::string& name)
    : AlgBase(name) , event(0)
{
  declProp("ShowEvent", showEvent=-1);
  declProp("ShowLPMT", showLPMT=false);
  declProp("ShowSPMT", showSPMT=true);
  declProp("OutFile", outRootName="ElecAnalysis_UserOutFile.root");
}

bool TestSpmtElecAlg::initialize()
{
    LogDebug << "initializing" << std::endl;

    SniperDataPtr<JM::NavBuffer>  navBuf(*getRoot(), "/Event");
    //SniperDataPtr<JM::NavBuffer>  navBuf(getScope(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    // Setting root outputs
    outUserFile = new TFile(outRootName.c_str(),"new");    
    if(!outUserFile) LogError << " Unable to create User Output File " << std::endl;
    histoBlocksNum = new TH1F("blocksNum","blocksNum",10000,0,10000);
    histoSinglePMT = new TH1F("singlePMT","singlePMT",10000,0,10000);
    histoAllPMTs = new TH1F("allPMTs","allPMTs",30000000,0,30000000);
    histoSpmtTruthHits = new TH1F("SpmtTruthHits","SpmtTruthHits",1000000,0,1000000);
    histoSpmtElecTruthHits =  new TH1F("SpmtElecTruthHits","SpmtElecTruthHits",1000000,0,1000000);

    return true;
}

bool TestSpmtElecAlg::execute()
{
  LogDebug << "executing" << std::endl;

  JM::EvtNavigator* nav = m_buf->curEvt();
  elec_header = static_cast<JM::ElecHeader*>(nav->getHeader("/Event/Elec"));
  elec_truth_header = dynamic_cast<JM::ElecTruthHeader*>(nav->getHeader("/Event/Sim/Truth"));
  sim_hdrs = nav->getHeaders("/Event/Sim");
  LogInfo << " Event " << event << std::endl;
  LogInfo << "elec_truth_header: " << elec_truth_header << std::endl;
  LogInfo << "elec_header: " << elec_header << std::endl;
  for (size_t i = 0; i < sim_hdrs.size(); ++i) {
    LogInfo << "sim header [" << i << "]: " << sim_hdrs[i] << std::endl;
  }

  if(showEvent==event){
    LogInfo << "ShowEvent: " << showEvent << std::endl;
    ShowEvent();
  }

  FillHistos();
  
  ++event;
  return true;
}

bool TestSpmtElecAlg::finalize()
{
    LogDebug << "finalizing" << std::endl;

    histoBlocksNum->Write();
    histoSinglePMT->Write();
    histoAllPMTs->Write(); 
    histoSpmtTruthHits->Write();
    histoSpmtElecTruthHits->Write();
    outUserFile->Write();

    return true;
}

void TestSpmtElecAlg::FillHistos()
{
  if(elec_header){
    JM::SpmtElecEvent* spmt_elecEvent = elec_header->spmtEvent();
    if(spmt_elecEvent&&showSPMT){
      const std::vector<JM::SpmtElecAbcBlock>& blocks = spmt_elecEvent->SpmtBlocks();
      histoBlocksNum->Fill(blocks.size());
      LogInfo << " Blocks size: " << blocks.size() << std::endl;
      int allCharge=0;
      for( int i = 0; i< blocks.size() ; ++i){      
  // (int) blocks[i].ChNb(); (int) blocks[i].AsicNb();(int) blocks[i].CardNb();
  // blocks[i].Gain();blocks[i].CoarseTime();blocks[i].EventCounter();
  // blocks[i].FineTime();blocks[i].Charge();     
        histoSinglePMT->Fill(blocks[i].Charge());
        allCharge+=blocks[i].Charge();
      }
      histoAllPMTs->Fill(allCharge);
    }
  }

  // Fill Truth info 
  int spmt_hits = 0;
  for (size_t i = 0; i < sim_hdrs.size(); ++i) {
    LogInfo << " Show Truth " << i << std::endl;
    JM::SimHeader* simhdr = dynamic_cast<JM::SimHeader*>(sim_hdrs[i]);
    if(!simhdr){
      LogInfo << " Problem dynamic cast " << std::endl;
      continue;
    }
    JM::EventObject* evtobj =  (simhdr)->event();
    if(evtobj){
      JM::SimEvent* simevt = dynamic_cast<JM::SimEvent*>(evtobj);
      if(simevt){
        const std::vector<JM::SimPMTHit*>& hits = simevt->getCDHitsVec();
        LogInfo << " Sim Event ID " << simevt->getEventID() << " hits: " << hits.size() << std::endl;
        for(size_t j = 0; j < hits.size(); ++j){
          if(hits[j]->getPMTID()>20000) ++spmt_hits;
        } 
      }
    }
  }

  if(elec_truth_header)
  {
    JM::SpmtElecTruthEvent* truthevt = elec_truth_header->spmtTruth();
    LogInfo << " Truthevt: " << truthevt << std::endl;
    if(truthevt)
    {
      LogInfo << " truths size: " << truthevt->truths().size() << std::endl;
      histoSpmtElecTruthHits->Fill( truthevt->truths().size() ); 
    }
  }
 

  LogInfo << " SpmtHits " << spmt_hits << std::endl;
  histoSpmtTruthHits->Fill(spmt_hits);
 
}

void TestSpmtElecAlg::ShowEvent()
{
  
  // show Truth info 
  for (size_t i = 0; i < sim_hdrs.size(); ++i) {
    LogInfo << " Show Truth " << i << std::endl;
    JM::SimHeader* simhdr = dynamic_cast<JM::SimHeader*>(sim_hdrs[i]);
    if(!simhdr){
      LogInfo << " Problem dynamic cast " << std::endl;
      continue;
    }
    JM::EventObject* evtobj =  (simhdr)->event();
    if(evtobj){
      JM::SimEvent* simevt = dynamic_cast<JM::SimEvent*>(evtobj);
      if(simevt){
        LogInfo << " Sim Event ID " << simevt->getEventID() << std::endl;
      }
    }
  }
 
  if(elec_header){
    // show SPMT infos
    JM::SpmtElecEvent* spmt_elecEvent = elec_header->spmtEvent();
    if(spmt_elecEvent&&showSPMT){
      const std::vector<JM::SpmtElecAbcBlock>& blocks = spmt_elecEvent->SpmtBlocks();
      LogInfo << " SPMT blocks " << blocks.size() << std::endl;
      std::ostringstream s;
      for( int i = 0; i< blocks.size() ; ++i){      
        s << " Blocks: " << i << " ";
        s << " ChNb: " << (int) blocks[i].ChNb();
        s << " AsicNb: " << (int) blocks[i].AsicNb();
        s << " CardNb: " << (int) blocks[i].CardNb();
        s << " Gain: " << blocks[i].Gain();
        s << " CoarseTime: " << blocks[i].CoarseTime();
        s << " EventCounter: " << blocks[i].EventCounter();
        s << " FineTime: " << blocks[i].FineTime();
        s << " Charge: " << blocks[i].Charge();     
        s << std::endl; 
      }
      LogInfo << s.str() << std::endl;
    }
    else LogInfo << " No SPMT event shown " << std::endl;
  
    // show LPMT infos
    JM::ElecEvent* elec_event = elec_header->event();
    if (elec_event&&showLPMT) {
      LogInfo << " Elec Event Pointer " << elec_event << std::endl;
      const JM::ElecFeeCrate& elec_feecrate = elec_event->elecFeeCrate();
      JM::ElecFeeCrate* crate = NULL;
      crate = const_cast<JM::ElecFeeCrate*>(&elec_feecrate);  
      if(&elec_feecrate != NULL) {
        LogInfo << " ElecEvent TimeStamp " << crate->TriggerTime() << std::endl;
        int tdc_size = 0;
        for (int i = 0; i < 17746; ++i) {
          std::ostringstream s;
          s << " LPMT " << i << " ";
          int tdc_size = (*crate).channelData()[i].tdc().size();
          if (tdc_size > 0) {
            for(int j=0; j<tdc_size; j++){
              s << " | " << double((*crate).channelData()[i].tdc()[j]) << " ";
              s << (*crate).channelData()[i].adc()[j] << " ";
            }
          }
          LogInfo << s.str() << std::endl;
        }
      }
      else LogInfo << " No ElecFeeCrate found " << std::endl;
    }
    else LogInfo << " No Elec Event shown " << std::endl;
  }
}
