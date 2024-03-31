{
  TFile f("RootFiles/Plots_ElecSim_Electrons_1MeV_Top.root");
//  SpmtElecTruthHits->Rebin(4);
  SpmtElecTruthHits->GetXaxis()->SetRange(0,100);
  SpmtElecTruthHits->SetTitle("Spmt ElecSim Truth Hits");
  SpmtElecTruthHits->SetName("Electrons 1MeV Top");
  SpmtElecTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtElecTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtElecTruthHits->Draw();
  c1->SaveAs("Histo4-Electrons_1MeV_Top.pdf");
  c1->SaveAs("Histo4-Electrons_1MeV_Top.svg");
  c1->SaveAs("Histo4-Electrons_1MeV_Top.jpg");
}
