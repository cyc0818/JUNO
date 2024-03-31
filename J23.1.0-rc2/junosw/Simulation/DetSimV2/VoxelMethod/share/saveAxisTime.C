/*
 * Merge the root file with histograms with the TAxis objects.
 */

void saveAxisTime(TString fn_input="hist-time.r200t180.20210114v2.root") {
    TFile* inputf = TFile::Open(fn_input);
    if (!inputf) {
        std::cerr << "Failed to open " << fn_input << std::endl;
        return;
    }

    TIter next(inputf->GetListOfKeys());
    TKey *key;

    TFile* f = TFile::Open("hist3d-with-att.root", "RECREATE");
    Int_t xbinnum = 200;
    Int_t ybinnum = 180;
    // theta
    TAxis* bins_r = new TAxis(xbinnum, 0, 17.7);
    bins_r->SetName("bins_r");
    bins_r->Write();
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
