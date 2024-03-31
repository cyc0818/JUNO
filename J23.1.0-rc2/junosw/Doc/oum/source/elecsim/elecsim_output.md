# The electronics simulation output
There are two output file for electronics simulation,  EDM file and user-root file

## Waveform
The PMT waveform is saved in the tree `Event/CdWaveform/CdWaveformEvt` of the EDM file.
In this tree, the object `JM::CdWaveformEvt` is saved, which contains PMTID-ElecWaveform pairs. And the ElecWaveform contains the adc and tdc value of each wavefrom. 
The following root script can be used to read the wavefrom information:


    {	
    	int Npmt = 17612;
    	TFile* f = new TFile("elecsim.root","read");
    	TTree* t = (TTree*)f->Get("Event/CdWaveform/CdWaveformEvt");
    	TFile* waveform = new TFile("waveform.root", "recreate");
    
    	JM::CdWaveformEvt* aEvent = NULL;
    	t->SetBranchAddress("CdWaveformEvt",&aEvent);
    	
    	int nEvent = t->GetEntries();
    	cout << "Total ElecEvents : " << nEvent << endl;
    	waveform->cd();
    	for(int i = 0; i < nEvent; i++)
    	{
		if(i > 1) continue; /*just read the waveform of first event*/
    		t->GetEntry(i);
    		for(int pmtid = 0; pmtid < Npmt; pmtid++)
    		{
    			if(pmtid > 100) continue; /*just read the waveform of 100 PMTs*/
    			string s = "Waveform: EventID = " + to_string(i) + ", PMTID = " + to_string(pmtid);
    			TH1F h(s.data(),s.data(),1300,0,1300);
    			h.SetStats(kFALSE);
    			JM::ElecWaveform* achannel = aEvent->getChannel(pmtid);
    			if(!achannel)
    			{
    				cout << "find a empty pointer" << endl;
    				continue;
    			}
    			/*const std::vector<unsigned int>& a_adc = achannel->adc();*/ /*for the version before J22.2.0-rc1*/
    			const std::vector<uint16_t>& a_adc = achannel->adc();/*use this from J22.2.0-rc1*/
    			if(a_adc.size() == 0) continue;
    
                            unsigned int gain_marker = 0;
                            gain_marker = a_adc[0]>>14;
    
                            if(gain_marker == 0)
                            {
                                    cout << "high gain!!" << endl; 
                                    int m_length = a_adc.size();
    
                                    for(int j=0; j<m_length; j++) {
                                            h.AddBinContent(j, a_adc.at(j));
                                    }
                            }
                            else if(gain_marker == 1)
                            {
    
                                    cout << "low gain!!" << endl;
                                    int m_length = a_adc.size();
    
                                    for(int j=0; j<m_length; j++)
                                    {
    					h.AddBinContent(j, (a_adc.at(j) & 0X3FFF));
                                    }
                            }
                            else
                            {
                                    cout << "Unknow gain, please check...." << endl;
                            }
                    	h.Write();
                    }
    	}
    	waveform->Close();
    }

## FPGA TQ
The CD LPMT FPGA TQ is saved in the `Event/CdLpmtElec/CdLpmtElecEvt` of the EDM file.
In this tree, the object `JM::CdLpmtElecEvt` is saved, which contains PMTID-ElecChannel pairs. And the ElecChannel contains the t/q pairs of each channel. 
The following root script can be used to read the FPGA TQ:

    {
    	TFile* f = new TFile("elecsim.root","read");
    	TString treename = "Event/CdLpmtElec/CdLpmtElecEvt";
    	TTree* t = (TTree*)f->Get(treename);
    	
    	JM::CdLpmtElecEvt* aEvent = NULL;
    	t->SetBranchAddress("CdLpmtElecEvt",&aEvent);
    	int nEvent = t->GetEntries();
    	
    	for(int i = 0; i < nEvent; i++)
    	{
    		t->GetEntry(i);
    		cout << "EventID: " << i << endl;
    		const std::map<int,JM::ElecChannel*>& data = aEvent->channelData();
    
    		std::map<int,JM::ElecChannel*>::const_iterator it = data.begin();
    		std::map<int,JM::ElecChannel*>::const_iterator it_end = data.end();
    		for(; it != it_end; it++)
    		{
    			cout << "pmtID = " << it->first << endl;
    			auto achannel = it->second;
    			std::vector<unsigned int>& charge = achannel->charge();
    			std::vector<unsigned int>& time = achannel->time();
    			if(charge.size() != time.size()) 
    			{ 
    				cout << "Error!!!, charge count is not equal to time count, please check..." << endl;
    			}
    			int length = charge.size();
    			for(int j = 0; j < length; j++)
    			{
    				cout << "t = " << time[j] << endl;
    				cout << "q = " << charge[j] << endl;
    			}
    
    		}	
    	}
    }

## MC Truth

### Pulse-level truth
The CD LPMT pulse-level truth is saved in `Event/CdLpmtTruth/CdLpmtElecTruthEvt` of the EDM file.
In this tree, the object `JM::CdLpmtElecTruthEvt` is saved, which contains the collection of LpmtElecTruth.
The following code can be used to read the pulse-level truth.
 
    {
    	TFile* f = new TFile("test/elecsim/elecsim.root","read");
    	TString treename = "Event/CdLpmtTruth/CdLpmtElecTruthEvt";
    	TTree* t = (TTree*)f->Get(treename);
    	
    	JM::CdLpmtElecTruthEvt* aEvent = NULL;
    	t->SetBranchAddress("CdLpmtElecTruthEvt",&aEvent);
    	int nEvent = t->GetEntries();
    	
    	for(int i = 0; i < nEvent; i++)
    	{
    		t->GetEntry(i);
    		cout << "EventID: " << i << endl;
    		const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();
    
    		int nPulse = truth.size();
    		for(int i = 0; i < nPulse; i++)
    		{
    			cout << "================================" << endl;
    			cout << "Pulse type = " << truth[i].pulsetype() << "\n"
    			     << "PMTID = " << truth[i].pmtId() << "\n"
    			     << "NPE = " << truth[i].npe() << "\n"
    			     << "hittime(in detsim) = " << truth[i].hitTime() << "\n"
    			     << "amplitude = " << truth[i].amplitude() << "\n"
    			     << "transient time spread (TTS) = " << truth[i].tts() << "ns" << "\n"
    			     << "timeoffset = " << truth[i].timeoffset() << "ns"<< "\n"
    			     << "The absolute time of this pulse = " << truth[i].pulseHitTime() << endl;
    			cout << "================================" << endl;
    
    		}	
    	}
    }

### Track-level truth

The CD LPMT track-level truth is saved in `Event/TrackTruth/TrackElecTruthEvt` of the EDM file.
In this tree, the object `JM::TrackElecTruthEvt` is saved, which contains the collection of TrackElecTruth.
The following code can be used to read the track-level truth.

    {
    	TFile* f = new TFile("test/elecsim/elecsim.root","read");
    	TString treename = "Event/TrackTruth/TrackElecTruthEvt";
    	TTree* t = (TTree*)f->Get(treename);
    	
    	JM::TrackElecTruthEvt* aEvent = NULL;
    	t->SetBranchAddress("TrackElecTruthEvt",&aEvent);
    	int nEvent = t->GetEntries();
    	
    	for(int i = 0; i < nEvent; i++)
    	{
    		t->GetEntry(i);
    		cout << "EventID: " << i << endl;
    		const std::vector<JM::TrackElecTruth>& truth = aEvent->truths();
    
    		int nTrack = truth.size();
    		for(int i = 0; i < nTrack; i++)
    		{
    			cout << "================================" << endl;
    			cout << "PDGID = " << truth[i].pdgid() << "\n"
    			     << "Edep = " << truth[i].edep() << "\n"
    			     << "QEdep = " << truth[i].Qedep() << "\n"
    			     << "Track Timestamp = " << truth[i].trackTime() << "\n"
    			     << "(Edep_X, Edep_Y, Edep_Z) = (" << truth[i].edepX() << ", " << truth[i].edepY() << ", " << truth[i].edepZ() << ")" << "\n"
    			     << "(initX,initY,initZ,initT) = (" << truth[i].initX() << ", " << truth[i].initY() << ", " << truth[i].initZ() << ", " << truth[i].initT() << ")" << "\n"
    			     << "(initPx,initPy,initPz,initMass) = (" << truth[i].initPx() << ", " << truth[i].initPy() << ", " << truth[i].initPz() << ", " << truth[i].initMass() << ")" << endl;
    			cout << "================================" << endl;
    
    		}	
    	}
    }

## Event correlation

The correlation between CD electronics simulation and detector simulation is recorded in the tree `eventindex` of user_root.
The following branch are included in this tree:

* `eventid`: EventID in electronics simulation(In other words: TriggerID).
* `nevents`: How many detsim events are mixed in this elecsim event.
* `tags`: Tags of the mixed detsim events.
* `filenames`: Files name of the mixed detsim events.
* `entries`: Entry number of the mixed detsim events in the corresponding input file.
* `nhits`: How many physical hits are read out in this elecsim event.

The following code can be used to get the CD simulation event correlation information.


    {
    	TFile* f = TFile::Open("user_elecsim.root","read");
    	TTree* t = (TTree*)f->Get("eventindex");
    
    	int evtid = 0;
    	int nevt = 0;
    	vector<string>* tag = NULL;
    	vector<string>* filename = NULL;
    	vector<int>* detentries = NULL;
    	vector<int>* nhit = NULL;
    
    	t->SetBranchAddress("eventid",&evtid);
    	t->SetBranchAddress("nevents",&nevt);
    	t->SetBranchAddress("tags",&tag);
    	t->SetBranchAddress("filenames",&filename);
    	t->SetBranchAddress("entries",&detentries);
    	t->SetBranchAddress("nhits",&nhit);
    
    	int nEvent = t->GetEntries();
    	cout << "ElecSim nEvent = " << nEvent << endl;
    	for(int i = 0; i < nEvent; i++)
    	{
    		cout << "=============================" << endl;
    		t->GetEntry(i);
    		cout << "EventID(TriggerID) = " << evtid << endl;
    		cout << nevt << " detsim events are mixed in this elecsim event"  << endl;
    		bool error = (nevt != tag->size()) || (nevt != filename->size()) || (nevt != detentries->size()) || (nevt != nhit->size());
    		if(error) cout << "Error! Please check....." << endl;
    		for(int j = 0; j < nevt; j++)
    		{
    			cout << "*****" << j+1 <<"******" << endl;
    			cout << "Tag: " << tag->at(j) << "\n"
    			     << "File name: " << filename->at(j) << "\n"
    			     << "DetSim File Entry ID: " << detentries->at(j) << "\n"
    			     << "nhits: " << nhit->at(j) << endl;
    			
    		}
    		cout << "=============================" << endl;
    	}
    }
