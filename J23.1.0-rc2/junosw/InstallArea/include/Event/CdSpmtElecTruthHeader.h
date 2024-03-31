
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

#ifndef SNIPER_ELECTRUTH_CDSPMTELECTRUTHHEADER_H
#define SNIPER_ELECTRUTH_CDSPMTELECTRUTHHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdSpmtElecTruthEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdSpmtElecTruthHeader CdSpmtElecTruthHeader.h
   *
   * Header for Elec Truth info 
   *
   * @author Tao Lin
   * created Sun Sep 24 17:52:24 2023
   * 
   */

  class CdSpmtElecTruthHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the SPMT Truth
  

  protected:


  public:

    /// Default Constructor
    CdSpmtElecTruthHeader() {}
  
    /// Default Destructor
    virtual ~CdSpmtElecTruthHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the SPMT Truth
    JM::CdSpmtElecTruthEvt* event();
  
    /// Update referenced 
    /// SmartRef to the SPMT Truth
    void setEvent(JM::CdSpmtElecTruthEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdSpmtElecTruthHeader,1)
  

  }; // class CdSpmtElecTruthHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdSpmtElecTruthEvt* JM::CdSpmtElecTruthHeader::event() 
{
  return (JM::CdSpmtElecTruthEvt*)m_event.GetObject();
}

inline void JM::CdSpmtElecTruthHeader::setEvent(JM::CdSpmtElecTruthEvt* value) 
{
  m_event = value;
}

inline void JM::CdSpmtElecTruthHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdSpmtElecTruthEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdSpmtElecTruthHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdSpmtElecTruthEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdSpmtElecTruthHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECTRUTH_CDSPMTELECTRUTHHEADER_H
