#pragma once
#ifdef WITH_G4OPTICKS

#include "G4ThreeVector.hh"
#include <string>
#include <vector>

class junoSD_PMT_v2 ;

class PMTEfficiencyCheck 
{
    public:
        PMTEfficiencyCheck(const junoSD_PMT_v2* jpmt);
        virtual ~PMTEfficiencyCheck();
    public:
        int getVolnameIndex(const std::string& volname) const ;

    public:
        void addHitRecord(
            int pmtID,
            const G4ThreeVector& global_pos, 
            const G4ThreeVector& local_pos, 
            double qe, 
            double ce, 
            double de, 
            const std::string& volname, 
            int ce_cat 
            );

        void Initialize(int eventID) ;
        void EndOfEvent(int eventID) ;
        void SaveRecords(int eventID);
    private:
        void init(); 
    private:
        const junoSD_PMT_v2*     m_jpmt ; 
        const char*              m_basedir ; 
        bool                     m_dump ; 

        bool                     m_assert_match ; 
        unsigned                 m_record_count ; 
        unsigned                 m_record_limit ; 
        bool                     m_record_saved ; 
        int                      m_eventID ;          

        std::vector<std::string> m_uvolname ; 
        std::vector<std::string> m_volnames ; 
        std::vector<double>      m_hitrecords ;
        std::vector<std::string> m_names ;  

        unsigned                 m_parcat_mismatch_count ; 
        unsigned                 m_simcat_mismatch_count ; 
};

#endif
