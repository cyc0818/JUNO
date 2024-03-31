{
  TFile f("RootFiles/Plots_ElecSim_IBD.root");
  SpmtElecTruthHits->Rebin(10);
  SpmtElecTruthHits->GetXaxis()->SetRange(0,100);
  SpmtElecTruthHits->SetTitle("Spmt ElecSim Truth Hits");
  SpmtElecTruthHits->SetName("IBD");
  SpmtElecTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtElecTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtElecTruthHits->Draw();
  c1->SaveAs("Histo4-IBD.pdf");
  c1->SaveAs("Histo4-IBD.svg");
  c1->SaveAs("Histo4-IBD.jpg");
}
