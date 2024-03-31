// create histograms
TH1F* h_hitTime_r[18];
Int_t nbin;
Double_t xmin;
Double_t xmax;

void hitTimeUser_Begin(TTree* t) {

    // initialize the histograms
    for (int i = 0; i < 18; ++i) {
        h_hitTime_r[i] = 0;
    }

    nbin = 3000;
    xmin = 0;
    xmax = 3000;
}

double hitTimeUser() {

    double edepR  = sqrt( (edepX*edepX) + (edepY*edepY) + (edepZ*edepZ) ) / 1e3; // unit: meter

    // get the corresponding histogram
    Int_t hist_idx = (int)floor(edepR);
    if (h_hitTime_r[hist_idx] == 0) {
        TString name = Form("h_hitTimePEUser_%d", hist_idx);
        h_hitTime_r[hist_idx] = new TH1F(name, name, nbin, xmin, xmax);
        h_hitTime_r[hist_idx]->SetDirectory(0);
    }

    for(int i_phot=0; i_phot<nPhotons; ++i_phot)
    {
        h_hitTime_r[hist_idx]->Fill(hitTime[i_phot]);
    }


    return 0.;
}

void hitTimeUser_Terminate() {
    // save the histogram, maybe not show them.
    for (int i = 0; i < 18; ++i) {
        if (h_hitTime_r[i]) {
            h_hitTime_r[i]->Write();
        }
    }
}
