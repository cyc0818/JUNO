
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

#ifndef SNIPER_ELECEVENT_SPMTELECDISCRWORD_H
#define SNIPER_ELECEVENT_SPMTELECDISCRWORD_H

// Include files
#include "TObject.h"

// Forward declarations

namespace JM 
{

  // Forward declarations

  /** @class SpmtElecDiscrWord SpmtElecDiscrWord.h
   *
   * SpmtElec information of Abc Card discriminator word 
   *
   * @author Pietro Chimenti
   * created Sun Sep 24 17:52:13 2023
   * 
   */

  class SpmtElecDiscrWord: public TObject
  {
  private:

    Char_t  m_ChNb;       // Channel Number within catiroc
    Char_t  m_AsicNb;     // Asic Number within board
    UChar_t m_CardNb;     // card number
    Bool_t  m_Edge;       // Edge (rising or lowering)
    Int_t   m_EvtCounter; // Event Counter as within Abc Board
    Int_t   m_Time;       // Time Stamp as within Abc Board
    Char_t  m_FineTime;   // Fine time of discriminator word
  

  protected:


  public:

    /// Default Constructor
    SpmtElecDiscrWord() : m_ChNb(),
                          m_AsicNb(),
                          m_CardNb(),
                          m_Edge(),
                          m_EvtCounter(),
                          m_Time(),
                          m_FineTime() {}
  
    /// Default Destructor
    virtual ~SpmtElecDiscrWord() {}
  
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
    /// Edge (rising or lowering)
    const Bool_t& Edge() const;
  
    /// Update  
    /// Edge (rising or lowering)
    void setEdge(const Bool_t& value);
  
    /// Retrieve const  
    /// Event Counter as within Abc Board
    const Int_t& EvtCounter() const;
  
    /// Update  
    /// Event Counter as within Abc Board
    void setEvtCounter(const Int_t& value);
  
    /// Retrieve const  
    /// Time Stamp as within Abc Board
    const Int_t& Time() const;
  
    /// Update  
    /// Time Stamp as within Abc Board
    void setTime(const Int_t& value);
  
    /// Retrieve const  
    /// Fine time of discriminator word
    const Char_t& FineTime() const;
  
    /// Update  
    /// Fine time of discriminator word
    void setFineTime(const Char_t& value);
  
    ClassDef(SpmtElecDiscrWord,1)
  

  }; // class SpmtElecDiscrWord

} // namespace JM;

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const Char_t& JM::SpmtElecDiscrWord::ChNb() const 
{
  return m_ChNb;
}

inline void JM::SpmtElecDiscrWord::setChNb(const Char_t& value) 
{
  m_ChNb = value;
}

inline const Char_t& JM::SpmtElecDiscrWord::AsicNb() const 
{
  return m_AsicNb;
}

inline void JM::SpmtElecDiscrWord::setAsicNb(const Char_t& value) 
{
  m_AsicNb = value;
}

inline const UChar_t& JM::SpmtElecDiscrWord::CardNb() const 
{
  return m_CardNb;
}

inline void JM::SpmtElecDiscrWord::setCardNb(const UChar_t& value) 
{
  m_CardNb = value;
}

inline const Bool_t& JM::SpmtElecDiscrWord::Edge() const 
{
  return m_Edge;
}

inline void JM::SpmtElecDiscrWord::setEdge(const Bool_t& value) 
{
  m_Edge = value;
}

inline const Int_t& JM::SpmtElecDiscrWord::EvtCounter() const 
{
  return m_EvtCounter;
}

inline void JM::SpmtElecDiscrWord::setEvtCounter(const Int_t& value) 
{
  m_EvtCounter = value;
}

inline const Int_t& JM::SpmtElecDiscrWord::Time() const 
{
  return m_Time;
}

inline void JM::SpmtElecDiscrWord::setTime(const Int_t& value) 
{
  m_Time = value;
}

inline const Char_t& JM::SpmtElecDiscrWord::FineTime() const 
{
  return m_FineTime;
}

inline void JM::SpmtElecDiscrWord::setFineTime(const Char_t& value) 
{
  m_FineTime = value;
}


#endif ///SNIPER_ELECEVENT_SPMTELECDISCRWORD_H
