
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

#ifndef SNIPER_ELECTRUTH_WPELECTRUTHEVT_H
#define SNIPER_ELECTRUTH_WPELECTRUTHEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/WpElecTruth.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpElecTruthEvt WpElecTruthEvt.h
   *
   * This data model is a container of WpElecTruth objects 
   *
   * @author Haoqi Lu
   * created Sun Sep 24 17:52:29 2023
   * 
   */

  class WpElecTruthEvt: public EventObject
  {
  private:

    std::vector<JM::WpElecTruth> m_truths; // Vector of WpElecTruth
  

  protected:


  public:

    /// Default Constructor
    WpElecTruthEvt() : m_truths() {}
  
    /// Default Destructor
    virtual ~WpElecTruthEvt() {}
  
    /// Retrieve const  
    /// Vector of WpElecTruth
    const std::vector<JM::WpElecTruth>& truths() const;
  
    /// Update  
    /// Vector of WpElecTruth
    void setTruths(const std::vector<JM::WpElecTruth>& value);
  
    ClassDef(WpElecTruthEvt,1)
  

  }; // class WpElecTruthEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<JM::WpElecTruth>& JM::WpElecTruthEvt::truths() const 
{
  return m_truths;
}

inline void JM::WpElecTruthEvt::setTruths(const std::vector<JM::WpElecTruth>& value) 
{
  m_truths = value;
}


#endif ///SNIPER_ELECTRUTH_WPELECTRUTHEVT_H
