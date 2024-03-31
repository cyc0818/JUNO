
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

#ifndef SNIPER_ELECEVENT_WPELECHEADER_H
#define SNIPER_ELECEVENT_WPELECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpElecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpElecHeader WpElecHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:05 2023
   * 
   */

  class WpElecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the WP Elec Event
  

  protected:


  public:

    /// Default Constructor
    WpElecHeader() {}
  
    /// Default Destructor
    virtual ~WpElecHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the WP Elec Event
    JM::WpElecEvt* event();
  
    /// Update referenced 
    /// SmartRef to the WP Elec Event
    void setEvent(JM::WpElecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpElecHeader,1)
  

  }; // class WpElecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpElecEvt* JM::WpElecHeader::event() 
{
  return (JM::WpElecEvt*)m_event.GetObject();
}

inline void JM::WpElecHeader::setEvent(JM::WpElecEvt* value) 
{
  m_event = value;
}

inline void JM::WpElecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpElecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpElecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpElecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpElecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_WPELECHEADER_H
