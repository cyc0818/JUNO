{
  TFile f("RootFiles/Plots_ElecSim_Positrons_To20MeV_Center.root");
  allPMTs->Rebin(400);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("Positrons 0-20MeV Center");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-Positrons_To20MeV_Center.pdf");
  c1->SaveAs("Histo3-Positrons_To20MeV_Center.svg");
  c1->SaveAs("Histo3-Positrons_To20MeV_Center.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-Positrons_To20MeV_Center-Log.pdf");
  c1->SaveAs("Histo3-Positrons_To20MeV_Center-Log.svg");
  c1->SaveAs("Histo3-Positrons_To20MeV_Center-Log.jpg");
}
