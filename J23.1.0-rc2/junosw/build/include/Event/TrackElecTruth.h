
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

#ifndef SNIPER_ELECTRUTH_TRACKELECTRUTH_H
#define SNIPER_ELECTRUTH_TRACKELECTRUTH_H

// Include files
#include "Event/EventObject.h"
#include "Context/TimeStamp.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TrackElecTruth TrackElecTruth.h
   *
   * This data model reflects the content of the track truth info. ChangeLog: v1)
 * first version. v2) use float. 
   *
   * @author Miao Yu
   * created Sun Sep 24 17:52:34 2023
   * 
   */

  class TrackElecTruth: public EventObject
  {
  private:

    float     m_edepX;     // track truth edepX
    float     m_edepY;     // track truth edepY
    float     m_edepZ;     // track truth edepZ
    int       m_pdgid;     // track truth pdgid
    float     m_edep;      // track truth edep
    float     m_Qedep;     // track truth Qedep
    double    m_initT;     // track truth initT
    float     m_initX;     // track truth initX
    float     m_initY;     // track truth initY
    float     m_initZ;     // track truth initZ
    float     m_initPx;    // track truth initPx
    float     m_initPy;    // track truth initPy
    float     m_initPz;    // track truth initPz
    float     m_initMass;  // track truth initMass
    TimeStamp m_trackTime; // track timestamp
  

  protected:


  public:

    /// Default Constructor
    TrackElecTruth() : m_edepX(0.0),
                       m_edepY(0.0),
                       m_edepZ(0.0),
                       m_pdgid(0),
                       m_edep(0.0),
                       m_Qedep(0.0),
                       m_initT(0.0),
                       m_initX(0.0),
                       m_initY(0.0),
                       m_initZ(0.0),
                       m_initPx(0.0),
                       m_initPy(0.0),
                       m_initPz(0.0),
                       m_initMass(0.0),
                       m_trackTime() {}
  
    /// Default Destructor
    virtual ~TrackElecTruth() {}
  
    /// Retrieve const  
    /// track truth edepX
    float edepX() const;
  
    /// Update  
    /// track truth edepX
    void setEdepX(float value);
  
    /// Retrieve const  
    /// track truth edepY
    float edepY() const;
  
    /// Update  
    /// track truth edepY
    void setEdepY(float value);
  
    /// Retrieve const  
    /// track truth edepZ
    float edepZ() const;
  
    /// Update  
    /// track truth edepZ
    void setEdepZ(float value);
  
    /// Retrieve const  
    /// track truth pdgid
    int pdgid() const;
  
    /// Update  
    /// track truth pdgid
    void setPdgid(int value);
  
    /// Retrieve const  
    /// track truth edep
    float edep() const;
  
    /// Update  
    /// track truth edep
    void setEdep(float value);
  
    /// Retrieve const  
    /// track truth Qedep
    float Qedep() const;
  
    /// Update  
    /// track truth Qedep
    void setQedep(float value);
  
    /// Retrieve const  
    /// track truth initT
    double initT() const;
  
    /// Update  
    /// track truth initT
    void setInitT(double value);
  
    /// Retrieve const  
    /// track truth initX
    float initX() const;
  
    /// Update  
    /// track truth initX
    void setInitX(float value);
  
    /// Retrieve const  
    /// track truth initY
    float initY() const;
  
    /// Update  
    /// track truth initY
    void setInitY(float value);
  
    /// Retrieve const  
    /// track truth initZ
    float initZ() const;
  
    /// Update  
    /// track truth initZ
    void setInitZ(float value);
  
    /// Retrieve const  
    /// track truth initPx
    float initPx() const;
  
    /// Update  
    /// track truth initPx
    void setInitPx(float value);
  
    /// Retrieve const  
    /// track truth initPy
    float initPy() const;
  
    /// Update  
    /// track truth initPy
    void setInitPy(float value);
  
    /// Retrieve const  
    /// track truth initPz
    float initPz() const;
  
    /// Update  
    /// track truth initPz
    void setInitPz(float value);
  
    /// Retrieve const  
    /// track truth initMass
    float initMass() const;
  
    /// Update  
    /// track truth initMass
    void setInitMass(float value);
  
    /// Retrieve const  
    /// track timestamp
    const TimeStamp& trackTime() const;
  
    /// Update  
    /// track timestamp
    void setTrackTime(const TimeStamp& value);
  
    ClassDef(TrackElecTruth,2)
  

  }; // class TrackElecTruth

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline float JM::TrackElecTruth::edepX() const 
{
  return m_edepX;
}

inline void JM::TrackElecTruth::setEdepX(float value) 
{
  m_edepX = value;
}

inline float JM::TrackElecTruth::edepY() const 
{
  return m_edepY;
}

inline void JM::TrackElecTruth::setEdepY(float value) 
{
  m_edepY = value;
}

inline float JM::TrackElecTruth::edepZ() const 
{
  return m_edepZ;
}

inline void JM::TrackElecTruth::setEdepZ(float value) 
{
  m_edepZ = value;
}

inline int JM::TrackElecTruth::pdgid() const 
{
  return m_pdgid;
}

inline void JM::TrackElecTruth::setPdgid(int value) 
{
  m_pdgid = value;
}

inline float JM::TrackElecTruth::edep() const 
{
  return m_edep;
}

inline void JM::TrackElecTruth::setEdep(float value) 
{
  m_edep = value;
}

inline float JM::TrackElecTruth::Qedep() const 
{
  return m_Qedep;
}

inline void JM::TrackElecTruth::setQedep(float value) 
{
  m_Qedep = value;
}

inline double JM::TrackElecTruth::initT() const 
{
  return m_initT;
}

inline void JM::TrackElecTruth::setInitT(double value) 
{
  m_initT = value;
}

inline float JM::TrackElecTruth::initX() const 
{
  return m_initX;
}

inline void JM::TrackElecTruth::setInitX(float value) 
{
  m_initX = value;
}

inline float JM::TrackElecTruth::initY() const 
{
  return m_initY;
}

inline void JM::TrackElecTruth::setInitY(float value) 
{
  m_initY = value;
}

inline float JM::TrackElecTruth::initZ() const 
{
  return m_initZ;
}

inline void JM::TrackElecTruth::setInitZ(float value) 
{
  m_initZ = value;
}

inline float JM::TrackElecTruth::initPx() const 
{
  return m_initPx;
}

inline void JM::TrackElecTruth::setInitPx(float value) 
{
  m_initPx = value;
}

inline float JM::TrackElecTruth::initPy() const 
{
  return m_initPy;
}

inline void JM::TrackElecTruth::setInitPy(float value) 
{
  m_initPy = value;
}

inline float JM::TrackElecTruth::initPz() const 
{
  return m_initPz;
}

inline void JM::TrackElecTruth::setInitPz(float value) 
{
  m_initPz = value;
}

inline float JM::TrackElecTruth::initMass() const 
{
  return m_initMass;
}

inline void JM::TrackElecTruth::setInitMass(float value) 
{
  m_initMass = value;
}

inline const TimeStamp& JM::TrackElecTruth::trackTime() const 
{
  return m_trackTime;
}

inline void JM::TrackElecTruth::setTrackTime(const TimeStamp& value) 
{
  m_trackTime = value;
}


#endif ///SNIPER_ELECTRUTH_TRACKELECTRUTH_H
