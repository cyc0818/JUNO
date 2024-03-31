
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

#ifndef SNIPER_ELECEVENT_WPWAVEFORMHEADER_H
#define SNIPER_ELECEVENT_WPWAVEFORMHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpWaveformEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpWaveformHeader WpWaveformHeader.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:06 2023
   * 
   */

  class WpWaveformHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the WP Waveform Event
  

  protected:


  public:

    /// Default Constructor
    WpWaveformHeader() {}
  
    /// Default Destructor
    virtual ~WpWaveformHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the WP Waveform Event
    JM::WpWaveformEvt* event();
  
    /// Update referenced 
    /// SmartRef to the WP Waveform Event
    void setEvent(JM::WpWaveformEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpWaveformHeader,1)
  

  }; // class WpWaveformHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpWaveformEvt* JM::WpWaveformHeader::event() 
{
  return (JM::WpWaveformEvt*)m_event.GetObject();
}

inline void JM::WpWaveformHeader::setEvent(JM::WpWaveformEvt* value) 
{
  m_event = value;
}

inline void JM::WpWaveformHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpWaveformEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpWaveformHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpWaveformEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpWaveformHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECEVENT_WPWAVEFORMHEADER_H
