
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

#ifndef SNIPER_ELECEVENT_SPMTELECABCBLOCK_H
#define SNIPER_ELECEVENT_SPMTELECABCBLOCK_H

// Include files
#include "TObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class SpmtElecAbcBlock SpmtElecAbcBlock.h
   *
   * SpmtElecSim information of Abc Card data block 
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:12 2023
   * 
   */

  class SpmtElecAbcBlock: public TObject
  {
  private:

    Char_t   m_ChNb;         // Channel Number within catiroc
    Char_t   m_AsicNb;       // Asic Number within board
    UChar_t  m_CardNb;       // card number
    Bool_t   m_Gain;         // High or Low catiroc gain
    Double_t m_CoarseTime;   // 4-byte coarse time info
    UShort_t m_EventCounter; // Per channel Catiroc Event counter
    UShort_t m_FineTime;     // 2-byte fine time info
    UShort_t m_Charge;       // 2-byte charge info
  

  protected:


  public:

    /// Default Constructor
    SpmtElecAbcBlock() : m_ChNb(),
                         m_AsicNb(),
                         m_CardNb(),
                         m_Gain(),
                         m_CoarseTime(),
                         m_EventCounter(),
                         m_FineTime(),
                         m_Charge() {}
  
    /// Default Destructor
    virtual ~SpmtElecAbcBlock() {}
  
    /// Retrieve const  
    /// Channel Number within catiroc
    const Char_t& ChNb() const;
  
    /// Update  
    /// Channel Number within catiroc
    void setChNb(const Char_t& value);
  
    /// Retrieve const  
    /// Asic Number within board
    const Char_t& AsicNb() const;
  
    /// Update  
    /// Asic Number within board
    void setAsicNb(const Char_t& value);
  
    /// Retrieve const  
    /// card number
    const UChar_t& CardNb() const;
  
    /// Update  
    /// card number
    void setCardNb(const UChar_t& value);
  
    /// Retrieve const  
    /// High or Low catiroc gain
    const Bool_t& Gain() const;
  
    /// Update  
    /// High or Low catiroc gain
    void setGain(const Bool_t& value);
  
    /// Retrieve const  
    /// 4-byte coarse time info
    const Double_t& CoarseTime() const;
  
    /// Update  
    /// 4-byte coarse time info
    void setCoarseTime(const Double_t& value);
  
    /// Retrieve const  
    /// Per channel Catiroc Event counter
    const UShort_t& EventCounter() const;
  
    /// Update  
    /// Per channel Catiroc Event counter
    void setEventCounter(const UShort_t& value);
  
    /// Retrieve const  
    /// 2-byte fine time info
    const UShort_t& FineTime() const;
  
    /// Update  
    /// 2-byte fine time info
    void setFineTime(const UShort_t& value);
  
    /// Retrieve const  
    /// 2-byte charge info
    const UShort_t& Charge() const;
  
    /// Update  
    /// 2-byte charge info
    void setCharge(const UShort_t& value);
  
    ClassDef(SpmtElecAbcBlock,1)
  

  }; // class SpmtElecAbcBlock

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const Char_t& JM::SpmtElecAbcBlock::ChNb() const 
{
  return m_ChNb;
}

inline void JM::SpmtElecAbcBlock::setChNb(const Char_t& value) 
{
  m_ChNb = value;
}

inline const Char_t& JM::SpmtElecAbcBlock::AsicNb() const 
{
  return m_AsicNb;
}

inline void JM::SpmtElecAbcBlock::setAsicNb(const Char_t& value) 
{
  m_AsicNb = value;
}

inline const UChar_t& JM::SpmtElecAbcBlock::CardNb() const 
{
  return m_CardNb;
}

inline void JM::SpmtElecAbcBlock::setCardNb(const UChar_t& value) 
{
  m_CardNb = value;
}

inline const Bool_t& JM::SpmtElecAbcBlock::Gain() const 
{
  return m_Gain;
}

inline void JM::SpmtElecAbcBlock::setGain(const Bool_t& value) 
{
  m_Gain = value;
}

inline const Double_t& JM::SpmtElecAbcBlock::CoarseTime() const 
{
  return m_CoarseTime;
}

inline void JM::SpmtElecAbcBlock::setCoarseTime(const Double_t& value) 
{
  m_CoarseTime = value;
}

inline const UShort_t& JM::SpmtElecAbcBlock::EventCounter() const 
{
  return m_EventCounter;
}

inline void JM::SpmtElecAbcBlock::setEventCounter(const UShort_t& value) 
{
  m_EventCounter = value;
}

inline const UShort_t& JM::SpmtElecAbcBlock::FineTime() const 
{
  return m_FineTime;
}

inline void JM::SpmtElecAbcBlock::setFineTime(const UShort_t& value) 
{
  m_FineTime = value;
}

inline const UShort_t& JM::SpmtElecAbcBlock::Charge() const 
{
  return m_Charge;
}

inline void JM::SpmtElecAbcBlock::setCharge(const UShort_t& value) 
{
  m_Charge = value;
}


#endif ///SNIPER_ELECEVENT_SPMTELECABCBLOCK_H
