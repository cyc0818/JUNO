{
  TFile f("RootFiles/Plots_ElecSim_IBD.root");
  allPMTs->Rebin(200);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("IBD");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-IBD.pdf");
  c1->SaveAs("Histo3-IBD.svg");
  c1->SaveAs("Histo3-IBD.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-IBD-Log.pdf");
  c1->SaveAs("Histo3-IBD-Log.svg");
  c1->SaveAs("Histo3-IBD-Log.jpg");
}
