{
  TFile f("RootFiles/Plots_ElecSim_IBD_n.root");
  SpmtElecTruthHits->Rebin(5);
  SpmtElecTruthHits->GetXaxis()->SetRange(0,100);
  SpmtElecTruthHits->SetTitle("Spmt ElecSim Truth Hits");
  SpmtElecTruthHits->SetName("IBD - Neutron");
  SpmtElecTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtElecTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtElecTruthHits->Draw();
  c1->SaveAs("Histo4-IBD_n.pdf");
  c1->SaveAs("Histo4-IBD_n.svg");
  c1->SaveAs("Histo4-IBD_n.jpg");
}
