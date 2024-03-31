#pragma once

#include <string>
#include <map>
class G4DataInterpolation ; 

class PMTEfficiency
{
   public:
        enum { NBIN = 9, N_PMTCAT = 5 } ; 
   private:
        static int s_pmt_categories[N_PMTCAT] ; 
        static double MISSING_EFFICIENCY ; 
   public:  
        static int* PMTCategories(); 
        static int  NumPMTCategories(); 
   private:
        static double s_theta_Unknown[NBIN] ; 
        static double s_ce_Unknown[NBIN] ; 

        static double s_theta_NNVT[NBIN] ; 
        static double s_ce_NNVT[NBIN] ; 

        static double s_theta_Hamamatsu[NBIN] ; 
        static double s_ce_Hamamatsu[NBIN] ; 

        static double s_theta_NNVT_HighQE[NBIN] ; 
        static double s_ce_NNVT_HighQE[NBIN] ; 

        static double s_theta_HZC[NBIN] ; 
        static double s_ce_HZC[NBIN] ; 
   public:
        static int     NumElements(int pmtcat); 
        static double* ThetaArray(int pmcat);
        static double* EfficiencyArray( int pmtcat );
        static int     FindIndex(double theta, int pmtcat, double epsilon); 
        static void    Dump(const char* msg="PMTEfficiency::Dump");
        static void    Scan(const char* msg="PMTEfficiency::Scan");
        static void    Dump(const char* msg, int pmtcat);
        static std::string PresentSample(int pmtcat, int idx);
   public:
        PMTEfficiency(); 
        double getEfficiency( double theta, int pmtcat ) const ;
   public:
        void   scan(const char* msg, int pmtcat) const ;
   private:
        int                  getLocateArgument(double theta, int pmtcat) const ; 
        G4DataInterpolation* getInterpolator(int pmtcat) const ;
       bool                  hasInterpolator(int pmtcat) const ;
   private:
        std::map<int, G4DataInterpolation*> m_di ; 
};


