
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

#ifndef SNIPER_ELECTRUTH_CDLPMTELECTRUTHHEADER_H
#define SNIPER_ELECTRUTH_CDLPMTELECTRUTHHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/CdLpmtElecTruthEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdLpmtElecTruthHeader CdLpmtElecTruthHeader.h
   *
   * Header for Elec Truth info 
   *
   * @author Tao Lin
   * created Sun Sep 24 17:52:23 2023
   * 
   */

  class CdLpmtElecTruthHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the LPMT Truth
  

  protected:


  public:

    /// Default Constructor
    CdLpmtElecTruthHeader() {}
  
    /// Default Destructor
    virtual ~CdLpmtElecTruthHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the LPMT Truth
    JM::CdLpmtElecTruthEvt* event();
  
    /// Update referenced 
    /// SmartRef to the LPMT Truth
    void setEvent(JM::CdLpmtElecTruthEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(CdLpmtElecTruthHeader,1)
  

  }; // class CdLpmtElecTruthHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdLpmtElecTruthEvt* JM::CdLpmtElecTruthHeader::event() 
{
  return (JM::CdLpmtElecTruthEvt*)m_event.GetObject();
}

inline void JM::CdLpmtElecTruthHeader::setEvent(JM::CdLpmtElecTruthEvt* value) 
{
  m_event = value;
}

inline void JM::CdLpmtElecTruthHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::CdLpmtElecTruthEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::CdLpmtElecTruthHeader::event(const std::string& eventName)
{
  if (eventName == "JM::CdLpmtElecTruthEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::CdLpmtElecTruthHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECTRUTH_CDLPMTELECTRUTHHEADER_H
