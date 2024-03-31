#ifdef WITH_G4CXOPTICKS
#include <plog/Log.h>
#include "SLOG_INIT.hh"
#include "SLOG.hh"

#include "PhysiSim/OK_PHYSISIM_LOG.hh"

    
void OK_PHYSISIM_LOG::Initialize(int level, void* app1, void* app2 )
{
    SLOG_INIT(level, app1, app2);
}
void OK_PHYSISIM_LOG::Check(const char* msg)
{
    SLOG_CHECK(msg);
}


template<int IDX>
void OK_PHYSISIM_LOG::Init(int level, void* app1, void* app2 )
{
    SLOG_INIT_(level, app1, app2, IDX );  
}

template void OK_PHYSISIM_LOG::Init<0>( int, void*, void* ); 
template void OK_PHYSISIM_LOG::Init<1>( int, void*, void* ); 


#endif

