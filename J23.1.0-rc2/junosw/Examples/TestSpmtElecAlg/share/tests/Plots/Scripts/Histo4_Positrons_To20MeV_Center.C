{
  TFile f("RootFiles/Plots_ElecSim_Positrons_To20MeV_Center.root");
  SpmtElecTruthHits->Rebin(15);
  SpmtElecTruthHits->GetXaxis()->SetRange(0,100);
  SpmtElecTruthHits->SetTitle("Spmt ElecSim Truth Hits");
  SpmtElecTruthHits->SetName("Positrons 0-20MeV Center");
  SpmtElecTruthHits->GetXaxis()->SetTitle("#p.e.");
  SpmtElecTruthHits->GetYaxis()->SetTitle("a.u.");
  SpmtElecTruthHits->Draw();
  c1->SaveAs("Histo4-Positrons_To20MeV_Center.pdf");
  c1->SaveAs("Histo4-Positrons_To20MeV_Center.svg");
  c1->SaveAs("Histo4-Positrons_To20MeV_Center.jpg");
}
