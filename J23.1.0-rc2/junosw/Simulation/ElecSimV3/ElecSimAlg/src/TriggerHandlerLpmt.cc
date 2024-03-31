#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdTriggerHeader.h"

#include "Event/CdLpmtElecHeader.h"

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
#include "Geometry/IPMTParamSvc.h"
#include "Geometry/PMT.h"


using namespace std;
/*
 * This is actually the wavefrom simulation of LPMT
 */

class TriggerHandlerLpmt: public ToolBase, public ITriggerHandler {
public:
    TriggerHandlerLpmt(const std::string& name): ToolBase(name),
                                                 m_elecsvc(0), m_pmtparamsvc(0) {

        declProp("preTimeTolerance", m_preTimeTolerance=300); // ns
        declProp("postTimeTolerance", m_postTimeTolerance=700); // ns

        //declProp("preWaveSimWindow",m_preWaveSimWindow=500); 
        declProp("PulseSampleWidth", m_PulseSampleWidth=150);  //unit ns, the time window for one Ideal pulse waveform width
        declProp("enableOvershoot", m_enableOvershoot=true);
        declProp("enableSaturation", m_enableSaturation=true);
        declProp("enableNoise", m_enableNoise=true);
        declProp("enableFADC", m_enableFADC=true);
        declProp("enableElecNonl", m_enableElecNonL=true);
        declProp("simFrequency", m_simFrequency=1e9);
        declProp("noiseAmp", m_noiseAmp=0.56e-3);
        declProp("HamaspeAmp", m_HamaspeAmp=6.5e-3);
        declProp("NNVTspeAmp", m_NNVTspeAmp=7.6e-3);
        declProp("PmtTotal", m_PmtTotal=17613);
        declProp("waveform_width", m_width=13e-9);
        declProp("waveform_mu", m_mu=0.43);

        declProp("FadcRange", m_FadcRange=1.2288);//unit: the number of pe.
        //declProp("FadcBit", m_FadcBit=14);
        declProp("FadcOffset", m_FadcOffset=0.25);//percentage of the full range

        declProp("storeElecTruth", m_enableStoreElecTruth=true);

        declProp("enableAssignSimTime", m_enableAssignSimTime=false);
        //declProp("simTime", m_simTime=1250);

        // time window for event split:
        declProp("preSplitTrackTime", m_preSplitTrackTime=400);
        declProp("postSplitTrackTime", m_postSplitTrackTime=0);
	declProp("enableSplitTrackRange",m_enableSlitTrackRange=false);

	//tq information
	declProp("enableCDLpmtTQ",m_CDLpmtTQ = true);
	declProp("enableTagNotOverThresholdTQ",m_TagNotOverThresholdTQ = false);

    }
private:
    void loadResponse();
    void generateOneChannel(int channelId,
                            std::vector<Pulse*>& channelPulses,
                            JM::ElecWaveform& channel,
                            TimeStamp TriggerTime
                            );
    double pmtPulse(double deltaT, int nPulse);
    double pmtPulseMCP(double deltaT, int nPulse);
    double overshoot(double deltaT);

    double noise();

    double ampSatFactor(int nPulse);

    double chargeSatFactor(int nPulse);

    double pmtSatFactor(int nPulse);

    double saturationModel(double q, double qSat, double a);

    double electronicsNonl(double amp);


    int FADC_sample(double adc_range, double amp_val, double FadcBit);

    uint16_t FADCsample(double amp_val, int gain_marker);

    JM::ElecChannel* calculate_tq(int pmtID, const JM::ElecWaveform& channel);
    int integral(int index, double threshold, double baseline, std::vector<uint16_t>& a_adc, const std::vector<uint16_t>& a_tdc, JM::ElecChannel* ltq_channel);
    double calculate_threshold(std::deque<unsigned int>& baseline_buffer, double& baseline);
    void slide_baseline_buffer(std::deque<unsigned int>& baseline_buffer,int index, std::vector<uint16_t>& a_adc);
    TimeStamp simTimeEarliest;

    TimeStamp forceTriggerTime;

    ~TriggerHandlerLpmt() {

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


        auto elec_header = JM::getHeaderObject<JM::CdWaveformHeader>(evtnav);
	if(!elec_header){
		elec_header = new JM::CdWaveformHeader();
		JM::addHeaderObject(evtnav,elec_header);
	}
        auto elec_truth_header = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(evtnav);
	if(!elec_truth_header and m_enableStoreElecTruth){
		elec_truth_header = new JM::CdLpmtElecTruthHeader();
		JM::addHeaderObject(evtnav,elec_truth_header);
	}	
        auto elec_truth_header_track = JM::getHeaderObject<JM::TrackElecTruthHeader>(evtnav);
	if(!elec_truth_header_track and m_enableStoreElecTruth){
		elec_truth_header_track = new JM::TrackElecTruthHeader();
		JM::addHeaderObject(evtnav, elec_truth_header_track);
	}
        auto trig_header = JM::getHeaderObject<JM::CdTriggerHeader>(evtnav);
	if(!trig_header){
		trig_header = new JM::CdTriggerHeader();
		JM::addHeaderObject(evtnav, trig_header);
	}
	//Haosen Zhang add:
	auto elec_ltq_header = JM::getHeaderObject<JM::CdLpmtElecHeader>(evtnav);
	if(!elec_ltq_header){
		elec_ltq_header = new JM::CdLpmtElecHeader();
		JM::addHeaderObject(evtnav, elec_ltq_header);
	}
	JM::CdLpmtElecEvt* lpmtElecEvt = new JM::CdLpmtElecEvt(); 

        JM::CdLpmtElecTruthEvt* lpmtElecTruthEvent = nullptr;
        if (m_enableStoreElecTruth) {
            lpmtElecTruthEvent = new JM::CdLpmtElecTruthEvt();
        }
        JM::TrackElecTruthEvt* trackElecTruthEvent = nullptr;
        if (m_enableStoreElecTruth) {
            trackElecTruthEvent = new JM::TrackElecTruthEvt();
        }

        // generate waveforms first, then remove pulses.
        // if the pulse is earlier than the trigger time window,
        // we will also remove it.
        auto elec_evt = new JM::CdWaveformEvt;
        auto trig_evt = new JM::CdTriggerEvt;
        std::deque<Trigger>& CD_event_trigger_buffer = m_elecsvc->get_event_trigger_buffer();

        std::vector<TimeStamp> vecTrigTime;
        std::vector<string>    vecTrigType;
        std::vector<int>       vecTrignFiredPmt;
        std::vector<int>       vecTrigVolumeId;

        for (int i=0; i<CD_event_trigger_buffer.size(); i++) {
            // only Lpmt included ...
            if(CD_event_trigger_buffer[i].type != kTriggerSpmt) {
                vecTrigTime.push_back(CD_event_trigger_buffer[i].time);
                vecTrignFiredPmt.push_back(CD_event_trigger_buffer[i].nFiredPmt);
                vecTrigVolumeId.push_back(CD_event_trigger_buffer[i].volume_id);
                if (CD_event_trigger_buffer[i].type == kTriggerLpmt) {
                    vecTrigType.push_back("TriggerLpmt");
                } else if (CD_event_trigger_buffer[i].type == kTriggerLpmtStd) {
                    vecTrigType.push_back("TriggerLpmtStd");
                } else if (CD_event_trigger_buffer[i].type == kTriggerLpmtVFL) {
                    vecTrigType.push_back("TriggerLpmtVFL");
                }
            }
        }

        trig_evt->setTriggerType(vecTrigType);
        trig_evt->setTriggerTime(vecTrigTime);
        trig_evt->setNFiredPmt(vecTrignFiredPmt);
        trig_evt->setVolumeId(vecTrigVolumeId);


        std::deque<Pulse>& buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
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

        // split track time window: 
        splitTrackEarliest = simTimeEarliest;
        splitTrackEarliest.Subtract(m_preSplitTrackTime*1e-9);
        splitTrackLatest   = simTimeLatest;
        splitTrackLatest.Subtract(m_postSplitTrackTime*1e-9);

        // group by pmt id
        std::map<int, std::vector<Pulse*> > pulseMap;
        bool firstPE_flag = true;
        for (int idx=0; idx<sz; ++idx) {
            Pulse& pulse = buffer[idx];
            // only consider LPMT in CD.
            // omit other PMTs.
            if ((not PMT::IsCD(pulse.pmtID)) or (not PMT::Is20inch(pulse.pmtID))) {
                continue;
            }

            // only select in time window
            if (pulse.pulseHitTime < simTimeEarliest) {
                continue;
            } else if (pulse.pulseHitTime >= simTimeLatest) {
                continue;
            }
            
            forceTriggerTime = pulse.evtTimeStamp;

            if(firstPE_flag) m_firstPE_PMTID =  pulse.pmtID;
            firstPE_flag = false;
            // truth information.
            if(pulse.truth and lpmtElecTruthEvent)
	    {
            	JM::LpmtElecTruth aTruth;

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
            	lpmtElecTruthEvent->AddTruth(aTruth);
	    }

            // For Event Keeper
            if (pulse.truth) {
            	if(pulse.type == kNormalPulse) keeper.add(*(pulse.truth->m_entry_pointer));
            }

            pulseMap[pulse.pmtID].push_back(&pulse);
            m_Amplitude.push_back(pulse.amplitude);
            m_PMTID_perPulse.push_back(pulse.pmtID);

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
            std::shared_ptr<JM::EvtNavigator> Nav = it->first.evtnav;
            TTimeStamp m_TimeStamp = Nav->TimeStamp();
            LogDebug << "Nav TimeStamp : " << m_TimeStamp << std::endl;
            auto cur_simhdr = dynamic_cast<JM::SimHeader*>(it->first.header);
            
            // For Track Truth
            if(cur_simhdr->hasEvent() and trackElecTruthEvent) {
                auto cur_simevt = dynamic_cast<JM::SimEvt*>(cur_simhdr->event());
                const auto& trc = cur_simevt->getTracksVec();
                for(auto it_track = trc.begin(); it_track!=trc.end(); ++it_track) {
                    auto track = *it_track;

                    TimeStamp trackT(m_TimeStamp.GetSec()+m_TimeStamp.GetNanoSec()*1e-9);
                    trackT.Add(track->getInitT()*1e-9);
                    if((trackT<splitTrackEarliest or trackT>splitTrackLatest)&&m_enableSlitTrackRange) continue;

                    JM::TrackElecTruth aTrackTruth;
                    aTrackTruth.setEdepX(track->getEdepX());
                    aTrackTruth.setEdepY(track->getEdepY());
                    aTrackTruth.setEdepZ(track->getEdepZ());
                    aTrackTruth.setPdgid(track->getPDGID());
                    aTrackTruth.setEdep(track->getEdep());
                    aTrackTruth.setQedep(track->getQEdep());
                    aTrackTruth.setInitX(track->getInitX());
                    aTrackTruth.setInitY(track->getInitY());
                    aTrackTruth.setInitZ(track->getInitZ());
                    aTrackTruth.setInitT(trackT.GetNanoSec()+trackT.GetSec()*1e9);
                    aTrackTruth.setInitPx(track->getInitPx());
                    aTrackTruth.setInitPy(track->getInitPy());
                    aTrackTruth.setInitPz(track->getInitPz());
                    aTrackTruth.setInitMass(track->getInitMass());
                    aTrackTruth.setTrackTime(trackT);

                    trackElecTruthEvent->AddTrackTruth(aTrackTruth);
                }
            }
        }
        m_index_tree->Fill();

        //set truth
        if (elec_truth_header and lpmtElecTruthEvent) {
            elec_truth_header->setEvent(lpmtElecTruthEvent);
        }
        if (elec_truth_header_track and trackElecTruthEvent) {
            elec_truth_header_track->setEvent(trackElecTruthEvent);
        }

        if (elec_truth_header and lpmtElecTruthEvent
            and elec_truth_header_track and trackElecTruthEvent) {
            LogInfo << " lpmtTruths: " << elec_truth_header->event()->truths().size() << " track_truths: "<< elec_truth_header_track->event()->truths().size() <<std::endl; 
        }

        // generate waveform
        // auto crate = new std::map<int,JM::ElecWaveform>();

        // if(crate->size() == 0){

        //     for(int pmtIdx = 0; pmtIdx < m_PmtTotal; pmtIdx++){
        //         (*crate)[pmtIdx];
        //     }

        // }else{
        //     LogInfo << "does not initialize crate" << std::endl;
        // }

        // crate->setEvtTimeStamp(forceTriggerTime);
        // //crate->setEvtTimeStamp(simTimeEarliest); 
        // crate->setTriggerTime(trigger.time);

        TimeStamp triggerTime = trigger.time;

        m_nPMT = 0;
	

        for(int channelId=0; channelId<m_PmtTotal; channelId++){

            if(pulseMap[channelId].size() > 0){
                m_PMTID.push_back(channelId);
                m_nPE_perPMT.push_back(pulseMap[channelId].size());
            	auto& channel = *(elec_evt->addChannel(channelId));
                generateOneChannel(channelId, pulseMap[channelId], channel, triggerTime);
		if(m_CDLpmtTQ)
		{
			auto tq_channel = calculate_tq(channelId, channel);
			if(tq_channel != nullptr) lpmtElecEvt->addChannel(channelId,tq_channel);
		}
                if(channelId == m_firstPE_PMTID){
                    for(int i=0; i<m_simTime; i++){
                        m_adc_firstPE.push_back(channel.adc()[i]);
                    }
                }
                m_nPMT ++;
            }
        }
	

        // remove pulse
        // TODO: is that better to remove pulses in the main algorithm?

        LogDebug << "Start to remove pulses in buffer." << std::endl;
        LogDebug << "Size of buffer before removing: " << buffer.size() << std::endl;
        LogDebug << "Trigger time: " << trigger.time << "; End time of readout: " << simTimeLatest << std::endl;
        if(buffer.size() > 0) LogDebug << "First pulse time before removing: " << buffer[0].pulseHitTime << std::endl;
        for (it = buffer.begin(); it != buffer.end(); ) {
            if ((it->pulseHitTime>=simTimeLatest)) {
                break;
            }
            bool flag = PMT::IsCD(it->pmtID) and PMT::Is20inch(it->pmtID);
            if (flag) {
                it = buffer.erase(it);
            } else {
                ++it;
            }
        }

        LogDebug << "Stop to remove pulses in buffer." << std::endl;
        LogDebug << "Size of buffer after removing: " << buffer.size() << std::endl;
        if(buffer.size() > 0) LogDebug << "First pulse time in buffer after removing: " << buffer[0].pulseHitTime << std::endl;
	m_elecsvc->setCDDrakPulseStartTime(simTimeLatest);
	m_elecsvc->SetEarliestCDTriggerTime(trigger.time);

        // elec_evt->setChannelData(*crate);
        elec_header->setEvent(elec_evt);
        trig_header->setEvent(trig_evt);
	elec_ltq_header->setEvent(lpmtElecEvt);
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

        SniperPtr<IPMTParamSvc> m_pmtsvc(*getRoot(), "PMTParamSvc");
        if (m_pmtsvc.invalid()) {
            LogError << "can't find service PMTParamSvc" << std::endl;
            return false;
        }
        m_pmtparamsvc = m_pmtsvc.data();
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

        m_evt_tree = svc->bookTree(*m_par, "USER_OUTPUT/CDLPMTSIMEVT", "TTS and Amplitude information");

        m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
        m_evt_tree->Branch("nPMT", &m_nPMT, "nPMT/I");
        m_evt_tree->Branch("nPulse", &m_nPulse, "nPulse/I");
        m_evt_tree->Branch("nPE_perPMT", &m_nPE_perPMT);
        m_evt_tree->Branch("PMTID", &m_PMTID);
        m_evt_tree->Branch("adc_firstPE", &m_adc_firstPE);
        m_evt_tree->Branch("PMTID_perPulse", &m_PMTID_perPulse);
        m_evt_tree->Branch("Amplitude", &m_Amplitude);


        m_index_tree = svc->bookTree(*m_par, "USER_OUTPUT/eventindex",
                                     "event index between SimEvent and ElecEvent");
        m_index_tree->Branch("eventid", &m_eventID);
        m_index_tree->Branch("nevents", &m_nevents);
        m_index_tree->Branch("tags", &m_tags);
        m_index_tree->Branch("filenames", &m_filenames);
        m_index_tree->Branch("entries", &m_entries);
        m_index_tree->Branch("nhits", &m_nhits);

   	m_LFadcOffsetValue = m_LSB/m_lowGainLsbFactor*(TMath::Power(2,m_FadcBit)-1)*m_FadcOffset;//offset for low gain
	m_HFadcOffsetValue = m_LSB/m_highGainLsbFactor*(TMath::Power(2,m_FadcBit)-1)*m_FadcOffset;//offset for high gain
	m_low_gain_range = m_LSB/m_lowGainLsbFactor*(TMath::Power(2,m_FadcBit)-1)*(1-m_FadcOffset); // currently set 0.75 as full range

    	m_adc_offset = int((TMath::Power(2,m_FadcBit)-1)*m_FadcOffset);

        return true;
    }

    ElecSimSvc* m_elecsvc;
    IPMTParamSvc* m_pmtparamsvc;
   
    double m_preTimeTolerance;
    double m_postTimeTolerance;
        
    //double m_preWaveSimWindow;
    TimeStamp m_simTimeEarliest; 
    int m_simTime;
    double m_FadcOffset;

    bool m_enableSlitTrackRange;
    double m_preSplitTrackTime;
    double m_postSplitTrackTime;
    TimeStamp splitTrackEarliest;
    TimeStamp splitTrackLatest;

    bool m_enableOvershoot;
    bool m_enableSaturation;
    bool m_enableElecNonL;
    bool m_enableNoise;
    bool m_enableAssignSimTime;

    constexpr static double m_gainFactor = 1.0;
    double m_simFrequency;
    double m_noiseAmp;
    double m_HamaspeAmp;
    double m_NNVTspeAmp;
    double m_width;
    double m_mu;
    constexpr static double m_linearityThreshold = 20;
    double m_PulseSampleWidth;
    bool m_enableFADC;
    const int m_FadcBit = 14; //ADC bit
    double m_FadcRange;

    // By default, the Elec Truth will be created and stored into EvtNavigator.
    // In order to reduce the memory usage, user can disable this part. 
    bool m_enableStoreElecTruth;

    int m_PmtTotal;
    bool m_CDLpmtTQ;
    bool m_TagNotOverThresholdTQ;
    const double m_LSB = 0.075e-3;//V
    const double m_highGainLsbFactor = 0.55;
    const double m_lowGainLsbFactor = 0.08;
    double m_LFadcOffsetValue;
    double m_HFadcOffsetValue;
    double m_low_gain_range;

    int m_adc_offset;

    constexpr static double m_pulseCountSlotWidth = 10;
    // Number of counted slots before and after the pulse.
    constexpr static int m_pulseCountWindow = 1;
    // Ideal PMT pulse shape.
    std::vector<double> m_pmtPulse;
    std::vector<double> m_pmtPulseMCP;

    // Ideal overshoot shape Unit:V
    std::vector<double> m_overshoot;
    std::vector<double> m_rawSignal;

    TTree* m_evt_tree;
    int m_eventID;
    int m_nPMT;
    int m_nPulse;
    std::vector<int> m_PulseTime_Offset;
    std::vector<int> m_nPE_perPMT;
    std::vector<int> m_PMTID;
    //int m_adc_firstPE[1250];
    std::vector<int> m_adc_firstPE;
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

void TriggerHandlerLpmt::loadResponse(){
    double dT_seconds = (1. / m_simFrequency);
    double PulseSampleWidth = m_PulseSampleWidth*1e-9; //unit s

    int nPulseSamples = int(PulseSampleWidth/dT_seconds);
    int nOvershootSamples = int(16000/(dT_seconds*1e9)); //1 second == 1e9 ns
    m_pmtPulse.resize(nPulseSamples);
    m_pmtPulseMCP.resize(nPulseSamples);

    m_overshoot.resize(nOvershootSamples);


    for (int i=0; i<nPulseSamples; i++) {
        m_pmtPulse[i] = pmtPulse(i*dT_seconds,1);
        m_pmtPulseMCP[i] = pmtPulseMCP(i*dT_seconds,1);
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

double TriggerHandlerLpmt::pmtPulse(double deltaT, int nPulse) {
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


double TriggerHandlerLpmt::pmtPulseMCP(double deltaT, int nPulse) {
    // MCP pulse template.
    double pulseAmp[150] = {-0.00,-0.000764915,-0.00403849,-0.0258191,-0.0549896,-0.313375,-0.522333,-0.73988,-0.893803,-0.974086,-0.999983,-0.969776,-0.9232,-0.866285,-0.80869,-0.757577,-0.706139,-0.657457,-0.585892,-0.510243,-0.441336,-0.385134,-0.349644,-0.331558,-0.321413,-0.298174,-0.282325,-0.264887,-0.240903,-0.217502,-0.175334,-0.127592,-0.0905451,-0.0708713,-0.058328,-0.0499982,-0.0474377,-0.0431917,-0.0371956,-0.0304216,-0.0264349,-0.0193368,-0.0186561,-0.0231614,-0.0270183,-0.0359315,-0.0401126,-0.0380059,-0.0335007,-0.0294492,-0.0233882,-0.0207953,-0.019985,-0.0149288,-0.0129841,-0.0131137,-0.0149288,-0.0114932,-0.00958088,-0.00345508,0.00189284,-0.000570445,-0.00270961,-0.0105856,-0.015415,-0.0178458,-0.0160956,-0.0127896,-0.00776583,-0.00335785,0.000207434,-0.00164003,-0.00027874,-0.00773342,-0.0121738,-0.0110718,-0.0129193,-0.00812236,-0.0074093,-0.00497843,-0.00283926,-0.00442743,-0.00672865,-0.00971053,-0.0119145,-0.0152853,-0.0145074,-0.00996982,-0.00653419,-0.00251514,-0.00222344,0.000531551,-0.00358473,-0.00507};
    int index = deltaT/1e-9;
    if(index>42) index = 0;     
    return pulseAmp[index];
}

double TriggerHandlerLpmt::overshoot(double deltaT) {
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


void TriggerHandlerLpmt::generateOneChannel(int channelId, vector<Pulse*>& channelPulses, JM::ElecWaveform& channel, TimeStamp TriggerTime){

    //TimeStamp WaveSimFirstTime(0);
    //WaveSimFirstTime = TriggerTime;
    //WaveSimFirstTime.Subtract(m_preWaveSimWindow*1e-9);

    int simSamples = int(m_simTime * 1e-9 * m_simFrequency);
    if(simSamples != m_simTime){
        LogInfo<<"error, simSamples != m_simTime, simSamples: " << simSamples<<endl;
    }

    // Prepare Raw Signal Unit: V
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
            int timeSlot = int(  ((*pulseIter)->pulseHitTime - simTimeEarliest).GetSeconds()*1e9
                                 / m_pulseCountSlotWidth );

            int pulseNo = int((*pulseIter)->amplitude + 0.5);
            if(timeSlot>=numPulseTimeSlots) continue;
            pulseTimeSlots[timeSlot]+=pulseNo;
            
        }
        double* pmtPulseStart;
        if(m_pmtparamsvc->isHamamatsu(channelId)==1){ 
            pmtPulseStart = &m_pmtPulse[0];
        }else{
            pmtPulseStart = &m_pmtPulseMCP[0];
        }
        double* overshootStart = &m_overshoot[0];

        int nPulseSamples = m_pmtPulse.size();
        int nOvershootSamples = m_overshoot.size();

        // Loop over pulses, add each to raw signal
        for(pulseIter=channelPulses.begin(); pulseIter != pulseDone; ++pulseIter){

            int tOffset = int(  ((*pulseIter)->pulseHitTime - simTimeEarliest).GetSeconds() * m_simFrequency  );
            float amplitude = 1;
            if(m_pmtparamsvc->isHamamatsu(channelId)==1){
                amplitude = (*pulseIter)->amplitude * m_HamaspeAmp * m_gainFactor;
            }else{
                amplitude = (*pulseIter)->amplitude * m_NNVTspeAmp * m_gainFactor;
            }
            unsigned int nPulse=0;
            unsigned int nCoincPulse=0;
            float satAmp = amplitude;
            float satCharge = amplitude;
            float satFactor = amplitude;
            // Count the total number of pulses within a nearby time window
            // This number is used to determine the nonlinearity
            if(channelPulsesN>5){

                int timeSlot = int(  ((*pulseIter)->pulseHitTime - simTimeEarliest).GetSeconds()*1e9
                                     / m_pulseCountSlotWidth);

                nCoincPulse = pulseTimeSlots[timeSlot];

                for (int iSlot = timeSlot - m_pulseCountWindow;
                     iSlot <= timeSlot + m_pulseCountWindow; iSlot++){
                    if(iSlot>=0 && iSlot<numPulseTimeSlots)
                        nPulse += pulseTimeSlots[iSlot];
                }
                // Get saturated amplitudes
                if (m_enableSaturation) { 
                    satAmp *= ampSatFactor(nPulse);
                    satCharge *= chargeSatFactor(nPulse);
                    satFactor *= pmtSatFactor(nPulse);
                }
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
                        if(m_pmtparamsvc->isHamamatsu(channelId)==1){
                            *(rawStart+sampleIdx) -= satFactor * pmtPulse(idxTime, nPulse);
                        } else {
                            *(rawStart+sampleIdx) -= satFactor * pmtPulseMCP(idxTime, nPulse);
                        }
    

                        /*
                          if(nPulse>m_linearityThreshold){
                          if(m_pmtparamsvc->isHamamatsu(channelId)==1){
                          *(rawStart + sampleIdx) -= satAmp * pmtPulse(idxTime, nPulse);
                          }else{
                          *(rawStart + sampleIdx) -= satAmp * pmtPulseMCP(idxTime, nPulse); 
                          }
                          } else

                          *(rawStart + sampleIdx) -= satCharge * (*(pmtPulseStart + index));
                          */
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

    int m_tdc_temp = 0;
    for(sig_it = m_rawSignal.begin();
        sig_it != m_rawSignal.end();
        sig_it++){
        // if(*sig_it!=0)   // fangxiao change it so I save all point 

        if(m_enableFADC){
           
            if(m_enableElecNonL)
                channel.adc().push_back( FADCsample( (*sig_it)*elecRatio, gain_marker));
            //channel.adc().push_back( int(FADC_sample(m_FadcRange, *sig_it, m_FadcBit)*elecRatio  ));
            else 
                //channel.adc().push_back( FADC_sample(m_FadcRange, *sig_it, m_FadcBit) );
                channel.adc().push_back( FADCsample( *sig_it, gain_marker));

        }else{
            channel.adc().push_back(*sig_it);
        }
        m_tdc_temp++;
    }
}

double TriggerHandlerLpmt::noise(){
    // Simple noise model
    // Approximate Gaussian-distributed noise
    return gRandom->Gaus(0,1) * m_noiseAmp; // no offset

}

double TriggerHandlerLpmt::ampSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 533.98;
    double a = 2.884;
    return saturationModel(q, qSat, a);

}

double TriggerHandlerLpmt::chargeSatFactor(int nPulse){
    double q = double(nPulse);
    double qSat = 939.77;
    double a = 2.113;
    return saturationModel(q, qSat, a);
}

double TriggerHandlerLpmt::pmtSatFactor(int nPulse){
    // by miaoyu@ihep.ac.cn
    double q = double(nPulse);
    double qSat = 8858.09;
    double a =1.30865;
    return saturationModel(q, qSat, a);
}

double TriggerHandlerLpmt::saturationModel(double q, double qSat, double a){
    if(q<1) return 1;
    double num = pow(1 + 8*pow(q/qSat,a),0.5) - 1;
    double denom = 4* pow(q/qSat,a);
    return pow(num/denom,0.5);
}

double TriggerHandlerLpmt::electronicsNonl( double amp ){
    // miaoyu@ihep.ac.cn
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

int TriggerHandlerLpmt::FADC_sample(double adc_range, double amp_val, double FadcBit){
    //double adc_range = 1; //unit v
    double LSB = 0.075e-3;
    //double LSB = adc_range/TMath::Power(2, FadcBit);
    if((amp_val+m_FadcOffset)<0) return 0;
    unsigned int amp_adc =  int( (amp_val+m_FadcOffset)/LSB );    //with noise some point<0

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
    */
    //amp_adc = int( (amp_val+m_FadcOffset)/LSB );    //with noise some point<0
    return amp_adc;

}


uint16_t TriggerHandlerLpmt::FADCsample(double amp_val, int gain_marker)
{
    double LSB = m_LSB; //V
    int amp_adc = 0;
    unsigned int amp_Fadc = 0;
    //double LFadcOffsetValue = LSB/m_lowGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*m_FadcOffset;//offset for low gain
    //double HFadcOffsetValue = LSB/m_highGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*m_FadcOffset;//offset for high gain
    //double LFadcOffsetValue = m_LFadcOffsetValue;//offset for low gain
    //double HFadcOffsetValue = m_HFadcOffsetValue;//offset for high gain

    //double low_gain_range = LSB/m_lowGainLsbFactor*(TMath::Power(2,int(m_FadcBit))-1)*(1-m_FadcOffset); // currently set 0.75 as full range
    //double low_gain_range = m_low_gain_range; // currently set 0.75 as full range
    // dynamic range : 1000 p.e.
    if(gain_marker ==1 and amp_val>m_low_gain_range) {
        LSB /= m_lowGainLsbFactor;
        amp_adc = int( (m_low_gain_range+m_LFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
        amp_Fadc = amp_adc+(low_gain_marker<<m_FadcBit);
        return (amp_Fadc & 0X0000FFFF);
    }

    if(gain_marker == 0) {   // high gain
        LSB /= m_highGainLsbFactor;
        amp_adc = int( (amp_val+m_HFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
        amp_Fadc = amp_adc+(high_gain_marker<<m_FadcBit);
    } else if (gain_marker == 1) {   // low gain
        LSB /= m_lowGainLsbFactor;
        amp_adc = int( (amp_val+m_LFadcOffsetValue)/LSB );
	if(amp_adc < 0) amp_adc = 0;
        amp_Fadc = amp_adc+(low_gain_marker<<m_FadcBit);
    } else {
        LogError << " Unknown Gain Type " << std::endl;
    }

    return (amp_Fadc & 0X0000FFFF);
}

JM::ElecChannel* TriggerHandlerLpmt::calculate_tq(int pmtID, const JM::ElecWaveform& channel)
{
    //Ref Doc-6025: COTI waveform reconstruction
    auto ltq_channel = new JM::ElecChannel();
    auto& raw_adc = channel.adc();
    auto& a_tdc = channel.tdc();
    std::vector<uint16_t> a_adc;

    std::deque<unsigned int> baseline_buffer;
    int baseline_buffer_length = 32;//Unit: nanosecond
    double threshold;
    double baseline;
   
    //int adc_offset = int((TMath::Power(2,int(m_FadcBit))-1)*m_FadcOffset); 
    //int adc_offset = m_adc_offset;
    for(auto& ival : raw_adc)
    {
    	a_adc.push_back((ival & 0X3FFF));
    }
    

    //Initialize the baseline buffer
    for(int i = 0;i < baseline_buffer_length;i++)
    {
	baseline_buffer.push_back(a_adc[i]);
    }
   
    int length = a_adc.size();
    for(int index = baseline_buffer_length; index < length - 4; index++)
    {
       threshold = calculate_threshold(baseline_buffer,baseline) + m_adc_offset;

       bool over_threshold = 
            (a_adc[index] > threshold) && 
            (a_adc[index + 1] > threshold) &&
            (a_adc[index + 2] > threshold) &&
            (a_adc[index + 3] > threshold) &&
            (a_adc[index + 4] > threshold) ;

       if(over_threshold) index = integral(index, threshold, baseline, a_adc, a_tdc, ltq_channel);
       
       slide_baseline_buffer(baseline_buffer, index, a_adc);
    }

    if(ltq_channel->charge().size()==0 && m_TagNotOverThresholdTQ)
    {
        ltq_channel->charge().push_back(0);
        ltq_channel->time().push_back(0);
    }
    else if(ltq_channel->charge().size()==0 && (!m_TagNotOverThresholdTQ))
    {
        delete ltq_channel;
	ltq_channel = nullptr;
    }

    return ltq_channel;

}

int TriggerHandlerLpmt::integral(int index, double threshold, double baseline, std::vector<uint16_t>& a_adc, const std::vector<uint16_t>& a_tdc, JM::ElecChannel* ltq_channel)
{

    std::vector<unsigned int>& thecharge = ltq_channel->charge();
    std::vector<unsigned int>& thetime = ltq_channel->time();
    int length = a_adc.size();

    thetime.push_back(a_tdc[index]);

    double q = 0;
    for(int i = index; i  < length - 2; i++)
    {
	index = i;
        bool under_threshold = (a_adc[i] < threshold) && (a_adc[i + 1] < threshold) && (a_adc[i + 2] < threshold);
	if(under_threshold) break;
        q = q + double(a_adc[i]*(a_tdc[i+1] - a_tdc[i])) - double(baseline*(a_tdc[i+1] - a_tdc[i]));
    }
    thecharge.push_back(q);

    return index;
}

double TriggerHandlerLpmt::calculate_threshold(std::deque<unsigned int>& baseline_buffer, double& baseline)
{
    int baseline_buffer_length = baseline_buffer.size();
    double buffer_average = 0;
    double buffer_rms = 0;
    double threshold_factor = 1.5;
    double threshold;
    baseline = 0;

    for(int i = 0; i < baseline_buffer_length; i++)
    {
	buffer_average = buffer_average + baseline_buffer[i];
    }
    buffer_average = buffer_average/baseline_buffer_length;
    baseline = buffer_average;

    for(int i = 0; i < baseline_buffer_length; i++)
    {
	buffer_rms = buffer_rms + (baseline_buffer[i] - buffer_average)*(baseline_buffer[i] - buffer_average);
    }
    buffer_rms = buffer_rms/baseline_buffer_length;
    buffer_rms = sqrt(buffer_rms);


    threshold = buffer_rms*threshold_factor;
    return threshold;
}

void TriggerHandlerLpmt::slide_baseline_buffer(std::deque<unsigned int>& baseline_buffer,int index, std::vector<uint16_t>& a_adc)
{
    baseline_buffer.pop_front();
    baseline_buffer.push_back(a_adc[index]);
}

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerHandlerLpmt);
