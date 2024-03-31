{
  TFile f("RootFiles/Plots_ElecSim_Electrons_1MeV_Top.root");
  allPMTs->Rebin(20);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("Electrons 1MeV Top");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-Electrons_1MeV_Top.pdf");
  c1->SaveAs("Histo3-Electrons_1MeV_Top.svg");
  c1->SaveAs("Histo3-Electrons_1MeV_Top.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-Electrons_1MeV_Top-Log.pdf");
  c1->SaveAs("Histo3-Electrons_1MeV_Top-Log.svg");
  c1->SaveAs("Histo3-Electrons_1MeV_Top-Log.jpg");
}
