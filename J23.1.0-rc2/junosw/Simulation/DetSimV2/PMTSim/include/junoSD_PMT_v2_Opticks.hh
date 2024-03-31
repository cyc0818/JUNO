#pragma once

#include <string>
class junoSD_PMT_v2 ;
struct junoSD_PMT_v2_Debug ;
class junoHit_PMT ;
class PMTHitMerger ; 

class G4HCofThisEvent ;


#ifdef WITH_G4CXOPTICKS 
#include "plog/Severity.h"
struct U4Hit ; 
struct U4HitExtra ; 
#endif


/**
junoSD_PMT_v2_Opticks
=======================

Invokes Opticks GPU optical photon simulation at the EndOfEvent.

**/
class junoSD_PMT_v2_Opticks 
{
    public:
        bool gpu_simulation() const ; 
        junoSD_PMT_v2_Opticks( junoSD_PMT_v2* jpmt);
        std::string desc() const ; 
        virtual ~junoSD_PMT_v2_Opticks();  
#ifdef WITH_G4CXOPTICKS
    public:
        static const plog::Severity LEVEL ; 
    public:
        void Initialize(G4HCofThisEvent*, G4int eventID);
        void EndOfEvent(G4HCofThisEvent*, G4int eventID);
    private:
        void EndOfEvent_Simulate(int eventID ); 
        void EndOfEvent_Debug(int eventID ); 
        void TerminateEvent();
    public:
        void collectHit(const U4Hit* hit, const U4HitExtra* hit_extra, int& merged_count, int& savehit_count );
        junoHit_PMT* convertHit(const U4Hit* hit, const U4HitExtra* hit_extra ) const ;
        std::string descHit(unsigned idx, const U4Hit* hit, const U4HitExtra* hit_extra ) const ;
#endif
    private: 
        junoSD_PMT_v2*        m_jpmt ; 
        junoSD_PMT_v2_Debug*  m_jpmt_dbg ; 
        int                   m_opticksMode ; 
        PMTHitMerger*         m_pmthitmerger_opticks ;
    private: 
        unsigned              m_event_total ; 
        unsigned              m_genstep_total ; 
        unsigned              m_photon_total ; 
        unsigned              m_hit_total ; 
        unsigned              m_merged_total ; 
        unsigned              m_savehit_total ; 
};


