{
  TH1F * totalMu;
  TH1F * addMu;
  TFile * f = new TFile("RootFiles/Plots_DetSim_Mu_0.root");
  f->GetObject("SpmtTruthHits",totalMu);
  cout << totalMu->GetEntries() << endl;
  cout << totalMu->GetNbinsX() << endl;
  cout << totalMu->GetBinContent(100001) << endl;

  TFile* g = new TFile("RootFiles/Plots_DetSim_Mu_1.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_DetSim_Mu_2.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_DetSim_Mu_3.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_DetSim_Mu_4.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();
  
  g = new TFile("RootFiles/Plots_DetSim_Mu_5.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_DetSim_Mu_6.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_DetSim_Mu_7.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_DetSim_Mu_8.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_DetSim_Mu_9.root");
  g->GetObject("SpmtTruthHits",addMu);
  totalMu->Add(addMu);
  g->Close();

  cout << totalMu->GetEntries() << endl;

  totalMu->SetTitle("Spmt DetSim Hits");
  totalMu->SetName("Mu");
  totalMu->GetXaxis()->SetTitle("#p.e.");
  totalMu->GetYaxis()->SetTitle("a.u.");
  cout << totalMu->GetBinContent(100001) << endl;
  totalMu->Rebin(1000);
//  totalMu->GetXaxis()->SetRange(0,100);
  totalMu->Draw();
  c1->SaveAs("Histo1-Mu.pdf");
  c1->SaveAs("Histo1-Mu.svg");
  c1->SaveAs("Histo1-Mu.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo1-Mu-Log.pdf");
  c1->SaveAs("Histo1-Mu-Log.svg");
  c1->SaveAs("Histo1-Mu-Log.jpg");



}
