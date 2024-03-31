#include "AnalysisTool.h"


void PlotNewDataWave(FadcWaveform fadcWaveform,int evtIdx,int chnIdx){
  TGraphErrors waveG  = fadcWaveform.DrawWaveform();
  TGraph       roiG   = fadcWaveform.DrawRois();
  TGraph       peakG  = fadcWaveform.DrawPeaks();
  TGraph       pulseG = fadcWaveform.DrawPulses(1.);
  //TGraph       pulseG = fadcWaveform.DrawPulses(20./112.85);
  TGraph       fitG   = fadcWaveform.DrawFit();
  //waveG.GetXaxis()->SetLimits(0,400);
  //waveG.GetXaxis()->SetLimits(300,700);
  //waveG.GetXaxis()->SetLimits(600,900);
  //waveG.GetYaxis()->SetRangeUser(-100,100);
  stringstream ssChi2; ssChi2  << fadcWaveform.GetChi2();
  stringstream ssRatio;ssRatio << 1;
  stringstream ssFadc; ssFadc  << fadcWaveform.GetCharge();
  TString title = "chi2 = " + ssChi2.str() + ", fadc = " + ssFadc.str() + ", r = " + ssRatio.str();
  //waveG.SetTitle(title);
  stringstream ssEvt;ssEvt<<evtIdx;
  stringstream ssChn;ssChn<<chnIdx;
  title = "Event #"+ssEvt.str()+" Chn #"+ssChn.str();
  //title = "CAEN NIM N6742 uncalibrated, 1 GHz, 12 bit, Event #"+ssEvt.str()+";Time [ns];ADC";
  //title = "CAEN Desktop DT5751, 1 GHz, 10 bit, Event #"+ssEvt.str()+", FEE Output;Time [ns];ADC";
  waveG.SetTitle(title);
  //TF1* f = new TF1("f","[0]*sin(2*3.14*x/[1]+[2])",0,1000);
  //f->SetNpx(1000);
  //f->SetParameter(0,7);
  //f->SetParameter(1,11);
  //f->SetParameter(2,0);
  //waveG.Fit(f,"","",600,800);
  TCanvas* testC1 = new TCanvas("testC1","",800,520);
  waveG. Draw("PEAZX"); 
  //roiG . Draw("F");
  waveG. Draw("PEZ");
  fitG.  Draw("C");
  //peakG. Draw("PL"); 
 // pulseG.Draw("P");
  testC1->SetGridx();
  testC1->SetGridy();
  stringstream sschn;sschn<<chnIdx;
  stringstream ssevt;ssevt<<evtIdx;
  TString saveName;
  //TString saveName = "./evt"+ssevt.str()+".png";
  //testC1->SaveAs(saveName);
  //saveName = "./evt"+ssevt.str()+".pdf";
  //testC1->SaveAs(saveName);
  // saveName = title+".C";
  saveName ="./evt"+ssevt.str()+"_chn_"+sschn.str()+".C";
  testC1->SaveAs(saveName);
}
