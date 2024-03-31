
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

#ifndef SNIPER_CALIBEVENT_CALIBTTCHANNEL_H
#define SNIPER_CALIBEVENT_CALIBTTCHANNEL_H

// Include files
#include "TObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CalibTtChannel CalibTtChannel.h
   *
   * Calibration TT header 
   *
   * @author A.Meregaglia, J.P.A.M. de Andre
   * created Sun Sep 24 17:52:45 2023
   * 
   */

  class CalibTtChannel: public TObject
  {
  private:

    float        m_nPE;   // Number of PE
    float        m_time;  // Time from trigger
    unsigned int m_pmtId; // Id of the pmt
  

  protected:


  public:

  /// constructor with channelid
  CalibTtChannel(unsigned int channelid);
  
    /// Default Constructor
    CalibTtChannel() : m_nPE(0.0),
                       m_time(0.0),
                       m_pmtId(0) {}
  
    /// Default Destructor
    virtual ~CalibTtChannel() {}
  
    /// Retrieve const  
    /// Number of PE
    float nPE() const;
  
    /// Update  
    /// Number of PE
    void setNPE(float value);
  
    /// Retrieve const  
    /// Time from trigger
    float time() const;
  
    /// Update  
    /// Time from trigger
    void setTime(float value);
  
    /// Retrieve const  
    /// Id of the pmt
    unsigned int pmtId() const;
  
    /// Update  
    /// Id of the pmt
    void setPmtId(unsigned int value);
  
    ClassDef(CalibTtChannel,2)
  

  }; // class CalibTtChannel

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CalibTtChannel::CalibTtChannel(unsigned int channelid) 
{

  m_pmtId = channelid;
              
}

inline float JM::CalibTtChannel::nPE() const 
{
  return m_nPE;
}

inline void JM::CalibTtChannel::setNPE(float value) 
{
  m_nPE = value;
}

inline float JM::CalibTtChannel::time() const 
{
  return m_time;
}

inline void JM::CalibTtChannel::setTime(float value) 
{
  m_time = value;
}

inline unsigned int JM::CalibTtChannel::pmtId() const 
{
  return m_pmtId;
}

inline void JM::CalibTtChannel::setPmtId(unsigned int value) 
{
  m_pmtId = value;
}


#endif ///SNIPER_CALIBEVENT_CALIBTTCHANNEL_H
