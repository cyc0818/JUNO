
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

#ifndef SNIPER_RECEVENT_WPRECEVT_H
#define SNIPER_RECEVENT_WPRECEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/RecTrack.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpRecEvt WpRecEvt.h
   *
   * Reconstructed WP track event 
   *
   * @author 
   * created Sun Sep 24 17:52:50 2023
   * 
   */

  class WpRecEvt: public EventObject
  {
  private:

    std::vector<JM::RecTrack*> m_tracks; // The WP tracks in this event
  

  protected:


  public:

    /// Default Constructor
    WpRecEvt() : m_tracks() {}
  
  /// destructor
  ~WpRecEvt();
  
    /// Get a WP track via index
    const JM::RecTrack * getTrack(int index) const;
  
    /// Add a WP track to this event
    void addTrack(JM::RecTrack* track);
  
    /// Retrieve const  
    /// The WP tracks in this event
    const std::vector<JM::RecTrack*>& tracks() const;
  
    ClassDef(WpRecEvt,1)
  

  }; // class WpRecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::WpRecEvt::~WpRecEvt() 
{

  std::vector<RecTrack*>::const_iterator it = m_tracks.begin();
  std::vector<RecTrack*>::const_iterator end = m_tracks.end();
  while(it!=end){
    if(*it) delete  (*it);
      it++;
  }
  m_tracks.clear();
                
}

inline const std::vector<JM::RecTrack*>& JM::WpRecEvt::tracks() const 
{
  return m_tracks;
}

inline const JM::RecTrack * JM::WpRecEvt::getTrack(int index) const 
{

  return m_tracks.at(index);
                
}

inline void JM::WpRecEvt::addTrack(JM::RecTrack* track) 
{

  m_tracks.push_back(track);
                
}


#endif ///SNIPER_RECEVENT_WPRECEVT_H
