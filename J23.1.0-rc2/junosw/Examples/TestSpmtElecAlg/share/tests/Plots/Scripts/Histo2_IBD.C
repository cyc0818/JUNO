{
  TFile f("RootFiles/Plots_ElecSim_IBD.root");
  singlePMT->Rebin(2);
  singlePMT->GetXaxis()->SetRange(0,100);
  singlePMT->SetTitle("Spmt ElecSim Single PMT");
  singlePMT->SetName("IBD");
  singlePMT->GetXaxis()->SetTitle("d.u.c.");
  singlePMT->GetYaxis()->SetTitle("a.u.");
  singlePMT->Draw();
  c1->SaveAs("Histo2-IBD.pdf");
  c1->SaveAs("Histo2-IBD.svg");
  c1->SaveAs("Histo2-IBD.jpg");
  c1->SetLogy();
  c1->SaveAs("Histo2-IBD-Log.pdf");
  c1->SaveAs("Histo2-IBD-Log.svg");
  c1->SaveAs("Histo2-IBD-Log.jpg");
}
