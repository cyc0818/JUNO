#include "Correction.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"
#include "SniperKernel/SniperLog.h"


  Correctiond1q::Correctiond1q(const std::string& name)
   :ToolBase(name)
  ,m_numBinDist(0)
   ,m_rangeDist(18000)
{
  mallocTablesPointVector(m_numBinDist);
}
Correctiond1q::~Correctiond1q(){
  freeTablesPointVector();
  if (m_corrfile->IsOpen())
    m_corrfile->Close();
}

void Correctiond1q::freeTablesPointVector(){
  delete []m_prof1dt_q;
}

void Correctiond1q::mallocTablesPointVector(int n){
  m_prof1dt_q    = new TProfile*[n] ;
}

bool Correctiond1q::setCorrectionFileAndNumBinDist(std::string filename, int nBin){
  freeTablesPointVector();
  mallocTablesPointVector(nBin);
  m_numBinDist = nBin;
  m_corrfilename = filename;

  if (not  retrieveCorrectionTables())
    return false;
  return true;
}

bool Correctiond1q::retrieveCorrectionTables(){
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
    m_prof1dt_q[i]->SetErrorOption("s");
  }
  return true;
}

double Correctiond1q::correction(const PmtProp& pmt, const TVector3& ri, const TVector3& dir) const{
  double qcut = 10000;
  double correction = 0;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return correction;
  }
  //if ( pmt.q<qcut ){
    correction = m_prof1dt_q[binDist]->Interpolate(pmt.q);
  //}
  return correction;
}

double Correctiond1q::sigma(const PmtProp& pmt, const TVector3& ri, const TVector3& dir, double* chi2comp) const{

  double qcut = 10000;
  double sigma = 3;
  double widthBinDist = m_rangeDist/m_numBinDist;
  double currentDist = ri.Cross(dir.Unit()).Mag();
  int binDist = (int)(currentDist/widthBinDist);
  if ( binDist>m_numBinDist || binDist<0 ){
    return sigma;
  }
  if (pmt.q<qcut){
    int bin = m_prof1dt_q[binDist]->FindBin(pmt.q);
    sigma = m_prof1dt_q[binDist]->GetBinError(bin);
  } 
  return sigma;
}
