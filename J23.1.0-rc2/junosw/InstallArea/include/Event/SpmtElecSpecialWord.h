
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

#ifndef SNIPER_ELECEVENT_SPMTELECSPECIALWORD_H
#define SNIPER_ELECEVENT_SPMTELECSPECIALWORD_H

// Include files
#include "TObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class SpmtElecSpecialWord SpmtElecSpecialWord.h
   *
   * SpmtElecSim information of Abc Card special word 
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:14 2023
   * 
   */

  class SpmtElecSpecialWord: public TObject
  {
  private:

    Char_t   m_ChNb;        // Channel Number within catiroc
    Char_t   m_AsicNb;      // Asic Number within board
    UChar_t  m_CardNb;      // card number
    UShort_t m_TriggerRate; // Trigger Rate
  

  protected:


  public:

    /// Default Constructor
    SpmtElecSpecialWord() : m_ChNb(),
                            m_AsicNb(),
                            m_CardNb(),
                            m_TriggerRate() {}
  
    /// Default Destructor
    virtual ~SpmtElecSpecialWord() {}
  
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
    /// Trigger Rate
    const UShort_t& TriggerRate() const;
  
    /// Update  
    /// Trigger Rate
    void setTriggerRate(const UShort_t& value);
  
    ClassDef(SpmtElecSpecialWord,1)
  

  }; // class SpmtElecSpecialWord

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const Char_t& JM::SpmtElecSpecialWord::ChNb() const 
{
  return m_ChNb;
}

inline void JM::SpmtElecSpecialWord::setChNb(const Char_t& value) 
{
  m_ChNb = value;
}

inline const Char_t& JM::SpmtElecSpecialWord::AsicNb() const 
{
  return m_AsicNb;
}

inline void JM::SpmtElecSpecialWord::setAsicNb(const Char_t& value) 
{
  m_AsicNb = value;
}

inline const UChar_t& JM::SpmtElecSpecialWord::CardNb() const 
{
  return m_CardNb;
}

inline void JM::SpmtElecSpecialWord::setCardNb(const UChar_t& value) 
{
  m_CardNb = value;
}

inline const UShort_t& JM::SpmtElecSpecialWord::TriggerRate() const 
{
  return m_TriggerRate;
}

inline void JM::SpmtElecSpecialWord::setTriggerRate(const UShort_t& value) 
{
  m_TriggerRate = value;
}


#endif ///SNIPER_ELECEVENT_SPMTELECSPECIALWORD_H
