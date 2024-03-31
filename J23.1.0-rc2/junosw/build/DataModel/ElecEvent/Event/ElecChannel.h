
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

#ifndef SNIPER_ELECEVENT_ELECCHANNEL_H
#define SNIPER_ELECEVENT_ELECCHANNEL_H

// Include files
#include "TObject.h"
#include <vector>
#include <cstdint>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class ElecChannel ElecChannel.h
   *
   * Container for t/q pairs of one PMT 
   *
   * @author J. P. A. M. de Andre
   * created Sun Sep 24 17:52:15 2023
   * 
   */

  class ElecChannel: public TObject
  {
  private:

    std::vector<unsigned int> m_charge;      // Charge info of t/q pairs
    std::vector<unsigned int> m_time;        // Time info of t/q pairs
    uint32_t                  m_channelFlag; // Waveform channel flag. 0: high/low range. 1-31: TBD
  

  protected:


  public:

    /// Default Constructor
    ElecChannel() : m_charge(),
                    m_time(),
                    m_channelFlag() {}
  
    /// Default Destructor
    virtual ~ElecChannel() {}
  
    /// 
    std::vector<unsigned int>&  charge();
  
    /// 
    std::vector<unsigned int>&  time();
  
    /// test if this channel is low range
    bool  isLowRange() const;
  
    /// test if this channel is high range
    bool  isHighRange() const;
  
    /// set this channel to be low range
    void  setLowRange();
  
    /// set this channel to be high range
    void  setHighRange();
  
    /// Retrieve const  
    /// Charge info of t/q pairs
    const std::vector<unsigned int>& charge() const;
  
    /// Update  
    /// Charge info of t/q pairs
    void setCharge(const std::vector<unsigned int>& value);
  
    /// Update  
    /// Time info of t/q pairs
    void setTime(const std::vector<unsigned int>& value);
  
    /// Retrieve const  
    /// Waveform channel flag. 0: high/low range. 1-31: TBD
    const uint32_t& channelFlag() const;
  
    /// Update  
    /// Waveform channel flag. 0: high/low range. 1-31: TBD
    void setChannelFlag(const uint32_t& value);
  
    ClassDef(ElecChannel,1)
  

  }; // class ElecChannel

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<unsigned int>& JM::ElecChannel::charge() const 
{
  return m_charge;
}

inline void JM::ElecChannel::setCharge(const std::vector<unsigned int>& value) 
{
  m_charge = value;
}

inline void JM::ElecChannel::setTime(const std::vector<unsigned int>& value) 
{
  m_time = value;
}

inline const uint32_t& JM::ElecChannel::channelFlag() const 
{
  return m_channelFlag;
}

inline void JM::ElecChannel::setChannelFlag(const uint32_t& value) 
{
  m_channelFlag = value;
}

inline std::vector<unsigned int>&  JM::ElecChannel::charge() 
{

  return m_charge;
                
}

inline std::vector<unsigned int>&  JM::ElecChannel::time() 
{

  if (m_time.size()==0 || m_time.size()!=m_charge.size()) {
    m_time.clear();

    for (size_t i = 0; i < m_charge.size(); ++i) {
      m_time.push_back(i);
    }
  }
  return m_time;
                
}

inline bool  JM::ElecChannel::isLowRange() const 
{

  return TESTBIT(m_channelFlag,0);
                
}

inline bool  JM::ElecChannel::isHighRange() const 
{

  return !TESTBIT(m_channelFlag,0);
                
}

inline void  JM::ElecChannel::setLowRange() 
{

  SETBIT(m_channelFlag,0);
                
}

inline void  JM::ElecChannel::setHighRange() 
{

  CLRBIT(m_channelFlag,0);
                
}


#endif ///SNIPER_ELECEVENT_ELECCHANNEL_H
