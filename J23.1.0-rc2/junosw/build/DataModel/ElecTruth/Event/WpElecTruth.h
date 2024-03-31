
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

#ifndef SNIPER_ELECTRUTH_WPELECTRUTH_H
#define SNIPER_ELECTRUTH_WPELECTRUTH_H

// Include files
#include "Event/EventObject.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class WpElecTruth WpElecTruth.h
   *
   * This data model reflects the content of the Pulse class. ChangeLog: v1)
 * first version. v2) use float for amplitude, TTS and timeoffset. 
   *
   * @author Haoqi Lu
   * created Sun Sep 24 17:52:29 2023
   * 
   */

  class WpElecTruth: public EventObject
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
    WpElecTruth() : m_pulsetype(),
                    m_pmtId(0),
                    m_npe(0),
                    m_hitTime(0.0),
                    m_amplitude(0.0),
                    m_TTS(0.0),
                    m_timeoffset(0.0),
                    m_pulseHitTime() {}
  
    /// Default Destructor
    virtual ~WpElecTruth() {}
  
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
  
    ClassDef(WpElecTruth,2)
  

  }; // class WpElecTruth

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const std::string& JM::WpElecTruth::pulsetype() const 
{
  return m_pulsetype;
}

inline void JM::WpElecTruth::setPulsetype(const std::string& value) 
{
  m_pulsetype = value;
}

inline int JM::WpElecTruth::pmtId() const 
{
  return m_pmtId;
}

inline void JM::WpElecTruth::setPmtId(int value) 
{
  m_pmtId = value;
}

inline int JM::WpElecTruth::npe() const 
{
  return m_npe;
}

inline void JM::WpElecTruth::setNpe(int value) 
{
  m_npe = value;
}

inline double JM::WpElecTruth::hitTime() const 
{
  return m_hitTime;
}

inline void JM::WpElecTruth::setHitTime(double value) 
{
  m_hitTime = value;
}

inline float JM::WpElecTruth::amplitude() const 
{
  return m_amplitude;
}

inline void JM::WpElecTruth::setAmplitude(float value) 
{
  m_amplitude = value;
}

inline float JM::WpElecTruth::tts() const 
{
  return m_TTS;
}

inline void JM::WpElecTruth::setTTS(float value) 
{
  m_TTS = value;
}

inline float JM::WpElecTruth::timeoffset() const 
{
  return m_timeoffset;
}

inline void JM::WpElecTruth::setTimeoffset(float value) 
{
  m_timeoffset = value;
}

inline const TimeStamp& JM::WpElecTruth::pulseHitTime() const 
{
  return m_pulseHitTime;
}

inline void JM::WpElecTruth::setPulseHitTime(const TimeStamp& value) 
{
  m_pulseHitTime = value;
}


#endif ///SNIPER_ELECTRUTH_WPELECTRUTH_H
