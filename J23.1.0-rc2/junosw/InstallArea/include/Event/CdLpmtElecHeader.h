
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

#ifndef SNIPER_ELECEVENT_CDLPMTELECHEADER_H
#define SNIPER_ELECEVENT_CDLPMTELECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdLpmtElecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdLpmtElecHeader CdLpmtElecHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:03 2023
   * 
   */

  class CdLpmtElecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the CD LPMT Elec Event
  

  protected:


  public:

    /// Default Constructor
    CdLpmtElecHeader() {}
  
    /// Default Destructor
    virtual ~CdLpmtElecHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the CD LPMT Elec Event
    JM::CdLpmtElecEvt* event();
  
    /// Update referenced 
    /// SmartRef to the CD LPMT Elec Event
    void setEvent(JM::CdLpmtElecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdLpmtElecHeader,1)
  

  }; // class CdLpmtElecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdLpmtElecEvt* JM::CdLpmtElecHeader::event() 
{
  return (JM::CdLpmtElecEvt*)m_event.GetObject();
}

inline void JM::CdLpmtElecHeader::setEvent(JM::CdLpmtElecEvt* value) 
{
  m_event = value;
}

inline void JM::CdLpmtElecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdLpmtElecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdLpmtElecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdLpmtElecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdLpmtElecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_CDLPMTELECHEADER_H