void script1()
{
  TFile* f = new TFile("SPE_v19_step1.root", "read");
  const int m_totalPMT = 17613;
  TH1D* m_Integral[m_totalPMT];
  for (int i = 0; i < m_totalPMT; i++) {
    ostringstream out1;
    out1 << "PMTID_" << i << "_Inte";
    m_Integral[i] = (TH1D*)f->Get(out1.str().c_str());
  }

  ofstream output1;
  output1.open("Integral.bck.txt");
  for (int i = 0; i < m_totalPMT; i++) {
    if (m_Integral[i]->GetEntries() > 0) {
      m_Integral[i]->Fit("gaus", "Q");  // specified for J16v2
      TF1* fun1 = m_Integral[i]->GetFunction("gaus");
      output1 << i << ' ' << fun1->GetParameter(1) << ' ' << fun1->GetParameter(2) << endl;
      delete fun1;
    } else {
      output1 << i << ' ' << 0 << ' ' << 0 << endl;
    }
  }
  output1.close();
}
