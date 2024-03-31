
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

#ifndef SNIPER_ELECTRUTH_CDLPMTELECTRUTHEVT_H
#define SNIPER_ELECTRUTH_CDLPMTELECTRUTHEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/LpmtElecTruth.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdLpmtElecTruthEvt CdLpmtElecTruthEvt.h
   *
   * This data model is a container of LpmtElecTruth objects 
   *
   * @author Yan Zhang
   * created Sun Sep 24 17:52:27 2023
   * 
   */

  class CdLpmtElecTruthEvt: public EventObject
  {
  private:

    std::vector<JM::LpmtElecTruth> m_truths; // Vector of LpmtElecTruth
  

  protected:


  public:

    /// Default Constructor
    CdLpmtElecTruthEvt() : m_truths() {}
  
    /// Default Destructor
    virtual ~CdLpmtElecTruthEvt() {}
  
    /// add a truth to m_truths
    void AddTruth(const JM::LpmtElecTruth& value);
  
    /// Retrieve const  
    /// Vector of LpmtElecTruth
    const std::vector<JM::LpmtElecTruth>& truths() const;
  
    /// Update  
    /// Vector of LpmtElecTruth
    void setTruths(const std::vector<JM::LpmtElecTruth>& value);
  
    ClassDef(CdLpmtElecTruthEvt,1)
  

  }; // class CdLpmtElecTruthEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<JM::LpmtElecTruth>& JM::CdLpmtElecTruthEvt::truths() const 
{
  return m_truths;
}

inline void JM::CdLpmtElecTruthEvt::setTruths(const std::vector<JM::LpmtElecTruth>& value) 
{
  m_truths = value;
}

inline void JM::CdLpmtElecTruthEvt::AddTruth(const JM::LpmtElecTruth& value) 
{

                    m_truths.push_back(value);
                
}


#endif ///SNIPER_ELECTRUTH_CDLPMTELECTRUTHEVT_H
