
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

#ifndef SNIPER_ELECEVENT_CDSPMTELECHEADER_H
#define SNIPER_ELECEVENT_CDSPMTELECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdSpmtElecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdSpmtElecHeader CdSpmtElecHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:04 2023
   * 
   */

  class CdSpmtElecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the Elec Event
  

  protected:


  public:

    /// Default Constructor
    CdSpmtElecHeader() {}
  
    /// Default Destructor
    virtual ~CdSpmtElecHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the Elec Event
    JM::CdSpmtElecEvt* event();
  
    /// Update referenced 
    /// SmartRef to the Elec Event
    void setEvent(JM::CdSpmtElecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdSpmtElecHeader,1)
  

  }; // class CdSpmtElecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdSpmtElecEvt* JM::CdSpmtElecHeader::event() 
{
  return (JM::CdSpmtElecEvt*)m_event.GetObject();
}

inline void JM::CdSpmtElecHeader::setEvent(JM::CdSpmtElecEvt* value) 
{
  m_event = value;
}

inline void JM::CdSpmtElecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdSpmtElecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdSpmtElecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdSpmtElecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdSpmtElecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_CDSPMTELECHEADER_H
