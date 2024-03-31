
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

#ifndef SNIPER_ELECEVENT_CDWAVEFORMEVT_H
#define SNIPER_ELECEVENT_CDWAVEFORMEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/ElecWaveform.h"
#include "Context/TimeStamp.h"
#include <vector>
#include <map>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdWaveformEvt CdWaveformEvt.h
   *
   *  
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:09 2023
   * 
   */

  class CdWaveformEvt: public EventObject
  {
  private:

    std::map<int,JM::ElecWaveform*> m_channelData; // Map between PmtId and ElecWaveform
  

  protected:


  public:

    /// Default Constructor
    CdWaveformEvt() : m_channelData() {}
  
  /// destructor for CdWaveformEvt
  ~CdWaveformEvt();
  
    /// Add a PMT channel to this event
    ElecWaveform* addChannel(int pmtID);
  
    /// Add a PMT channel to this event
    void addChannel(int pmtID,
                    JM::ElecWaveform* channel);
  
    /// Get a PMT channel from this event
    ElecWaveform* getChannel(int pmtID);
  
    /// Retrieve const  
    /// Map between PmtId and ElecWaveform
    const std::map<int,JM::ElecWaveform*>& channelData() const;
  
    /// Update  
    /// Map between PmtId and ElecWaveform
    void setChannelData(const std::map<int,JM::ElecWaveform*>& value);
  
    ClassDef(CdWaveformEvt,1)
  

  }; // class CdWaveformEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdWaveformEvt::~CdWaveformEvt() 
{

  for (auto it: m_channelData) {
    delete it.second;
  }
                
}

inline const std::map<int,JM::ElecWaveform*>& JM::CdWaveformEvt::channelData() const 
{
  return m_channelData;
}

inline void JM::CdWaveformEvt::setChannelData(const std::map<int,JM::ElecWaveform*>& value) 
{
  m_channelData = value;
}

inline JM::ElecWaveform* JM::CdWaveformEvt::addChannel(int pmtID) 
{

  JM::ElecWaveform* channel = new JM::ElecWaveform;
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

inline void JM::CdWaveformEvt::addChannel(int pmtID,
                                          JM::ElecWaveform* channel) 
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

inline JM::ElecWaveform* JM::CdWaveformEvt::getChannel(int pmtID) 
{

  auto it = m_channelData.find(pmtID);
  if (it != m_channelData.end()) {
    return it->second;
  }
  return 0;
            
}


#endif ///SNIPER_ELECEVENT_CDWAVEFORMEVT_H
