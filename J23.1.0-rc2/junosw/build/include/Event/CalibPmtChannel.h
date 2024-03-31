
//   **************************************************************************
//   *                                                                        *
//   *                      ! ! ! A T T E N T I O N ! ! !                     *
//   *                                                                        *
//   *  This file was created automatically by XmlObjDesc, please do not      *
//   *  delete it or edit it by hand.                                         *
//   *                                                                        *
//   *  If you want to change this file, first change the corresponding       *
//   *  xml-file and rerun the tools from XmlObjDesc (or run make if you      *
//   *  are using it from inside a Sniper-package).                           *
//   *                                                                        *
//   **************************************************************************

#ifndef SNIPER_CALIBEVENT_CALIBPMTCHANNEL_H
#define SNIPER_CALIBEVENT_CALIBPMTCHANNEL_H

// Include files
#include "TObject.h"
#include <vector>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CalibPmtChannel CalibPmtChannel.h
   *
   * Calibration PMT header. ChangeLog: v1) original version. v2) change double
 * to float for both nPE and time. 
   *
   * @author Kun ZHANG
   * created Sun Sep 24 17:52:44 2023
   * 
   */

  class CalibPmtChannel: public TObject
  {
  private:

    float              m_nPE;          // Number of PE
    unsigned int       m_pmtId;        // Id of the pmt
    float              m_firstHitTime; // Time of the first hit
    std::vector<float> m_charge;       // Vector of charges (PE)
    std::vector<float> m_time;         // Vector of times (ns)
    float              m_riseTime;     // Time of rise 10% -> 90%
  

  protected:


  public:

  /// constructor with pmtid
  CalibPmtChannel(unsigned int pmtid);
  
    /// Default Constructor
    CalibPmtChannel() : m_nPE(0.0),
                        m_pmtId(0),
                        m_firstHitTime(0.0),
                        m_charge(),
                        m_time(),
                        m_riseTime(0.0) {}
  
    /// Default Destructor
    virtual ~CalibPmtChannel() {}
  
    /// Return size of time vector(should be the same as charge vector)
    unsigned int size() const;
  
    /// Return charge with a given index
    float charge(unsigned int index) const;
  
    /// Return time with a given index
    float time(unsigned int index) const;
  
    /// Return the max charge index for this channel
    int maxChargeIndex() const;
  
    /// Return max charge for this channel
    float maxCharge() const;
  
    /// Return sum of charge for this channel
    float sumCharge() const;
  
    /// Return the index of earliest time in a given range; returns -1 if no hits found
    int earliestTimeIndex(float earlytime=-1000000,
                          float latetime=1000000) const;
  
    /// Return earliest time in a given range; returns 0 if no hits are found
    float earliestTime(float earlytime=-1000000,
                       float latetime=1000000) const;
  
    /// Return the earliest ADC value in a given range; returns 0 if no hits are found
    float earliestCharge(float earlytime=-1000000,
                         float latetime=1000000) const;
  
    /// Return the charge in a certain time range
    float properCharge(float earlytime,
                       float latetime) const;
  
    /// Retrieve const  
    /// Number of PE
    float nPE() const;
  
    /// Update  
    /// Number of PE
    void setNPE(float value);
  
    /// Retrieve const  
    /// Id of the pmt
    unsigned int pmtId() const;
  
    /// Update  
    /// Id of the pmt
    void setPmtId(unsigned int value);
  
    /// Retrieve const  
    /// Time of the first hit
    float firstHitTime() const;
  
    /// Update  
    /// Time of the first hit
    void setFirstHitTime(float value);
  
    /// Retrieve const  
    /// Vector of charges (PE)
    const std::vector<float>& charge() const;
  
    /// Update  
    /// Vector of charges (PE)
    void setCharge(const std::vector<float>& value);
  
    /// Retrieve const  
    /// Vector of times (ns)
    const std::vector<float>& time() const;
  
    /// Update  
    /// Vector of times (ns)
    void setTime(const std::vector<float>& value);
  
    /// Retrieve const  
    /// Time of rise 10% -> 90%
    float riseTime() const;
  
    /// Update  
    /// Time of rise 10% -> 90%
    void setRiseTime(float value);
  
    ClassDef(CalibPmtChannel,2)
  

  }; // class CalibPmtChannel

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CalibPmtChannel::CalibPmtChannel(unsigned int pmtid) 
{

                    m_pmtId = pmtid;
                
}

inline float JM::CalibPmtChannel::nPE() const 
{
  return m_nPE;
}

inline void JM::CalibPmtChannel::setNPE(float value) 
{
  m_nPE = value;
}

inline unsigned int JM::CalibPmtChannel::pmtId() const 
{
  return m_pmtId;
}

inline void JM::CalibPmtChannel::setPmtId(unsigned int value) 
{
  m_pmtId = value;
}

inline float JM::CalibPmtChannel::firstHitTime() const 
{
  return m_firstHitTime;
}

inline void JM::CalibPmtChannel::setFirstHitTime(float value) 
{
  m_firstHitTime = value;
}

inline const std::vector<float>& JM::CalibPmtChannel::charge() const 
{
  return m_charge;
}

inline void JM::CalibPmtChannel::setCharge(const std::vector<float>& value) 
{
  m_charge = value;
}

inline const std::vector<float>& JM::CalibPmtChannel::time() const 
{
  return m_time;
}

inline void JM::CalibPmtChannel::setTime(const std::vector<float>& value) 
{
  m_time = value;
}

inline float JM::CalibPmtChannel::riseTime() const 
{
  return m_riseTime;
}

inline void JM::CalibPmtChannel::setRiseTime(float value) 
{
  m_riseTime = value;
}

inline unsigned int JM::CalibPmtChannel::size() const 
{

  return m_time.size();
                
}

inline float JM::CalibPmtChannel::charge(unsigned int index) const 
{

  return index >= m_charge.size() ? 0 : m_charge[index];
                
}

inline float JM::CalibPmtChannel::time(unsigned int index) const 
{

  return index >= m_time.size() ? 0 : m_time[index];
                
}

inline int JM::CalibPmtChannel::maxChargeIndex() const 
{

  if( m_charge.empty() ) return 0;
  float maxCharge = 0;
  int maxChargeIndex = 0;
  for(unsigned i=0; i<m_charge.size(); i++) {
    if( m_charge[i] > maxCharge ) {
      maxCharge = m_charge[i];
      maxChargeIndex = i;
    }
  }
  return maxChargeIndex;
                
}

inline float JM::CalibPmtChannel::maxCharge() const 
{

  if( m_charge.empty() ) return 0;
  return m_charge[maxChargeIndex()];
                
}

inline float JM::CalibPmtChannel::sumCharge() const 
{

  if( m_charge.empty() ) return 0;
  float sumCharge = 0;
  for(unsigned i=0; i<m_charge.size(); i++) {
    sumCharge += m_charge[i];
  }
  return sumCharge;
                
}

inline int JM::CalibPmtChannel::earliestTimeIndex(float earlytime,
                                                  float latetime) const 
{
    
  if( m_time.empty() ) return -1;
  float earliestTime = 1e9;
  int earliestTimeIndex = -1;
  for(unsigned int i=0; i<m_time.size(); i++) {
    if( m_time[i]<earliestTime &&  m_time[i]<latetime && m_time[i]>earlytime ) {
      earliestTime = m_time[i];
      earliestTimeIndex = i;
    }
  }

  return earliestTimeIndex;
                
}

inline float JM::CalibPmtChannel::earliestTime(float earlytime,
                                               float latetime) const 
{

  if( m_time.empty() ) return 0;
  int index=earliestTimeIndex(earlytime,latetime);
  if(index < 0) return 0;
  else return m_time[index];
                
}

inline float JM::CalibPmtChannel::earliestCharge(float earlytime,
                                                 float latetime) const 
{

  if( m_charge.empty() ) return 0;
  int index=earliestTimeIndex(earlytime,latetime);
  if(index < 0) return 0;
  return m_charge[index];
                
}

inline float JM::CalibPmtChannel::properCharge(float earlytime,
                                               float latetime) const 
{

  float pcharge=0;
  for(unsigned int i=0;i < m_time.size();i++){
    if(m_time[i] > earlytime && m_time[i] < latetime)
      pcharge+=m_charge[i];
    }
  return pcharge;
                
}


#endif ///SNIPER_CALIBEVENT_CALIBPMTCHANNEL_H
