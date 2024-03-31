
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

#ifndef SNIPER_ELECTRUTH_CDSPMTELECTRUTHEVT_H
#define SNIPER_ELECTRUTH_CDSPMTELECTRUTHEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/SpmtElecTruth.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdSpmtElecTruthEvt CdSpmtElecTruthEvt.h
   *
   * This data model is a container of SpmtElecTruth objects 
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:32 2023
   * 
   */

  class CdSpmtElecTruthEvt: public EventObject
  {
  private:

    std::vector<JM::SpmtElecTruth> m_truths; // Vector of SpmtElecTruth
  

  protected:


  public:

    /// Default Constructor
    CdSpmtElecTruthEvt() : m_truths() {}
  
    /// Default Destructor
    virtual ~CdSpmtElecTruthEvt() {}
  
    /// add a truth to m_truths
    void AddTruth(const JM::SpmtElecTruth& value);
  
    /// Retrieve const  
    /// Vector of SpmtElecTruth
    const std::vector<JM::SpmtElecTruth>& truths() const;
  
    /// Update  
    /// Vector of SpmtElecTruth
    void setTruths(const std::vector<JM::SpmtElecTruth>& value);
  
    ClassDef(CdSpmtElecTruthEvt,1)
  

  }; // class CdSpmtElecTruthEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<JM::SpmtElecTruth>& JM::CdSpmtElecTruthEvt::truths() const 
{
  return m_truths;
}

inline void JM::CdSpmtElecTruthEvt::setTruths(const std::vector<JM::SpmtElecTruth>& value) 
{
  m_truths = value;
}

inline void JM::CdSpmtElecTruthEvt::AddTruth(const JM::SpmtElecTruth& value) 
{

                    m_truths.push_back(value);
                
}


#endif ///SNIPER_ELECTRUTH_CDSPMTELECTRUTHEVT_H
