#pragma once

class PmtSimData_LPMT
{

public:
  PmtSimData_LPMT(){}
  PmtSimData_LPMT(int id,
             double pde,
             double gain,
             double sigmaGain,
             double afterPulseProb,
             double prePulseProb,
             double darkRate,
             double timeSpread,
             double timeOffset) : m_pmtId(id),
                                  m_pad(0),
                                  m_pde(pde),
                                  m_gain(gain),
                                  m_sigmaGain(sigmaGain),
                                  m_afterPulseProb(afterPulseProb),
                                  m_prePulseProb(prePulseProb),
                                  m_darkRate(darkRate),
                                  m_timeSpread(timeSpread),
                                  m_timeOffset(timeOffset)
  {
  }
  PmtSimData_LPMT(const PmtSimData_LPMT &others)
      : m_pmtId(others.m_pmtId),
        m_pad(0),
        m_pde(others.m_pde),
        m_gain(others.m_gain),
        m_sigmaGain(others.m_sigmaGain),
        m_afterPulseProb(others.m_afterPulseProb),
        m_prePulseProb(others.m_prePulseProb),
        m_darkRate(others.m_darkRate),
        m_timeSpread(others.m_timeSpread),
        m_timeOffset(others.m_timeOffset)
  {
  }
  int pmtId() const ;
  int pad() const ;   // to quell -Wunused-private-field warning
  double pde() const ;
  double gain() const ;
  double sigmaGain() const ;
  double afterPulseProb() const ;
  double prePulseProb() const ;
  double darkRate() const ;
  double timeSpread() const ;
  double timeOffset() const ;

private:
  int m_pmtId;
  int m_pad ;   // two int occupy same memory as one double, hence this pad
  double m_pde;
  double m_gain;
  double m_sigmaGain;
  double m_afterPulseProb;
  double m_prePulseProb;
  double m_darkRate;
  double m_timeSpread;
  double m_timeOffset;
};

inline int PmtSimData_LPMT::pmtId() const 
{ 
    return m_pmtId;
}
inline int PmtSimData_LPMT::pad() const
{ 
    return m_pad;
}
inline double PmtSimData_LPMT::pde() const 
{
    return m_pde;
}
inline double PmtSimData_LPMT::gain() const 
{
    return m_gain;
}
inline double PmtSimData_LPMT::sigmaGain() const 
{
    return m_sigmaGain;
}
inline double PmtSimData_LPMT::afterPulseProb() const 
{
    return m_afterPulseProb;
}
inline double PmtSimData_LPMT::prePulseProb() const 
{
    return m_prePulseProb;
}
inline double PmtSimData_LPMT::darkRate() const 
{
    return m_darkRate;
}
inline double PmtSimData_LPMT::timeSpread() const 
{
    return m_timeSpread;
} 
inline double PmtSimData_LPMT::timeOffset() const 
{
    return m_timeOffset;
}



inline std::ostream& operator<<(std::ostream& os, const PmtSimData_LPMT& v)
{
    int w = 10 ; 
    os   
       << "PmtSimData_LPMT("  
       << std::setw(w) << v.pmtId()  
       << ";"  
       << " pde:" << std::setw(w) << std::fixed << std::setprecision(4) << v.pde()
       << " gain:" << std::setw(w) << std::fixed << std::setprecision(4) << v.gain()
       << ") "  
       ;        
    return os;  
}





