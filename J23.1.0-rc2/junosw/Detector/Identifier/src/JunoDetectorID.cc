//
//  Author: Zhengyun You  2013-11-20
//

#include "Identifier/JunoDetectorID.h"
#include <iostream>
#include <stdio.h>
#include <assert.h>

JunoDetectorID::JunoDetectorID(void) :
    m_CdId(JunoDetectorID::CD_ID),     
    m_WpId(JunoDetectorID::WP_ID),      	
    m_TtId(JunoDetectorID::TT_ID)	
{
}

JunoDetectorID::~JunoDetectorID(void)
{
}

bool JunoDetectorID::isCd (const Identifier& id)
{
    Identifier::value_type value = id.getValue(); 
    return ((value  &  SUB_MASK) >> SUB_INDEX) == CD_ID ? true : false;    
}

bool JunoDetectorID::isWp (const Identifier& id)
{
    Identifier::value_type value = id.getValue(); 
    return ((value  &  SUB_MASK) >> SUB_INDEX) == WP_ID ? true : false;
}

bool JunoDetectorID::isTt (const Identifier& id)
{
    Identifier::value_type value = id.getValue(); 
    return ((value  &  SUB_MASK) >> SUB_INDEX) == TT_ID ? true : false;   
}
 
Identifier JunoDetectorID::Cd(void) const
{
    Identifier id = Identifier(  m_CdId << SUB_INDEX );
    return id; 
}

Identifier JunoDetectorID::Wp(void) const
{
    Identifier id = Identifier(  m_WpId << SUB_INDEX );
    return id; 
}
  
Identifier JunoDetectorID::Tt(void) const
{
    Identifier id = Identifier(  m_TtId << SUB_INDEX );
    return id; 
}

