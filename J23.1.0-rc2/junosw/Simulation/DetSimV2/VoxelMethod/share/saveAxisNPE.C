/*
 * Merge the root file with histograms with the TAxis objects.
 */
void saveAxisNPE(TString fn_input="hist-npe.r3100t180.20210114r2.root") {
    TFile* inputf = TFile::Open(fn_input);
    if (!inputf) {
        std::cerr << "Failed to open " << fn_input << std::endl;
        return;
    }

    TIter next(inputf->GetListOfKeys());
    TKey *key;

    TFile* f = TFile::Open("npehist3d_single.root", "RECREATE");
    Int_t xbinnum = 100;
    Int_t ybinnum = 180;
    // theta
    TAxis* bins_r3 = new TAxis(xbinnum, 0, 5600);
    bins_r3->SetName("bins_r3");
    bins_r3->Write();
    TAxis* bins_theta = new TAxis(ybinnum, 0, 180.01*TMath::Pi()/180.);
    bins_theta->SetName("bins_theta");
    bins_theta->Write();

    while ((key=(TKey*)next())) {
        TH1F* h = (TH1F*)inputf->Get( key->GetName() );
        h->Write();
    }

    f->Write();
    f->Close();
}
