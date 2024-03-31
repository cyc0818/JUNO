#include "WaveFit.h"

#include "AnalysisTool.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Identifier/CdID.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"

#include <TRandom.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include <iostream>


DECLARE_ALGORITHM(WaveFitAlg);

WaveFitAlg::WaveFitAlg(const std::string& name)
      : AlgBase(name)
      , fit_tool(0)

{   
    declProp("OutFile", m_output_file);
    declProp("PmtTotal", m_PmtTotal);

    declProp("DrawWave", m_flag_draw_wave=false);
    declProp("FitTool", m_fit_tool_name="FadcWaveform");
}


WaveFitAlg::~WaveFitAlg()
{
}
bool WaveFitAlg::initialize()
{
    m_count=0;

    fit_tool = load_wave_fit_tool();
    return true;
}
bool
WaveFitAlg::execute()
{
    // = clear the cache =
    m_cache_first_hits.clear();
    // = Get Sim Header from Buffer =
    auto ee = load_elecsim_crate();
    const auto& waveforms = ee->channelData();

    // = Get FEE Crate, Fit the Wave =
    for (auto it = waveforms.begin(); it != waveforms.end(); ++it) {
        int chni = it->first;
        const auto& channel = *(it->second);

        int tdc_size = channel.tdc().size();
        if(tdc_size==0)continue;
        cout<<"!!!"<<"lala "<<chni<<endl;

        //read single channel buffer 
        int eventspantime=channel.tdc()[tdc_size-1]; //ns
        float waveform[eventspantime];
        //float *waveform=new float[eventspantime];
        //dealing with zero suppression
        for(int i=0;i<eventspantime;i++)
        {
            waveform[i]=0.0;
        }
        // non zero components
        for (int iw=0; iw<tdc_size; iw++) {
            int tdc = int(channel.tdc()[iw]);
            // convert from V to mV
            float adc = 1e3*float(channel.adc()[iw]);
            waveform[tdc]= adc;//spe amplitude 35
        }

        if (m_flag_draw_wave) {
            draw_wave(chni, eventspantime, waveform);
        }
        // MAGIC HERE
        //   FadcWaveform fadcWaveform(waveform,eventspantime,100);//(float *inputArray,int nSamples=0,int preWindow=200)
        //   cout<<" !!! "<<" Analyze "<<endl; 
        //     fadcWaveform.SetGain(1);
        //       // fadcWaveform.Analyze();
        //     //  if(m_count==0&&chni<5)
        //             // PlotNewDataWave(fadcWaveform,m_count,chni);
        //   cout<<" !!! "<<" end  Analyze "<<endl; 


        // // outTree->fdcQ[chni] = fadcWaveform.GetCharge();
        // // outTree->faPT[chni] = fadcWaveform.GetTime();
        // // outTree->chi2[chni] = fadcWaveform.GetChi2(); 
        // float hittime = fadcWaveform.GetTime();
        // float npe = fadcWaveform.GetCharge();
        std::cout << "#############" << fit_tool << std::endl;
        fit_tool->fit(waveform, eventspantime);
        float hittime = 0;
        float npe = 0;
        hittime = fit_tool->get_first_time();
        npe = fit_tool->get_total_charge();
        // create channel
        auto pmt_hdr = new JM::CalibPmtChannel;
        pmt_hdr->setPmtId(CdID::id(chni, 0)); 
        pmt_hdr->setFirstHitTime(hittime);
        pmt_hdr->setNPE(npe);

        m_cache_first_hits[chni] = pmt_hdr;
  
    }//end pmt channel;

    // outTree->Fill();

    m_count++;
    return save_calib_event();
}
bool
WaveFitAlg::finalize()
{

  // outTree->Write();
  return true;
}

JM::CdWaveformEvt*
WaveFitAlg::load_elecsim_crate()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return 0;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return 0;
    }
    auto elec_hdr = JM::getHeaderObject<JM::CdWaveformHeader>(evt_nav);
    if (not elec_hdr) {
        return 0;
    }

    return elec_hdr->event();
}

bool
WaveFitAlg::save_calib_event() {
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    if (not evt_nav) {
        LogError << "Can't get EvtNavigator. " << std::endl;
        return false;
    }

    // Build the Calib.
    auto ch = new JM::CdLpmtCalibHeader; 
    auto ce = new JM::CdLpmtCalibEvt; 

    std::list<JM::CalibPmtChannel*>vcph; 

    for (auto it = m_cache_first_hits.begin();
            it != m_cache_first_hits.end();
            ++it) {
        vcph.push_back(it->second);
    }

    ce->setCalibPMTCol(vcph); 
    ch->setEvent(ce); 
    JM::addHeaderObject(evt_nav, ch); 
    return true;
}

void
WaveFitAlg::draw_wave(int chni, int eventspantime, float* waveform)
{

    TGraph* gr=new TGraph(eventspantime);
    for(int i=0;i<eventspantime;i++)
    {
        gr->SetPoint(i,i,waveform[i]);
    }
    TCanvas* c1 = new TCanvas();
    gr->Draw("AC");
    c1 -> SaveAs(Form("adc_tdc_%d.C", chni));
    c1->Print(Form("adc_tdc_%d.png", chni),"png");
    delete gr;
    delete c1;
}

IWaveFitTool*
WaveFitAlg::load_wave_fit_tool() {
    IWaveFitTool* wavefittool = 0;
    ToolBase* t = 0;
    // find the tool first
    t = findTool(m_fit_tool_name);

    if (not t) {
        t = createTool(m_fit_tool_name);
    }

    if (t) {
        wavefittool = dynamic_cast<IWaveFitTool*>(t);

    }

    return wavefittool;
}

