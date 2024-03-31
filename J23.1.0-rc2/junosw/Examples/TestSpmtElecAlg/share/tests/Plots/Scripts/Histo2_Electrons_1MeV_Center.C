{
  TFile f("RootFiles/Plots_ElecSim_Electrons_1MeV_Center.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("Electrons 1MeV Center");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-Electrons_1MeV_Center.pdf");
  c1->SaveAs("Histo2-Electrons_1MeV_Center.svg");
  c1->SaveAs("Histo2-Electrons_1MeV_Center.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-Electrons_1MeV_Center-Log.pdf");
  c1->SaveAs("Histo2-Electrons_1MeV_Center-Log.svg");
  c1->SaveAs("Histo2-Electrons_1MeV_Center-Log.jpg");
}
