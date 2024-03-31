{
  TFile f("RootFiles/Plots_ElecSim_Positrons_To20MeV_CD.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("Positrons 0-20MeV CD");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-Positrons_To20MeV_CD.pdf");
  c1->SaveAs("Histo2-Positrons_To20MeV_CD.svg");
  c1->SaveAs("Histo2-Positrons_To20MeV_CD.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-Positrons_To20MeV_CD-Log.pdf");
  c1->SaveAs("Histo2-Positrons_To20MeV_CD-Log.svg");
  c1->SaveAs("Histo2-Positrons_To20MeV_CD-Log.jpg");
}
