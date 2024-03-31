#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond2qd::Correctiond2qd(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond2qd::~Correctiond2qd(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond2qd::freeTablesPointVector(){
  delete []m_prof2dtqd;
  delete []m_hdtqderr;
}

void Correctiond2qd::mallocTablesPointVector(int n){
  m_prof2dtqd = new TProfile2D*[n];
  m_hdtqderr = new TH2F*[n];
}

bool Correctiond2qd::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond2qd::retrieveCorrectionTables(){
  m_corrfile = new TFile(m_corrfilename.c_str());
  if(not m_corrfile->IsOpen()){
    LogError << " Failed to open FhtCorrFile ["
      << m_corrfilename << "]" << std::endl;
    return false;
  }

  for (int i = 0; i < m_numBinDist; ++i){
    TString tnameSuffix = TString::Format("%d",i);
    TString tname = TString::Format("prof2dtqd") + tnameSuffix;
    m_prof2dtqd[i] = (TProfile2D*)m_corrfile->Get(tname);
    if(not m_prof2dtqd[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("hdtqderr") + tnameSuffix;
    m_hdtqderr[i] = (TH2F*) m_corrfile->Get(tname);
    if(not m_hdtqderr[i]){
      LogError <<" Failed to retrieve"<<tname.Data()<<std::endl;
      return false;
    }
  }
  return true;
}

double Correctiond2qd::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
  double dLow = -19500;
  double dHigh = 37200;
  double correction = 0;
  double qcut = 10000;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return correction;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  //For shower muon event , pmt.q can be very huge, but this kind of pmt signals is few, so
  //the calibration for the method with factor pmt.q should give pmt.q a cut, and the correlated 
  //correciton should have this cut too. This cut can be added at the rec tool in pmtcut function.
  //And in the future the PMT CUT should be separated as a independant PMT SELECTION TOOL. In any
  //case the correction method should match with the corelated cut.
  if(d<dHigh&&d>dLow&&pmt.q<qcut)
    correction = m_prof2dtqd[binDist]->Interpolate(d,pmt.q);
  return correction;
}

double Correctiond2qd::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double qcut=10000;
  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ||pmt.q>qcut){
    return sigma;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  int binx = m_hdtqderr[binDist]->GetXaxis()->FindBin(d);
  int biny = m_hdtqderr[binDist]->GetYaxis()->FindBin(pmt.q);
  sigma = m_hdtqderr[binDist]->GetBinContent(binx,biny);
  int binx_minus = binx;
  int binx_plus = binx;
  int biny_minus = biny;
  int biny_plus = biny;
  for (int times=0;sigma<0.00001&&times<100;times++){
    binx_minus --;
    binx_plus ++;
    biny_minus --;
    biny_plus ++;
    double error[4];
    error[0] = m_hdtqderr[binDist]->GetBinContent(binx_minus,biny);
    error[1] = m_hdtqderr[binDist]->GetBinContent(binx_plus,biny);
    error[2] = m_hdtqderr[binDist]->GetBinContent(binx,biny_minus);
    error[3] = m_hdtqderr[binDist]->GetBinContent(binx,biny_plus);
    int tot = 0;
    sigma = 0;
    for(int i=0;i<4;i++){
      if (error[i] > 0.00001) {
        sigma += error[i];
        tot++;
      }
    }
    if(tot>0) sigma/=tot;
  }
  if (sigma<0.00001) sigma =3;
  if(chi2comp != NULL)
    *chi2comp = fabs(biny_plus-biny);
  return sigma;
}
