
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

#ifndef SNIPER_TESTEVENT_ATESTEVENTB_H
#define SNIPER_TESTEVENT_ATESTEVENTB_H

// Include files
#include "Event/EventObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class ATestEventB ATestEventB.h
   *
   * ATestB Event Class 
   *
   * @author LI Teng
   * created Sun Sep 24 17:52:57 2023
   * 
   */

  class ATestEventB: public EventObject
  {
  private:

    int m_id; // 
  

  protected:


  public:

    /// Default Constructor
    ATestEventB() : m_id(0) {}
  
    /// Default Destructor
    virtual ~ATestEventB() {}
  
    /// Retrieve const  
    /// 
    int id() const;
  
    /// Update  
    /// 
    void setId(int value);
  
    ClassDef(ATestEventB,1)
  

  }; // class ATestEventB

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline int JM::ATestEventB::id() const 
{
  return m_id;
}

inline void JM::ATestEventB::setId(int value) 
{
  m_id = value;
}


#endif ///SNIPER_TESTEVENT_ATESTEVENTB_H
