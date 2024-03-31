
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

#ifndef SNIPER_TESTEVENT_BTESTEVENTB_H
#define SNIPER_TESTEVENT_BTESTEVENTB_H

// Include files
#include "Event/EventObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class BTestEventB BTestEventB.h
   *
   * BTestB Event Class 
   *
   * @author LI Teng
   * created Sun Sep 24 17:52:59 2023
   * 
   */

  class BTestEventB: public EventObject
  {
  private:

    int m_id; // 
  

  protected:


  public:

    /// Default Constructor
    BTestEventB() : m_id(0) {}
  
    /// Default Destructor
    virtual ~BTestEventB() {}
  
    /// Retrieve const  
    /// 
    int id() const;
  
    /// Update  
    /// 
    void setId(int value);
  
    ClassDef(BTestEventB,1)
  

  }; // class BTestEventB

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline int JM::BTestEventB::id() const 
{
  return m_id;
}

inline void JM::BTestEventB::setId(int value) 
{
  m_id = value;
}


#endif ///SNIPER_TESTEVENT_BTESTEVENTB_H
