//
//  Author: Zhengyun You  2013-11-20 
//

#include "Identifier/WpID.h"
#include <assert.h>
#include <iostream>

unsigned int WpID::MODULE_MAX = 65535;
unsigned int WpID::MODULE_20INCH_MIN = 0;
unsigned int WpID::MODULE_20INCH_MAX = 2307;

WpID::WpID(void)
{
}

WpID::~WpID(void)
{
}

//----------------------------------------------------------------------------
bool WpID::valuesOk ( const unsigned int module,
                       const unsigned int pmt
                     )
{
    // Check values
    //std::cout << " module = " << module << " pmt = " << pmt <<std::endl;

    if ( module  > MODULE_MAX)  return false;
    if ( pmt > MODULE_PMT_MAX)  return false;

    return true;
}

//----------------------------------------------------------------------------
int WpID::module (const Identifier& id)
{
    return ((id.getValue() & WpID::MODULE_MASK) >>  WpID::MODULE_INDEX);
}

//----------------------------------------------------------------------------
int WpID::pmt (const Identifier& id)
{
    return ((id.getValue() & WpID::PMT_MASK) >>  WpID::PMT_INDEX);
}

//----------------------------------------------------------------------------
bool WpID::is20inch (const Identifier& id)
{
    return (module(id) >= MODULE_20INCH_MIN && module(id) <= MODULE_20INCH_MAX);
}

//----------------------------------------------------------------------------
int WpID::moduleMax()
{
    return MODULE_MAX;
}

//----------------------------------------------------------------------------
int WpID::moduleMin()
{
    return MODULE_MIN;
}

//----------------------------------------------------------------------------
int WpID::modulePmtMax()
{
    return MODULE_PMT_MAX;
}

//----------------------------------------------------------------------------
int WpID::modulePmtMin()
{
    return MODULE_PMT_MIN;
}

//----------------------------------------------------------------------------
int WpID::module20inchMin()
{
    return MODULE_20INCH_MIN;
}

//----------------------------------------------------------------------------
int WpID::module20inchMax()
{
    return MODULE_20INCH_MAX;
}

//----------------------------------------------------------------------------
int WpID::module20inchNumber()
{
    return module20inchMax() - module20inchMin() + 1;
}

//----------------------------------------------------------------------------
Identifier WpID::id ( unsigned int module,
                      unsigned int pmt
                    )
{
    if (module >= MODULE_20INCH_SHIFT) {
        module = MODULE_20INCH_MIN + (module - MODULE_20INCH_SHIFT);
    }
    assert ( valuesOk(module, pmt) );
    unsigned int value = (JunoDetectorID::WP_ID << SUB_INDEX) |
                         (module << MODULE_INDEX) |
                         (pmt << PMT_INDEX);
    return Identifier(value);
}

//---------------------------------------------------------------------------- 
Identifier WpID::id(int value)
{
    return Identifier(value);
}

//---------------------------------------------------------------------------- 
unsigned int WpID::getIntID ( unsigned int module,
                              unsigned int pmt
                            )
{
    unsigned int value = (JunoDetectorID::WP_ID << SUB_INDEX) |
                         (module << MODULE_INDEX) |
                         (pmt << PMT_INDEX);
    return value;
}

