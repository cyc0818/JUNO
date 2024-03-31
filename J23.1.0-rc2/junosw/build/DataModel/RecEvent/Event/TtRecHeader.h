
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

#ifndef SNIPER_RECEVENT_TTRECHEADER_H
#define SNIPER_RECEVENT_TTRECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TtRecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtRecHeader TtRecHeader.h
   *
   * Reconstruction Header Class 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:54 2023
   * 
   */

  class TtRecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the TtRecEvt
  

  protected:


  public:

    /// Default Constructor
    TtRecHeader() {}
  
    /// Default Destructor
    virtual ~TtRecHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the TtRecEvt
    JM::TtRecEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the TtRecEvt
    void setEvent(JM::TtRecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TtRecHeader,1)
  

  }; // class TtRecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtRecEvt* JM::TtRecHeader::event() 
{
  return (JM::TtRecEvt*)m_event.GetObject();
}

inline void JM::TtRecHeader::setEvent(JM::TtRecEvt* value) 
{
  m_event = value;
}

inline void JM::TtRecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TtRecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TtRecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TtRecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TtRecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_RECEVENT_TTRECHEADER_H
