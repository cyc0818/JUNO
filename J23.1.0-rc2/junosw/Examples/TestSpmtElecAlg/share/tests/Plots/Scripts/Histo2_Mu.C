{
  TH1F * totalMu;
  TH1F * addMu;
  TFile * f = new TFile("RootFiles/Plots_ElecSim_Mu_0.root");
  f->GetObject("singlePMT",totalMu);
  cout << totalMu->GetEntries() << endl;

  TFile* g = new TFile("RootFiles/Plots_ElecSim_Mu_1.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_2.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_3.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_4.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();
  
  g = new TFile("RootFiles/Plots_ElecSim_Mu_5.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_6.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_7.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_8.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_9.root");
  g->GetObject("singlePMT",addMu);
  totalMu->Add(addMu);
  g->Close();

  cout << totalMu->GetEntries() << endl;

//  totalMu->Rebin(1000);
  totalMu->GetXaxis()->SetRange(0,1000);
  totalMu->SetTitle("Spmt ElecSim Single PMT");
  totalMu->SetName("Mu");
  totalMu->GetXaxis()->SetTitle("d.u.c.");
  totalMu->GetYaxis()->SetTitle("a.u.");
  totalMu->Draw();
  c1->SaveAs("Histo2-Mu.pdf");
  c1->SaveAs("Histo2-Mu.svg");
  c1->SaveAs("Histo2-Mu.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-Mu-Log.pdf");
  c1->SaveAs("Histo2-Mu-Log.svg");
  c1->SaveAs("Histo2-Mu-Log.jpg");



}
