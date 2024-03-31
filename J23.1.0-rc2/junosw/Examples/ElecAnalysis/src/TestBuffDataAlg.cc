//******************************************************************
//This package shows how to process the output of ElecSim. If      *
//you want to do similar things, one of most efficient way is to   *
//take this package as an example. In this package, we showed how  *
//to achieve an ElecEvent from the buffer, how to get the          *
//information from the ElecEvent, and how to define the plain      *
//root file using RootWriter.                                      *
//                                                                 *
//A python script has also been implementd and located in share    *
//directory. It is used to run this package using following        *
//command:                                                         *
//                                                                 *
//   python draw_waveform.py --input output_elecsim.root           *
//                                                                 *
//Authors: Xiao Fang, Guofu Cao                                    *
//******************************************************************

// include all header files that you need
#include "TestBuffDataAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtElecTruthHeader.h"
#include <iostream>
#include "EvtNavigator/NavBuffer.h"
#include "RootWriter/RootWriter.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h" 
#include "SniperKernel/Task.h"
#include "Context/TimeStamp.h"
#include <TROOT.h>

// look, you can also directly include the header files from root
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>

using namespace std;

// don't forget the following line, since you are tyring to create an algorithm.
DECLARE_ALGORITHM(TestBuffDataAlg);

// to be sure that your class is inheritting an algorithm base class
// when you make an algorithm, you can follow this format here.
TestBuffDataAlg::TestBuffDataAlg(const std::string& name): AlgBase(name)
{
}

// nothing new here.
TestBuffDataAlg::~TestBuffDataAlg()
{
}

// for an algorithm, there are three member functions, initialize(), execute() and finalize().
// you need to implement them one by one.
bool TestBuffDataAlg::initialize()
{
    cout<<"hello world!" <<endl;
    m_EvtID = 0;

    // ok, when you want to create a plain root file, you can use RootWriter, so you can do the job like using root.
    // firtly, you need to get the pointer of RootWriter as following line.
    // it is also better to check the pointer you get, to avoid illegal usage of this pointer.
    gROOT->ProcessLine("#include <vector>");
    SniperPtr<RootWriter> svc(getRoot(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
            << "enalbe it in your job option file."
            << std::endl;                       
        return false;
    }   

    // using RootWriter to book a tree like this, the first parameter is the tree name that your need to specify
    // and the second one is the title name of this tree.
    m_evt_tree = svc->bookTree(*m_par, "SIMEVT", "TTS and Amplitude information");

    // to save what you want to save in the tree, it is same with that you are using root.
    // here, I defined several branches.
    // evtID: it's event id
    // fired_PMT_Num: the total number of fired PMTs in an event
    // PMTID: it's a 1D array, used to save the ID of fired PMTs in an event
    // tdc: it's a 2D array, the first dimention is PMT ID, and the second one is the time points in readout window (event).
    // adc: it's also a 2D array, the first dimention is PMT ID, and the second one is the ADC counts at each time point in readout window.
    // The number 1250 is arbitrary, it should be the length of readout window, our current baseline is 1000ns.
    m_evt_tree->Branch("evtID", &m_EvtID, "evtID/I");
    m_evt_tree->Branch("fired_PMT_Num", &m_Fired_PMT_Num, "fired_PMT_Num/I");
    m_evt_tree->Branch("PMTID", m_PMTID, "PMTID[fired_PMT_Num]/I");
    m_evt_tree->Branch("tdc", m_tdc, "tdc[20000][1250]/D");
    m_evt_tree->Branch("adc", m_adc, "adc[20000][1250]/D");

    m_evt_truth = svc->bookTree(*m_par, "SIMEVT/EVTTRUTH", "TTS and Amplitude information");
    m_evt_truth->Branch("pulsetype", &m_pulsetype);  
    m_evt_truth->Branch("npe", &m_npe);  
    m_evt_truth->Branch("pmtId", &m_pmtId);  
    m_evt_truth->Branch("amplitude", &m_amplitude);  
    m_evt_truth->Branch("hitTime", &m_hitTime);  
    m_evt_truth->Branch("TTS", &m_TTS);  
    m_evt_truth->Branch("timeoffset", &m_timeoffset);  
    m_evt_truth->Branch("pulseHitTime", &m_pulseHitTime);  
    // if everything is ok, then return true, otherwise return false.
    return true;
}

bool TestBuffDataAlg::execute()
{
    // Well, the following several lines are used to achieve ElecEvent from memory.
    // you can follow it when you want to get ElecEvent, and you can also make some minor 
    // changes to achieve other types of events defined in event model.

    // firstly, get the buffer.
    SniperDataPtr<JM::NavBuffer> navBuf(getRoot(), "/Event");
    if (navBuf.invalid()) {
        return 0;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;

    // get the current event from buffer
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return 0;
    }

    // get the ElecHeader from current event
    auto elec_hdr = dynamic_cast<JM::CdWaveformHeader*>(evt_nav->getHeader("/Event/Elec"));
    if (not elec_hdr) {
        return 0;
    }
    if (!elec_hdr->hasEvent()) {
        cout<<"no LPMT data is found, skip this event."<<endl;
        return true;
    }

    // get ElecEvent based on ElecHeader
    // in order to well understand this part, you need to know the event model of ElecEvent.
    auto elec_evt = dynamic_cast<JM::CdWaveformEvt*>(elec_hdr->event());
    const auto& waveforms = elec_evt->channelData();


    // initialize several variables
    int test_pmtID = 0;
    int tdc_size = 0;
    m_Fired_PMT_Num = 0;
    for(int i=0;i<20000;i++){
        for(int j=0; j<1250;j++){
            m_tdc[i][j] = -1.;
            m_adc[i][j] = -1.;
        }
    }

    // loop over all PMTs to find fired PMTs, and fill variables defined in tree.
    // if you know event model of ElecSim, it is not difficult to understand the following codes,
    // and you can even use more functions defined in event model.
    // 17746 is the total number of 20'' PMTs, it is not good to be hard coded here.
    for (auto it = waveforms.begin(); it != waveforms.end(); ++it) {
        const auto& waveform = *(it->second);
        // for each PMT, to check the tdc vector size, if it's zero, this means no signals in this PMT.
        tdc_size = waveform.tdc().size();
        // if this PMT has some signal, then we start to handle it.
        if (tdc_size > 0) {
            // to get its ID, in this example we use a simple way to identify each PMT.
            // the correct way is to use Identifier defined in Geomtry.
            m_PMTID[m_Fired_PMT_Num] = it->first;
            m_Fired_PMT_Num ++;
            // to get tdc and adc for each fired PMT
            for(int j=0; j<tdc_size; j++){
              m_tdc[it->first][j] = double(waveform.tdc()[j]);
              m_adc[it->first][j] = waveform.adc()[j];
           }
        }
    }

    cout<<"fired_PMT_Num: " << m_Fired_PMT_Num <<endl;

    // don't forget to fill the tree.
      m_evt_tree -> Fill();

      elec_truth_header = dynamic_cast<JM::CdLpmtElecTruthHeader*>(evt_nav->getHeader("/Event/Sim/Truth"));
      auto lpmtElecTruthEvent = elec_truth_header->event();

      LogInfo << " truths size: " << lpmtElecTruthEvent->truths().size() << std::endl;

      const auto& truths = lpmtElecTruthEvent->truths();

      for (auto it = truths.begin(); it != truths.end(); it++){
            m_pulsetype.push_back((*it).pulsetype());
            m_pmtId.push_back((*it).pmtId());
            m_npe.push_back((*it).npe());
            m_hitTime.push_back((*it).hitTime());
            m_amplitude.push_back((*it).amplitude());
            m_TTS.push_back((*it).tts());
            m_timeoffset.push_back((*it).timeoffset());
            m_pulseHitTime.push_back((*it).pulseHitTime());

       }
       m_evt_truth->Fill();

    m_EvtID ++;

    // everything is ok, return true;
    return true;
}

bool TestBuffDataAlg::finalize()
{
    // nothing to be done here. However, you can do something if you like.
    return true;
}
