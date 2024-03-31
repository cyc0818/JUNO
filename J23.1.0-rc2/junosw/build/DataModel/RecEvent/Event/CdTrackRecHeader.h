
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

#ifndef SNIPER_RECEVENT_CDTRACKRECHEADER_H
#define SNIPER_RECEVENT_CDTRACKRECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdTrackRecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdTrackRecHeader CdTrackRecHeader.h
   *
   * Reconstruction header class for track-like event 
   *
   * @author Teng
   * created Sun Sep 24 17:52:52 2023
   * 
   */

  class CdTrackRecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the CdRecTrackEvt
  

  protected:


  public:

    /// Default Constructor
    CdTrackRecHeader() {}
  
    /// Default Destructor
    virtual ~CdTrackRecHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the CdRecTrackEvt
    JM::CdTrackRecEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the CdRecTrackEvt
    void setEvent(JM::CdTrackRecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdTrackRecHeader,1)
  

  }; // class CdTrackRecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdTrackRecEvt* JM::CdTrackRecHeader::event() 
{
  return (JM::CdTrackRecEvt*)m_event.GetObject();
}

inline void JM::CdTrackRecHeader::setEvent(JM::CdTrackRecEvt* value) 
{
  m_event = value;
}

inline void JM::CdTrackRecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdTrackRecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdTrackRecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdTrackRecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdTrackRecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_RECEVENT_CDTRACKRECHEADER_H
