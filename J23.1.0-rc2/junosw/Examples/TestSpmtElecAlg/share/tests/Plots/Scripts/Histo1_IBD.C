{
  TFile f("RootFiles/Plots_DetSim_IBD.root");
  SpmtTruthHits->Rebin(10);
  SpmtTruthHits->GetXaxis()->SetRange(0,100);
  SpmtTruthHits->SetTitle("Spmt DetSim Hits");
  SpmtTruthHits->SetName("IBD");
  SpmtTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtTruthHits->Draw();
  c1->SaveAs("Histo1-IBD.pdf");
  c1->SaveAs("Histo1-IBD.svg");
  c1->SaveAs("Histo1-IBD.jpg");
}
