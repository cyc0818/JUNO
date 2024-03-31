{
  TFile f("RootFiles/Plots_DetSim_Electrons_1MeV_Center.root");
//  SpmtTruthHits->Rebin(10);
  SpmtTruthHits->GetXaxis()->SetRange(0,100);
  SpmtTruthHits->SetTitle("Spmt DetSim Hits");
  SpmtTruthHits->SetName("Electrons 1MeV Center");
  SpmtTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtTruthHits->Draw();
  c1->SaveAs("Histo1-Electrons_1MeV_Center.pdf");
  c1->SaveAs("Histo1-Electrons_1MeV_Center.svg");
  c1->SaveAs("Histo1-Electrons_1MeV_Center.jpg");
}
