
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

#ifndef SNIPER_CALIBEVENT_CDLPMTCALIBHEADER_H
#define SNIPER_CALIBEVENT_CDLPMTCALIBHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdLpmtCalibEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdLpmtCalibHeader CdLpmtCalibHeader.h
   *
   * Calibration header for CD LPMT 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:37 2023
   * 
   */

  class CdLpmtCalibHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the CD LPMT CalibEvent
  

  protected:


  public:

    /// Default Constructor
    CdLpmtCalibHeader() {}
  
    /// Default Destructor
    virtual ~CdLpmtCalibHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the CD LPMT CalibEvent
    JM::CdLpmtCalibEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the CD LPMT CalibEvent
    void setEvent(JM::CdLpmtCalibEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdLpmtCalibHeader,1)
  

  }; // class CdLpmtCalibHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdLpmtCalibEvt* JM::CdLpmtCalibHeader::event() 
{
  return (JM::CdLpmtCalibEvt*)m_event.GetObject();
}

inline void JM::CdLpmtCalibHeader::setEvent(JM::CdLpmtCalibEvt* value) 
{
  m_event = value;
}

inline void JM::CdLpmtCalibHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdLpmtCalibEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdLpmtCalibHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdLpmtCalibEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdLpmtCalibHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_CALIBEVENT_CDLPMTCALIBHEADER_H
