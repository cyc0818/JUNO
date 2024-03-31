
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

#ifndef SNIPER_ELECEVENT_TTELECHEADER_H
#define SNIPER_ELECEVENT_TTELECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TtElecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtElecHeader TtElecHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:07 2023
   * 
   */

  class TtElecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the TT Elec Event
  

  protected:


  public:

    /// Default Constructor
    TtElecHeader() {}
  
    /// Default Destructor
    virtual ~TtElecHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the TT Elec Event
    JM::TtElecEvt* event();
  
    /// Update referenced 
    /// SmartRef to the TT Elec Event
    void setEvent(JM::TtElecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TtElecHeader,1)
  

  }; // class TtElecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtElecEvt* JM::TtElecHeader::event() 
{
  return (JM::TtElecEvt*)m_event.GetObject();
}

inline void JM::TtElecHeader::setEvent(JM::TtElecEvt* value) 
{
  m_event = value;
}

inline void JM::TtElecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TtElecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TtElecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TtElecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TtElecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_TTELECHEADER_H
