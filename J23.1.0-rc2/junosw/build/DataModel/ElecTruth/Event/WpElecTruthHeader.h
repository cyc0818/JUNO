
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

#ifndef SNIPER_ELECTRUTH_WPELECTRUTHHEADER_H
#define SNIPER_ELECTRUTH_WPELECTRUTHHEADER_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/WpElecTruthEvt.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpElecTruthHeader WpElecTruthHeader.h
   *
   * Header for Elec Truth info 
   *
   * @author Tao Lin
   * created Sun Sep 24 17:52:24 2023
   * 
   */

  class WpElecTruthHeader: public HeaderObject
  {
  private:

    JM::SmartRef m_event; // ||SmartRef to the WP PMT Truth
  

  protected:


  public:

    /// Default Constructor
    WpElecTruthHeader() {}
  
    /// Default Destructor
    virtual ~WpElecTruthHeader() {}
  
    /// Retrieve referenced 
    /// SmartRef to the WP PMT Truth
    JM::WpElecTruthEvt* event();
  
    /// Update referenced 
    /// SmartRef to the WP PMT Truth
    void setEvent(JM::WpElecTruthEvt* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if event exists
    bool hasEvent();
  
    ClassDef(WpElecTruthHeader,1)
  

  }; // class WpElecTruthHeader

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpElecTruthEvt* JM::WpElecTruthHeader::event() 
{
  return (JM::WpElecTruthEvt*)m_event.GetObject();
}

inline void JM::WpElecTruthHeader::setEvent(JM::WpElecTruthEvt* value) 
{
  m_event = value;
}

inline void JM::WpElecTruthHeader::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::WpElecTruthEvt") { 
    m_event.setEntry(value);
  }
}

inline JM::EventObject* JM::WpElecTruthHeader::event(const std::string& eventName)
{
  if (eventName == "JM::WpElecTruthEvt") { 
    return m_event.GetObject();
  }
  return 0; 
}

inline bool JM::WpElecTruthHeader::hasEvent()
{
  return m_event.HasObject();
}



#endif ///SNIPER_ELECTRUTH_WPELECTRUTHHEADER_H
