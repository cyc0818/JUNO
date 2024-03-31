
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

#ifndef SNIPER_TESTEVENT_TESTHEADERB_H
#define SNIPER_TESTEVENT_TESTHEADERB_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/BTestEventA.h"
#include "Event/BTestEventB.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TestHeaderB TestHeaderB.h
   *
   * Test HeaderB Class 
   *
   * @author LI Teng
   * created Sun Sep 24 17:53:01 2023
   * 
   */

  class TestHeaderB: public HeaderObject
  {
  private:

    JM::SmartRef m_eventA; // ||SmartRef to the BTestEventA
    JM::SmartRef m_eventB; // ||SmartRef to the BTestEventB
  

  protected:


  public:

    /// Default Constructor
    TestHeaderB() {}
  
    /// Default Destructor
    virtual ~TestHeaderB() {}
  
    /// Retrieve referenced 
    /// SmartRef to the BTestEventA
    JM::BTestEventA* eventA();
  
    /// Update referenced 
    /// SmartRef to the BTestEventA
    void setEventA(JM::BTestEventA* value);
  
    /// Retrieve referenced 
    /// SmartRef to the BTestEventB
    JM::BTestEventB* eventB();
  
    /// Update referenced 
    /// SmartRef to the BTestEventB
    void setEventB(JM::BTestEventB* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if eventA exists
    bool hasEventA();
  
    //Check if eventB exists
    bool hasEventB();
  
    ClassDef(TestHeaderB,1)
  

  }; // class TestHeaderB

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::BTestEventA* JM::TestHeaderB::eventA() 
{
  return (JM::BTestEventA*)m_eventA.GetObject();
}

inline void JM::TestHeaderB::setEventA(JM::BTestEventA* value) 
{
  m_eventA = value;
}

inline JM::BTestEventB* JM::TestHeaderB::eventB() 
{
  return (JM::BTestEventB*)m_eventB.GetObject();
}

inline void JM::TestHeaderB::setEventB(JM::BTestEventB* value) 
{
  m_eventB = value;
}

inline void JM::TestHeaderB::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::BTestEventA") { 
    m_eventA.setEntry(value);
  }
  if (eventName == "JM::BTestEventB") { 
    m_eventB.setEntry(value);
  }
}

inline JM::EventObject* JM::TestHeaderB::event(const std::string& eventName)
{
  if (eventName == "JM::BTestEventA") { 
    return m_eventA.GetObject();
  }
  if (eventName == "JM::BTestEventB") { 
    return m_eventB.GetObject();
  }
  return 0; 
}

inline bool JM::TestHeaderB::hasEventA()
{
  return m_eventA.HasObject();
}


inline bool JM::TestHeaderB::hasEventB()
{
  return m_eventB.HasObject();
}



#endif ///SNIPER_TESTEVENT_TESTHEADERB_H
