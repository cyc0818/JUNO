
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

#ifndef SNIPER_TESTEVENT_TESTHEADERA_H
#define SNIPER_TESTEVENT_TESTHEADERA_H

// Include files
#include "Event/HeaderObject.h"
#include "Event/ATestEventA.h"
#include "Event/ATestEventB.h"
#include "EDMUtil/SmartRef.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TestHeaderA TestHeaderA.h
   *
   * Test HeaderA Class 
   *
   * @author LI Teng
   * created Sun Sep 24 17:53:00 2023
   * 
   */

  class TestHeaderA: public HeaderObject
  {
  private:

    JM::SmartRef m_eventA; // ||SmartRef to the ATestEventA
    JM::SmartRef m_eventB; // ||SmartRef to the ATestEventB
  

  protected:


  public:

    /// Default Constructor
    TestHeaderA() {}
  
    /// Default Destructor
    virtual ~TestHeaderA() {}
  
    /// Retrieve referenced 
    /// SmartRef to the ATestEventA
    JM::ATestEventA* eventA();
  
    /// Update referenced 
    /// SmartRef to the ATestEventA
    void setEventA(JM::ATestEventA* value);
  
    /// Retrieve referenced 
    /// SmartRef to the ATestEventB
    JM::ATestEventB* eventB();
  
    /// Update referenced 
    /// SmartRef to the ATestEventB
    void setEventB(JM::ATestEventB* value);
  
    /// Set entry number of events
    void setEventEntry(const std::string& eventName, Long64_t& value);
  
    /// Get event
    JM::EventObject* event(const std::string& eventName);
  
    //Check if eventA exists
    bool hasEventA();
  
    //Check if eventB exists
    bool hasEventB();
  
    ClassDef(TestHeaderA,1)
  

  }; // class TestHeaderA

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::ATestEventA* JM::TestHeaderA::eventA() 
{
  return (JM::ATestEventA*)m_eventA.GetObject();
}

inline void JM::TestHeaderA::setEventA(JM::ATestEventA* value) 
{
  m_eventA = value;
}

inline JM::ATestEventB* JM::TestHeaderA::eventB() 
{
  return (JM::ATestEventB*)m_eventB.GetObject();
}

inline void JM::TestHeaderA::setEventB(JM::ATestEventB* value) 
{
  m_eventB = value;
}

inline void JM::TestHeaderA::setEventEntry(const std::string& eventName, Long64_t& value)
{
  if (eventName == "JM::ATestEventA") { 
    m_eventA.setEntry(value);
  }
  if (eventName == "JM::ATestEventB") { 
    m_eventB.setEntry(value);
  }
}

inline JM::EventObject* JM::TestHeaderA::event(const std::string& eventName)
{
  if (eventName == "JM::ATestEventA") { 
    return m_eventA.GetObject();
  }
  if (eventName == "JM::ATestEventB") { 
    return m_eventB.GetObject();
  }
  return 0; 
}

inline bool JM::TestHeaderA::hasEventA()
{
  return m_eventA.HasObject();
}


inline bool JM::TestHeaderA::hasEventB()
{
  return m_eventB.HasObject();
}



#endif ///SNIPER_TESTEVENT_TESTHEADERA_H
