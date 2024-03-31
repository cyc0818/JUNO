#include "AbcBoardSvc.h"

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "TTree.h"
#include "TROOT.h"

DECLARE_SERVICE(AbcBoardSvc);


AbcBoardSvc::AbcBoardSvc(const std::string& name)
    : SvcBase(name)
{

}

AbcBoardSvc::~AbcBoardSvc()
{

}

bool AbcBoardSvc::initialize()
{
  LogInfo << " Initializing AbcBoardSvc " << std::endl;

  SniperPtr<SpmtElecConfigSvc> svc(*getRoot(), "SpmtElecConfigSvc");
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
  try{
    int TOTAL_BOARDS = m_SpmtConfigSvc->getInt("TOTAL_BOARDS");
    // initialize abc boards
    for( int i=0; i<TOTAL_BOARDS; i++){
      AbcBoard board(i,m_SpmtConfigSvc); 
      board.initialize();
      board.reset((TimeStamp)0);
      boards.push_back(board);
    }
  }
  catch(std::out_of_range& e){
    LogError << e.what() << std::endl;
  }

  channel_index=0;
  channel_mask = 0xFF<<channel_index; 
  abc_index=8;  
  abc_mask = 0xFF<<abc_index;  
  
 SniperPtr<RootWriter> Writesvc(*getRoot(),"RootWriter");
        if (Writesvc.invalid()) {
            LogError << "Can't Locate RootWriter. If you want to use it, please "
                     << "enalbe it in your job option file."
                     << std::endl;
            return false;
        }
  gROOT->ProcessLine("#include <vector>");
  m_spmt_tree = Writesvc->bookTree(*m_par, "USER_OUTPUT/SPMT","Time and Charge info for SPMT");
  m_spmt_tree->Branch("evtID",&m_eventID,"evtID/I");
  m_spmt_tree->Branch("SPMT_Id",&m_SPMT_Id,"SPMT_Id/I");
  m_spmt_tree->Branch("Charge",&m_Charge,"Charge/D");
  m_spmt_tree->Branch("Time",&m_Time,"Time/D");
  m_eventID=0;
  return true;
}

bool AbcBoardSvc::finalize()
{
  try{
    int TOTAL_BOARDS = m_SpmtConfigSvc->getInt("TOTAL_BOARDS");
    // initialize abc boards
    for( int i=0; i<TOTAL_BOARDS; i++){
        boards[i].delFunc();
    }
  }
  catch(std::out_of_range& e){
    LogError << e.what() << std::endl;
  }
  LogDebug << " Finalizing AbcBoardSvc " << std::endl;
  return true;
}


void AbcBoardSvc::pushPulses(int pmt, std::vector<Pulse*> pulses)
{

  try{
    int START_SPMT =  m_SpmtConfigSvc->getInt("START_SPMT");
    int CHANNELS =  m_SpmtConfigSvc->getInt("CHANNELS");

    // pass pulses to abc boards
    LogDebug << " Pushing pulses - PMT " << pmt << " : " << pulses.size() << " pulses " << std::endl;
    // temporary!
    int board = (pmt-START_SPMT)/CHANNELS;
    int channel = (pmt-START_SPMT)%CHANNELS;
    boards[board].pushPulses(channel,pulses);
  }
  catch(std::out_of_range& e){
    LogError << e.what() << std::endl;
  }
}


bool AbcBoardSvc::addElecDataModel(JM::EvtNavigator* evtnav, bool storeTruth)
{
    LogInfo<<"AbcBoardSvc addElecDataModel" << std::endl;

    int TOTAL_BOARDS =  m_SpmtConfigSvc->getInt("TOTAL_BOARDS");

    auto elec_header = JM::getHeaderObject<JM::CdSpmtElecHeader>(evtnav);
    if(!elec_header){
	elec_header = new JM::CdSpmtElecHeader();
	JM::addHeaderObject(evtnav, elec_header);
    }
    auto elec_truth_header = JM::getHeaderObject<JM::CdSpmtElecTruthHeader>(evtnav);
    // if storeTruth is false, don't create the elec truth header and object
    if(!elec_truth_header and storeTruth){
	elec_truth_header = new JM::CdSpmtElecTruthHeader();
	JM::addHeaderObject(evtnav, elec_truth_header);
    }


    if(elec_header){
      LogDebug << "Elec header: " << elec_header << std::endl;
      LogDebug << "Elec truth header: " << elec_truth_header << std::endl;


      auto spmtEvent = new JM::CdSpmtElecEvt();
      JM::CdSpmtElecTruthEvt* spmtElecTruthEvent = nullptr;
      if (elec_truth_header) {
          spmtElecTruthEvent = new JM::CdSpmtElecTruthEvt();
      }

      //TO DO: here process Abc Boards
      std::cout<<"TOTAL_BOARDS in AbcBoardSvc:"<<TOTAL_BOARDS<<std::endl; 
      for(int i=0; i<TOTAL_BOARDS; i++){
        boards[i].process(spmtEvent,spmtElecTruthEvent);
      }

      //set abc blocks ...
      elec_header->setEvent(spmtEvent);
  
      // set truth ...
      if (elec_truth_header) {
          elec_truth_header->setEvent(spmtElecTruthEvent);
          LogDebug << " Truths: " << elec_truth_header->event()->truths().size() << std::endl;
      }
     
    }
    else{
      LogError << "Unable to find data headers" << std::endl;
      return false;
    }

 SniperPtr<RootWriter> svc(*getRoot(),"RootWriter");
        if (svc.invalid()) {
            LogError << "Can't Locate RootWriter. If you want to use it, please "
                     << "enalbe it in your job option file."
                     << std::endl;
            return false;
        }
        auto spmtEvent = elec_header->event();

        auto itrend = (spmtEvent->SpmtBlocks()).end();
	for(auto itr = (spmtEvent->SpmtBlocks()).begin();
	    itr != itrend;++itr)
	  {
	    const auto& hit = *itr;
	    unsigned int dec_channel= ((hit.ChNb()+hit.AsicNb()*16) << channel_index) & channel_mask;
	    unsigned int dec_abc= (hit.CardNb() << abc_index) & abc_mask;
	    m_SPMT_Id=m_SpmtConfigSvc->getPMT(dec_abc|dec_channel);
	    if(hit.Gain()==true)
	      {
		m_Charge=((hit.Charge()-Const_HG)/PC2ADC_HG)/Q_Spe;
	      }  
	    else
	      {
		m_Charge=((hit.Charge()-Const_LG)/PC2ADC_LG)/Q_Spe;
	      }
	    if(hit.Charge()==1025.) m_Charge=0;
	    m_Time=hit.CoarseTime();
	    m_spmt_tree->Fill();
	  }
	m_eventID++;
  return true;
}

void AbcBoardSvc::clearVectors()
{
  try{
    int TOTAL_BOARDS =  m_SpmtConfigSvc->getInt("TOTAL_BOARDS");    
    for(int i=0; i<TOTAL_BOARDS; i++){
      boards[i].clearPulses();
    }
  }
  catch(std::out_of_range& e){
    LogError << e.what() << std::endl;
  }
}
