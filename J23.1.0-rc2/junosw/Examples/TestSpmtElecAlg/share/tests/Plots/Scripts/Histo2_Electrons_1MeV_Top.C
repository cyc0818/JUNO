{
  TFile f("RootFiles/Plots_ElecSim_Electrons_1MeV_Top.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("Electrons 1MeV Top");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-Electrons_1MeV_Top.pdf");
  c1->SaveAs("Histo2-Electrons_1MeV_Top.svg");
  c1->SaveAs("Histo2-Electrons_1MeV_Top.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-Electrons_1MeV_Top-Log.pdf");
  c1->SaveAs("Histo2-Electrons_1MeV_Top-Log.svg");
  c1->SaveAs("Histo2-Electrons_1MeV_Top-Log.jpg");
}
