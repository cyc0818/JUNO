#pragma once

class PmtSimData_SPMT
{
public:
  PmtSimData_SPMT(){}
  PmtSimData_SPMT(int id,
                  double QE,
                  double efficiency,
                  double gain,
                  double sigmaGain,
                  double afterPulseProb,
                  double prePulseProb,
                  double darkRate,
                  double timeSpread,
                  double timeOffset) : m_pmtId(id),
                                       m_pad(0),
                                       m_QE(QE),
                                       m_efficiency(efficiency),
                                       m_gain(gain),
                                       m_sigmaGain(sigmaGain),
                                       m_afterPulseProb(afterPulseProb),
                                       m_prePulseProb(prePulseProb),
                                       m_darkRate(darkRate),
                                       m_timeSpread(timeSpread),
                                       m_timeOffset(timeOffset)
  {
  }
  PmtSimData_SPMT(const PmtSimData_SPMT &others)
      : m_pmtId(others.m_pmtId),
        m_pad(0),
        m_QE(others.m_QE),
        m_efficiency(others.m_efficiency),
        m_gain(others.m_gain),
        m_sigmaGain(others.m_sigmaGain),
        m_afterPulseProb(others.m_afterPulseProb),
        m_prePulseProb(others.m_prePulseProb),
        m_darkRate(others.m_darkRate),
        m_timeSpread(others.m_timeSpread),
        m_timeOffset(others.m_timeOffset)
  {
  }
public:
  int pmtId() const;
  int pad() const;  // quell warning 
  double QE() const;
  double efficiency() const;
  double gain() const;
  double sigmaGain() const;
  double afterPulseProb() const;
  double prePulseProb() const;
  double darkRate() const;
  double timeSpread() const;
  double timeOffset() const;

private:
  int m_pmtId;
  int m_pad ;  // two int occupy same memory as one double 
  double m_QE;
  double m_efficiency;
  double m_gain;
  double m_sigmaGain;
  double m_afterPulseProb;
  double m_prePulseProb;
  double m_darkRate;
  double m_timeSpread;
  double m_timeOffset;
};

inline int PmtSimData_SPMT::pmtId() const
{
    return m_pmtId;
}
inline int PmtSimData_SPMT::pad() const
{
    return m_pad;
}
inline double PmtSimData_SPMT::efficiency() const
{
    return m_efficiency;
}
inline double PmtSimData_SPMT::QE() const
{
    return m_QE;
}
inline double PmtSimData_SPMT::gain() const
{
    return m_gain;
}
inline double PmtSimData_SPMT::sigmaGain() const
{
    return m_sigmaGain;
}
inline double PmtSimData_SPMT::afterPulseProb() const
{
    return m_afterPulseProb;
}
inline double PmtSimData_SPMT::prePulseProb() const
{
    return m_prePulseProb;
}
inline double PmtSimData_SPMT::darkRate() const
{
    return m_darkRate;
}
inline double PmtSimData_SPMT::timeSpread() const
{
    return m_timeSpread;
}
inline double PmtSimData_SPMT::timeOffset() const
{
    return m_timeOffset;
}


inline std::ostream& operator<<(std::ostream& os, const PmtSimData_SPMT& v)
{
    int w = 10 ; 
    os   
       << "PmtSimData_SPMT("  
       << std::setw(w) << v.pmtId()  
       << ";"  
       << " efficiency:" << std::setw(w) << std::fixed << std::setprecision(4) << v.efficiency()
       << " QE:" << std::setw(w) << std::fixed << std::setprecision(4) << v.QE()
       << " gain:" << std::setw(w) << std::fixed << std::setprecision(4) << v.gain()
       << ") "  
       ;        
    return os;  
}

