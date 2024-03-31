{
  TFile f("RootFiles/Plots_DetSim_Electrons_1MeV_Top.root");
//  SpmtTruthHits->Rebin(4);
  SpmtTruthHits->GetXaxis()->SetRange(0,100);
  SpmtTruthHits->SetTitle("Spmt DetSim Hits");
  SpmtTruthHits->SetName("Electrons 1MeV Top");
  SpmtTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtTruthHits->Draw();
  c1->SaveAs("Histo1-Electrons_1MeV_Top.pdf");
  c1->SaveAs("Histo1-Electrons_1MeV_Top.svg");
  c1->SaveAs("Histo1-Electrons_1MeV_Top.jpg");
}
