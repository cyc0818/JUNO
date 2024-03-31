#pragma once

#ifdef WITH_G4CXOPTICKS

#define OK_PHYSISIM_API  __attribute__ ((visibility ("default")))

//#define OK_PHYSISIM_LOG_( IDX ) {  OK_PHYSISIM_LOG::Initialize(plog::get<IDX>()->getMaxSeverity(), plog::get<IDX>(), nullptr ); }  
#define OK_PHYSISIM_LOG_( IDX ) {  OK_PHYSISIM_LOG::Init<IDX>( info, plog::get<IDX>(), nullptr ) ; }

struct OK_PHYSISIM_API OK_PHYSISIM_LOG 
{
    static void Initialize(int level, void* app1, void* app2 );
    static void Check(const char* msg);

    template<int IDX>
    static void Init(int level, void* app1, void* app2 );

};

#endif

