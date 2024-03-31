{
  TFile f("RootFiles/Plots_ElecSim_Positrons_To20MeV_CD.root");
  allPMTs->Rebin(400);
  allPMTs->GetXaxis()->SetRange(0,100);
  allPMTs->SetTitle("Spmt ElecSim All PMTs");
  allPMTs->SetName("Positrons 0-20MeV CD");
  allPMTs->GetXaxis()->SetTitle("d.u.c.");
  allPMTs->GetYaxis()->SetTitle("a.u.");
  allPMTs->Draw();
  c1->SaveAs("Histo3-Positrons_To20MeV_CD.pdf");
  c1->SaveAs("Histo3-Positrons_To20MeV_CD.svg");
  c1->SaveAs("Histo3-Positrons_To20MeV_CD.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo3-Positrons_To20MeV_CD-Log.pdf");
  c1->SaveAs("Histo3-Positrons_To20MeV_CD-Log.svg");
  c1->SaveAs("Histo3-Positrons_To20MeV_CD-Log.jpg");
}
