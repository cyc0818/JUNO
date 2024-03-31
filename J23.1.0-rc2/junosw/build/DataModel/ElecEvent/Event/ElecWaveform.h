
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

#ifndef SNIPER_ELECEVENT_ELECWAVEFORM_H
#define SNIPER_ELECEVENT_ELECWAVEFORM_H

// Include files
#include "TObject.h"
#include <vector>
#include <cstdint>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class ElecWaveform ElecWaveform.h
   *
   * Waveform informaiton of per PMT channel 
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:14 2023
   * 
   */

  class ElecWaveform
  {
  private:

    std::vector<uint16_t> m_adc;         // Amplitude of Waveform
    std::vector<uint16_t> m_tdc;         // Time info of Waveform
    uint32_t              m_channelFlag; // Waveform channel flag. 0: high/low range. 1-31: TBD
  

  protected:


  public:

    /// Default Constructor
    ElecWaveform() : m_adc(),
                     m_tdc(),
                     m_channelFlag() {}
  
    /// Default Destructor
    virtual ~ElecWaveform() {}
  
    /// 
    std::vector<uint16_t>&  adc();
  
    /// 
    std::vector<uint16_t>&  tdc();
  
    /// 
    const std::vector<uint16_t>&  tdc() const;
  
    /// test if this channel is low range
    bool  isLowRange() const;
  
    /// test if this channel is high range
    bool  isHighRange() const;
  
    /// set this channel to be low range
    void  setLowRange();
  
    /// set this channel to be high range
    void  setHighRange();
  
    /// Retrieve const  
    /// Amplitude of Waveform
    const std::vector<uint16_t>& adc() const;
  
    /// Update  
    /// Amplitude of Waveform
    void setAdc(const std::vector<uint16_t>& value);
  
    /// Update  
    /// Time info of Waveform
    void setTdc(const std::vector<uint16_t>& value);
  
    /// Retrieve const  
    /// Waveform channel flag. 0: high/low range. 1-31: TBD
    const uint32_t& channelFlag() const;
  
    /// Update  
    /// Waveform channel flag. 0: high/low range. 1-31: TBD
    void setChannelFlag(const uint32_t& value);
  
    ClassDef(ElecWaveform,2)
  

  }; // class ElecWaveform

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<uint16_t>& JM::ElecWaveform::adc() const 
{
  return m_adc;
}

inline void JM::ElecWaveform::setAdc(const std::vector<uint16_t>& value) 
{
  m_adc = value;
}

inline void JM::ElecWaveform::setTdc(const std::vector<uint16_t>& value) 
{
  m_tdc = value;
}

inline const uint32_t& JM::ElecWaveform::channelFlag() const 
{
  return m_channelFlag;
}

inline void JM::ElecWaveform::setChannelFlag(const uint32_t& value) 
{
  m_channelFlag = value;
}

inline std::vector<uint16_t>&  JM::ElecWaveform::adc() 
{

  return m_adc;
                
}

inline std::vector<uint16_t>&  JM::ElecWaveform::tdc() 
{

  if (m_tdc.size()==0 || m_tdc.size()!=m_adc.size()) {
    m_tdc.clear();
    m_tdc.reserve(m_adc.size());

    for (size_t i = 0; i < m_adc.size(); ++i) {
      m_tdc.push_back(i);
    }
  }
  return m_tdc;
                
}

inline const std::vector<uint16_t>&  JM::ElecWaveform::tdc() const 
{

  // FIXME
  // because this function is const, we could not modify m_tdc
  static std::vector<uint16_t> tdc;
  if (tdc.size()==0 || tdc.size()!=m_adc.size()) {
    tdc.clear();
    tdc.reserve(m_adc.size());

    for (size_t i = 0; i < m_adc.size(); ++i) {
      tdc.push_back(i);
    }
  }
  return tdc;
                
}

inline bool  JM::ElecWaveform::isLowRange() const 
{

  return TESTBIT(m_channelFlag,0);
                
}

inline bool  JM::ElecWaveform::isHighRange() const 
{

  return !TESTBIT(m_channelFlag,0);
                
}

inline void  JM::ElecWaveform::setLowRange() 
{

  SETBIT(m_channelFlag,0);
                
}

inline void  JM::ElecWaveform::setHighRange() 
{

  CLRBIT(m_channelFlag,0);
                
}


#endif ///SNIPER_ELECEVENT_ELECWAVEFORM_H
