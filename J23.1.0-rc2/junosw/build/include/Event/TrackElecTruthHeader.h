
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

#ifndef SNIPER_ELECTRUTH_TRACKELECTRUTHHEADER_H
#define SNIPER_ELECTRUTH_TRACKELECTRUTHHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TrackElecTruthEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TrackElecTruthHeader TrackElecTruthHeader.h
   *
   * Header for Elec Track Truth info 
   *
   * @author Miao Yu
   * created Sun Sep 24 17:52:26 2023
   * 
   */

  class TrackElecTruthHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the Track Truth
  

  protected:


  public:

    /// Default Constructor
    TrackElecTruthHeader() {}
  
    /// Default Destructor
    virtual ~TrackElecTruthHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the Track Truth
    JM::TrackElecTruthEvt* event();
  
    /// Update referenced 
    /// SmartRef to the Track Truth
    void setEvent(JM::TrackElecTruthEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TrackElecTruthHeader,1)
  

  }; // class TrackElecTruthHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TrackElecTruthEvt* JM::TrackElecTruthHeader::event() 
{
  return (JM::TrackElecTruthEvt*)m_event.GetObject();
}

inline void JM::TrackElecTruthHeader::setEvent(JM::TrackElecTruthEvt* value) 
{
  m_event = value;
}

inline void JM::TrackElecTruthHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TrackElecTruthEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TrackElecTruthHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TrackElecTruthEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TrackElecTruthHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECTRUTH_TRACKELECTRUTHHEADER_H
