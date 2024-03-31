{
  TFile f("RootFiles/Plots_ElecSim_Positrons_1MeV_Center.root");
  allPMTs->Rebin(40);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("Positrons 1MeV Center");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-Positrons_1MeV_Center.pdf");
  c1->SaveAs("Histo3-Positrons_1MeV_Center.svg");
  c1->SaveAs("Histo3-Positrons_1MeV_Center.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-Positrons_1MeV_Center-Log.pdf");
  c1->SaveAs("Histo3-Positrons_1MeV_Center-Log.svg");
  c1->SaveAs("Histo3-Positrons_1MeV_Center-Log.jpg");
}
