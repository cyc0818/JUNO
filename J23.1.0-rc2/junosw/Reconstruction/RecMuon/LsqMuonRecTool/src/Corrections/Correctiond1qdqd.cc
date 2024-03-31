#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond1qdqd::Correctiond1qdqd(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond1qdqd::~Correctiond1qdqd(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond1qdqd::freeTablesPointVector(){
  delete []m_prof1dt_q;
  delete []m_prof1dtq_d;
  delete []m_prof1dtqd_q;
  delete []m_prof1dtqdq_d;
  delete []m_prof1dtqdqd_d;
}

void Correctiond1qdqd::mallocTablesPointVector(int n){
  m_prof1dt_q    = new TProfile*[n] ;
  m_prof1dtq_d   = new TProfile*[n] ;
  m_prof1dtqd_q  = new TProfile*[n] ;
  m_prof1dtqdq_d = new TProfile*[n] ;
  m_prof1dtqdqd_d= new TProfile*[n] ;
}

bool Correctiond1qdqd::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond1qdqd::retrieveCorrectionTables(){
  m_corrfile = new TFile(m_corrfilename.c_str());
  if(not m_corrfile->IsOpen()){
    LogError << " Failed to open FhtCorrFile ["
      << m_corrfilename << "]" << std::endl;
    return false;
  }

  for (int i = 0; i < m_numBinDist; ++i){
    TString tnameSuffix = TString::Format("%d",i);
    TString tname = TString::Format("prof1dt_q") + tnameSuffix;
    m_prof1dt_q[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dt_q[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtq_d") + tnameSuffix;
    m_prof1dtq_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtq_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtqd_q") + tnameSuffix;
    m_prof1dtqd_q[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtqd_q[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtqdq_d") + tnameSuffix;
    m_prof1dtqdq_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtqdq_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtqdqd_d") + tnameSuffix;
    m_prof1dtqdqd_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtqdqd_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    m_prof1dtqdqd_d[i]->SetErrorOption("s");
  }
  return true;
}

double Correctiond1qdqd::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
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
  if(d<dHigh&&d>dLow){
    correction = m_prof1dt_q[binDist]->Interpolate(pmt.q)
      + m_prof1dtq_d[binDist]->Interpolate(d)
      + m_prof1dtqd_q[binDist]->Interpolate(pmt.q)
      + m_prof1dtqdq_d[binDist]->Interpolate(d);
  }
  return correction;
}

double Correctiond1qdqd::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double qcut = 10000;
  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return sigma;
  }
  double d = (pmt.pos-ri)*dir.Unit();
  if (pmt.q<qcut){
    int bin = m_prof1dtqdqd_d[binDist]->FindBin(d);
    sigma = m_prof1dtqdqd_d[binDist]->GetBinError(bin);
  } 
  return sigma;
}
