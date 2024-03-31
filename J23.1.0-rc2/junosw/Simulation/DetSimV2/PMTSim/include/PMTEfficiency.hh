#pragma once

#include <string>
#include <map>
class G4DataInterpolation ; 

/**
PMTEfficiency
==============

**CAUTION : there are two different PMT category enums in use**

parcat 
    Geometry/PMTParamSvc.h with kPMT_Unknown, ...
simcat
    IPMTSimParamSvc/IPMTSimParamSvc.h with kPMTType_Unknown,...

The enum category type that the *cat* arguments to PMTEfficiency 
are assumed to be using can be changed using::

    PMTEfficiency* eff = PMTEfficiency::Make('S') ;  // S:simcat    

* Note that this setting cannot be changed after instanciation.
* The results obtained from all PMTEfficiency methods with a *cat* argument 
  will depend on CATEGORY_TYPE setting. 

**/

class PMTEfficiency
{
   public:
        static PMTEfficiency* Make();
   public:
        enum { NBIN = 9, N_PMTCAT = 5 } ; 
   private:
        // static char    CATEGORY_TYPE ;  
        static PMTEfficiency* INSTANCE ; 
        static int s_pmt_categories_parcat[N_PMTCAT] ; 
//         static int s_pmt_categories_simcat[N_PMTCAT] ; 
        static double MISSING_EFFICIENCY ; 
//    public:  
//         // SimToPar and ParToSim methods do not depend on CATEGORY_TYPE
//         static int SimToPar(int simcat); 
//         static int ParToSim(int parcat); 
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

        static double s_theta_NNVT_High[NBIN] ; 
        static double s_ce_NNVT_High[NBIN] ; 

        static double s_theta_HZC[NBIN] ; 
        static double s_ce_HZC[NBIN] ; 
   private:
        static double s_theta_Hamamatsu_unused[NBIN] ; 
        static double s_ce_Hamamatsu_unused[NBIN] ; 

        static double s_theta_NNVT_unused[NBIN] ; 
        static double s_ce_NNVT_unused[NBIN] ; 
   private:
        //static void    SetPMTCategoryType( char category_type );   // MUST be invoked before instanciation with 'P' or 'S' argument 
   public:
        //static char    GetPMTCategoryType(); 
        static int     NumElements(int cat);   
        static double* ThetaArray(int cat);         
        static double* EfficiencyArray( int cat );   
        static int     FindIndex(double theta, int cat, double epsilon); 
        static void    Dump(const char* msg="PMTEfficiency::Dump");
        static void    Scan(const char* msg="PMTEfficiency::Scan");
        static void    Dump(const char* msg, int cat);
        static std::string PresentSample(int cat, int idx);
   private:
        PMTEfficiency(); 
   public:
        double getEfficiency( double theta, int cat ) const ;
        void   scan(const char* msg, int cat) const ;
   private:
        int                  getLocateArgument(double theta, int cat) const ; 
        G4DataInterpolation* getInterpolator(int cat) const ;
       bool                  hasInterpolator(int cat) const ;
   private:
        std::map<int, G4DataInterpolation*> m_di ; 

};


