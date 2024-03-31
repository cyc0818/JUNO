
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

#ifndef SNIPER_ELECEVENT_CDWAVEFORMHEADER_H
#define SNIPER_ELECEVENT_CDWAVEFORMHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdWaveformEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdWaveformHeader CdWaveformHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:05 2023
   * 
   */

  class CdWaveformHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the CD Waveform Event
  

  protected:


  public:

    /// Default Constructor
    CdWaveformHeader() {}
  
    /// Default Destructor
    virtual ~CdWaveformHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the CD Waveform Event
    JM::CdWaveformEvt* event();
  
    /// Update referenced 
    /// SmartRef to the CD Waveform Event
    void setEvent(JM::CdWaveformEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdWaveformHeader,1)
  

  }; // class CdWaveformHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdWaveformEvt* JM::CdWaveformHeader::event() 
{
  return (JM::CdWaveformEvt*)m_event.GetObject();
}

inline void JM::CdWaveformHeader::setEvent(JM::CdWaveformEvt* value) 
{
  m_event = value;
}

inline void JM::CdWaveformHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdWaveformEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdWaveformHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdWaveformEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdWaveformHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_CDWAVEFORMHEADER_H
