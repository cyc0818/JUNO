
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

#ifndef SNIPER_CALIBEVENT_CDSPMTCALIBHEADER_H
#define SNIPER_CALIBEVENT_CDSPMTCALIBHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdSpmtCalibEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdSpmtCalibHeader CdSpmtCalibHeader.h
   *
   * Calibration header for CD SPMT 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:38 2023
   * 
   */

  class CdSpmtCalibHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the CdSpmtCalibEvt
  

  protected:


  public:

    /// Default Constructor
    CdSpmtCalibHeader() {}
  
    /// Default Destructor
    virtual ~CdSpmtCalibHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the CdSpmtCalibEvt
    JM::CdSpmtCalibEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the CdSpmtCalibEvt
    void setEvent(JM::CdSpmtCalibEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdSpmtCalibHeader,1)
  

  }; // class CdSpmtCalibHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdSpmtCalibEvt* JM::CdSpmtCalibHeader::event() 
{
  return (JM::CdSpmtCalibEvt*)m_event.GetObject();
}

inline void JM::CdSpmtCalibHeader::setEvent(JM::CdSpmtCalibEvt* value) 
{
  m_event = value;
}

inline void JM::CdSpmtCalibHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdSpmtCalibEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdSpmtCalibHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdSpmtCalibEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdSpmtCalibHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_CALIBEVENT_CDSPMTCALIBHEADER_H
