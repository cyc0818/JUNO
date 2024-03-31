{
  TH1F * totalMu;
  TH1F * addMu;
  TFile * f = new TFile("RootFiles/Plots_ElecSim_Mu_0.root");
  f->GetObject("SpmtElecTruthHits",totalMu);
  cout << totalMu->GetEntries() << endl;
  cout << totalMu->GetNbinsX() << endl;
  cout << totalMu->GetBinContent(100001) << endl;

  TFile* g = new TFile("RootFiles/Plots_ElecSim_Mu_1.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_2.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_3.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_4.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
  
  g = new TFile("RootFiles/Plots_ElecSim_Mu_5.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_6.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_7.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_8.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_9.root");
  g->GetObject("SpmtElecTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  cout << totalMu->GetEntries() << endl;

  totalMu->SetTitle("Spmt ElecSim Truth Hits");
  totalMu->SetName("Mu");
  totalMu->GetXaxis()->SetTitle("#p.e.");
  totalMu->GetYaxis()->SetTitle("a.u.");
  cout << totalMu->GetBinContent(100001) << endl;
  totalMu->Rebin(1000);
//  totalMu->GetXaxis()->SetRange(0,100);
  totalMu->Draw();
  c1->SaveAs("Histo4-Mu.pdf");
  c1->SaveAs("Histo4-Mu.svg");
  c1->SaveAs("Histo4-Mu.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo4-Mu-Log.pdf");
  c1->SaveAs("Histo4-Mu-Log.svg");
  c1->SaveAs("Histo4-Mu-Log.jpg");



}
