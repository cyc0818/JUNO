
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

#ifndef SNIPER_BASEEVENT_EVENTOBJECT_H
#define SNIPER_BASEEVENT_EVENTOBJECT_H

// Include files
#include "TObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class EventObject EventObject.h
   *
   * Base class for event object 
   *
   * @author LI Teng
   * created Sun Sep 24 17:51:59 2023
   * 
   */

  class EventObject: public TObject
  {
  private:


  protected:

    unsigned int m_RefCount; //! reference count
  

  public:

    /// Default Constructor
    EventObject() : m_RefCount(0) {}
  
    /// Default Destructor
    virtual ~EventObject() {}
  
    /// Add the reference count by one
    unsigned int AddRef();
  
    /// Minus the reference count by one
    unsigned int DecRef();
  
    ClassDef(EventObject,1)
  

  }; // class EventObject

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations


#endif ///SNIPER_BASEEVENT_EVENTOBJECT_H
