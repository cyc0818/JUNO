
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

#ifndef SNIPER_ELECTRUTH_TTELECTRUTHEVT_H
#define SNIPER_ELECTRUTH_TTELECTRUTHEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/TtElecTruth.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtElecTruthEvt TtElecTruthEvt.h
   *
   * This data model is a container of TtElecTruth objects 
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:30 2023
   * 
   */

  class TtElecTruthEvt: public EventObject
  {
  private:

    std::vector<JM::TtElecTruth> m_truths; // Vector of TtElecTruth
  

  protected:


  public:

    /// Default Constructor
    TtElecTruthEvt() : m_truths() {}
  
    /// Default Destructor
    virtual ~TtElecTruthEvt() {}
  
    /// Retrieve const  
    /// Vector of TtElecTruth
    const std::vector<JM::TtElecTruth>& truths() const;
  
    /// Update  
    /// Vector of TtElecTruth
    void setTruths(const std::vector<JM::TtElecTruth>& value);
  
    ClassDef(TtElecTruthEvt,1)
  

  }; // class TtElecTruthEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<JM::TtElecTruth>& JM::TtElecTruthEvt::truths() const 
{
  return m_truths;
}

inline void JM::TtElecTruthEvt::setTruths(const std::vector<JM::TtElecTruth>& value) 
{
  m_truths = value;
}


#endif ///SNIPER_ELECTRUTH_TTELECTRUTHEVT_H
