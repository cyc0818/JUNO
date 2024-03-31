// ./PMTParamData_test.sh 

#include "PMTParamData.h"
#include "_PMTParamData.h"

const char* FOLD = getenv("FOLD");
const char* LOAD = getenv("LOAD");


void test_roundtrip()
{
    std::cout << "test_roundtrip writing and loading from FOLD " << FOLD << std::endl ; 

    PMTParamData d0 ; 

    _PMTParamData _d0(d0) ; 
    _d0.populate(); 
    _d0.save(FOLD); 
    std::cout << "d0" << d0 ; 

    PMTParamData d1 ; 
    _PMTParamData _d1(d1) ; 
    _d1.load(FOLD); 

    std::cout << "d1" << d1 ; 

    PMTParamData d2(d1) ; 
    std::cout << "d2" << d2 ; 
}

void test_load()
{
    std::cout << "test_load from LOAD " << ( LOAD ? LOAD : "-" ) << std::endl ; 
    if(LOAD == nullptr) return ; 

    PMTParamData d0 ; 
    _PMTParamData _d0(d0) ; 
    _d0.load(LOAD); 
    std::cout << "d0" << d0 ; 
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
