# Frequently asked questions
We list the questions which are asked frequently. Please check this section if you need 

## How to get trigger time? Why can not get trigger time from `Event/CdWaveform/CdWavefromEvt`?
First, you can definitely get trigger time from the tree `Meta/navigator` of elecsim EDM file. The trigger time in `Event/CdTrigger/CdTriggerEvt` is for the 179 sub volumes in vertex fitting algorithm, it's not the global trigger.
You can use the following code to get the trigger time:

    {
    	TFile* f = TFile::Open("/junofs/users/zhanghaosen/muon_data/check2/trigger_time/sample_classified.root","read");	
    	TTree* t = (TTree*)f->Get("Meta/navigator");
    
    	JM::EvtNavigator* Nav = NULL;
    	t->SetBranchAddress("EvtNavigator",&Nav);
    	
    	int nEvent = t->GetEntries();
    	cout << "Total ElecEvents : " << nEvent << endl;
    
    	for(int i = 0; i < nEvent; i++)
    	{
    		t->GetEntry(i);
    		TTimeStamp t1 = Nav->TimeStamp();
    		cout << "EventID = " << i << "  trigger_time: "  << t1 << endl;
    		
    		
    	}
    }

## Time issues in elecsim(for example: what's the relationship between hittime in detsim and pulseHittime in elecsim? )

Please check DocDB6864 
