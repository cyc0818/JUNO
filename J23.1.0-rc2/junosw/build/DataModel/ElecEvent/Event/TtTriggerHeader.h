
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

#ifndef SNIPER_ELECEVENT_TTTRIGGERHEADER_H
#define SNIPER_ELECEVENT_TTTRIGGERHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TtTriggerEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtTriggerHeader TtTriggerHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:19 2023
   * 
   */

  class TtTriggerHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the TT Trigger Event
  

  protected:


  public:

    /// Default Constructor
    TtTriggerHeader() {}
  
    /// Default Destructor
    virtual ~TtTriggerHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the TT Trigger Event
    JM::TtTriggerEvt* event();
  
    /// Update referenced 
    /// SmartRef to the TT Trigger Event
    void setEvent(JM::TtTriggerEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TtTriggerHeader,1)
  

  }; // class TtTriggerHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtTriggerEvt* JM::TtTriggerHeader::event() 
{
  return (JM::TtTriggerEvt*)m_event.GetObject();
}

inline void JM::TtTriggerHeader::setEvent(JM::TtTriggerEvt* value) 
{
  m_event = value;
}

inline void JM::TtTriggerHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TtTriggerEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TtTriggerHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TtTriggerEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TtTriggerHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_TTTRIGGERHEADER_H
