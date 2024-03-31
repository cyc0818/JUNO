{
  TFile f("RootFiles/Plots_ElecSim_IBD_n.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("IBD - Neutrons");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-IBD_n.pdf");
  c1->SaveAs("Histo2-IBD_n.svg");
  c1->SaveAs("Histo2-IBD_n.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-IBD_n-Log.pdf");
  c1->SaveAs("Histo2-IBD_n-Log.svg");
  c1->SaveAs("Histo2-IBD_n-Log.jpg");
}
