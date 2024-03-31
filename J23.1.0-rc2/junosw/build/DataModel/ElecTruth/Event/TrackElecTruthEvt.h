
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

#ifndef SNIPER_ELECTRUTH_TRACKELECTRUTHEVT_H
#define SNIPER_ELECTRUTH_TRACKELECTRUTHEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/TrackElecTruth.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TrackElecTruthEvt TrackElecTruthEvt.h
   *
   * This data model is a container of TrackElecTruth objects 
   *
   * @author Miao Yu
   * created Sun Sep 24 17:52:34 2023
   * 
   */

  class TrackElecTruthEvt: public EventObject
  {
  private:

    std::vector<JM::TrackElecTruth> m_truths; // Vector of TrackElecTruth
  

  protected:


  public:

    /// Default Constructor
    TrackElecTruthEvt() : m_truths() {}
  
    /// Default Destructor
    virtual ~TrackElecTruthEvt() {}
  
    /// add a track thuth to m_truths
    void AddTrackTruth(const JM::TrackElecTruth& value);
  
    /// Retrieve const  
    /// Vector of TrackElecTruth
    const std::vector<JM::TrackElecTruth>& truths() const;
  
    /// Update  
    /// Vector of TrackElecTruth
    void setTruths(const std::vector<JM::TrackElecTruth>& value);
  
    ClassDef(TrackElecTruthEvt,1)
  

  }; // class TrackElecTruthEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::vector<JM::TrackElecTruth>& JM::TrackElecTruthEvt::truths() const 
{
  return m_truths;
}

inline void JM::TrackElecTruthEvt::setTruths(const std::vector<JM::TrackElecTruth>& value) 
{
  m_truths = value;
}

inline void JM::TrackElecTruthEvt::AddTrackTruth(const JM::TrackElecTruth& value) 
{

                    m_truths.push_back(value);
                
}


#endif ///SNIPER_ELECTRUTH_TRACKELECTRUTHEVT_H
