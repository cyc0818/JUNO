
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

#ifndef SNIPER_CALIBEVENT_TTCALIBEVT_H
#define SNIPER_CALIBEVENT_TTCALIBEVT_H

// Include files
#include "Event/EventObject.h"
#include "CalibTtChannel.h"
#include <list>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtCalibEvt TtCalibEvt.h
   *
   * Calibration event class 
   *
   * @author A.Meregaglia, J.P.A.M. de Andre
   * created Sun Sep 24 17:52:43 2023
   * 
   */

  class TtCalibEvt: public EventObject
  {
  private:

    std::list<JM::CalibTtChannel*> m_calibTtCol; // Collection of CalibTtChannel in a evt
  

  protected:


  public:

    /// Default Constructor
    TtCalibEvt() : m_calibTtCol() {}
  
  /// destructor TtCalibEvt.
  ~TtCalibEvt();
  
    /// Get a TT channel via chid
    const JM::CalibTtChannel* getFirstCalibTtChannel(unsigned int chid) const;
  
    /// Add a Calibrate Tt channel to this event
    CalibTtChannel* addCalibTtChannel(unsigned int chid);
  
    /// Retrieve const  
    /// Collection of CalibTtChannel in a evt
    const std::list<JM::CalibTtChannel*>& calibTtCol() const;
  
    /// Update  
    /// Collection of CalibTtChannel in a evt
    void setCalibTtCol(const std::list<JM::CalibTtChannel*>& value);
  
    ClassDef(TtCalibEvt,2)
  

  }; // class TtCalibEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtCalibEvt::~TtCalibEvt() 
{

  for (std::list<JM::CalibTtChannel*>::iterator it = m_calibTtCol.begin();
      it != m_calibTtCol.end(); ++it) {
    if (*it) {
      delete (*it);
    }
  }
  m_calibTtCol.clear();
                
}

inline const std::list<JM::CalibTtChannel*>& JM::TtCalibEvt::calibTtCol() const 
{
  return m_calibTtCol;
}

inline void JM::TtCalibEvt::setCalibTtCol(const std::list<JM::CalibTtChannel*>& value) 
{
  m_calibTtCol = value;
}

inline const JM::CalibTtChannel* JM::TtCalibEvt::getFirstCalibTtChannel(unsigned int chid) const 
{

  std::list<JM::CalibTtChannel*>::const_iterator cpIter;
  std::list<JM::CalibTtChannel*>::const_iterator cpFound = m_calibTtCol.end();
  cpIter = m_calibTtCol.begin();
  while(cpIter!=m_calibTtCol.end())
  {
    if((*cpIter)->pmtId()==chid){
      if(cpFound == m_calibTtCol.end()){
        cpFound = cpIter;
      }
      else{
        if((*cpFound)->time() > (*cpIter)->time()){
          cpFound = cpIter;
        }
      }
    }
    cpIter++;
  }
  if(cpFound==m_calibTtCol.end()) return 0;
  return *cpFound;
                
}

inline JM::CalibTtChannel* JM::TtCalibEvt::addCalibTtChannel(unsigned int chid) 
{

  JM::CalibTtChannel* cp = new JM::CalibTtChannel(chid);
  m_calibTtCol.push_back(cp);
  return cp;
                
}


#endif ///SNIPER_CALIBEVENT_TTCALIBEVT_H
