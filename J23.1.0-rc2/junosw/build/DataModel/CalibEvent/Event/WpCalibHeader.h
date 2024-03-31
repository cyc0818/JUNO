
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

#ifndef SNIPER_CALIBEVENT_WPCALIBHEADER_H
#define SNIPER_CALIBEVENT_WPCALIBHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpCalibEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpCalibHeader WpCalibHeader.h
   *
   * Calibration header for WP 
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:39 2023
   * 
   */

  class WpCalibHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the WpCalibEvt
  

  protected:


  public:

    /// Default Constructor
    WpCalibHeader() {}
  
    /// Default Destructor
    virtual ~WpCalibHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the WpCalibEvt
    JM::WpCalibEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the WpCalibEvt
    void setEvent(JM::WpCalibEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpCalibHeader,1)
  

  }; // class WpCalibHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpCalibEvt* JM::WpCalibHeader::event() 
{
  return (JM::WpCalibEvt*)m_event.GetObject();
}

inline void JM::WpCalibHeader::setEvent(JM::WpCalibEvt* value) 
{
  m_event = value;
}

inline void JM::WpCalibHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpCalibEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpCalibHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpCalibEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpCalibHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_CALIBEVENT_WPCALIBHEADER_H
