
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

#ifndef SNIPER_ELECTRUTH_SPMTELECTRUTH_H
#define SNIPER_ELECTRUTH_SPMTELECTRUTH_H

// Include files
#include "Event/EventObject.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class SpmtElecTruth SpmtElecTruth.h
   *
   * This data model reflects the content of the Pulse class excluding time
 * stamps. ChangeLog: v3) use float for timeWindow, amplitude and TTS 
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:33 2023
   * 
   */

  class SpmtElecTruth: public EventObject
  {
  private:

    std::string m_tagId;        // Tag of det sim source from mixer
    std::string m_pulsetype;    // sourse type of the pulse('tag' setting  by user, DN, AP or Unknow)
    int         m_pmtId;        // pmt id of the pulse
    int         m_npe;          // photo-electron number of the pulse
    double      m_hitTime;      // hit time as from detector simulation
    float       m_timeWindow;   // timeWindow as from detector simulation
    float       m_amplitude;    // amplitude as from PMT simulation
    float       m_TTS;          // transit time as from PMT simulation
    TimeStamp   m_pulseHitTime; // TimeStamp of the pulse = event time stamp + transit time
    TimeStamp   m_evtHitTime;   // TimeStamp of the event as from mixing
  

  protected:


  public:

    /// Default Constructor
    SpmtElecTruth() : m_tagId(),
                      m_pulsetype(),
                      m_pmtId(0),
                      m_npe(0),
                      m_hitTime(0.0),
                      m_timeWindow(0.0),
                      m_amplitude(0.0),
                      m_TTS(0.0),
                      m_pulseHitTime(),
                      m_evtHitTime() {}
  
    /// Default Destructor
    virtual ~SpmtElecTruth() {}
  
    /// Retrieve const  
    /// Tag of det sim source from mixer
    const std::string& tagId() const;
  
    /// Update  
    /// Tag of det sim source from mixer
    void setTagId(const std::string& value);
  
    /// Retrieve const  
    /// sourse type of the pulse('tag' setting  by user, DN, AP or Unknow)
    const std::string& pulsetype() const;
  
    /// Update  
    /// sourse type of the pulse('tag' setting  by user, DN, AP or Unknow)
    void setPulsetype(const std::string& value);
  
    /// Retrieve const  
    /// pmt id of the pulse
    int pmtId() const;
  
    /// Update  
    /// pmt id of the pulse
    void setPmtId(int value);
  
    /// Retrieve const  
    /// photo-electron number of the pulse
    int npe() const;
  
    /// Update  
    /// photo-electron number of the pulse
    void setNpe(int value);
  
    /// Retrieve const  
    /// hit time as from detector simulation
    double hitTime() const;
  
    /// Update  
    /// hit time as from detector simulation
    void setHitTime(double value);
  
    /// Retrieve const  
    /// timeWindow as from detector simulation
    float timeWindow() const;
  
    /// Update  
    /// timeWindow as from detector simulation
    void setTimeWindow(float value);
  
    /// Retrieve const  
    /// amplitude as from PMT simulation
    float amplitude() const;
  
    /// Update  
    /// amplitude as from PMT simulation
    void setAmplitude(float value);
  
    /// Retrieve const  
    /// transit time as from PMT simulation
    float tts() const;
  
    /// Update  
    /// transit time as from PMT simulation
    void setTTS(float value);
  
    /// Retrieve const  
    /// TimeStamp of the pulse = event time stamp + transit time
    const TimeStamp& pulseHitTime() const;
  
    /// Update  
    /// TimeStamp of the pulse = event time stamp + transit time
    void setPulseHitTime(const TimeStamp& value);
  
    /// Retrieve const  
    /// TimeStamp of the event as from mixing
    const TimeStamp& evtHitTime() const;
  
    /// Update  
    /// TimeStamp of the event as from mixing
    void setEvtHitTime(const TimeStamp& value);
  
    ClassDef(SpmtElecTruth,3)
  

  }; // class SpmtElecTruth

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::string& JM::SpmtElecTruth::tagId() const 
{
  return m_tagId;
}

inline void JM::SpmtElecTruth::setTagId(const std::string& value) 
{
  m_tagId = value;
}

inline const std::string& JM::SpmtElecTruth::pulsetype() const 
{
  return m_pulsetype;
}

inline void JM::SpmtElecTruth::setPulsetype(const std::string& value) 
{
  m_pulsetype = value;
}

inline int JM::SpmtElecTruth::pmtId() const 
{
  return m_pmtId;
}

inline void JM::SpmtElecTruth::setPmtId(int value) 
{
  m_pmtId = value;
}

inline int JM::SpmtElecTruth::npe() const 
{
  return m_npe;
}

inline void JM::SpmtElecTruth::setNpe(int value) 
{
  m_npe = value;
}

inline double JM::SpmtElecTruth::hitTime() const 
{
  return m_hitTime;
}

inline void JM::SpmtElecTruth::setHitTime(double value) 
{
  m_hitTime = value;
}

inline float JM::SpmtElecTruth::timeWindow() const 
{
  return m_timeWindow;
}

inline void JM::SpmtElecTruth::setTimeWindow(float value) 
{
  m_timeWindow = value;
}

inline float JM::SpmtElecTruth::amplitude() const 
{
  return m_amplitude;
}

inline void JM::SpmtElecTruth::setAmplitude(float value) 
{
  m_amplitude = value;
}

inline float JM::SpmtElecTruth::tts() const 
{
  return m_TTS;
}

inline void JM::SpmtElecTruth::setTTS(float value) 
{
  m_TTS = value;
}

inline const TimeStamp& JM::SpmtElecTruth::pulseHitTime() const 
{
  return m_pulseHitTime;
}

inline void JM::SpmtElecTruth::setPulseHitTime(const TimeStamp& value) 
{
  m_pulseHitTime = value;
}

inline const TimeStamp& JM::SpmtElecTruth::evtHitTime() const 
{
  return m_evtHitTime;
}

inline void JM::SpmtElecTruth::setEvtHitTime(const TimeStamp& value) 
{
  m_evtHitTime = value;
}


#endif ///SNIPER_ELECTRUTH_SPMTELECTRUTH_H
