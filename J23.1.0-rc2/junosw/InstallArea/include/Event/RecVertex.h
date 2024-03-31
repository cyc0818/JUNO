
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

#ifndef SNIPER_RECEVENT_RECVERTEX_H
#define SNIPER_RECEVENT_RECVERTEX_H

// Include files
#include "TObject.h"
#include <string>

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class RecVertex RecVertex.h
   *
   * Reconstructed vertex class. ChangeLog: v1) original version. v2) change from
 * double to float. 
   *
   * @author ZHANG Kun
   * created Sun Sep 24 17:52:48 2023
   * 
   */

  class RecVertex: public TObject
  {
  private:

    float m_PESum;           // The total number of p.e.
    float m_energy;          // Best estimation of deposit energy. Unit:MeV
    float m_eprec;           // Reconstructed positron energy. Unit:MeV
    float m_x;               // x position. Unit:mm
    float m_y;               // y position. Unit:mm
    float m_z;               // z position. Unit:mm
    float m_px;              // x direction. Unit:mm
    float m_py;              // y direction. Unit:mm
    float m_pz;              // z direction. Unit:mm
    float m_t0;              // start time. Unit:ns
    float m_chisq;           // goodness of the fit
    float m_energyQuality;   // quality of energy recontruction
    float m_positionQuality; // quality of position recontruction
  

  protected:


  public:

    /// Default Constructor
    RecVertex() : m_PESum(0.0),
                  m_energy(0.0),
                  m_eprec(0.0),
                  m_x(0.0),
                  m_y(0.0),
                  m_z(0.0),
                  m_px(0.0),
                  m_py(0.0),
                  m_pz(0.0),
                  m_t0(0.0),
                  m_chisq(0.0),
                  m_energyQuality(0.0),
                  m_positionQuality(0.0) {}
  
    /// Default Destructor
    virtual ~RecVertex() {}
  
    /// Retrieve const  
    /// The total number of p.e.
    float peSum() const;
  
    /// Update  
    /// The total number of p.e.
    void setPESum(float value);
  
    /// Retrieve const  
    /// Best estimation of deposit energy. Unit:MeV
    float energy() const;
  
    /// Update  
    /// Best estimation of deposit energy. Unit:MeV
    void setEnergy(float value);
  
    /// Retrieve const  
    /// Reconstructed positron energy. Unit:MeV
    float eprec() const;
  
    /// Update  
    /// Reconstructed positron energy. Unit:MeV
    void setEprec(float value);
  
    /// Retrieve const  
    /// x position. Unit:mm
    float x() const;
  
    /// Update  
    /// x position. Unit:mm
    void setX(float value);
  
    /// Retrieve const  
    /// y position. Unit:mm
    float y() const;
  
    /// Update  
    /// y position. Unit:mm
    void setY(float value);
  
    /// Retrieve const  
    /// z position. Unit:mm
    float z() const;
  
    /// Update  
    /// z position. Unit:mm
    void setZ(float value);
  
    /// Retrieve const  
    /// x direction. Unit:mm
    float px() const;
  
    /// Update  
    /// x direction. Unit:mm
    void setPx(float value);
  
    /// Retrieve const  
    /// y direction. Unit:mm
    float py() const;
  
    /// Update  
    /// y direction. Unit:mm
    void setPy(float value);
  
    /// Retrieve const  
    /// z direction. Unit:mm
    float pz() const;
  
    /// Update  
    /// z direction. Unit:mm
    void setPz(float value);
  
    /// Retrieve const  
    /// start time. Unit:ns
    float t0() const;
  
    /// Update  
    /// start time. Unit:ns
    void setT0(float value);
  
    /// Retrieve const  
    /// goodness of the fit
    float chisq() const;
  
    /// Update  
    /// goodness of the fit
    void setChisq(float value);
  
    /// Retrieve const  
    /// quality of energy recontruction
    float energyQuality() const;
  
    /// Update  
    /// quality of energy recontruction
    void setEnergyQuality(float value);
  
    /// Retrieve const  
    /// quality of position recontruction
    float positionQuality() const;
  
    /// Update  
    /// quality of position recontruction
    void setPositionQuality(float value);
  
    ClassDef(RecVertex,2)
  

  }; // class RecVertex

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline float JM::RecVertex::peSum() const 
{
  return m_PESum;
}

inline void JM::RecVertex::setPESum(float value) 
{
  m_PESum = value;
}

inline float JM::RecVertex::energy() const 
{
  return m_energy;
}

inline void JM::RecVertex::setEnergy(float value) 
{
  m_energy = value;
}

inline float JM::RecVertex::eprec() const 
{
  return m_eprec;
}

inline void JM::RecVertex::setEprec(float value) 
{
  m_eprec = value;
}

inline float JM::RecVertex::x() const 
{
  return m_x;
}

inline void JM::RecVertex::setX(float value) 
{
  m_x = value;
}

inline float JM::RecVertex::y() const 
{
  return m_y;
}

inline void JM::RecVertex::setY(float value) 
{
  m_y = value;
}

inline float JM::RecVertex::z() const 
{
  return m_z;
}

inline void JM::RecVertex::setZ(float value) 
{
  m_z = value;
}

inline float JM::RecVertex::px() const 
{
  return m_px;
}

inline void JM::RecVertex::setPx(float value) 
{
  m_px = value;
}

inline float JM::RecVertex::py() const 
{
  return m_py;
}

inline void JM::RecVertex::setPy(float value) 
{
  m_py = value;
}

inline float JM::RecVertex::pz() const 
{
  return m_pz;
}

inline void JM::RecVertex::setPz(float value) 
{
  m_pz = value;
}

inline float JM::RecVertex::t0() const 
{
  return m_t0;
}

inline void JM::RecVertex::setT0(float value) 
{
  m_t0 = value;
}

inline float JM::RecVertex::chisq() const 
{
  return m_chisq;
}

inline void JM::RecVertex::setChisq(float value) 
{
  m_chisq = value;
}

inline float JM::RecVertex::energyQuality() const 
{
  return m_energyQuality;
}

inline void JM::RecVertex::setEnergyQuality(float value) 
{
  m_energyQuality = value;
}

inline float JM::RecVertex::positionQuality() const 
{
  return m_positionQuality;
}

inline void JM::RecVertex::setPositionQuality(float value) 
{
  m_positionQuality = value;
}


#endif ///SNIPER_RECEVENT_RECVERTEX_H
