{
    TH1S* hist = nullptr;
    ULong64_t time;
    UInt_t chNum;
    UShort_t PmtID[100];
    UShort_t GCUID[100];
    UShort_t chSize[100];

    //TFile file("events_tree.root");
    TFile file("tree_from_edm.root");
    TTree* tree = (TTree*)file.Get("data");

    tree->SetBranchAddress("waveform", &hist);
    tree->SetBranchAddress("Time", &time);
    tree->SetBranchAddress("ChNum", &chNum);
    tree->SetBranchAddress("PMTId", &PmtID);
    tree->SetBranchAddress("GCUId", &GCUID);
    tree->SetBranchAddress("ChSize", &chSize);

    //for ( int i = 0; i < tree->GetEntries(); ++i ) {
        int i = 1;
        tree->GetEntry(i);
        std::cout << "event " << i << "  time " << time << "  num_channels " << chNum << std::endl;
        for ( auto j = 0; j < chNum; ++j) {
            std::cout << "\tch_" << j << " \t" << GCUID[j] << "\t" << PmtID[j] << "\t" << chSize[j] << std::endl;
        }
        hist->Draw();
    //}
}
