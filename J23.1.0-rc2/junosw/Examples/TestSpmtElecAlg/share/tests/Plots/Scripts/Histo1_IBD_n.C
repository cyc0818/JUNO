{
  TFile f("RootFiles/Plots_DetSim_IBD_n.root");
  SpmtTruthHits->Rebin(5);
  SpmtTruthHits->GetXaxis()->SetRange(0,100);
  SpmtTruthHits->SetTitle("Spmt DetSim Hits");
  SpmtTruthHits->SetName("IBD - Neutron");
  SpmtTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtTruthHits->Draw();
  c1->SaveAs("Histo1-IBD_n.pdf");
  c1->SaveAs("Histo1-IBD_n.svg");
  c1->SaveAs("Histo1-IBD_n.jpg");
}
