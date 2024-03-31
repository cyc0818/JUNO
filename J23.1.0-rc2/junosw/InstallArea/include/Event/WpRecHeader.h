
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

#ifndef SNIPER_RECEVENT_WPRECHEADER_H
#define SNIPER_RECEVENT_WPRECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpRecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpRecHeader WpRecHeader.h
   *
   * Reconstruction Header Class 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:53 2023
   * 
   */

  class WpRecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the WpRecEvt (WP)
  

  protected:


  public:

    /// Default Constructor
    WpRecHeader() {}
  
    /// Default Destructor
    virtual ~WpRecHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the WpRecEvt (WP)
    JM::WpRecEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the WpRecEvt (WP)
    void setEvent(JM::WpRecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpRecHeader,1)
  

  }; // class WpRecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpRecEvt* JM::WpRecHeader::event() 
{
  return (JM::WpRecEvt*)m_event.GetObject();
}

inline void JM::WpRecHeader::setEvent(JM::WpRecEvt* value) 
{
  m_event = value;
}

inline void JM::WpRecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpRecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpRecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpRecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpRecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_RECEVENT_WPRECHEADER_H
