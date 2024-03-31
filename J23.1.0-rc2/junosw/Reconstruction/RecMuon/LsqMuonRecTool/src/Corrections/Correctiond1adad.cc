#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond1adad::Correctiond1adad(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond1adad::~Correctiond1adad(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond1adad::freeTablesPointVector(){
  delete []m_prof1dt_a;
  delete []m_prof1dta_d;
  delete []m_prof1dtad_a;
  delete []m_prof1dtada_d;
  delete []m_prof1dtadad_d;
}

void Correctiond1adad::mallocTablesPointVector(int n){
  m_prof1dt_a    = new TProfile*[n] ;
  m_prof1dta_d   = new TProfile*[n] ;
  m_prof1dtad_a  = new TProfile*[n] ;
  m_prof1dtada_d = new TProfile*[n] ;
  m_prof1dtadad_d= new TProfile*[n] ;
}

bool Correctiond1adad::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond1adad::retrieveCorrectionTables(){
  m_corrfile = new TFile(m_corrfilename.c_str());
  if(not m_corrfile->IsOpen()){
    LogError << " Failed to open FhtCorrFile ["
      << m_corrfilename << "]" << std::endl;
    return false;
  }

  for (int i = 0; i < m_numBinDist; ++i){
    TString tnameSuffix = TString::Format("%d",i);
    TString tname = TString::Format("prof1dt_a") + tnameSuffix;
    m_prof1dt_a[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dt_a[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dta_d") + tnameSuffix;
    m_prof1dta_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dta_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtad_a") + tnameSuffix;
    m_prof1dtad_a[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtad_a[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtada_d") + tnameSuffix;
    m_prof1dtada_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtada_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtadad_d") + tnameSuffix;
    m_prof1dtadad_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtadad_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    m_prof1dtadad_d[i]->SetErrorOption("s");
  }
  return true;
}

double Correctiond1adad::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
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
  double a = vecNP.Angle(vecOCentrk);
  if(d<dHigh&&d>dLow){
    correction = m_prof1dt_a[binDist]->Interpolate(a)
      + m_prof1dta_d[binDist]->Interpolate(d)
      + m_prof1dtad_a[binDist]->Interpolate(a)
      + m_prof1dtada_d[binDist]->Interpolate(d);
  }
  return correction;
}

double Correctiond1adad::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return sigma;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  int bin = m_prof1dtadad_d[binDist]->FindBin(d);
  sigma = m_prof1dtadad_d[binDist]->GetBinError(bin);
  return sigma;
}
