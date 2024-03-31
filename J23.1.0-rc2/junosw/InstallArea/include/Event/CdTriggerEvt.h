
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

#ifndef SNIPER_ELECEVENT_CDTRIGGEREVT_H
#define SNIPER_ELECEVENT_CDTRIGGEREVT_H

// Include files
#include "Event/EventObject.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdTriggerEvt CdTriggerEvt.h
   *
   *  
   *
   * @author miao yu
   * created Sun Sep 24 17:52:17 2023
   * 
   */

  class CdTriggerEvt: public EventObject
  {
  private:

    std::vector<std::string> m_triggerType; // trigger type
    std::vector<TimeStamp>   m_triggerTime; // trigger time
    std::vector<int>         m_nFiredPmt;   // trigger fired pmt number
    std::vector<int>         m_volumeId;    // triggerd volume id
  

  protected:


  public:

    /// Default Constructor
    CdTriggerEvt() : m_triggerType(),
                     m_triggerTime(),
                     m_nFiredPmt(),
                     m_volumeId() {}
  
    /// Default Destructor
    virtual ~CdTriggerEvt() {}
  
    /// Retrieve const  
    /// trigger type
    const std::vector<std::string>& triggerType() const;
  
    /// Update  
    /// trigger type
    void setTriggerType(const std::vector<std::string>& value);
  
    /// Retrieve const  
    /// trigger time
    const std::vector<TimeStamp>& triggerTime() const;
  
    /// Update  
    /// trigger time
    void setTriggerTime(const std::vector<TimeStamp>& value);
  
    /// Retrieve const  
    /// trigger fired pmt number
    const std::vector<int>& nFiredPmt() const;
  
    /// Update  
    /// trigger fired pmt number
    void setNFiredPmt(const std::vector<int>& value);
  
    /// Retrieve const  
    /// triggerd volume id
    const std::vector<int>& volumeId() const;
  
    /// Update  
    /// triggerd volume id
    void setVolumeId(const std::vector<int>& value);
  
    ClassDef(CdTriggerEvt,1)
  

  }; // class CdTriggerEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<std::string>& JM::CdTriggerEvt::triggerType() const 
{
  return m_triggerType;
}

inline void JM::CdTriggerEvt::setTriggerType(const std::vector<std::string>& value) 
{
  m_triggerType = value;
}

inline const std::vector<TimeStamp>& JM::CdTriggerEvt::triggerTime() const 
{
  return m_triggerTime;
}

inline void JM::CdTriggerEvt::setTriggerTime(const std::vector<TimeStamp>& value) 
{
  m_triggerTime = value;
}

inline const std::vector<int>& JM::CdTriggerEvt::nFiredPmt() const 
{
  return m_nFiredPmt;
}

inline void JM::CdTriggerEvt::setNFiredPmt(const std::vector<int>& value) 
{
  m_nFiredPmt = value;
}

inline const std::vector<int>& JM::CdTriggerEvt::volumeId() const 
{
  return m_volumeId;
}

inline void JM::CdTriggerEvt::setVolumeId(const std::vector<int>& value) 
{
  m_volumeId = value;
}


#endif ///SNIPER_ELECEVENT_CDTRIGGEREVT_H
