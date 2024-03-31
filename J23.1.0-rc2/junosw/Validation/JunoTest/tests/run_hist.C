run_hist() {
  TRandom r;
  TFile f("histos.root", "recreate");
  TH1F h1("hgaus", "histo from a gaussian", 100, -3, 3);
  for (int i = 0; i<50000; ++i) {
    h1.Fill(r.Gaus(0,0.507));
  }
  h1.Write();
}
