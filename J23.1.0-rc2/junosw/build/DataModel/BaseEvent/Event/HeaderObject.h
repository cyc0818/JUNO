
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

#ifndef SNIPER_BASEEVENT_HEADEROBJECT_H
#define SNIPER_BASEEVENT_HEADEROBJECT_H

// Include files
#include "EventObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class HeaderObject HeaderObject.h
   *
   * Base class for event header 
   *
   * @author LI Teng
   * created Sun Sep 24 17:52:00 2023
   * 
   */

  class HeaderObject: public EventObject
  {
  private:


  protected:


  public:

    /// Default Constructor
    HeaderObject() {}
  
    /// Default Destructor
    virtual ~HeaderObject() {}
  
    /// Set the entry number of the event object
    virtual void setEventEntry(const std::string& eventName,
                               Long64_t& value) = 0;
  
    /// Get the event object of header
    virtual EventObject* event(const std::string& value) = 0;
  
    ClassDef(HeaderObject,2)
  

  }; // class HeaderObject

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations


#endif ///SNIPER_BASEEVENT_HEADEROBJECT_H
