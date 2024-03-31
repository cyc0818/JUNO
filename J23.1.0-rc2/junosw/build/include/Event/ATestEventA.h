
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

#ifndef SNIPER_TESTEVENT_ATESTEVENTA_H
#define SNIPER_TESTEVENT_ATESTEVENTA_H

// Include files
#include "Event/EventObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class ATestEventA ATestEventA.h
   *
   * ATestA Event Class 
   *
   * @author LI Teng
   * created Sun Sep 24 17:52:57 2023
   * 
   */

  class ATestEventA: public EventObject
  {
  private:

    int m_id; // 
  

  protected:


  public:

    /// Default Constructor
    ATestEventA() : m_id(0) {}
  
    /// Default Destructor
    virtual ~ATestEventA() {}
  
    /// Retrieve const  
    /// 
    int id() const;
  
    /// Update  
    /// 
    void setId(int value);
  
    ClassDef(ATestEventA,1)
  

  }; // class ATestEventA

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline int JM::ATestEventA::id() const 
{
  return m_id;
}

inline void JM::ATestEventA::setId(int value) 
{
  m_id = value;
}


#endif ///SNIPER_TESTEVENT_ATESTEVENTA_H
