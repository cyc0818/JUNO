//
//  Author: Zhengyun You  2013-11-20
//

#include "Identifier/CdID.h"
#include "Identifier/IDService.h"
#include <assert.h>
#include <iostream>

unsigned int CdID::MODULE_MAX = 65535;  
unsigned int CdID::MODULE_20INCH_MIN = 0;
unsigned int CdID::MODULE_20INCH_MAX = 17612; // 17738; // 17745; // findPmt20inchNum()-1
unsigned int CdID::MODULE_3INCH_MIN  = 17613; // 17739; // 17746; // findPmt20inchNum()
unsigned int CdID::MODULE_3INCH_MAX  = 47485; // 54310; // 54317; // findPmt20inchNum()+findPmt3inchNum()-1


//extern Identifier getPmtId(int Copy_ID);


// ------some value, used to test new Identifier---------------------


CdID::CdID(void)
{
}

CdID::~CdID(void)
{
}

//----------------------------------------------------------------------------
bool CdID::valuesOk ( const unsigned int module,  
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
int CdID::module (const Identifier& id)
{
    IDService* idServ = IDService::getIdServ();
    //std::cout << "wuchxdbug IDServ1" << idServ << std::endl;
    idServ -> init();
    return (idServ -> id2CopyNo(id));
    //return ((id.getValue() & CdID::MODULE_MASK) >>  CdID::MODULE_INDEX);    
}


//----------------------------------------------------------------------------

bool CdID::is20inch (const Identifier& id)
{
    return (! CdID::largeOrSmall (id) );  // 0 is large; 1 is small;
    // return (module(id) >= MODULE_20INCH_MIN && module(id) <= MODULE_20INCH_MAX);
}

//----------------------------------------------------------------------------
bool CdID::is3inch (const Identifier& id)
{
    return (CdID::largeOrSmall (id));
    //return (module(id) >= MODULE_3INCH_MIN && module(id) <= MODULE_3INCH_MAX);
}

bool CdID::isNorth (const Identifier& id)
{
    return (! CdID::northOrSouth (id) );  // 0 is large; 1 is small;
}

//----------------------------------------------------------------------------
bool CdID::isSouth (const Identifier& id)
{
    return (CdID::northOrSouth (id));
}

//----------------------------------------------------------------------------
int CdID::moduleMax()
{
    return MODULE_MAX;
}

//----------------------------------------------------------------------------
int CdID::moduleMin()
{
    return MODULE_MIN;
}

//----------------------------------------------------------------------------
int CdID::modulePmtMax()
{
    return MODULE_PMT_MAX;
}

//----------------------------------------------------------------------------
int CdID::modulePmtMin()
{
    return MODULE_PMT_MIN;
}

//----------------------------------------------------------------------------
int CdID::module20inchMin()
{
    return MODULE_20INCH_MIN;
}

//----------------------------------------------------------------------------
int CdID::module20inchMax()
{
    return MODULE_20INCH_MAX; 
}

//----------------------------------------------------------------------------
int CdID::module3inchMin()
{
    return MODULE_3INCH_MIN; 
}

//----------------------------------------------------------------------------
int CdID::module3inchMax()
{
    return MODULE_3INCH_MAX;
}

//----------------------------------------------------------------------------
int CdID::module20inchNumber() 
{
    return module20inchMax() - module20inchMin() + 1;
}

//----------------------------------------------------------------------------
int CdID::module3inchNumber() 
{ 
    return module3inchMax() - module3inchMin() + 1;
}

//----------------------------------------------------------------------------
// Identifier CdID::id ( unsigned int NorthOrSouth,
//                       unsigned int module,
//                       unsigned int pmt
//                     )
Identifier CdID::id ( unsigned int module,
                      unsigned int pmt
                    )
{
    
    if (module >= MODULE_3INCH_SHIFT) {
        module = MODULE_3INCH_MIN + (module - MODULE_3INCH_SHIFT);
    }
 
    assert ( valuesOk(module, pmt) );

    // unsigned int value = (JunoDetectorID::CD_ID << CD_INDEX) | 
    //                      (module << MODULE_INDEX) |
    //                      (pmt << PMT_INDEX);

    
    //return Identifier(value); 
    //std::cout << module <<"  " << MODULE_3INCH_MIN << std::endl;
    IDService* idServ = IDService::getIdServ();
    idServ -> init();

    // std::cout << "the path of singletom is " << idServ << std::endl;

    return idServ -> copyNo2Id(module); 
    

}


//---------------------------------------------------------------------------
Identifier CdID::id ( unsigned int LARGEORSMALL,
                      unsigned int NORTHORSOUTH,
                      unsigned int CIRCLENO,
                      unsigned int POSITIONNO,
                      unsigned int UPORDOWN,
                      unsigned int PMTTYPE
                    )
{


    // if (module >= MODULE_3INCH_SHIFT) {
    //     module = MODULE_3INCH_MIN + (module - MODULE_3INCH_SHIFT);
    // }
    //assert ( valuesOk(module, pmt) ); 
    unsigned int value = (JunoDetectorID::CD_ID << SUB_INDEX) | 
                         (LARGEORSMALL << LargeOrSmall_INDEX) |
                         (NORTHORSOUTH << NorthOrSouth_INDEX) |
                         (CIRCLENO << Circle_INDEX) |
                         (POSITIONNO << Position_INDEX) |
                         (UPORDOWN << UpOrDown_INDEX) |
                         (PMTTYPE << PmtType_INDEX);


    return Identifier(value); 
}



//----------------------------------------------------------------------------
Identifier CdID::id(unsigned int value)
{
    return Identifier(value);
}

// //----------------------------------------------------------------------------
// unsigned int CdID::getIntID ( unsigned int module,
//                               unsigned int pmt
//                             )
// {
//     unsigned int value = (JunoDetectorID::CD_ID << CD_INDEX) | 
//                          (module << MODULE_INDEX) |
//                          (pmt << PMT_INDEX);
//     return value;
// }


//
//-----------------------ADD BY WUCHX 2022-10-7-----------------
int CdID::largeOrSmall (const Identifier& id)    //judge pmt is 20inch (large) or 3inch (small); 0 is 20inch, 1 is 3inch;
{
    return ((id.getValue() & CdID::LargeOrSmall_MASK) >>  LargeOrSmall_INDEX);
}

int CdID::northOrSouth (const Identifier& id)    //judge pmt is in north or south; 0 is north,1 is south
{
    return ((id.getValue() & CdID::NorthOrSouth_MASK) >>  NorthOrSouth_INDEX);
}

int CdID::circleNo (const Identifier& id)   //get the cilcle number of PMT;
{
    return ((id.getValue() & CdID::Circle_MASK) >>  Circle_INDEX);
}

int CdID::positionNo (const Identifier& id)  //get the position number of PMT;
{
    return ((id.getValue() & CdID::Positon_MASK) >>  Position_INDEX);
}

int CdID::upOrDown (const Identifier& id)    //judge small pmt is up or down; 0 is up, 1 is down;
{
    return ((id.getValue() & CdID::UpOrDown_MASK) >>  UpOrDown_INDEX);
}

int CdID::pmtType (const Identifier& id)    //get the pmt type; 1 is hamamasut, 2 is NVVT, 3 is small pmt
{
    return ((id.getValue() & CdID::PmtType_MASK) >>  PmtType_INDEX);
}







