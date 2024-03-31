{
  TFile f("RootFiles/Plots_DetSim_IBD_e.root");
  SpmtTruthHits->Rebin(10);
  SpmtTruthHits->GetXaxis()->SetRange(0,100);
  SpmtTruthHits->SetTitle("Spmt DetSim Hits");
  SpmtTruthHits->SetName("IBD - Positrons");
  SpmtTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtTruthHits->Draw();
  c1->SaveAs("Histo1-IBD_e.pdf");
  c1->SaveAs("Histo1-IBD_e.svg");
  c1->SaveAs("Histo1-IBD_e.jpg");
}
