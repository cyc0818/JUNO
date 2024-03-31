
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

#ifndef SNIPER_CALIBEVENT_TTCALIBHEADER_H
#define SNIPER_CALIBEVENT_TTCALIBHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TtCalibEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtCalibHeader TtCalibHeader.h
   *
   * Calibration header for TT 
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:40 2023
   * 
   */

  class TtCalibHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the TtCalibEvt
  

  protected:


  public:

    /// Default Constructor
    TtCalibHeader() {}
  
    /// Default Destructor
    virtual ~TtCalibHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the TtCalibEvt
    JM::TtCalibEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the TtCalibEvt
    void setEvent(JM::TtCalibEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TtCalibHeader,1)
  

  }; // class TtCalibHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtCalibEvt* JM::TtCalibHeader::event() 
{
  return (JM::TtCalibEvt*)m_event.GetObject();
}

inline void JM::TtCalibHeader::setEvent(JM::TtCalibEvt* value) 
{
  m_event = value;
}

inline void JM::TtCalibHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TtCalibEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TtCalibHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TtCalibEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TtCalibHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_CALIBEVENT_TTCALIBHEADER_H
