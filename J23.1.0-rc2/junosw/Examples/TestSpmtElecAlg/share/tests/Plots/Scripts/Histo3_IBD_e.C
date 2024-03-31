{
  TFile f("RootFiles/Plots_ElecSim_IBD_e.root");
  allPMTs->Rebin(200);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("IBD - Positrons");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-IBD_e.pdf");
  c1->SaveAs("Histo3-IBD_e.svg");
  c1->SaveAs("Histo3-IBD_e.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-IBD_e-Log.pdf");
  c1->SaveAs("Histo3-IBD_e-Log.svg");
  c1->SaveAs("Histo3-IBD_e-Log.jpg");
}
