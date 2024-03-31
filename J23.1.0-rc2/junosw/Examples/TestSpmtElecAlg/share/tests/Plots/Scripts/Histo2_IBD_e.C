{
  TFile f("RootFiles/Plots_ElecSim_IBD_e.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("IBD - Positrons");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-IBD_e.pdf");
  c1->SaveAs("Histo2-IBD_e.svg");
  c1->SaveAs("Histo2-IBD_e.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-IBD_e-Log.pdf");
  c1->SaveAs("Histo2-IBD_e-Log.svg");
  c1->SaveAs("Histo2-IBD_e-Log.jpg");
}
