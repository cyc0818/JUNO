
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

#ifndef SNIPER_ELECEVENT_TTELECEVT_H
#define SNIPER_ELECEVENT_TTELECEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/ElecChannel.h"
#include <map>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtElecEvt TtElecEvt.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:12 2023
   * 
   */

  class TtElecEvt: public EventObject
  {
  private:

    std::map<int,JM::ElecChannel*> m_channelData; // Map between PmtId and ElecChannel
  

  protected:


  public:

    /// Default Constructor
    TtElecEvt() : m_channelData() {}
  
  /// destructor for TtElecEvt
  ~TtElecEvt();
  
    /// Add a PMT channel to this event
    ElecChannel* addChannel(int pmtID);
  
    /// Add a PMT channel to this event
    void addChannel(int pmtID,
                    JM::ElecChannel* channel);
  
    /// Get a PMT channel from this event
    ElecChannel* getChannel(int pmtID);
  
    /// Retrieve const  
    /// Map between PmtId and ElecChannel
    const std::map<int,JM::ElecChannel*>& channelData() const;
  
    /// Update  
    /// Map between PmtId and ElecChannel
    void setChannelData(const std::map<int,JM::ElecChannel*>& value);
  
    ClassDef(TtElecEvt,1)
  

  }; // class TtElecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::TtElecEvt::~TtElecEvt() 
{

  for (auto it: m_channelData) {
    delete it.second;
  }
                
}

inline const std::map<int,JM::ElecChannel*>& JM::TtElecEvt::channelData() const 
{
  return m_channelData;
}

inline void JM::TtElecEvt::setChannelData(const std::map<int,JM::ElecChannel*>& value) 
{
  m_channelData = value;
}

inline JM::ElecChannel* JM::TtElecEvt::addChannel(int pmtID) 
{

  JM::ElecChannel* channel = new JM::ElecChannel;
  auto it = m_channelData.find(pmtID);
  if (it == m_channelData.end()) {
    m_channelData[pmtID] = channel;
  }
  else {
    delete it->second;
    it->second = channel;
  }
  return channel;
            
}

inline void JM::TtElecEvt::addChannel(int pmtID,
                                      JM::ElecChannel* channel) 
{

  auto it = m_channelData.find(pmtID);
  if (it == m_channelData.end()) {
    m_channelData[pmtID] = channel;
  }
  else {
    delete it->second;
    it->second = channel;
  }
            
}

inline JM::ElecChannel* JM::TtElecEvt::getChannel(int pmtID) 
{

  auto it = m_channelData.find(pmtID);
  if (it != m_channelData.end()) {
    return it->second;
  }
  return 0;
            
}


#endif ///SNIPER_ELECEVENT_TTELECEVT_H
