#ifndef SMART_REF_H
#define SMART_REF_H 0

#include "TObject.h"

class TProcessID;

namespace JM 
{
  class EventObject;
  class SmartRef;

  bool operator==(const SmartRef& r1, const SmartRef& r2);
  bool operator!=(const SmartRef& r1, const SmartRef& r2);

  class SmartRef: public TObject
  {
  private:

    Long64_t      m_entry;       //  Entry number of the referenced object
    EventObject*  m_refObjTemp;  //! Transient pointer to the referenced object
    TProcessID*   m_pid;         //! Transient pointer to TProcessID when SmartRef was written 
    bool          m_isLoaded;    //! To mark whether this SmartRef instance is loaded from file
  

  protected:


  public:
  // Default Constructor
  SmartRef();
  
  // Copy Constructor
  SmartRef(const SmartRef&);

  // Destructor
  virtual ~SmartRef();
  
  // Set the referenced object
  void operator=(EventObject* value);

  // Assignment operator
  SmartRef& operator=(const SmartRef&);

  // Get the referenced object
  EventObject* operator->();

  // Get the referenced object
  EventObject& operator*();

  // Clear SmartRef
  void clear();
  
  // Set the referenced object
  void SetObject(EventObject* value);
  
  // Get the refernced object
  EventObject* GetObject();

  // Return whether SmartRef has a referenced object
  bool HasObject();

  // Get the TProcessID of the SmartRef
  TProcessID* GetPID() const { return m_pid; }
  
  // Get the entry number of the referenced object
  const Long64_t& entry() const { return m_entry; }
  
  // Set the entry number of the referenced object
  void setEntry(const Long64_t& value) { m_entry = value; }

  friend bool operator==(const SmartRef& r1, const SmartRef& r2);
  friend bool operator!=(const SmartRef& r1, const SmartRef& r2);
  
  ClassDef(SmartRef,2)
  

  }; // class SmartRef

} // namespace JM;


#endif ///ROOTIOUTIL_SMARTREF_H
