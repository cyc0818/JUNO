//  ./PMTSimParamData_test.sh

#include <iostream>
#include "PMTSimParamData.h"
#include "_PMTSimParamData.h"


const char* FOLD = getenv("FOLD"); 
const char* LOAD = getenv("LOAD") ; 

void test_roundtrip()
{
    std::cout << "test_roundtrip FOLD " << FOLD << std::endl ; 

    PMTSimParamData d0 ; 
    _PMTSimParamData _d0(d0) ; 

    _d0.populate(); 
    std::cout << "d0" << d0 ; 
    _d0.save(FOLD); 

     

    PMTSimParamData d1 ; 
    _PMTSimParamData _d1(d1) ; 
    _d1.load(FOLD); 
    std::cout << "d1" << d1 ; 


    double d1_v0 = d1.get_pmtcat_const_prop(1, "HELLO") ; 
    std::cout << " d1_v0 " << d1_v0 << std::endl ; 

    double d1_v1 = d1.get_pmtcat_prop(1, "ARC_KINDEX", 2.*eV );  
    std::cout << " d1_v1 " << d1_v1 << std::endl ; 

    PMTSimParamData d2(d1) ; 
    std::cout << "d2" << d2 ; 

    NP* scan = _PMTSimParamData::Scan_pmtid_qe<PMTSimParamData>(&d2, 100, true);
    std::cout << " scan " << scan->sstr() << std::endl ; 
    scan->save(FOLD, "scan.npy"); 



}


void test_load()
{
    std::cout << "test_load LOAD " << ( LOAD ? LOAD : "-" ) << std::endl ; 
    if(LOAD == nullptr) return ; 

    PMTSimParamData d0 ; 
    _PMTSimParamData _d0(d0) ; 
    _d0.load(LOAD);
 
    std::cout << "d0" << d0 ; 

    double en = 5.0*CLHEP::eV ; 
    double qe0 = d0.get_pmtid_qe( 0, en );  
    std::cout << " en " << en << " qe0 " << qe0 << std::endl ;  
    double qe30k = d0.get_pmtid_qe( 30000, en );  
    std::cout << " en " << en << " qe30k " << qe30k << std::endl ;  

    NP* scan = _PMTSimParamData::Scan_pmtid_qe<PMTSimParamData>(&d0, 100, true );
    std::cout << " scan " << scan->sstr() << std::endl ; 
    scan->save(FOLD, "scan_PMTSimParamData.npy"); 

}



int main(int argc, char** argv)
{
    if(LOAD)
    {
        test_load(); 
    }
    else
    {
        test_roundtrip(); 
    }
    return 0 ; 
}
