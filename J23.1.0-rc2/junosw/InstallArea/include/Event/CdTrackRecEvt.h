
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

#ifndef SNIPER_RECEVENT_CDTRACKRECEVT_H
#define SNIPER_RECEVENT_CDTRACKRECEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/RecTrack.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdTrackRecEvt CdTrackRecEvt.h
   *
   * Reconstruction Event Class 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:49 2023
   * 
   */

  class CdTrackRecEvt: public EventObject
  {
  private:

    std::vector<JM::RecTrack*> m_tracks; // The Cd tracks in this event
  

  protected:


  public:

    /// Default Constructor
    CdTrackRecEvt() : m_tracks() {}
  
  /// destructor
  ~CdTrackRecEvt();
  
    /// Get a Cd track via index
    const JM::RecTrack * getTrack(int index) const;
  
    /// Add a Cd track to this event
    void addTrack(JM::RecTrack* track);
  
    /// Get number of tracks
    int nTracks() const;
  
    /// Retrieve const  
    /// The Cd tracks in this event
    const std::vector<JM::RecTrack*>& tracks() const;
  
    ClassDef(CdTrackRecEvt,1)
  

  }; // class CdTrackRecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdTrackRecEvt::~CdTrackRecEvt() 
{

  std::vector<RecTrack*>::const_iterator it = m_tracks.begin();
  std::vector<RecTrack*>::const_iterator end = m_tracks.end();
  while(it!=end){
    if(*it) delete  (*it);
    it++;
  }
  m_tracks.clear();
                
}

inline const std::vector<JM::RecTrack*>& JM::CdTrackRecEvt::tracks() const 
{
  return m_tracks;
}

inline const JM::RecTrack * JM::CdTrackRecEvt::getTrack(int index) const 
{

  return m_tracks.at(index);
                
}

inline void JM::CdTrackRecEvt::addTrack(JM::RecTrack* track) 
{

  m_tracks.push_back(track);
                
}

inline int JM::CdTrackRecEvt::nTracks() const 
{

  return m_tracks.size();;
                
}


#endif ///SNIPER_RECEVENT_CDTRACKRECEVT_H
