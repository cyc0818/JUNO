#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Event/WpElecTruthHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/WpWaveformHeader.h"
#include "Event/WpTriggerHeader.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "ElecSimSvc.h"
#include "TTree.h"
#include "TRandom.h"
#include "Pulse.h"
#include "TMath.h"
#include "EventKeeper.h"
#include "RootWriter/RootWriter.h"
#include <TROOT.h>
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/PMT.h"


using namespace std;
/*
 * This is actually the wavefrom simulation of WPLPMT, refer from the LPMT TriggerHandlerLpmt
 */

class TriggerHandlerWPLpmt: public ToolBase, public ITriggerHandler {
public:
    TriggerHandlerWPLpmt(const std::string& name): ToolBase(name),
                                                 m_elecsvc(0) {

    declProp("preTimeTolerance", m_preTimeTolerance=300); // ns
    declProp("postTimeTolerance", m_postTimeTolerance=700); // ns

    declProp("preWaveSimWindow",m_preWaveSimWindow=500); 
    declProp("PulseSampleWidth", m_PulseSampleWidth=150);  //unit ns, the time window for one Ideal pulse waveform width
    declProp("enableOvershoot", m_enableOvershoot=true);
    declProp("enableSaturation", m_enableSaturation=true);
    declProp("enableNoise", m_enableNoise=false);
    declProp("enableFADC", m_enableFADC=true);
    declProp("simFrequency", m_simFrequency=1e9);
    declProp("enableElecNonl", m_enableElecNonL=true);
    declProp("noiseAmp", m_noiseAmp=0.56e-3);
    declProp("speAmp", m_speAmp=7.6e-3);
    declProp("PmtTotal", m_PmtTotal=32400);
    declProp("waveform_width", m_width=13e-9);
    declProp("waveform_mu", m_mu=0.43);

    declProp("FadcRange", m_FadcRange=1.2288);//unit: the number of pe.
    declProp("FadcBit", m_FadcBit=14);
    declProp("FadcOffset", m_FadcOffset=0.25);
    declProp("storeElecTruth", m_enableStoreElecTruth=true);

    declProp("enableAssignSimTime", m_enableAssignSimTime=false);
    //declProp("simTime", m_simTime=1250);

    }
    private:
        void loadResponse();
        void generateOneChannel(int channelId,
                std::vector<Pulse*>& channelPulses,
                JM::ElecWaveform& channel,
                TimeStamp TriggerTime
                );
        double pmtPulse(double deltaT, int nPulse);
        double overshoot(double deltaT);

        double noise();

        double ampSatFactor(int nPulse);

        double chargeSatFactor(int nPulse);

        double saturationModel(double q, double qSat, double a);

        double electronicsNonl(double amp);

        int FADC_sample(double adc_range, double amp_val, double FadcBit);

        int FADCsample(double amp_val, int gain_marker);
        TimeStamp simTimeEarliest;

    ~TriggerHandlerWPLpmt() {

    }


    bool handle(const Trigger& trigger, JM::EvtNavigator* evtnav) {

        LogInfo << "Readout event : "<< m_eventID << std::endl;
        //clrar vector
        m_PulseTime_Offset.clear();
        m_nPE_perPMT.clear();
        m_PMTID.clear();

        m_Amplitude.clear();
        m_PMTID_perPulse.clear();

        m_nevents = 0;
        m_tags.clear();
        m_filenames.clear();
        m_entries.clear();
        m_nhits.clear();

        m_simTime = m_preTimeTolerance + m_postTimeTolerance;

        auto elec_header = JM::getHeaderObject<JM::WpWaveformHeader>(evtnav);
	if(!elec_header){
	    elec_header = new JM::WpWaveformHeader();
	    JM::addHeaderObject(evtnav, elec_header);
	}
        auto elec_truth_header = JM::getHeaderObject<JM::WpElecTruthHeader>(evtnav);
	if(!elec_truth_header and m_enableStoreElecTruth) {
	    elec_truth_header = new JM::WpElecTruthHeader();
	    JM::addHeaderObject(evtnav, elec_truth_header);
	}
          std::vector<JM::WpElecTruth>  truths;
        // generate waveforms first, then remove pulses.
       //Add Wptrigger
       auto trig_header = JM::getHeaderObject<JM::WpTriggerHeader>(evtnav);
        if(!trig_header){
                trig_header = new JM::WpTriggerHeader();
                JM::addHeaderObject(evtnav, trig_header);
        }
        auto wpEvent = new JM::WpWaveformEvt;
        auto trig_evt = new JM::WpTriggerEvt;
      //WP
 /*   
     std::deque<Trigger>& WP_event_trigger_buffer = m_elecsvc->get_event_trigger_buffer();
        std::vector<TimeStamp> vecTrigTime;
        std::vector<string>    vecTrigType;
        std::vector<int>       vecTrignFiredPmt;
        std::vector<int>       vecTrigVolumeId;
        LogInfo << " trigger size= "<<WP_event_trigger_buffer.size()<<endl;
        for (int i=0; i<WP_event_trigger_buffer.size(); i++) {
           LogInfo<< " trigger type "<<WP_event_trigger_buffer[i].type<<endl;
           if(WP_event_trigger_buffer[i].type == kTriggerWP) {
                vecTrigType.push_back("TriggerWP");
                vecTrigTime.push_back(WP_event_trigger_buffer[i].time);
                vecTrignFiredPmt.push_back(WP_event_trigger_buffer[i].nFiredPmt);
                vecTrigVolumeId.push_back(WP_event_trigger_buffer[i].volume_id);
                LogInfo << " trigger type ="<<"Wp,time="<<WP_event_trigger_buffer[i].time<<",nPMT="<<WP_event_trigger_buffer[i].nFiredPmt<<"; volume_id="<<WP_event_trigger_buffer[i].volume_id<<endl; 
            }
        }
        trig_evt->setTriggerType(vecTrigType);
        trig_evt->setTriggerTime(vecTrigTime);
        trig_evt->setNFiredPmt(vecTrignFiredPmt);
        trig_evt->setVolumeId(vecTrigVolumeId); 
   */ 
      // we will also remove it.
        std::deque<Pulse>& buffer = m_elecsvc->get_WPLPMT_pulse_buffer(); 
        int sz = buffer.size();
        // empty buffer, so we just return.
        if (sz==0) {
            return false;
        }


        std::deque<Pulse>::iterator it = buffer.begin();
        m_firstPE_PMTID =  it->pmtID; 

        // we only select pulses in a time window

        simTimeEarliest = trigger.time;
        simTimeEarliest.Subtract(m_preTimeTolerance*1e-9);
        TimeStamp simTimeLatest = trigger.time;
        simTimeLatest.Add(m_postTimeTolerance*1e-9);
        EventKeeper& keeper = EventKeeper::Instance();

        // group by pmt id
        std::map<int, std::vector<Pulse*> > pulseMap;

        for (int idx=0; idx<sz; ++idx) {
            Pulse& pulse = buffer[idx];
            // only consider LPMT in WP.
            // omit other PMTs.
            if (not PMT::IsWP(pulse.pmtID)) {
                continue;
            }

            // only select in time window
            if (pulse.pulseHitTime < simTimeEarliest) {
                continue;
            } else if (pulse.pulseHitTime >= simTimeLatest) {
                continue;
            }
            // truth information.
            if(pulse.truth and elec_truth_header)
	    {
            	JM::WpElecTruth aTruth;
            	if(pulse.type == kNormalPulse) {aTruth.setPulsetype(pulse.truth->m_entry_pointer->tag); }
            	else if(pulse.type == kDarkPulse) {aTruth.setPulsetype("DarkPulse"); }
           	else if(pulse.type == kAfterPulse) {aTruth.setPulsetype("AfterPulse"); }
            	else if(pulse.type == kDNAfterPulse) {aTruth.setPulsetype("DNAfterPulse");}
            	else{
                	LogError << "Unknown pulse type!" << std::endl;
                	return false;
            	    }

            	aTruth.setPmtId(pulse.pmtID);
           	aTruth.setNpe(pulse.truth->npe);
            	if(pulse.type == kNormalPulse or pulse.type == kAfterPulse) {
                	aTruth.setHitTime(pulse.truth->hitTime);
            	} else if (pulse.type == kDarkPulse or pulse.type == kDNAfterPulse) {
                	aTruth.setHitTime( (pulse.pulseHitTime - simTimeEarliest).GetSeconds()*1e9 );
            	}
            	aTruth.setAmplitude(pulse.amplitude);
            	aTruth.setTTS(pulse.truth->TTS);
            	aTruth.setTimeoffset(pulse.truth->timeoffset);
           	aTruth.setPulseHitTime(pulse.pulseHitTime);
            	truths.push_back(aTruth);
            	if(pulse.type == kNormalPulse) keeper.add(*(pulse.truth->m_entry_pointer));
            }
            pulseMap[pulse.pmtID].push_back(&pulse);
            m_Amplitude.push_back(pulse.amplitude);
            m_PMTID_perPulse.push_back(pulse.pmtID);
            //if(pulse.type == kNormalPulse) keeper.add(pulse.m_entry);
            //keeper.add(pulse.m_entry);

          
        }
            m_nPulse = m_PMTID_perPulse.size();

            const EventKeeper::Record& record = keeper.current_record();
            m_nevents = record.simevents.size();
            for (std::map<EventKeeper::Entry, int>::const_iterator it = record.simevents.begin();
              it != record.simevents.end(); ++it) {
               m_tags.push_back(it->first.tag);
               m_filenames.push_back(it->first.filename);
               m_entries.push_back(it->first.entry);
               m_nhits.push_back(it->second);
             }
            m_index_tree->Fill();
        // set truth
     if (elec_truth_header) {
        auto wppmtElecTruthEvent = new JM::WpElecTruthEvt();
        wppmtElecTruthEvent->setTruths(truths);
        elec_truth_header->setEvent(wppmtElecTruthEvent);
        }
        // std::cout << " WppmtTruths: " << elec_truth_header->wppmtTruth()->truths().size() <<std::endl;
     //   LogInfo << " WppmtTruths: " << elec_truth_header->wppmtTruth()->truths().size() <<std::endl;
        truths.clear();

        // generate waveform
        // auto crate = new std::map<int,JM::ElecWaveform>();

        // if(crate->size() == 0){

        //     for(int pmtIdx = 0; pmtIdx < m_PmtTotal; pmtIdx++){
        //         (*crate)[pmtIdx];
        //    }

        //    }else{
        //    LogInfo << "does not initialize crate" << std::endl;
        //    }

           // crate->setEvtTimeStamp(simTimeEarliest); 
           // crate->setTriggerTime(trigger.time);
           TimeStamp triggerTime = trigger.time;

           m_nPMT = 0;
           int total_pe =0;
           for(int channelId=0; channelId<m_PmtTotal; channelId++){
               auto& channel = *(wpEvent->addChannel(channelId));

              if(pulseMap[channelId].size() > 0){
                 m_PMTID.push_back(channelId);
                 m_nPE_perPMT.push_back(pulseMap[channelId].size());
                 total_pe = total_pe + pulseMap[channelId].size();
           //       LogInfo << " Channel Id: " << channelId<<": pe num="<<pulseMap[channelId].size() <<"total_pe="<<total_pe<<std::endl; 
                 generateOneChannel(channelId, pulseMap[channelId], channel, triggerTime);
              if(channelId == m_firstPE_PMTID){
                 for(int i=0; i<m_simTime; i++){
                 m_adc_firstPE[i] = channel.adc()[i];
                   }
                  }
               m_nPMT ++;
           }
         }


        // remove pulse
        // TODO: is that better to remove pulses in the main algorithm?
       
           for (std::deque<Pulse>::iterator it = buffer.begin();
               it != buffer.end(); ) {
             if ((it->pulseHitTime>=simTimeLatest)) {
                break;
              }
           bool flag = (PMT::IsWP(it->pmtID));
           if (flag) {
               it = buffer.erase(it);
             } else {
                ++it;
            }
         }
           // wpEvent->setChannelData(*crate);
           elec_header-> setEvent(wpEvent);
           trig_header->setEvent(trig_evt);
           m_evt_tree -> Fill();
           m_eventID++;
           // delete crate;
           return true;
        }

       bool init() {
        SniperPtr<ElecSimSvc> m_svc(*getRoot(), "ElecSimSvc");
        if (m_svc.invalid()) {
            LogError << "can't find service ElecSimSvc" << std::endl;
            return false;
        }
        m_elecsvc = m_svc.data();
        m_eventID = 0;
        LogInfo << "init trigger tool (LPMT). " << std::endl;
        loadResponse();
 
         //generate user data.

        SniperPtr<RootWriter> svc(*getRoot(),"RootWriter");
        if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
            << "enalbe it in your job option file."
            << std::endl;
        return false;
         }

        gROOT->ProcessLine("#include <vector>");

        m_evt_tree = svc->bookTree(*m_par, "USER_OUTPUT/WPSIMEVT", "WP TTS and Amplitude information");
        m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
        m_evt_tree->Branch("nPMT", &m_nPMT, "nPMT/I");
        m_evt_tree->Branch("nPulse", &m_nPulse, "nPulse/I");
        m_evt_tree->Branch("nPE_perPMT", &m_nPE_perPMT);
        m_evt_tree->Branch("PMTID", &m_PMTID);
        m_evt_tree->Branch("adc_firstPE", m_adc_firstPE, "adc_firstPE[1250]/I");
        m_evt_tree->Branch("PMTID_perPulse", &m_PMTID_perPulse);
        m_evt_tree->Branch("Amplitude", &m_Amplitude);


        m_index_tree = svc->bookTree(*m_par, "USER_OUTPUT/WPeventindex",
                               "event index between SimEvent and ElecEvent");
        m_index_tree->Branch("eventid", &m_eventID);
        m_index_tree->Branch("nevents", &m_nevents);
        m_index_tree->Branch("tags", &m_tags);
        m_index_tree->Branch("filenames", &m_filenames);
        m_index_tree->Branch("entries", &m_entries);
        m_index_tree->Branch("nhits", &m_nhits);

        return true;
         }

        ElecSimSvc* m_elecsvc;
   
        double m_preTimeTolerance;
        double m_postTimeTolerance;
        
        double m_preWaveSimWindow;
        TimeStamp m_simTimeEarliest; 
        double m_simTime;
        double m_FadcOffset;

        bool m_enableOvershoot;
        bool m_enableSaturation;
        bool m_enableElecNonL;
        bool m_enableNoise;
        bool m_enableAssignSimTime;
        bool m_enableStoreElecTruth;

        constexpr static double m_gainFactor = 1.0;
        double m_simFrequency;
        double m_noiseAmp;
        double m_speAmp;
        double m_width;
        double m_mu;
        constexpr static double m_linearityThreshold = 20;
        double m_PulseSampleWidth;
        bool m_enableFADC;
        double m_FadcBit;
        double m_FadcRange;

        int m_PmtTotal;
        const double m_LSB = 0.075e-3;//V
        const double m_highGainLsbFactor = 0.55;
        const double m_lowGainLsbFactor = 0.08;

        constexpr static double m_pulseCountSlotWidth = 10;
        // Number of counted slots before and after the pulse.
        constexpr static int m_pulseCountWindow = 1;
        // Ideal PMT pulse shape.
        std::vector<double> m_pmtPulse;
        // Ideal overshoot shape
        std::vector<double> m_overshoot;
        std::vector<double> m_rawSignal;

        TTree* m_evt_tree;
        int m_eventID;
        int m_nPMT;
        int m_nPulse;
        std::vector<int> m_PulseTime_Offset;
        std::vector<int> m_nPE_perPMT;
        std::vector<int> m_PMTID;
        int m_adc_firstPE[1250];
        int m_firstPE_PMTID;
        std::vector<double> m_Amplitude;
        std::vector<int> m_PMTID_perPulse;
        constexpr static unsigned int high_range_marker = 0;
        constexpr static unsigned int medium_range_marker = 1;
        constexpr static unsigned int low_range_marker = 2;
        constexpr static unsigned int high_gain_marker = 0;
        constexpr static unsigned int low_gain_marker = 1;
        constexpr static double gain_cut = 830;//Unit: mV

        // Save the index between SimEvent and ElecEvent
        TTree* m_index_tree;
        int m_nevents;
        std::vector<std::string> m_tags;
        std::vector<std::string> m_filenames;
        std::vector<int> m_entries;
        std::vector<int> m_nhits;

};

void TriggerHandlerWPLpmt::loadResponse(){
    double dT_seconds = (1. / m_simFrequency);
    double PulseSampleWidth = m_PulseSampleWidth*1e-9; //unit s

    int nPulseSamples = int(PulseSampleWidth/dT_seconds);
    int nOvershootSamples = int(16000/(dT_seconds*1e9)); //1 second == 1e9 ns
    m_pmtPulse.resize(nPulseSamples);
    m_overshoot.resize(nOvershootSamples);


    for (int i=0; i<nPulseSamples; i++) {
        m_pmtPulse[i] = pmtPulse(i*dT_seconds,1);
    }
    for (int i=0; i<nOvershootSamples; i++) {

        m_overshoot[i] = 0;
        // Store overshoot for raw signal
        if(m_enableOvershoot){
            m_overshoot[i] += overshoot(i*dT_seconds);
        }
    }

    if( !m_enableOvershoot ){
        LogInfo<<"Overshoot disabled"<<endl;
    }else{
        LogInfo<<"Overshoot enabled"<<endl;
    }


    if( !m_enableSaturation ){
        LogInfo << "Saturation disabled" << endl;
    }else{
        LogInfo << "Saturation enabled" << endl;
    }
}

double TriggerHandlerWPLpmt::pmtPulse(double deltaT, int nPulse) {
    // Return ideal single pe pulse with amplitude 1 V
    double width; // pulse width parameter
    double mu; // parameter determining the degree of asymmetry
    width = m_width;
    mu = m_mu;

    double shift = 6e-9 -width/1.5;
    if (deltaT-shift<0) return 0.;


    return - exp( -pow( log( (deltaT-shift)/width),2)
            / ( 2*pow(mu,2) ) ) ;  // unit V
}

double TriggerHandlerWPLpmt::overshoot(double deltaT) {
    if (deltaT < 0) return 0.;
    double amp = 0.01; // Relative overshoot amplitude for spe pulses
    // Fermi onset
    double t0   = 80e-9;
    double t1   = 20e-9;
    double fermi = 1. / (exp( (t0 - deltaT) / t1) + 1.);
    // Exponential overshoot component
    double tau = 880.e-9; // Overshoot decay time in s
    double expoOS = exp(-(deltaT-87e-9)/tau);
    // Slower overshoot component
    double mean = 0.6e-6;
    double sigma = 0.1e-6;
    double t = deltaT -mean;
    double gausOS = 0.12 * exp(pow(t,2)/(-2*pow(sigma,2)));
    // Undershoot 
    mean = 0.7e-6;
    sigma = 0.14e-6;
    t = deltaT -mean;
    double undershoot = -0.03 * exp(pow(t,2)/(-2*pow(sigma,2)));

    return  amp * fermi * (expoOS + gausOS + undershoot)
        ; //unit V
}


void TriggerHandlerWPLpmt::generateOneChannel(int channelId, vector<Pulse*>& channelPulses, JM::ElecWaveform& channel, TimeStamp TriggerTime){

    TimeStamp WaveSimFirstTime(0);
    WaveSimFirstTime = TriggerTime;
    WaveSimFirstTime.Subtract(m_preWaveSimWindow*1e-9);

    int simSamples = int(m_simTime * 1e-9 * m_simFrequency);
    if(simSamples != m_simTime){
        LogInfo<<"error, simSamples != m_simTime, simSamples: " << simSamples<<endl;
    }

    // Prepare Raw Signal
    if(m_rawSignal.size() != simSamples) m_rawSignal.resize( simSamples );

    double* rawStart = &m_rawSignal[0];

    for( unsigned int sigIdx = 0; sigIdx!=simSamples; sigIdx++){
        *(rawStart + sigIdx) = 0;
    }

    // Add noise to raw signal if requested
    if( m_enableNoise ){
        for(unsigned int index=0; index < simSamples; index++){
            *(rawStart + index) += noise();
        }
    }

    int channelPulsesN = channelPulses.size();

    if( channelPulsesN > 0 ){
        // Prepare time slots for pulse counting
        int numPulseTimeSlots = int(m_simTime
                /m_pulseCountSlotWidth) + 1;

        std::vector<int> pulseTimeSlots(numPulseTimeSlots);


        // Fill pulse count time slots
        // Count the number of pulses for each time slot to model nonlinearity
        for (int i = 0; i < numPulseTimeSlots; i++ ){
            pulseTimeSlots[i] = 0;
        }
        std::vector<Pulse*>::iterator pulseIter, pulseDone = channelPulses.end();

        for(pulseIter=channelPulses.begin(); pulseIter != pulseDone; ++pulseIter){
            int timeSlot = int(  ((*pulseIter)->pulseHitTime - WaveSimFirstTime).GetSeconds()*1e9
                    / m_pulseCountSlotWidth );

            int pulseNo = int((*pulseIter)->amplitude + 0.5);
            if(timeSlot>=numPulseTimeSlots) continue;
            pulseTimeSlots[timeSlot]+=pulseNo;
        }

        double* pmtPulseStart = &m_pmtPulse[0];
        double* overshootStart = &m_overshoot[0];

        int nPulseSamples = m_pmtPulse.size();
        int nOvershootSamples = m_overshoot.size();

        // Loop over pulses, add each to raw signal
        for(pulseIter=channelPulses.begin(); pulseIter != pulseDone; ++pulseIter){

            int tOffset = int(  ((*pulseIter)->pulseHitTime - simTimeEarliest).GetSeconds() * m_simFrequency  );
            float amplitude = (*pulseIter)->amplitude * m_speAmp * m_gainFactor;
            unsigned int nPulse=0;
            unsigned int nCoincPulse=0;
            float satAmp = amplitude;
            float satCharge = amplitude;
            // Count the total number of pulses within a nearby time window
            // This number is used to determine the nonlinearity
            if(channelPulsesN>5){

                int timeSlot = int(  ((*pulseIter)->pulseHitTime - WaveSimFirstTime).GetSeconds()*1e9
                        / m_pulseCountSlotWidth);

                nCoincPulse = pulseTimeSlots[timeSlot];

                for (int iSlot = timeSlot - m_pulseCountWindow;
                        iSlot <= timeSlot + m_pulseCountWindow; iSlot++){
                    if(iSlot>=0 && iSlot<numPulseTimeSlots)
                        nPulse += pulseTimeSlots[iSlot];
                }
                // Get saturated amplitudes
                satAmp *= ampSatFactor(nPulse);
                satCharge *= chargeSatFactor(nPulse);
            }else{
                nCoincPulse =1;
                nPulse = 1;
            }

            unsigned int nSamples = int(8.33 * nPulseSamples);

            // Now add pulses to the raw and shaped signal
            for(unsigned int index = 0; index < nSamples; index++){
                unsigned int sampleIdx = tOffset + index;
                double idxTime = index * (1. / m_simFrequency);
                if(sampleIdx>0 && sampleIdx<simSamples){
                    if(index<nPulseSamples) {
                        if(nPulse>m_linearityThreshold){
                            *(rawStart + sampleIdx) -= satAmp * pmtPulse(idxTime, nPulse);

                         } else

                            *(rawStart + sampleIdx) -= satCharge * (*(pmtPulseStart + index));
                      }
                    // Add overshoot
                    if(m_enableOvershoot){
                        *(rawStart + sampleIdx) -= satCharge * (*(overshootStart+index));     
                    }
                  }   
                }   
              }// end loop pulse
            }

    vector<double>::iterator sig_it ;
     // determine electronics charge nonlinearity: maximum amplitude in one channel
    float max_amp = -1;
    for(sig_it = m_rawSignal.begin();
        sig_it != m_rawSignal.end();
        sig_it++){
        if(TMath::Abs(*sig_it) >= max_amp ) max_amp = TMath::Abs(*sig_it);
    }

    int gain_marker = 0;
    if(max_amp*1000 <= gain_cut) { gain_marker = 0;}  else if(max_amp*1000 > gain_cut) {gain_marker = 1;}
    float elecRatio = electronicsNonl(max_amp);
    //LogInfo<<"max_amp= "<<max_amp<<";gain_marker ="<<gain_marker<<"; elecRatio=" << elecRatio<<endl;   

    int m_tdc_temp = 0;
    for(sig_it = m_rawSignal.begin();
            sig_it != m_rawSignal.end();
            sig_it++){
        //if(*sig_it!=0)   // fangxiao change it so I save all point 
       // cout<<"adc: " << *sig_it<<endl;
        if(m_enableFADC){
             if(m_enableElecNonL){
               channel.adc().push_back( int(FADCsample( (*sig_it)*elecRatio, gain_marker)));
               // channel.adc().push_back( FADC_sample(m_FadcRange*m_speAmp, *sig_it, m_FadcBit) );
              //  LogInfo<<"signal amp = "<<(*sig_it)*elecRatio<<"; tdc="<<m_tdc_temp<<endl; 
            }
             else { 
              channel.adc().push_back( FADCsample( *sig_it, gain_marker));
             }
        }else{
            channel.adc().push_back(*sig_it);
        }
       m_tdc_temp++;
      
    }
}

double TriggerHandlerWPLpmt::noise(){
    // Simple noise model
    // Approximate Gaussian-distributed noise
    return gRandom->Gaus(0,1) * m_noiseAmp; // no offset

}

double TriggerHandlerWPLpmt::ampSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 533.98;
    double a = 2.884;
    return saturationModel(q, qSat, a);

}

double TriggerHandlerWPLpmt::chargeSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 939.77;
    double a = 2.113;
    return saturationModel(q, qSat, a);
}

double TriggerHandlerWPLpmt::saturationModel(double q, double qSat, double a){
    if(q<1) return 1;
    double num = pow(1 + 8*pow(q/qSat,a),0.5) - 1;
    double denom = 4* pow(q/qSat,a);
    return pow(num/denom,0.5);
}
double TriggerHandlerWPLpmt::electronicsNonl( double amp ){
   // reference from Lpmt handle
    amp = amp*1000;  // V->mV
    double ratio;
    double p10 =  -230.548; double p11 = 0.47968; double p12 = 0.00131823; double p13 = -2.03957e-06;
    double p20 = -586.598; double p21 = 0.920295; double p22 = -0.000290151; double p23 = 1.86565e-08;

    if(amp<100){ ratio = 1; }
    else if(amp >=100 and amp<200){ ratio = 1-(1-0.9957)*(amp-100)/100; }
    else if(amp>=200 and amp<gain_cut ) { ratio = 1+(p10+p11*amp+p12*amp*amp + p13*TMath::Power(amp,3))/(145.9893*amp-5814.0936); }
    else if(amp>=gain_cut) {ratio = 1+(p20+p21*amp+p22*TMath::Power(amp,2)+p23*TMath::Power(amp,3))/(21.1727*amp+658.8386);}
    return ratio;
}


int TriggerHandlerWPLpmt::FADC_sample(double adc_range, double amp_val, double FadcBit){
      //double adc_range = 1; //unit v
     double LSB = 0.075e-3;
     //double LSB = adc_range/TMath::Power(2, FadcBit);
     if((amp_val+m_FadcOffset)<0) return 0;
    unsigned int amp_adc =  int( (amp_val+m_FadcOffset)/LSB ); //with noise some point<0
   /*
    double LSB = 0;
    double High_gain = 0.06;   //0.06pe/bit , for 8 bit: range=0.06 * 2^8
    double medium_gain = 0.4;
    double low_gain = 8;
    int    amp_adc = 0;
    unsigned int amp_Fadc = 0;
    double High_range = High_gain * TMath::Power(2, FadcBit) * m_speAmp;
    double medium_range = medium_gain * TMath::Power(2, FadcBit) * m_speAmp;
    double low_range = low_gain * TMath::Power(2, FadcBit) * m_speAmp;
    //LogInfo<<"high range pe: " << High_gain * TMath::Power(2,FadcBit)<<endl;

    if((amp_val+m_FadcOffset)<0){
     amp_Fadc = 0;
    }else if( 0 <= (amp_val+m_FadcOffset) && (amp_val+m_FadcOffset) < High_range){
        LSB = High_range/TMath::Power(2,FadcBit);
     amp_adc = int( (amp_val+m_FadcOffset)/LSB );
     amp_Fadc = amp_adc+(high_range_marker<<8);
    }else if( High_range <= (amp_val+m_FadcOffset) && (amp_val+m_FadcOffset) < medium_range ){
        LSB = medium_range/TMath::Power(2,FadcBit);
     amp_adc = int( (amp_val+m_FadcOffset)/LSB );
     amp_Fadc = amp_adc+(medium_range_marker<<8);
    }else if(( amp_val+m_FadcOffset) >= medium_range ){
        LSB = low_range/TMath::Power(2,FadcBit);
     amp_adc = int( (amp_val+m_FadcOffset)/LSB );
     amp_Fadc = amp_adc+(low_range_marker<<8);
    }

     //amp_adc = int( (amp_val+m_FadcOffset)/LSB );    //with noise some point<0
 */
    return amp_adc;

}

int TriggerHandlerWPLpmt::FADCsample(double amp_val, int gain_marker)
{
    double LSB = m_LSB; //V
    int amp_adc = 0;
    unsigned int amp_Fadc = 0;
    double LFadcOffsetValue = LSB/m_lowGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*m_FadcOffset;//offset for low gain
    double HFadcOffsetValue = LSB/m_highGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*m_FadcOffset;//offset for high gain

    double low_gain_range = LSB/m_lowGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*(1-m_FadcOffset); // currently set 0.75 as full range
    
//    LogInfo<<"LFadcOffsetValue = "<<LFadcOffsetValue<<";HFadcOffsetValue ="<<HFadcOffsetValue<<"; low_gain_range=" << low_gain_range<<endl; 
    //  dynamic range : 1000 p.e.
    if(gain_marker ==1 and amp_val>low_gain_range) {
        LSB /= m_lowGainLsbFactor;
        amp_adc = int( (low_gain_range+LFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
        amp_Fadc = amp_adc+(low_gain_marker<<int(m_FadcBit));
        return amp_Fadc;
     //   LogInfo<<"gain_maker 1 and  >log gain range, amp_Fadc = "<<amp_Fadc<<endl;
    }

    if(gain_marker == 0) {   // high gain
        LSB /= m_highGainLsbFactor;
        amp_adc = int( (amp_val+HFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
        // LogInfo<<"gain_maker 0 before , high gain, amp_adc = "<<amp_adc<<endl;
        amp_Fadc = amp_adc+(high_gain_marker<<int(m_FadcBit));
        //LogInfo<<"gain_maker 0, high gain, amp_Fadc = "<<amp_Fadc<<endl;
    } else if (gain_marker == 1) {   // low gain
        LSB /= m_lowGainLsbFactor;
        amp_adc = int( (amp_val+LFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
       //LogInfo<<"gain_maker 1 before , high gain, amp_adc = "<<amp_adc<<endl;
        amp_Fadc = amp_adc+(low_gain_marker<<int(m_FadcBit));
       // LogInfo<<"gain_maker 1 and  <log gain range, amp_Fadc = "<<amp_Fadc<<endl;
    } else {
        LogError << " Unknown Gain Type " << std::endl;
    }
    //LogInfo<<"Finally, amp_Fadc = "<<amp_Fadc<<endl;
    return amp_Fadc;
}



// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerHandlerWPLpmt);
