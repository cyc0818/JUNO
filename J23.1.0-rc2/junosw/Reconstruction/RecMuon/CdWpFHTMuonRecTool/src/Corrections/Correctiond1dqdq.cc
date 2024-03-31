#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond1dqdq::Correctiond1dqdq(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond1dqdq::~Correctiond1dqdq(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond1dqdq::freeTablesPointVector(){
  delete []m_prof1dt_d;
  delete []m_prof1dtd_q;
  delete []m_prof1dtdq_d;
  delete []m_prof1dtdqd_q;
  delete []m_prof1dtdqdq_q;
}

void Correctiond1dqdq::mallocTablesPointVector(int n){
  m_prof1dt_d    = new TProfile*[n] ;
  m_prof1dtd_q   = new TProfile*[n] ;
  m_prof1dtdq_d  = new TProfile*[n] ;
  m_prof1dtdqd_q = new TProfile*[n] ;
  m_prof1dtdqdq_q= new TProfile*[n] ;
}

bool Correctiond1dqdq::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond1dqdq::retrieveCorrectionTables(){
  m_corrfile = new TFile(m_corrfilename.c_str());
  if(not m_corrfile->IsOpen()){
    LogError << " Failed to open FhtCorrFile ["
      << m_corrfilename << "]" << std::endl;
    return false;
  }

  for (int i = 0; i < m_numBinDist; ++i){
    TString tnameSuffix = TString::Format("%d",i);
    TString tname = TString::Format("prof1dt_d") + tnameSuffix;
    m_prof1dt_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dt_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtd_q") + tnameSuffix;
    m_prof1dtd_q[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtd_q[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtdq_d") + tnameSuffix;
    m_prof1dtdq_d[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtdq_d[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtdqd_q") + tnameSuffix;
    m_prof1dtdqd_q[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtdqd_q[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    tname = TString::Format("prof1dtdqdq_q") + tnameSuffix;
    m_prof1dtdqdq_q[i] = (TProfile*)m_corrfile->Get(tname);
    if(not m_prof1dtdqdq_q[i]){
      LogError <<" Failed to retrieve "<<tname.Data()<<std::endl;
      return false;
    }
    m_prof1dtdqdq_q[i]->SetErrorOption("s");
  }
  return true;
}

double Correctiond1dqdq::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
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
    correction = m_prof1dt_d[binDist]->Interpolate(d)
      + m_prof1dtd_q[binDist]->Interpolate(pmt.q)
      + m_prof1dtdq_d[binDist]->Interpolate(d)
      + m_prof1dtdqd_q[binDist]->Interpolate(pmt.q);
  }
  return correction;
}

double Correctiond1dqdq::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return sigma;
  }
  int bin = m_prof1dtdqdq_q[binDist]->FindBin(pmt.q);
  sigma = m_prof1dtdqdq_q[binDist]->GetBinError(bin);
  return sigma;
}
