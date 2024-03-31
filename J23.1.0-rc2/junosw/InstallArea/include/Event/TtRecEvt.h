
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

#ifndef SNIPER_RECEVENT_TTRECEVT_H
#define SNIPER_RECEVENT_TTRECEVT_H

// Include files
#include "Event/EventObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class TtRecEvt TtRecEvt.h
   *
   * Reconstructed TT track event. ChangeLog: v1) original version. v2) use float
 * instead of double. 
   *
   * @author 
   * created Sun Sep 24 17:52:51 2023
   * 
   */

  class TtRecEvt: public EventObject
  {
  private:

    int                m_NTotPoints; // total points
    std::vector<float> m_PointX;     // vector of point x
    std::vector<float> m_PointY;     // vector of point y
    std::vector<float> m_PointZ;     // vector of point z
    int                m_NTracks;    // total points
    std::vector<int>   m_NPoints;    // N points for each track
    std::vector<float> m_Coeff0;     // vector of Coeff0
    std::vector<float> m_Coeff1;     // vector of Coeff1
    std::vector<float> m_Coeff2;     // vector of Coeff2
    std::vector<float> m_Coeff3;     // vector of Coeff3
    std::vector<float> m_Coeff4;     // vector of Coeff4
    std::vector<float> m_Coeff5;     // vector of Coeff5
    std::vector<float> m_Chi2;       // vector of Chi2
  

  protected:


  public:

    /// Default Constructor
    TtRecEvt() : m_NTotPoints(0),
                 m_PointX(),
                 m_PointY(),
                 m_PointZ(),
                 m_NTracks(0),
                 m_NPoints(0),
                 m_Coeff0(),
                 m_Coeff1(),
                 m_Coeff2(),
                 m_Coeff3(),
                 m_Coeff4(),
                 m_Coeff5(),
                 m_Chi2() {}
  
    /// Default Destructor
    virtual ~TtRecEvt() {}
  
    /// Add a point to this event
    void addPoint(float x,
                  float y,
                  float z);
  
    /// Add a reconstructed track to this event
    void addTrack(int npoints,
                  float coeff[6],
                  float chi2);
  
    /// Retrieve const  
    /// total points
    int nTotPoints() const;
  
    /// Retrieve const  
    /// vector of point x
    const std::vector<float>& PointX() const;
  
    /// Retrieve const  
    /// vector of point y
    const std::vector<float>& PointY() const;
  
    /// Retrieve const  
    /// vector of point z
    const std::vector<float>& PointZ() const;
  
    /// Retrieve const  
    /// total points
    int nTracks() const;
  
    /// Retrieve const  
    /// N points for each track
    const std::vector<int>& nPoints() const;
  
    /// Retrieve const  
    /// vector of Coeff0
    const std::vector<float>& Coeff0() const;
  
    /// Retrieve const  
    /// vector of Coeff1
    const std::vector<float>& Coeff1() const;
  
    /// Retrieve const  
    /// vector of Coeff2
    const std::vector<float>& Coeff2() const;
  
    /// Retrieve const  
    /// vector of Coeff3
    const std::vector<float>& Coeff3() const;
  
    /// Retrieve const  
    /// vector of Coeff4
    const std::vector<float>& Coeff4() const;
  
    /// Retrieve const  
    /// vector of Coeff5
    const std::vector<float>& Coeff5() const;
  
    /// Retrieve const  
    /// vector of Chi2
    const std::vector<float>& Chi2() const;
  
    ClassDef(TtRecEvt,2)
  

  }; // class TtRecEvt

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline int JM::TtRecEvt::nTotPoints() const 
{
  return m_NTotPoints;
}

inline const std::vector<float>& JM::TtRecEvt::PointX() const 
{
  return m_PointX;
}

inline const std::vector<float>& JM::TtRecEvt::PointY() const 
{
  return m_PointY;
}

inline const std::vector<float>& JM::TtRecEvt::PointZ() const 
{
  return m_PointZ;
}

inline int JM::TtRecEvt::nTracks() const 
{
  return m_NTracks;
}

inline const std::vector<int>& JM::TtRecEvt::nPoints() const 
{
  return m_NPoints;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff0() const 
{
  return m_Coeff0;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff1() const 
{
  return m_Coeff1;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff2() const 
{
  return m_Coeff2;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff3() const 
{
  return m_Coeff3;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff4() const 
{
  return m_Coeff4;
}

inline const std::vector<float>& JM::TtRecEvt::Coeff5() const 
{
  return m_Coeff5;
}

inline const std::vector<float>& JM::TtRecEvt::Chi2() const 
{
  return m_Chi2;
}

inline void JM::TtRecEvt::addPoint(float x,
                                   float y,
                                   float z) 
{

  m_PointX.push_back(x);
  m_PointY.push_back(y);
  m_PointZ.push_back(z);

  ++m_NTotPoints;
                
}

inline void JM::TtRecEvt::addTrack(int npoints,
                                   float coeff[6],
                                   float chi2) 
{

  m_NPoints.push_back(npoints);
  m_Coeff0.push_back(coeff[0]);
  m_Coeff1.push_back(coeff[1]);
  m_Coeff2.push_back(coeff[2]);
  m_Coeff3.push_back(coeff[3]);
  m_Coeff4.push_back(coeff[4]);
  m_Coeff5.push_back(coeff[5]);
  m_Chi2.push_back(chi2);

  ++m_NTracks;
                
}


#endif ///SNIPER_RECEVENT_TTRECEVT_H
