
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

#ifndef SNIPER_ELECTRUTH_TTELECTRUTH_H
#define SNIPER_ELECTRUTH_TTELECTRUTH_H

// Include files
#include "Event/EventObject.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtElecTruth TtElecTruth.h
   *
   * This data model reflects the content of the Pulse class. ChangeLog: v1)
 * first version. v2) use float for amplitude, TTS and timeoffset. 
   *
   * @author Teng Li
   * created Sun Sep 24 17:52:31 2023
   * 
   */

  class TtElecTruth: public EventObject
  {
  private:

    std::string m_pulsetype;    // sourse type of the pulse('tag' setting  by user, DN, AP or Unknow)
    int         m_pmtId;        // pmt id of the pulse
    int         m_npe;          // photo-electron number of the pulse
    double      m_hitTime;      // hit time as from detector simulation
    float       m_amplitude;    // amplitude as from PMT simulation
    float       m_TTS;          // transit time of the hit
    float       m_timeoffset;   // time offset of the hit
    TimeStamp   m_pulseHitTime; // TimeStamp of the pulse = event time stamp + transit time + time offset
  

  protected:


  public:

    /// Default Constructor
    TtElecTruth() : m_pulsetype(),
                    m_pmtId(0),
                    m_npe(0),
                    m_hitTime(0.0),
                    m_amplitude(0.0),
                    m_TTS(0.0),
                    m_timeoffset(0.0),
                    m_pulseHitTime() {}
  
    /// Default Destructor
    virtual ~TtElecTruth() {}
  
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
    /// amplitude as from PMT simulation
    float amplitude() const;
  
    /// Update  
    /// amplitude as from PMT simulation
    void setAmplitude(float value);
  
    /// Retrieve const  
    /// transit time of the hit
    float tts() const;
  
    /// Update  
    /// transit time of the hit
    void setTTS(float value);
  
    /// Retrieve const  
    /// time offset of the hit
    float timeoffset() const;
  
    /// Update  
    /// time offset of the hit
    void setTimeoffset(float value);
  
    /// Retrieve const  
    /// TimeStamp of the pulse = event time stamp + transit time + time offset
    const TimeStamp& pulseHitTime() const;
  
    /// Update  
    /// TimeStamp of the pulse = event time stamp + transit time + time offset
    void setPulseHitTime(const TimeStamp& value);
  
    ClassDef(TtElecTruth,2)
  

  }; // class TtElecTruth

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::string& JM::TtElecTruth::pulsetype() const 
{
  return m_pulsetype;
}

inline void JM::TtElecTruth::setPulsetype(const std::string& value) 
{
  m_pulsetype = value;
}

inline int JM::TtElecTruth::pmtId() const 
{
  return m_pmtId;
}

inline void JM::TtElecTruth::setPmtId(int value) 
{
  m_pmtId = value;
}

inline int JM::TtElecTruth::npe() const 
{
  return m_npe;
}

inline void JM::TtElecTruth::setNpe(int value) 
{
  m_npe = value;
}

inline double JM::TtElecTruth::hitTime() const 
{
  return m_hitTime;
}

inline void JM::TtElecTruth::setHitTime(double value) 
{
  m_hitTime = value;
}

inline float JM::TtElecTruth::amplitude() const 
{
  return m_amplitude;
}

inline void JM::TtElecTruth::setAmplitude(float value) 
{
  m_amplitude = value;
}

inline float JM::TtElecTruth::tts() const 
{
  return m_TTS;
}

inline void JM::TtElecTruth::setTTS(float value) 
{
  m_TTS = value;
}

inline float JM::TtElecTruth::timeoffset() const 
{
  return m_timeoffset;
}

inline void JM::TtElecTruth::setTimeoffset(float value) 
{
  m_timeoffset = value;
}

inline const TimeStamp& JM::TtElecTruth::pulseHitTime() const 
{
  return m_pulseHitTime;
}

inline void JM::TtElecTruth::setPulseHitTime(const TimeStamp& value) 
{
  m_pulseHitTime = value;
}


#endif ///SNIPER_ELECTRUTH_TTELECTRUTH_H
