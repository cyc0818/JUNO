#include "FadcReadout.h"
#include "SniperKernel/ToolFactory.h"
DECLARE_TOOL(FadcWaveform);
 
FadcWaveform::FadcWaveform(const std::string& name)
: m_graph(0),
  m_fit(0),
  m_raw(0),
  m_dec(0),
  ToolBase(name) { 

  m_preWindow = 100;  
  m_fadcGain = 1;

  m_threshold   = 0.25*10;// 1/4 p.e.  *10^4
  m_minGap      = 20;
  m_adcMax      = 1000;
}

bool
FadcWaveform::fit(const float* inputArray, int nSamples)
{
  clear();
  //TSpectrum testS;
  //testS.SmoothMarkov (inputArray,1000,4);
  //m_fit =  new TGraph(); 
  /// Read in waveform  
  m_nSamples    = nSamples;
  m_isSaturated = 0;
  m_fixOs       = 0;
  m_bits        = 8; 
  float pedChi2Max  = 0.6;
  m_graph.GetXaxis()->SetTitle("Time [ns]");
  m_raw.GetXaxis()->SetTitle("Time [ns]");
  m_dec.GetXaxis()->SetTitle("Time [ns]");
  m_fit.GetXaxis()->SetTitle("Time [ns]");
  m_graph.SetLineColor(kBlue+1);
  m_graph.SetMarkerColor(kBlue+1);
  m_graph.SetMarkerStyle(7);
  m_graph.SetFillColor(kBlue+1);
  m_raw.SetLineColor(kBlue+1);
  m_raw.SetMarkerColor(kBlue+1);
  m_raw.SetMarkerStyle(7);
  m_raw.SetFillColor(kBlue+1);
  m_dec.SetLineColor(kGreen+2+1);
  m_dec.SetMarkerColor(kGreen+2);
  m_dec.SetMarkerStyle(7);
  m_dec.SetFillColor(kGreen+2);
  TH1F pedestalH("pedestalH","",500,-5,5);// pedestal vary from event to event 
  TGraphErrors pedestalG(0);
  int pointIdx=0;
  std::cout << "#########" << __LINE__ << std::endl;
  for(int sampleIdx=0; sampleIdx<m_nSamples; sampleIdx++) {
    //if(  inputArray[sampleIdx] > pow(2.0,m_bits)-2
    if(  inputArray[sampleIdx] > m_adcMax
      // ||inputArray[sampleIdx]==0
       ) {
        
      m_isSaturated = 1;
       m_rawWaveform.push_back(-1);
     } 
    else 
      m_rawWaveform.push_back(inputArray[sampleIdx]);
     
    if(sampleIdx<m_preWindow){
      pedestalG.SetPoint(pointIdx,pointIdx,m_rawWaveform[sampleIdx]);
      pedestalH.Fill(m_rawWaveform[sampleIdx]);
      pointIdx++;
    }
  }
  /// Calibrate pedestal
  TFitResultPtr fitPol0 = pedestalG.Fit("pol0","QS0");
  m_pedChi2  = fitPol0->Chi2() / fitPol0->Ndf();
  if(m_pedChi2>pedChi2Max) {
    fitPol0 = pedestalG.Fit("pol0","QS0","",0,m_preWindow/4);
    m_pedChi2  = fitPol0->Chi2() / fitPol0->Ndf();
    // m_preWindow = 0; 
  }
  std::cout << "#########" << __LINE__ << std::endl;
  pedestalH.Draw();
  std::cout << "#########" << __LINE__ << std::endl;
  TFitResultPtr fitGaus = pedestalH.Fit("gaus","QS0");
  m_pedestal       = 50.;
  m_pedError       = 0.04;
  // TODO
  m_pedestal = fitPol0->Parameters()[0];
  m_pedError = fitGaus->Parameters()[2];
  std::cout << "#########" << __LINE__ << std::endl;
  //cout << " m_pedChi2 = " << m_pedChi2 << endl;
  //fitPol0->Delete();
  //fitGaus->Delete();
  int satBins = 0;
  for(int sampleIdx=0; sampleIdx<m_nSamples; sampleIdx++) {
    m_raw.SetPoint(sampleIdx,sampleIdx,m_rawWaveform[sampleIdx]);
    m_dec.SetPoint(sampleIdx,sampleIdx,0);
    if(m_rawWaveform[sampleIdx]<-100){
    //if(m_rawWaveform[sampleIdx]<0){
      m_waveform.push_back(-1000);
      satBins++; 
      // }
    }
    else {
      m_waveform.push_back(m_rawWaveform[sampleIdx]-m_pedestal);
      m_graph.SetPoint     (sampleIdx-satBins,sampleIdx,m_waveform[sampleIdx]);
      m_graph.SetPointError(sampleIdx-satBins,0,m_pedError);
    }
  }

  // MAGIC HERE
  // TODO, set the gain for every channel
  Analyze();

  std::cout << "#########" << __LINE__ << std::endl;
  return true;
}
TGraph FadcWaveform::DrawRois(){
  TGraph roiG(0);
  roiG.SetMarkerStyle(20);
  roiG.SetMarkerColor(kRed);
  roiG.SetFillColor(kGray);
  if(m_rois.size()==0){
    roiG.SetPoint(0,0,0);
    return roiG;
  }
  int pointIdx=0;
  for(unsigned int roiIdx=0; roiIdx<m_rois.size(); roiIdx++)  {
    roiG.SetPoint(4*pointIdx  ,m_rois[roiIdx].start,0);
    roiG.SetPoint(4*pointIdx+1,m_rois[roiIdx].start,m_rois[roiIdx].maxAdc);
    roiG.SetPoint(4*pointIdx+2,m_rois[roiIdx].end,  m_rois[roiIdx].maxAdc);
    roiG.SetPoint(4*pointIdx+3,m_rois[roiIdx].end,0);
    pointIdx++;
  }
  return roiG;
}
TGraph FadcWaveform::DrawPeaks(){
  TGraph peakG(0);
  peakG.SetMarkerStyle(23);
  peakG.SetMarkerColor(kRed);
  peakG.SetLineColor(kRed);
  peakG.SetMarkerSize(1.5);
  if(m_peaks.size()==0){
    peakG.SetPoint(0,0,0);
    return peakG;
  }
  int pointIdx = 0;
  Peaks::iterator peakIt = m_peaks.begin();
  for(;peakIt != m_peaks.end();peakIt++) {
    peakG.SetPoint(pointIdx*3  ,peakIt->first,0);
    peakG.SetPoint(pointIdx*3+1,peakIt->first,peakIt->second);
    peakG.SetPoint(pointIdx*3+2,peakIt->first,0);
    pointIdx++;
  }
  return peakG;
}
TGraph FadcWaveform::DrawPulses(float scale){
  TGraph pulseG(0);
  pulseG.SetMarkerStyle(20);
  pulseG.SetMarkerColor(kGreen+2);
  pulseG.SetLineColor(kGreen+2);
  pulseG.SetMarkerSize(1.2);
  if(m_pulses.size()==0){
    pulseG.SetPoint(0,0,0);
    return pulseG;
  }
  int pointIdx = 0;
  Pulses::iterator pulseIt = m_pulses.begin();
  for(;pulseIt != m_pulses.end();pulseIt++) {
    pulseG.SetPoint(pointIdx,pulseIt->first+5.,scale*pulseIt->second);
    pointIdx++;
  }
  return pulseG;
}
void FadcWaveform::Analyze(){
  if(m_isSaturated) {cout<<" yp saturated ";return;}
 cout<<" !!! "<<" prescan "<<endl; 
  PreScan();
  Deconvolve();
}
void FadcWaveform::PreScan(){
  bool isBelowThr = true;
  float currAdc   = 0;
  int  lastCros   = -1000;
  int  lastDrop   = -1000;
  int  startCycle = -1000;
  float  peakFadc = 0;
  float chargeSum = 0;
  
  int   roiTdc   = 0;
  float roiAdc   = 0;
  float roiQdc   = 0;
  int   roiWidth = 0;
  int   roiEnd   = 0;
  int   roiGap   = 0;
  //float roiBaseline = 0;
  
  //bool roiBuffer = true;
  bool roiBuffer = false;
  float baseline = 0;
  for (int sampleIdx=m_preWindow; sampleIdx<m_nSamples; sampleIdx++)  {
    currAdc = m_waveform[sampleIdx];
    if(isBelowThr){
      baseline  = m_waveform[sampleIdx-2];
      baseline += m_waveform[sampleIdx-3];
      baseline += m_waveform[sampleIdx-4];
      baseline /= 3.;
    }
    float crossAdc = currAdc;
    if(baseline < -2*m_pedError)
    {    crossAdc  -= baseline+m_pedError*2;
    }
    //if(sampleIdx>350 && sampleIdx<400)
      //cout << sampleIdx << ": " << baseline << ", " << currAdc << endl;
    /// threshold crossing
    if(crossAdc>=m_threshold && isBelowThr) {
      //cout << " cross at " << sampleIdx << " with " << currAdc << endl;
      lastCros   = sampleIdx;
      isBelowThr = false;
      peakFadc   = crossAdc;
      startCycle = sampleIdx;
      chargeSum  = m_waveform[sampleIdx-1];
      if(lastDrop == sampleIdx-1)
          chargeSum = 0;
      if(roiBuffer){
        lastCros  = roiTdc;
        peakFadc  = roiAdc;
        chargeSum = roiQdc;
      }
      roiBuffer  = false;
    }
    /// compute simple charge sum
    if(!isBelowThr && currAdc>0) {
      chargeSum += currAdc;
      if(currAdc>peakFadc) peakFadc = currAdc;
    }
    /// drop below threshold for more that m_minGap
    if(currAdc<m_threshold && !isBelowThr) {
      //cout << " drop at " << sampleIdx << " with " << currAdc+baseline << endl;
      roiTdc   = lastCros;
      roiAdc   = peakFadc;
      roiWidth = sampleIdx - lastCros;
      roiQdc   = chargeSum;
      roiGap   = lastCros  - lastDrop;
      roiEnd   = sampleIdx;
      
      lastDrop   = sampleIdx;
      isBelowThr = true;
      peakFadc   = -1;
      roiBuffer  = true;
    }
    if(sampleIdx-lastDrop > m_minGap && roiBuffer){
      Roi roi;
      if(roiWidth<10) roiEnd = roiTdc + 15;
      roi.start   = roiTdc;
      roi.end     = roiEnd;
      //roi.preGap  = roiGap;
      roi.postGap = m_nSamples-roiEnd;
      roi.maxAdc  = roiAdc;
      roi.qdc     = roiQdc;
      roi.fitPar.clear(); 
      roi.lastFitPar.clear(); 
      
      int nRoi = m_rois.size();
      roi.preGap = roiTdc;
      if(nRoi>0) roi.preGap = roiTdc - m_rois[nRoi-1].end; 
      if(nRoi>0) m_rois[nRoi-1].postGap = roi.preGap;
      
      m_rois.push_back(roi); 

      roiBuffer = false;
    }
  }
  unsigned int nRoi = m_rois.size();
  for(unsigned int roiIdx=0; roiIdx<nRoi; roiIdx++)  {
    if(roiIdx<nRoi-1){
      if(m_rois[roiIdx+1].start - m_rois[roiIdx].end < 10){
        m_rois[roiIdx+1].start  = m_rois[roiIdx].start;
        m_rois[roiIdx+1].preGap = m_rois[roiIdx].preGap;
        m_rois[roiIdx+1].qdc   += m_rois[roiIdx].qdc;
        m_rois.erase(m_rois.begin()+roiIdx);
        cout<<" nRoi "<<nRoi<<" break at "<<roiIdx<<endl;break;
        //break;
      }
    }
  }
  for(unsigned int roiIdx=0; roiIdx<m_rois.size(); roiIdx++)  {
    cout << " ******************** " << endl;
    cout << " Start:    " << m_rois[roiIdx].start   << endl;
    cout << " End:      " << m_rois[roiIdx].end     << endl;
    cout << " ADC:      " << m_rois[roiIdx].maxAdc  << endl;
    cout << " QDC:      " << m_rois[roiIdx].qdc     << endl;
    cout << " PreGap:   " << m_rois[roiIdx].preGap  << endl;
    cout << " PostGap:  " << m_rois[roiIdx].postGap << endl;
    cout << " ******************** " << endl;
  }
}  

FadcWaveform::Pulses FadcWaveform::FitPeaks (Roi& roi,Peaks peaks){
  int nFix   = roi.lastFitPar.size();
  FadcFit fadcFit(peaks,nFix);
  fadcFit.FixParameters(roi.lastFitPar);
  int startIdx  = roi.start - m_minGap;
  int fitLength = roi.end   - roi.start + 2*m_minGap;
  m_fixOs = 1;
  if(roi.postGap>60) {
    fitLength += 50 ;
    fadcFit.UnfixOS();
    m_fixOs = 0;
  }
  
  TGraphErrors tmpG(0);
  for(int idx=0;idx<fitLength;idx++) {
    tmpG.SetPoint     (idx,startIdx+idx,m_waveform[startIdx+idx]);
    tmpG.SetPointError(idx,0           ,m_pedError);
  }
 
  float chi2Red = fadcFit.Fit(tmpG);
  roi.chi2 = chi2Red;
  //cout << " chi2 / Ndf = " << chi2Red << endl;
  //m_fit->Delete();
  m_fit         = fadcFit.DrawFit();
  
  Pulses pulses = fadcFit.GetPulses();
  double* par   = fadcFit.GetParameters();
  int nFitPar   = peaks.size()*3 + 1;
  if(nFix>0)
    nFitPar += nFix -1;
  std::vector<double> fitPar;
  for(int parIdx=0; parIdx<nFitPar; parIdx++)  {
    fitPar.push_back(par[parIdx]);
    //cout << par[parIdx] << endl;
  }
  roi.fitPar  = fitPar;
  Pulses roiPulses;
  int nPulseRoi = peaks.size();
  int nPulse    = pulses.size();
  float roiCharge = 0;
  Pulses::iterator pulseIt = pulses.begin();
  for(int i=0; i<nPulse-nPulseRoi; i++) pulseIt++;
  for(;pulseIt!=pulses.end();pulseIt++)  {
    float time   = pulseIt->first;
    float charge = pulseIt->second;

    roiPulses[time] = charge;
    roiCharge += charge/m_fadcGain;
  }
  float avgCharge = roiCharge/float(nPulseRoi);
  roi.charge    = roiCharge;
  roi.avgCharge = avgCharge;
  roi.pulses    = roiPulses;
  m_pulses      = pulses;
  return pulses;
}
FadcWaveform::Peaks FadcWaveform::FindPeaks(Roi& roi,int itrnPf,int itrnDc) {
  int startIdx  = roi.start - m_minGap;
  int arraySize = roi.end   - roi.start + m_minGap;
  double rawArray[m_maxSize];
  double decArray[m_maxSize];
  Peaks peaks;
  FadcFit singleHitTemplate(1);
  double* response = singleHitTemplate.GetTemplate();
  
  int nFix   = roi.lastFitPar.size();
  FadcFit fadcFit(peaks,nFix);
  fadcFit.FixParameters(roi.lastFitPar);
  TGraph lastFit = fadcFit.DrawFit();
  for(int idx=0; idx<arraySize; idx++){
    float baseline = lastFit.Eval(float(startIdx+idx));
    rawArray[idx]  = m_waveform[startIdx+idx]-baseline;
  }
  TSpectrum simpleSpec(20);
  TSpectrum deconvSpec(20);
  //TSpectrum simpleSpec(1);
  //TSpectrum deconvSpec(1);
  
  simpleSpec.SearchHighRes(rawArray,decArray,arraySize,2,15,true,itrnPf,false,5);//change sigma of peaks from 1 to 2 change threshold from 3.5->15 
            
  deconvSpec.SmoothMarkov (rawArray,arraySize,3);
  deconvSpec.Deconvolution(rawArray,response,arraySize,itrnDc,1,1);
  for(int i=0; i<arraySize; i++)  {
    double x,y;
    m_dec.GetPoint(startIdx+i,x,y);
    double newY = y + rawArray[i];
    m_dec.SetPoint(startIdx+i,startIdx+i,newY);
  }
  
  deconvSpec.SearchHighRes(rawArray,decArray,arraySize,2,15,true,itrnPf,false,5);//same as above
  
  int    nPulseSimple = simpleSpec.GetNPeaks();
  double *tPulseSimple = simpleSpec.GetPositionX();
  int    nPulseDeconv = deconvSpec.GetNPeaks();
  double *tPulseDeconv = deconvSpec.GetPositionX();
  
  for(int pulseIdx=0; pulseIdx<nPulseDeconv; pulseIdx++) {
    float pulseTime  = tPulseDeconv[pulseIdx] + startIdx;
    float baseline   = lastFit.Eval(pulseTime);
    float pulseAmp   = m_waveform[int(pulseTime)]-baseline;
    if(pulseAmp<m_threshold-1.) continue;
    peaks[pulseTime] = pulseAmp; 
    //cout << " ----> adding pulse " << pulseIdx+1 << endl;
    //cout << " time = " << pulseTime << endl;
    //cout << " amp  = " << pulseAmp << endl;
  }
  //cout << " =====> size = " << peaks.size() << endl;
  for(int pulseIdx=0; pulseIdx<nPulseSimple; pulseIdx++) {
    float simpleTime = tPulseSimple[pulseIdx] + startIdx;;
    float simpleAmp  = m_waveform[int(simpleTime)];
    if(simpleAmp<m_threshold-1.) continue;
    bool isClose = false;
    Peaks::iterator peakIt = peaks.begin();
    for(;peakIt != peaks.end();peakIt++) {
      float pulseTime = peakIt->first;
      if(fabs(pulseTime-simpleTime) < 6)
        isClose = true;
    }
    if(isClose) continue;
    peaks[simpleTime] = simpleAmp;
    //cout << " ----> adding simple pulse " << pulseIdx+1 << endl;
    //cout << " time = " << simpleTime << endl;
    //cout << " amp  = " << simpleAmp << endl;
  }
  roi.peaks = peaks;
  return peaks;
}

void FadcWaveform::Deconvolve(){
  //for(unsigned int roiIdx=0; roiIdx<m_rois.size(); roiIdx++)  {
    //cout << " ******************** " << endl;
    //cout << " Start:    " << m_rois[roiIdx].start   << endl;
    //cout << " End:      " << m_rois[roiIdx].end     << endl;
    //cout << " ADC:      " << m_rois[roiIdx].maxAdc  << endl;
    //cout << " QDC:      " << m_rois[roiIdx].qdc     << endl;
    //cout << " PreGap:   " << m_rois[roiIdx].preGap  << endl;
    //cout << " PostGap:  " << m_rois[roiIdx].postGap << endl;
    //cout << " ******************** " << endl;
  //}
  m_peaks.    clear(); 
  m_pulses.   clear();
  m_chi2 = 0;
  m_fit.Set(0);
  int nRoi = m_rois.size();
  /// no ROI found
  if(nRoi==0) {

    m_peaks [0] = 0;
    m_pulses[0] = 0; 
    m_fit.Set(0);
    m_fit.SetPoint(0,0,0);
    m_fit.SetPoint(1,m_nSamples,0);
    return;
  }
  /// loop over ROIs
  for(int roiIdx=0; roiIdx<nRoi; roiIdx++)  {
    //cout << " ----> ROI " << roiIdx << endl;
    Roi roi = m_rois[roiIdx];
    Peaks  peaks  = FindPeaks(roi,5,1000);
    Pulses pulses = FitPeaks (roi,peaks);
    //cout << " ROI n         = " << roi.peaks.size() << endl;
    //cout << " ROI chi2      = " << roi.chi2      << endl;
    //cout << " ROI avgCharge = " << roi.avgCharge << endl;
    if(roi.chi2>15 && roi.avgCharge>1.2){
      peaks  = FindPeaks(roi,25,5000);
      pulses = FitPeaks (roi,peaks);
      //cout << " ** refined fit: " << endl;
      //cout << " ROI n         = " << roi.peaks.size() << endl;
      //cout << " ROI chi2      = " << roi.chi2      << endl;
      //cout << " ROI avgCharge = " << roi.avgCharge << endl;
    }
    m_pulses      = pulses;
    m_peaks       = peaks;

    m_rois[roiIdx].fitPar     = roi.fitPar;
    m_fitPar = roi.fitPar; 
    m_chi2  += roi.chi2; 
    if(roiIdx<nRoi-1){
      m_rois[roiIdx+1].lastFitPar = roi.fitPar;
    }
  }
  ///// Final check 1: total fit
  //Peaks peaks;
  ////peaks[200] = 0;
  //peaks[100] = 0;
  //Roi roi = m_rois[nRoi-1];
  //roi.start = 250;
  //roi.end   = 550;
  //int nFix   = roi.fitPar.size();
  //FadcFit fadcFit(peaks,nFix);
  //fadcFit.FixParameters(roi.fitPar);
  
  //int startIdx  = 250;
  //int fitLength = 350;
  //TGraphErrors tmpG(0);
  //for(int idx=0;idx<fitLength;idx++) {
    //tmpG.SetPoint     (idx,startIdx+idx,m_waveform[startIdx+idx]);
    //tmpG.SetPointError(idx,0           ,m_pedError);
  //}
  //float chi2Total = fadcFit.Fit(tmpG);
  //cout << " chi2Total = " << chi2Total << endl;
}

void FadcWaveform::clear() {
    m_nSamples = 0;
    m_rawWaveform.clear();
    m_waveform.clear();

    m_pedestal = 0;
    m_pedError = 0;
    m_pedChi2 = 0;
    // m_preWindow = 0;
    m_bits = 0;
    // m_adcMax = 0;
    // m_fadcGain = 0;
    // m_minGap = 0;
    m_isSaturated = 0;
    m_fixOs = 0;
    // m_threshold = 0;

    m_graph.Set(0);
    m_fit.Set(0);
    m_raw.Set(0);
    m_dec.Set(0);
    m_rois.clear();

    m_peaks.clear();
    m_pulses.clear();
    m_charge = 0;
    m_fitPar .clear();
    m_chi2 = 0;
}
