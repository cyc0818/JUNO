
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

#ifndef SNIPER_ELECEVENT_WPTRIGGERHEADER_H
#define SNIPER_ELECEVENT_WPTRIGGERHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpTriggerEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpTriggerHeader WpTriggerHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:18 2023
   * 
   */

  class WpTriggerHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the WP Trigger Event
  

  protected:


  public:

    /// Default Constructor
    WpTriggerHeader() {}
  
    /// Default Destructor
    virtual ~WpTriggerHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the WP Trigger Event
    JM::WpTriggerEvt* event();
  
    /// Update referenced 
    /// SmartRef to the WP Trigger Event
    void setEvent(JM::WpTriggerEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpTriggerHeader,1)
  

  }; // class WpTriggerHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpTriggerEvt* JM::WpTriggerHeader::event() 
{
  return (JM::WpTriggerEvt*)m_event.GetObject();
}

inline void JM::WpTriggerHeader::setEvent(JM::WpTriggerEvt* value) 
{
  m_event = value;
}

inline void JM::WpTriggerHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpTriggerEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpTriggerHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpTriggerEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpTriggerHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_WPTRIGGERHEADER_H
