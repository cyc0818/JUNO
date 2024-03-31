{
  TFile f("RootFiles/Plots_ElecSim_IBD_n.root");
  allPMTs->Rebin(200);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("IBD - Neutrons");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-IBD_n.pdf");
  c1->SaveAs("Histo3-IBD_n.svg");
  c1->SaveAs("Histo3-IBD_n.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-IBD_n-Log.pdf");
  c1->SaveAs("Histo3-IBD_n-Log.svg");
  c1->SaveAs("Histo3-IBD_n-Log.jpg");
}
