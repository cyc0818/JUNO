#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond2ad::Correctiond2ad(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond2ad::~Correctiond2ad(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond2ad::freeTablesPointVector(){
  delete []m_prof2dtad;
  delete []m_hdtaderr;
}

void Correctiond2ad::mallocTablesPointVector(int n){
  m_prof2dtad = new TProfile2D*[n];
  m_hdtaderr = new TH2F*[n];
}

bool Correctiond2ad::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond2ad::retrieveCorrectionTables(){
  m_corrfile = new TFile(m_corrfilename.c_str());
  if(not m_corrfile->IsOpen()){
    LogError << " Failed to open FhtCorrFile ["
      << m_corrfilename << "]" << std::endl;
    return false;
  }

  for (int i = 0; i < m_numBinDist; ++i){
    TString tnameSuffix = TString::Format("%d",i);
    TString tname = TString::Format("prof2dtad") + tnameSuffix;
    m_prof2dtad[i] = (TProfile2D*)m_corrfile->Get(tname);
    if(not m_prof2dtad[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("hdtaderr") + tnameSuffix;
    m_hdtaderr[i] = (TH2F*) m_corrfile->Get(tname);
    if(not m_hdtaderr[i]){
      LogError <<" Failed to retrieve"<<tname.Data()<<std::endl;
      return false;
    }
  }
  return true;
}

double Correctiond2ad::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
  double dLow = -19500;
  double dHigh = 37200;
  double correction = 0;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return correction;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  TVector3 vecOCentrk = ri + (-ri*dir)*dir;
  TVector3 vecSP = pmt.pos-ri;
  TVector3 vecNP = vecSP-(vecSP*dir)*dir;
  //double a = vecNP.Angle(vecOCentrk);
  TVector3 vecOP1 = vecOCentrk + vecNP;
 double a = vecOP1.Angle(vecOCentrk);
  

  if(d<dHigh&&d>dLow)
    correction = m_prof2dtad[binDist]->Interpolate(d,a);
  return correction;
}

double Correctiond2ad::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return sigma;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  TVector3 vecOCentrk = ri + (-ri*dir)*dir;
  TVector3 vecSP = pmt.pos-ri;
  TVector3 vecNP = vecSP-(vecSP*dir)*dir;
  //double a = vecNP.Angle(vecOCentrk);
  TVector3 vecOP1 = vecOCentrk + vecNP;
  double a = vecOP1.Angle(vecOCentrk);
  int binx = m_hdtaderr[binDist]->GetXaxis()->FindBin(d);
  int biny = m_hdtaderr[binDist]->GetYaxis()->FindBin(a);
  sigma = m_hdtaderr[binDist]->GetBinContent(binx,biny);
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
    error[0] = m_hdtaderr[binDist]->GetBinContent(binx_minus,biny);
    error[1] = m_hdtaderr[binDist]->GetBinContent(binx_plus,biny);
    error[2] = m_hdtaderr[binDist]->GetBinContent(binx,biny_minus);
    error[3] = m_hdtaderr[binDist]->GetBinContent(binx,biny_plus);
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
  if(sigma<0.00001) sigma = 3;
  if(chi2comp != NULL)
    *chi2comp = fabs(biny_plus-biny);
  return sigma;
}
