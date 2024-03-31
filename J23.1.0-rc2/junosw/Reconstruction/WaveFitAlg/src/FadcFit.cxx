#include "FadcFit.h"

unsigned int FadcFit::s_count = 0;

FadcFit::FadcFit(int nPulseMax){
  s_count++;
  m_nPulseMax = nPulseMax;
  Reset(m_nPulseMax);
}
FadcFit::FadcFit(std::map<float,float> peaks,int nFix){
  s_count++;
  unsigned int nFixPulse = 0;
  if(nFix>0) nFixPulse = (nFix-s_nParOvershoot)/s_nParPeak;
  m_nPulseMax = peaks.size()+nFixPulse;
  Reset(m_nPulseMax);
  SetParameters(peaks,nFix); 
}
void FadcFit::Reset(int nPulseMax){
  /// setup fit function 
  m_nPulseMax = 10;
  if(nPulseMax>0) m_nPulseMax = nPulseMax;
  int   m_nPar = m_nPulseMax * s_nParPeak + s_nParOvershoot;
  stringstream ssCount;ssCount<<s_count;
  TString fitName = "multiPulse" + ssCount.str();
  m_fit = new TF1(fitName, this, &FadcFit::MultiPulse,0,m_nSamples,m_nPar);
  m_fit->SetParameter(0,0.0);
  //m_fit->FixParameter(0,0);
  for(unsigned int pulseIdx=0;pulseIdx<m_nPulseMax;pulseIdx++) {
    for(unsigned int parIdx=0;parIdx<s_nParPeak;parIdx++) {
      int idx = s_nParPeak*pulseIdx + parIdx + s_nParOvershoot;
      if(parIdx==0) m_fit->SetParameter(idx,10);
      if(parIdx==1) m_fit->SetParameter(idx,50);
      if(parIdx==2) m_fit->SetParameter(idx,7.5 );
    }
  }
 // if(nPulseMax>0) m_fit->SetParameter(1,20.);
  /// generate peak template
  for(unsigned int i=0; i<m_nSamples; i++)  {
    double x = double(i);
    m_peakTemplate[i] = m_fit->Eval(x);
  }
}   
float FadcFit::Fit(TGraphErrors gr){
  m_fit->SetNDF(10000);
  m_fit->SetLineColor(kRed); 
  gr.Fit(m_fit,"0",""); 
  m_parameters = m_fit->GetParameters();
  m_pulses.clear();
  cout << " ==== Fit results ===== " << endl;
  for(unsigned int pulseIdx=0; pulseIdx<m_nPulseMax; pulseIdx++) {
    float pulseTime   =  m_fit->GetParameter(3*pulseIdx+2);
    float pulseCharge =  m_fit->GetParameter(3*pulseIdx+1)
                        *m_fit->GetParameter(3*pulseIdx+3);
    m_pulses[pulseTime] = pulseCharge;
  }
 // TCanvas* c1 = new TCanvas("c1","",800,520);
  gr.Draw("AP");
 // stringstream ssCount;ssCount<<s_count;
 // TString fitName = "fit"+ssCount.str()+".png";
//  c1->SaveAs(fitName);
  float  chi2Red = m_fit->GetChisquare()/m_fit->GetNDF();
  return chi2Red; 
}
TGraph FadcFit::DrawFit(){
  TGraph gr;
  gr.SetMarkerStyle(3);
  gr.SetMarkerColor(kRed+1);
  gr.SetLineColor(kRed+1);
  for(unsigned int i=0; i<25000; i++)  {
    float time = float(i) / 5.;
    float amp  = m_fit->Eval(time);
    gr.SetPoint(i,time,amp);
  }
  return gr;
}
TGraph FadcFit::DrawPulses(){
  TGraph gr(0);
  gr.SetMarkerStyle(20);
  gr.SetMarkerColor(kGreen+2);
  unsigned int nPulse = m_pulses.size();
  std::map<float,float>::iterator pulseIt = m_pulses.begin();
  int pointIdx=0;
  for(;pulseIt != m_pulses.end();pulseIt++) {
    gr.SetPoint(pointIdx,pulseIt->first,pulseIt->second);
    pointIdx++;
  }
  if(nPulse==0) gr.SetPoint(0,0,0);
  return gr;
}
double FadcFit::MultiPulse  (double *x, double *par){
  double pulseF = 0; 
  for (unsigned int pulseIdx=0; pulseIdx<m_nPulseMax; pulseIdx++) {
    int startIdx = pulseIdx*s_nParPeak+s_nParOvershoot;
    if(par[startIdx]>0)
      pulseF +=  GetPulseExp (x,&par[startIdx]) 
                - par[0]*GetOvershoot(x,&par[startIdx]); 
  } 
  return pulseF;
}
void FadcFit::SetParameters(std::map<float,float> peaks,int nFix){
  unsigned int nPulseFix = 0;
  if(nFix>0)   nPulseFix = (nFix-s_nParOvershoot) / s_nParPeak;
  unsigned int nPulse    = peaks.size() + nPulseFix;
  std::map<float,float>::iterator peakIt = peaks.begin();
  m_fit->SetParameter(0,0.045);
  //m_fit->SetParLimits(0,0.008,0.01);
  for(unsigned int pulseIdx=nPulseFix;pulseIdx<m_nPulseMax;pulseIdx++){
    /// overshoot amplitude
    double time = peakIt->first - 6;
    double amp  = peakIt->second;
    if(pulseIdx<nPulse) peakIt++;
    /// peak par
    for(unsigned int parIdx=0;parIdx<s_nParPeak;parIdx++){
      int idx = s_nParPeak*pulseIdx + parIdx + s_nParOvershoot;
      if(pulseIdx<nPulse){
        /// ---- set relevant parameters ----
        /// pulse amplitude
        if(parIdx==0){
          m_fit->SetParameter(idx,amp);
          m_fit->SetParLimits(idx,0.05*amp,5*amp);
        }
        /// hit time  
        if(parIdx==1){
          m_fit->SetParameter(idx,time);
          m_fit->SetParLimits(idx,time-100,time+100);
        }
        /// pulse width
        if(parIdx==2){
          m_fit->SetParameter(idx,7.5);
          m_fit->SetParLimits(idx,0,100);
        }
      }
      else{
        /// ---- fix dummy parameters to zero ----
        m_fit->FixParameter(idx,0);
      }
    }
  }
}
float FadcFit::GetPulseExp (double* x, double* par) {
 double deltaT=x[0];
 double width=par[2]; // pulse width parameter
 double mu=0.45;
 double shift = par[1];
 if (deltaT-shift<0) return 0.;
                                                                            
 return par[0]*exp( -pow( log( (deltaT-shift)/width),2) 
         / ( 2*pow(mu,2) ) ) ;  // unit V

}
// *old pulsefloat FadcFit::GetPulseExp (double* x, double* par) {
// *old pulse  float amp   = par[0] * 4.2;
// *old pulse  float offset= par[1];
// *old pulse  float width = par[2];
// *old pulse  float tau = (x[0]- offset)/width;
// *old pulse  if (tau<0) return 0.;
// *old pulse  return amp * (1-exp(-pow(tau,2)))*exp(-tau); 
// *old pulse  //return 0;
// *old pulse}

float FadcFit::GetOvershoot(double* x, double* par) {
  float amp    = par[0]*par[2];
  float offset = par[1];
  float deltaT = x[0]-offset;
  //// Fermi onset
  float t0   = 50; 
  float t1   = 10;
  float fermi = 1. / (exp( (t0 - deltaT) / t1) + 1.);
  //// Exponential overshoot component
  float tau = 145; // Overshoot decay time in ns
  float expoOS = exp(-(deltaT-87)/tau);
  //// Slower overshoot component
  float mean = 400;
  float sigma = 80;
  float t = deltaT -mean;
  float gausOS = 0.12 * exp(pow(t,2)/(-2*pow(sigma,2)));
  //// Undershoot 
  mean = 650;
  sigma = 120;
  t = deltaT -mean;
  float undershoot = -0.03 * exp(pow(t,2)/(-2*pow(sigma,2)));
  return amp * fermi * (expoOS + gausOS + undershoot);
  ////return fermi;
}
