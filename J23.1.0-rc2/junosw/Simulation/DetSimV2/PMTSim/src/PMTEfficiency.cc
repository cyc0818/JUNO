#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>

//#include "IPMTSimParamSvc/IPMTSimParamSvc.h"  // simcat category enum    
#include "Geometry/IPMTParamSvc.h"             // parcat category enum

#include "G4DataInterpolation.hh"
#include "G4SystemOfUnits.hh"
using CLHEP::deg ; 

#include "PMTEfficiency.hh"


PMTEfficiency* PMTEfficiency::Make() // static
{
    //PMTEfficiency::SetPMTCategoryType(category_type);  // S:simcat P:parcat
    PMTEfficiency* eff = new PMTEfficiency ; 
    return eff ; 
}


// int PMTEfficiency::SimToPar(int simcat)  // static 
// {
//     int parcat = kPMT_Unknown ; 
//     switch(simcat)
//     {    
//         case kPMTType_Unknown:    parcat = kPMT_Unknown     ; break ; 
//         case kPMTType_NormalNNVT: parcat = kPMT_NNVT        ; break ; 
//         case kPMTType_Hamamatsu:  parcat = kPMT_Hamamatsu   ; break ; 
//         case kPMTType_HZC:        parcat = kPMT_HZC         ; break ; 
//         case kPMTType_HiQENNVT:   parcat = kPMT_NNVT_HighQE ; break ; 
//     }    
//     return parcat ; 
// }

// int PMTEfficiency::ParToSim(int parcat)  // static 
// {
//     int simcat = kPMTType_Unknown ; 
//     switch(parcat)
//     {    
//         case kPMT_Unknown:     simcat = kPMTType_Unknown    ; break ; 
//         case kPMT_NNVT:        simcat = kPMTType_NormalNNVT ; break ; 
//         case kPMT_Hamamatsu:   simcat = kPMTType_Hamamatsu  ; break ; 
//         case kPMT_HZC:         simcat = kPMTType_HZC        ; break ; 
//         case kPMT_NNVT_HighQE: simcat = kPMTType_HiQENNVT   ; break ; 
//     }    
//     return simcat ; 
// }



PMTEfficiency* PMTEfficiency::INSTANCE = nullptr ; 
double PMTEfficiency::MISSING_EFFICIENCY = -999.0 ; 
// char PMTEfficiency::CATEGORY_TYPE      = '?' ; 

/**
PMTEfficiency::SetPMTCategoryType
------------------------------------

This static method must be invoked prior to instanciating the PMTEfficiency object.
Calling this after instanciation will fail with an assert.

**/
// void  PMTEfficiency::SetPMTCategoryType( char category_type )  // static 
// {
//     assert( category_type == 'P' || category_type == 'S' ); 
//     assert( INSTANCE == nullptr ) ; 
//     CATEGORY_TYPE = category_type ; 
// }
// char PMTEfficiency::GetPMTCategoryType()
// { 
//     return CATEGORY_TYPE ; 
// }




int PMTEfficiency::s_pmt_categories_parcat[N_PMTCAT] = {kPMT_Unknown,     kPMT_NNVT,          kPMT_Hamamatsu,    kPMT_HZC,            kPMT_NNVT_HighQE } ; 
// int PMTEfficiency::s_pmt_categories_simcat[N_PMTCAT] = {kPMTType_Unknown, kPMTType_Hamamatsu, kPMTType_HiQENNVT, kPMTType_NormalNNVT, kPMTType_HZC } ; 

// /**
// PMTEfficiency::PMTCategories
// ------------------------------

// The array of enum values returned defines the order of items in the PMTEfficiencyTable and 
// hence in the GPU texture. The order is arranged to be in ascending enum value order. 

// **/
int* PMTEfficiency::PMTCategories()
{
    int* cats = NULL ; 
    // switch(CATEGORY_TYPE)
    // {
    //     case 'P': cats = s_pmt_categories_parcat ; break ;  
    //     case 'S': cats = s_pmt_categories_simcat ; break ;  
    //     default : assert(0 && "must PMTEfficiency::SetPMTCategoryType before instanciating PMTEfficiency" ) ; break ; 
    // }
    cats = s_pmt_categories_parcat ;
    return cats ; 
}
int PMTEfficiency::NumPMTCategories()
{
    return N_PMTCAT ;  
}

/**
These numbers need to match those from junoSD_PMT_v2::get_ce
TODO: get that to adopt this

**/

double PMTEfficiency::s_theta_Unknown[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_Unknown[NBIN]     = { 1.0,     1.0,     1.0,     1.0,      1.0,     1.0,     1.0,      1.0,      1.0, };  

double PMTEfficiency::s_theta_HZC[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_HZC[NBIN]     = { 1.0,     1.0,     1.0,     1.0,      1.0,     1.0,     1.0,      1.0,      1.0, };  


/**

 833     } else if (m_ce_mode == "20inch") {
 834         // only 20inch PMT will be affected
 835          //G4cout << volname << G4endl;
 836         if (volname == "PMT_20inch_body_phys") {
 837             // calculate the angle theta
 838             double theta = localpos.theta();
 839             // do interpolate
 840             static double s_theta_NNVT[] = {
 841                 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg,
 842                 77.5*deg, 85.*deg, 90.*deg,
 843             };
 844             static double s_ce_NNVT[] =    {
 845                 0.9,    0.9,   0.845,     0.801,    0.775,    0.802,
 846                 0.802,   0.771,    0.66,
 847             };
 848             static double s_theta_hamamatsu[] = {
 849                 0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
 850                 79.*deg, 85.*deg, 90.*deg,
 851             };
 852             static double s_ce_hamamatsu[] =    {
 853                 0.873,    0.873,   0.888,     0.896,    0.881,    0.9,
 854                 0.881,     0.627,    0.262,
 855             };
 856             static G4DataInterpolation s_di(s_theta_NNVT, s_ce_NNVT, 9, 0., 0.);
 857             if(pmt_type){
 858                 ce_cat = 2;
 859                 static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);
 860             }
 861             else
 862             {  
 863                ce_cat = 3 ;
 864             }
**/

// There values seem unused, as do not meet volname : PMT_20inch_body_phys 
double PMTEfficiency::s_theta_NNVT_unused[NBIN]      = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_NNVT_unused[NBIN]         = {   0.9,     0.9,    0.845,   0.801,    0.775,   0.802,   0.802,    0.771,    0.66, };

double PMTEfficiency::s_theta_Hamamatsu_unused[NBIN] = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_Hamamatsu_unused[NBIN]   = {   0.873, 0.873,    0.888,   0.896,    0.881,     0.9,   0.881,    0.627,    0.262, } ;



/**
 904         else if (volname == "HamamatsuR12860_PMT_20inch_body_phys") {
 905             double theta = localpos.theta();
 906 
 907             static double s_theta_hamamatsu[] = {
 908                 0.*deg, 13.*deg, 28.*deg, 41.*deg, 55.*deg, 66.*deg,
 909                 79.*deg, 85.*deg, 90.*deg,
 910             };
 911             static double s_ce_hamamatsu[] =    {
 912                 0.911,    0.911,    0.9222,     0.9294,     0.9235,     0.93,
 913                 0.9095, 0.6261, 0.2733,
 914             };
 915             static G4DataInterpolation s_di(s_theta_hamamatsu, s_ce_hamamatsu, 9, 0., 0.);
 916             ce_cat = 4 ;
 917             return s_di.CubicSplineInterpolation(theta);
 918         }
 919 
**/

double PMTEfficiency::s_theta_Hamamatsu[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_Hamamatsu[NBIN]   = { 0.911,   0.911,   0.9222,  0.9294,   0.9235,  0.93,    0.9095,   0.6261,   0.2733, };  


/**
 921         else if (volname == "NNVTMCPPMT_PMT_20inch_body_phys") {
 922             // calculate the angle theta
 923             double theta = localpos.theta();
 924             // do interpolate
 925             static double s_theta_NNVT[] = {
 926                 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg,
 927                 77.5*deg, 85.*deg, 90.*deg,
 928             };
 929             static double s_ce_NNVT[] =    {
 930                 1.0,    1.0,    0.9453,     0.9105,     0.8931,     0.9255,
 931                 0.9274,     0.8841,     0.734,
 932             };
 933             static double s_ce_NNVT_highQE[] = {
 934                1.0,     1.0,    0.9772,     0.9723,     0.9699,     0.9697,
 935                0.9452,  0.9103,     0.734,
 936             };

**/


double PMTEfficiency::s_theta_NNVT[NBIN]      = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_NNVT[NBIN]      =    { 1.0,     1.0,     0.9453,  0.9105,   0.8931,  0.9255,  0.9274,   0.8841,  0.734, };
 
double PMTEfficiency::s_theta_NNVT_High[NBIN] = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_NNVT_High[NBIN]    = { 1.0,     1.0,     0.9772,  0.9723,   0.9699,  0.9697,  0.9452,   0.9103,  0.734, };





int PMTEfficiency::NumElements(int /*cat*/)  // static
{
    return NBIN  ; 
}
double* PMTEfficiency::ThetaArray(int cat)   // static
{
    double* theta = NULL ; 

    // if(CATEGORY_TYPE == 'S')
    // {
    //     switch(cat)
    //     {
    //         case kPMTType_Unknown      : theta = s_theta_Unknown     ; break ; 
    //         case kPMTType_NormalNNVT   : theta = s_theta_NNVT        ; break ; 
    //         case kPMTType_Hamamatsu    : theta = s_theta_Hamamatsu   ; break ; 
    //         case kPMTType_HZC          : theta = s_theta_HZC         ; break ; 
    //         case kPMTType_HiQENNVT     : theta = s_theta_NNVT_High   ; break ; 
    //     }
    // } 
    // else if(CATEGORY_TYPE == 'P')
    // {
    //     switch(cat)
    //     {
    //         case kPMT_Unknown          : theta = s_theta_Unknown     ; break ; 
    //         case kPMT_NNVT             : theta = s_theta_NNVT        ; break ; 
    //         case kPMT_Hamamatsu        : theta = s_theta_Hamamatsu   ; break ; 
    //         case kPMT_HZC              : theta = s_theta_HZC         ; break ; 
    //         case kPMT_NNVT_HighQE      : theta = s_theta_NNVT_High   ; break ; 
    //     }
    // }
    switch(cat)
    {
        case kPMT_Unknown          : theta = s_theta_Unknown     ; break ; 
        case kPMT_NNVT             : theta = s_theta_NNVT        ; break ; 
        case kPMT_Hamamatsu        : theta = s_theta_Hamamatsu   ; break ; 
        case kPMT_HZC              : theta = s_theta_HZC         ; break ; 
        case kPMT_NNVT_HighQE      : theta = s_theta_NNVT_High   ; break ; 
    }
    return theta ; 
}
double* PMTEfficiency::EfficiencyArray(int cat)  // static
{
    double* ce = NULL ; 
    // if(CATEGORY_TYPE == 'S')
    // {
    //     switch(cat)
    //     {
    //         case kPMTType_Unknown      : ce = s_ce_Unknown     ; break ; 
    //         case kPMTType_NormalNNVT   : ce = s_ce_NNVT        ; break ; 
    //         case kPMTType_Hamamatsu    : ce = s_ce_Hamamatsu   ; break ; 
    //         case kPMTType_HZC          : ce = s_ce_HZC         ; break ; 
    //         case kPMTType_HiQENNVT     : ce = s_ce_NNVT_High   ; break ; 
    //     }
    // } 
    // else if(CATEGORY_TYPE == 'P')
    // {
    //     switch(cat)
    //     {
    //         case kPMT_Unknown          : ce = s_ce_Unknown     ; break ; 
    //         case kPMT_NNVT             : ce = s_ce_NNVT        ; break ; 
    //         case kPMT_Hamamatsu        : ce = s_ce_Hamamatsu   ; break ; 
    //         case kPMT_HZC              : ce = s_ce_HZC         ; break ; 
    //         case kPMT_NNVT_HighQE      : ce = s_ce_NNVT_High   ; break ; 
    //     }
    // }

    switch(cat)
    {
        case kPMT_Unknown          : ce = s_ce_Unknown     ; break ; 
        case kPMT_NNVT             : ce = s_ce_NNVT        ; break ; 
        case kPMT_Hamamatsu        : ce = s_ce_Hamamatsu   ; break ; 
        case kPMT_HZC              : ce = s_ce_HZC         ; break ; 
        case kPMT_NNVT_HighQE      : ce = s_ce_NNVT_High   ; break ; 
    }
    return ce ; 
}

int PMTEfficiency::FindIndex(double theta, int cat, double epsilon)  // static 
{
    int num = NumElements(cat);  
    double* th = ThetaArray(cat); 
    if(!th) return -2 ; 
    for(int i=0 ; i < num ; i++)
    {
        if(std::abs(th[i]-theta) < epsilon ) return i  ; 
    }
    return -1 ; 
}

void PMTEfficiency::Dump(const char* msg, int cat)  // static
{
    int num = NumElements(cat);  
    std::cout 
        << msg 
        << " cat " << cat 
        << " num " << num
        << std::endl
        ; 

    double* th = ThetaArray(cat); 
    double* ce = EfficiencyArray(cat); 
    double epsilon = 1e-6 ; 
 
    for(int i=0 ; i < num ; i++)
    {
        int j = FindIndex(th[i], cat, epsilon) ; 
        std::cout 
            << " i " << std::setw(2) << i  
            << " j " << std::setw(2) << j 
            << " th " << std::setw(10) << std::fixed << std::setprecision(6) << th[i] 
            << " ce " << std::setw(10) << std::fixed << std::setprecision(6) << ce[i] 
            << std::endl 
            ;
        assert( i == j ); 
    } 
}

std::string PMTEfficiency::PresentSample(int cat, int idx)  // static
{
    double* th = ThetaArray(cat); 
    double* ce = EfficiencyArray(cat); 
    std::stringstream ss ; 
    if(idx > -1)
    {
        ss  
           << " ( " 
           << " idx " << std::setw(3) << idx 
           << std::setw(10) << std::fixed << std::setprecision(6) << th[idx] 
           << std::setw(10) << std::fixed << std::setprecision(6) << ce[idx] 
           << " ) "
           ;
     }
    return ss.str(); 
}

PMTEfficiency::PMTEfficiency()
{
    INSTANCE = this ; 

    // if(CATEGORY_TYPE == 'P' || CATEGORY_TYPE == 'S') 
    // {
    //     std::cout << __FILE__ << "+" << __LINE__ << " CATEGORY_TYPE " << CATEGORY_TYPE << std::endl ; 
    // }
    // else
    // {
    //     std::cout << __FILE__ << "+" << __LINE__ << " FATAL : INVALID CATEGORY_TYPE " << CATEGORY_TYPE << std::endl ; 
    //     assert(0); 
    // }

    double pFirstDerStart = 0. ; 
    double pFirstDerFinish = 0. ; 

    // int Unknown   =  CATEGORY_TYPE == 'S' ? (int)kPMTType_Unknown     : (int)kPMT_Unknown     ; 
    // int NNVT      =  CATEGORY_TYPE == 'S' ? (int)kPMTType_NormalNNVT  : (int)kPMT_NNVT        ;
    // int NNVT_High =  CATEGORY_TYPE == 'S' ? (int)kPMTType_HiQENNVT    : (int)kPMT_NNVT_HighQE ; 
    // int HZC       =  CATEGORY_TYPE == 'S' ? (int)kPMTType_HZC         : (int)kPMT_HZC         ;
    // int Hamamatsu =  CATEGORY_TYPE == 'S' ? (int)kPMTType_Hamamatsu   : (int)kPMT_Hamamatsu   ;

    int Unknown   =  (int)kPMT_Unknown     ; 
    int NNVT      =  (int)kPMT_NNVT        ;
    int NNVT_High =  (int)kPMT_NNVT_HighQE ; 
    int HZC       =  (int)kPMT_HZC         ;
    int Hamamatsu =  (int)kPMT_Hamamatsu   ;

    m_di[Unknown]   = new G4DataInterpolation(s_theta_Unknown    , s_ce_Unknown    , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[NNVT]      = new G4DataInterpolation(s_theta_NNVT       , s_ce_NNVT       , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[Hamamatsu] = new G4DataInterpolation(s_theta_Hamamatsu  , s_ce_Hamamatsu  , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[HZC]       = new G4DataInterpolation(s_theta_HZC        , s_ce_HZC        , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[NNVT_High] = new G4DataInterpolation(s_theta_NNVT_High  , s_ce_NNVT_High  , NBIN, pFirstDerStart,  pFirstDerFinish );
}

G4DataInterpolation* PMTEfficiency::getInterpolator(int cat) const 
{
    G4DataInterpolation* di = m_di.count(cat) == 1 ? m_di.at(cat) : NULL ;  
    return di ; 
}
bool PMTEfficiency::hasInterpolator(int cat) const 
{
    return getInterpolator(cat) != NULL ; 
}

double PMTEfficiency::getEfficiency(double theta, int cat) const 
{
    G4DataInterpolation* di = getInterpolator(cat); 
    return di ? di->CubicSplineInterpolation(theta) : MISSING_EFFICIENCY ; 
}
int PMTEfficiency::getLocateArgument(double theta, int cat) const
{
    G4DataInterpolation* di = getInterpolator(cat); 
    return di ? di->LocateArgument(theta) : -999 ;  
}

void PMTEfficiency::scan(const char* msg, int cat) const 
{
    bool interpol = hasInterpolator(cat) ;  
    std::cout 
           << msg 
           << " cat " << cat 
           << ( interpol ? "" : " NO-INTERPOLATOR" )
           << std::endl
           ; 

    if(!interpol) return ; 

    for(int ith=0 ; ith < 91 ; ith++ )
    {
         double theta = double(ith)*deg ; 
         double epsilon = 1e-6 ; 
         int idx = FindIndex(theta, cat, epsilon); 

         double efficiency = getEfficiency(theta, cat ); 
         int arg = getLocateArgument(theta, cat); 

         std::cout 
              << " ith " << std::setw(3) << ith
              << std::setw(10) << std::fixed << std::setprecision(6) << theta 
              << std::setw(10) << std::fixed << std::setprecision(6) << efficiency 
              << std::setw(3) << arg 
              << PresentSample(cat, idx)
              << std::endl 
              ;
    }
}

void PMTEfficiency::Dump(const char* msg) // static
{
    std::cout << msg << std::endl ; 

    int* cats = PMTEfficiency::PMTCategories(); 
    int ncat = PMTEfficiency::NumPMTCategories();
    for(int i=0 ; i < ncat ; i++) PMTEfficiency::Dump("Dump", cats[i]); 
}

void PMTEfficiency::Scan(const char* msg)  // static 
{
    std::cout << msg << std::endl ; 

    int* cats = PMTEfficiency::PMTCategories(); 
    int ncat = PMTEfficiency::NumPMTCategories();

    PMTEfficiency eff ;
    for(int i=0 ; i < ncat ; i++) eff.scan("scan", cats[i]); 
}

