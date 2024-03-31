
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

#ifndef SNIPER_ELECEVENT_CDTRIGGERHEADER_H
#define SNIPER_ELECEVENT_CDTRIGGERHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdTriggerEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdTriggerHeader CdTriggerHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:16 2023
   * 
   */

  class CdTriggerHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the CD Trigger Event
  

  protected:


  public:

    /// Default Constructor
    CdTriggerHeader() {}
  
    /// Default Destructor
    virtual ~CdTriggerHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the CD Trigger Event
    JM::CdTriggerEvt* event();
  
    /// Update referenced 
    /// SmartRef to the CD Trigger Event
    void setEvent(JM::CdTriggerEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdTriggerHeader,1)
  

  }; // class CdTriggerHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdTriggerEvt* JM::CdTriggerHeader::event() 
{
  return (JM::CdTriggerEvt*)m_event.GetObject();
}

inline void JM::CdTriggerHeader::setEvent(JM::CdTriggerEvt* value) 
{
  m_event = value;
}

inline void JM::CdTriggerHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdTriggerEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdTriggerHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdTriggerEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdTriggerHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_CDTRIGGERHEADER_H
