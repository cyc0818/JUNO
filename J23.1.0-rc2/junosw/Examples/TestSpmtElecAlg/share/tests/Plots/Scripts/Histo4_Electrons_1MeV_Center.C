{
  TFile f("RootFiles/Plots_ElecSim_Electrons_1MeV_Center.root");
//  SpmtElecTruthHits->Rebin(10);
  SpmtElecTruthHits->GetXaxis()->SetRange(0,100);
  SpmtElecTruthHits->SetTitle("Spmt ElecSim Truth Hits");
  SpmtElecTruthHits->SetName("Electrons 1MeV Center");
  SpmtElecTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtElecTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtElecTruthHits->Draw();
  c1->SaveAs("Histo4-Electrons_1MeV_Center.pdf");
  c1->SaveAs("Histo4-Electrons_1MeV_Center.svg");
  c1->SaveAs("Histo4-Electrons_1MeV_Center.jpg");
}
