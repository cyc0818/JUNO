
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

#ifndef SNIPER_CALIBEVENT_CDLPMTCALIBEVT_H
#define SNIPER_CALIBEVENT_CDLPMTCALIBEVT_H

// Include files
#include "Event/EventObject.h"
#include "CalibPmtChannel.h"
#include <list>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdLpmtCalibEvt CdLpmtCalibEvt.h
   *
   * Calibration event class 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:40 2023
   * 
   */

  class CdLpmtCalibEvt: public EventObject
  {
  private:

    std::list<JM::CalibPmtChannel*> m_calibPMTCol; // Collection of CalibPmtChannel in a evt
  

  protected:


  public:

    /// Default Constructor
    CdLpmtCalibEvt() : m_calibPMTCol() {}
  
  /// destructor for SimPMTHeader.
  ~CdLpmtCalibEvt();
  
    /// Get a pmt channel via pmtid
    const JM::CalibPmtChannel* getCalibPmtChannel(unsigned int pmtid) const;
  
    /// Add a Calibrate pmt channel to this event
    CalibPmtChannel* addCalibPmtChannel(unsigned int pmtid);
  
    /// Retrieve const  
    /// Collection of CalibPmtChannel in a evt
    const std::list<JM::CalibPmtChannel*>& calibPMTCol() const;
  
    /// Update  
    /// Collection of CalibPmtChannel in a evt
    void setCalibPMTCol(const std::list<JM::CalibPmtChannel*>& value);
  
    ClassDef(CdLpmtCalibEvt,1)
  

  }; // class CdLpmtCalibEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdLpmtCalibEvt::~CdLpmtCalibEvt() 
{

  for (std::list<JM::CalibPmtChannel*>::iterator it = m_calibPMTCol.begin();
      it != m_calibPMTCol.end(); ++it) {
    if (*it) {
      delete (*it);
    }
  }
  m_calibPMTCol.clear();
                
}

inline const std::list<JM::CalibPmtChannel*>& JM::CdLpmtCalibEvt::calibPMTCol() const 
{
  return m_calibPMTCol;
}

inline void JM::CdLpmtCalibEvt::setCalibPMTCol(const std::list<JM::CalibPmtChannel*>& value) 
{
  m_calibPMTCol = value;
}

inline const JM::CalibPmtChannel* JM::CdLpmtCalibEvt::getCalibPmtChannel(unsigned int pmtid) const 
{

  std::list<JM::CalibPmtChannel*>::const_iterator cpIter;
  cpIter = m_calibPMTCol.begin();
  while(cpIter!=m_calibPMTCol.end())
  {
    if((*cpIter)->pmtId()==pmtid) break;
    cpIter++;
  }
  if(cpIter==m_calibPMTCol.end()) return 0;
  return *cpIter;
                
}

inline JM::CalibPmtChannel* JM::CdLpmtCalibEvt::addCalibPmtChannel(unsigned int pmtid) 
{

  JM::CalibPmtChannel* cp = 0;
  if(!this->getCalibPmtChannel(pmtid)){
    cp = new JM::CalibPmtChannel(pmtid);
    m_calibPMTCol.push_back(cp);
  }
    return cp;
                
}


#endif ///SNIPER_CALIBEVENT_CDLPMTCALIBEVT_H
