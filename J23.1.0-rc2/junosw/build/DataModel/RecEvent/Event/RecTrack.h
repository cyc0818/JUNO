
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

#ifndef SNIPER_RECEVENT_RECTRACK_H
#define SNIPER_RECEVENT_RECTRACK_H

// Include files
#include "CLHEP/Vector/LorentzVector.h"
#include "TObject.h"
#include <string>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class RecTrack RecTrack.h
   *
   * Reconstructed track class. ChangeLog: v1) original version. v2) use float
 * instead of double 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:47 2023
   * 
   */

  class RecTrack: public TObject
  {
  private:

    float                   m_PESum;   // The total p.e. for the muon track
    float                   m_quality; // Quality of the reconstructed track
    CLHEP::HepLorentzVector m_start;   // The start point in center detector of this track
    CLHEP::HepLorentzVector m_end;     // The end point in center detector of this track
  

  protected:


  public:

  /// Constructor
  RecTrack(const CLHEP::HepLorentzVector& start,
             const CLHEP::HepLorentzVector& end) : m_start(start),
                                                     m_end(end) {}
  
    /// Default Constructor
    RecTrack() : m_PESum(0),
                 m_quality(0.0),
                 m_start(0),
                 m_end(0) {}
  
    /// Default Destructor
    virtual ~RecTrack() {}
  
    /// The direction of this track
    const CLHEP::HepLorentzVector direction() const;
  
    /// Retrieve const  
    /// The total p.e. for the muon track
    float peSum() const;
  
    /// Update  
    /// The total p.e. for the muon track
    void setPESum(float value);
  
    /// Retrieve const  
    /// Quality of the reconstructed track
    float quality() const;
  
    /// Update  
    /// Quality of the reconstructed track
    void setQuality(float value);
  
    /// Retrieve const  
    /// The start point in center detector of this track
    const CLHEP::HepLorentzVector& start() const;
  
    /// Update  
    /// The start point in center detector of this track
    void setStart(const CLHEP::HepLorentzVector& value);
  
    /// Retrieve const  
    /// The end point in center detector of this track
    const CLHEP::HepLorentzVector& end() const;
  
    /// Update  
    /// The end point in center detector of this track
    void setEnd(const CLHEP::HepLorentzVector& value);
  
    ClassDef(RecTrack,2)
  

  }; // class RecTrack

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline float JM::RecTrack::peSum() const 
{
  return m_PESum;
}

inline void JM::RecTrack::setPESum(float value) 
{
  m_PESum = value;
}

inline float JM::RecTrack::quality() const 
{
  return m_quality;
}

inline void JM::RecTrack::setQuality(float value) 
{
  m_quality = value;
}

inline const CLHEP::HepLorentzVector& JM::RecTrack::start() const 
{
  return m_start;
}

inline void JM::RecTrack::setStart(const CLHEP::HepLorentzVector& value) 
{
  m_start = value;
}

inline const CLHEP::HepLorentzVector& JM::RecTrack::end() const 
{
  return m_end;
}

inline void JM::RecTrack::setEnd(const CLHEP::HepLorentzVector& value) 
{
  m_end = value;
}

inline const CLHEP::HepLorentzVector JM::RecTrack::direction() const 
{

  CLHEP::HepLorentzVector vDiff = m_end - m_start;
  vDiff /= vDiff.rho();
  return vDiff;
                
}


#endif ///SNIPER_RECEVENT_RECTRACK_H
