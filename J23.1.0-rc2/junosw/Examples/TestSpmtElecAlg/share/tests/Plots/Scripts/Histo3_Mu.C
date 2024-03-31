{
  TH1F * totalMu;
  TH1F * addMu;
  TFile * f = new TFile("RootFiles/Plots_ElecSim_Mu_0.root");
  f->GetObject("allPMTs",totalMu);
  cout << totalMu->GetEntries() << endl;

  TFile* g = new TFile("RootFiles/Plots_ElecSim_Mu_1.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_2.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_3.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();
 
  g = new TFile("RootFiles/Plots_ElecSim_Mu_4.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();
  
  g = new TFile("RootFiles/Plots_ElecSim_Mu_5.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_6.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_7.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_8.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  g = new TFile("RootFiles/Plots_ElecSim_Mu_9.root");
  g->GetObject("allPMTs",addMu);
  totalMu->Add(addMu);
  g->Close();

  cout << totalMu->GetEntries() << endl;

  totalMu->Rebin(30000);
//  totalMu->GetXaxis()->SetRange(0,1000);
  totalMu->SetTitle("Spmt ElecSim All PMTs");
  totalMu->SetName("Mu");
  totalMu->GetXaxis()->SetTitle("d.u.c.");
  totalMu->GetYaxis()->SetTitle("a.u.");
  totalMu->Draw();
  c1->SaveAs("Histo3-Mu.pdf");
  c1->SaveAs("Histo3-Mu.svg");
  c1->SaveAs("Histo3-Mu.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-Mu-Log.pdf");
  c1->SaveAs("Histo3-Mu-Log.svg");
  c1->SaveAs("Histo3-Mu-Log.jpg");
}
