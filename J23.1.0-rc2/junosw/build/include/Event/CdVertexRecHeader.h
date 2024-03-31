
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

#ifndef SNIPER_RECEVENT_CDVERTEXRECHEADER_H
#define SNIPER_RECEVENT_CDVERTEXRECHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdVertexRecEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdVertexRecHeader CdVertexRecHeader.h
   *
   * Reconstruction header class for point-like event 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:52 2023
   * 
   */

  class CdVertexRecHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||Smart pointer to the CdRecEvt
  

  protected:


  public:

    /// Default Constructor
    CdVertexRecHeader() {}
  
    /// Default Destructor
    virtual ~CdVertexRecHeader() {}
  
    /// Retrieve referenced 
    /// Smart pointer to the CdRecEvt
    JM::CdVertexRecEvt* event();
  
    /// Update referenced 
    /// Smart pointer to the CdRecEvt
    void setEvent(JM::CdVertexRecEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdVertexRecHeader,1)
  

  }; // class CdVertexRecHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdVertexRecEvt* JM::CdVertexRecHeader::event() 
{
  return (JM::CdVertexRecEvt*)m_event.GetObject();
}

inline void JM::CdVertexRecHeader::setEvent(JM::CdVertexRecEvt* value) 
{
  m_event = value;
}

inline void JM::CdVertexRecHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdVertexRecEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdVertexRecHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdVertexRecEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdVertexRecHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_RECEVENT_CDVERTEXRECHEADER_H
