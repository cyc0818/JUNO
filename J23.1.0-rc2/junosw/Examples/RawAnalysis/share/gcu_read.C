{
    TH1S* hist = nullptr;
    ULong64_t time;

    TFile file("gcu_test.root");
    TTree* tree = (TTree*)file.Get("data");

    tree->SetBranchAddress("waveform", &hist);
    tree->SetBranchAddress("Time", &time);

    tree->GetEntry(0);
    hist->Draw();
    std::cout << time << std::endl;
}
