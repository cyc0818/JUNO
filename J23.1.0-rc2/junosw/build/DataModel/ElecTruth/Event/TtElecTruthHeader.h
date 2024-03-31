
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

#ifndef SNIPER_ELECTRUTH_TTELECTRUTHHEADER_H
#define SNIPER_ELECTRUTH_TTELECTRUTHHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/TtElecTruthEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtElecTruthHeader TtElecTruthHeader.h
   *
   * Header for Elec Truth info 
   *
   * @author Tao Lin
   * created Sun Sep 24 17:52:25 2023
   * 
   */

  class TtElecTruthHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the TT Truth
  

  protected:


  public:

    /// Default Constructor
    TtElecTruthHeader() {}
  
    /// Default Destructor
    virtual ~TtElecTruthHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the TT Truth
    JM::TtElecTruthEvt* event();
  
    /// Update referenced 
    /// SmartRef to the TT Truth
    void setEvent(JM::TtElecTruthEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(TtElecTruthHeader,1)
  

  }; // class TtElecTruthHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtElecTruthEvt* JM::TtElecTruthHeader::event() 
{
  return (JM::TtElecTruthEvt*)m_event.GetObject();
}

inline void JM::TtElecTruthHeader::setEvent(JM::TtElecTruthEvt* value) 
{
  m_event = value;
}

inline void JM::TtElecTruthHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::TtElecTruthEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::TtElecTruthHeader::event(const std::string& eventName)
{
  if (eventName == "JM::TtElecTruthEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::TtElecTruthHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECTRUTH_TTELECTRUTHHEADER_H
