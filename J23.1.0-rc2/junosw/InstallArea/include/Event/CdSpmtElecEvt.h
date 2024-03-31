
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

#ifndef SNIPER_ELECEVENT_CDSPMTELECEVT_H
#define SNIPER_ELECEVENT_CDSPMTELECEVT_H

// Include files
#include "Event/EventObject.h"
#include "Event/SpmtElecAbcBlock.h"
#include "Event/SpmtElecSpecialWord.h"
#include "Event/SpmtElecDiscrWord.h"
#include "Event/ElecChannel.h"
#include <map>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class CdSpmtElecEvt CdSpmtElecEvt.h
   *
   *  
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:08 2023
   * 
   */

  class CdSpmtElecEvt: public EventObject
  {
  private:

    std::vector<JM::SpmtElecAbcBlock>    m_SpmtBlocks;       // Vector of SpmtBlocks
    std::vector<JM::SpmtElecSpecialWord> m_SpmtSpecialWords; // Vector of Special Words
    std::vector<JM::SpmtElecDiscrWord>   m_SpmtDiscrWords;   // Vector of Discriminator Words
    std::map<int,JM::ElecChannel*>       m_channelData;      // Map between PmtId and ElecChannel
  

  protected:


  public:

    /// Default Constructor
    CdSpmtElecEvt() : m_SpmtBlocks(),
                      m_SpmtSpecialWords(),
                      m_SpmtDiscrWords(),
                      m_channelData() {}
  
  /// destructor
  ~CdSpmtElecEvt();
  
    /// add a block to m_SpmtBlocks
    void AddAbcBlock(const JM::SpmtElecAbcBlock& value);
  
    /// Add a PMT channel to this event
    ElecChannel* addChannel(int pmtID);
  
    /// Add a PMT channel to this event
    void addChannel(int pmtID,
                    JM::ElecChannel* channel);
  
    /// Get a PMT channel from this event
    ElecChannel* getChannel(int pmtID);
  
    /// Retrieve const  
    /// Vector of SpmtBlocks
    const std::vector<JM::SpmtElecAbcBlock>& SpmtBlocks() const;
  
    /// Update  
    /// Vector of SpmtBlocks
    void setSpmtBlocks(const std::vector<JM::SpmtElecAbcBlock>& value);
  
    /// Retrieve const  
    /// Vector of Special Words
    const std::vector<JM::SpmtElecSpecialWord>& SpmtSpecialWords() const;
  
    /// Update  
    /// Vector of Special Words
    void setSpmtSpecialWords(const std::vector<JM::SpmtElecSpecialWord>& value);
  
    /// Retrieve const  
    /// Vector of Discriminator Words
    const std::vector<JM::SpmtElecDiscrWord>& SpmtDiscrWords() const;
  
    /// Update  
    /// Vector of Discriminator Words
    void setSpmtDiscrWords(const std::vector<JM::SpmtElecDiscrWord>& value);
  
    /// Retrieve const  
    /// Map between PmtId and ElecChannel
    const std::map<int,JM::ElecChannel*>& channelData() const;
  
    /// Update  
    /// Map between PmtId and ElecChannel
    void setChannelData(const std::map<int,JM::ElecChannel*>& value);
  
    ClassDef(CdSpmtElecEvt,1)
  

  }; // class CdSpmtElecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline JM::CdSpmtElecEvt::~CdSpmtElecEvt() 
{

  for (auto it = m_channelData.begin(); it!=m_channelData.end(); ++it) {
    delete it->second;
  }
                
}

inline const std::vector<JM::SpmtElecAbcBlock>& JM::CdSpmtElecEvt::SpmtBlocks() const 
{
  return m_SpmtBlocks;
}

inline void JM::CdSpmtElecEvt::setSpmtBlocks(const std::vector<JM::SpmtElecAbcBlock>& value) 
{
  m_SpmtBlocks = value;
}

inline const std::vector<JM::SpmtElecSpecialWord>& JM::CdSpmtElecEvt::SpmtSpecialWords() const 
{
  return m_SpmtSpecialWords;
}

inline void JM::CdSpmtElecEvt::setSpmtSpecialWords(const std::vector<JM::SpmtElecSpecialWord>& value) 
{
  m_SpmtSpecialWords = value;
}

inline const std::vector<JM::SpmtElecDiscrWord>& JM::CdSpmtElecEvt::SpmtDiscrWords() const 
{
  return m_SpmtDiscrWords;
}

inline void JM::CdSpmtElecEvt::setSpmtDiscrWords(const std::vector<JM::SpmtElecDiscrWord>& value) 
{
  m_SpmtDiscrWords = value;
}

inline const std::map<int,JM::ElecChannel*>& JM::CdSpmtElecEvt::channelData() const 
{
  return m_channelData;
}

inline void JM::CdSpmtElecEvt::setChannelData(const std::map<int,JM::ElecChannel*>& value) 
{
  m_channelData = value;
}

inline void JM::CdSpmtElecEvt::AddAbcBlock(const JM::SpmtElecAbcBlock& value) 
{

  m_SpmtBlocks.push_back(value);
                
}

inline JM::ElecChannel* JM::CdSpmtElecEvt::addChannel(int pmtID) 
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

inline void JM::CdSpmtElecEvt::addChannel(int pmtID,
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

inline JM::ElecChannel* JM::CdSpmtElecEvt::getChannel(int pmtID) 
{

  auto it = m_channelData.find(pmtID);
  if (it != m_channelData.end()) {
    return it->second;
  }
  return 0;
            
}


#endif ///SNIPER_ELECEVENT_CDSPMTELECEVT_H
