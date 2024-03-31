#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>

#include "Geometry/PMTParamSvc.h"  // for PMTCategory enum

#include "G4DataInterpolation.hh"
#include "G4SystemOfUnits.hh"
using CLHEP::deg ; 

#include "PMTEfficiency.hh"

int PMTEfficiency::s_pmt_categories[N_PMTCAT] = {kPMT_Unknown, kPMT_NNVT, kPMT_Hamamatsu, kPMT_HZC, kPMT_NNVT_HighQE } ; 
double PMTEfficiency::MISSING_EFFICIENCY = -999.0 ; 

int* PMTEfficiency::PMTCategories()
{
    return s_pmt_categories ; 
}
int PMTEfficiency::NumPMTCategories()
{
    return N_PMTCAT ;  
}

double PMTEfficiency::s_theta_Unknown[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_Unknown[NBIN]     = { 1.0,     1.0,     1.0,     1.0,      1.0,     1.0,     1.0,      1.0,      1.0, };  

double PMTEfficiency::s_theta_NNVT[NBIN]      = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_NNVT[NBIN]      =    { 1.0,     1.0,     0.9453,  0.9105,   0.8931,  0.9255,  0.9274,   0.8841,  0.734, };

double PMTEfficiency::s_theta_Hamamatsu[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_Hamamatsu[NBIN]     = { 0.911,   0.911,   0.9222,  0.9294,   0.9235,  0.93,    0.9095,   0.6261,   0.2733, };  

double PMTEfficiency::s_theta_NNVT_HighQE[NBIN] = { 0.*deg, 14.*deg, 30.*deg, 42.5*deg, 55.*deg, 67.*deg, 77.5*deg, 85.*deg, 90.*deg, };
double PMTEfficiency::s_ce_NNVT_HighQE[NBIN]    = { 1.0,     1.0,     0.9772,  0.9723,   0.9699,  0.9697,  0.9452,   0.9103,  0.734, };

double PMTEfficiency::s_theta_HZC[NBIN]  = { 0.*deg, 13.*deg, 28.*deg, 41.*deg,  55.*deg, 66.*deg, 79.*deg,  85.*deg,  90.*deg, };
double PMTEfficiency::s_ce_HZC[NBIN]     = { 1.0,     1.0,     1.0,     1.0,      1.0,     1.0,     1.0,      1.0,      1.0, };  


int PMTEfficiency::NumElements(int pmtcat) 
{
    int nel = 0 ; 
    switch(pmtcat)
    {
      case kPMT_Unknown:     nel = NBIN ; break ; 
      case kPMT_NNVT:        nel = NBIN ; break ; 
      case kPMT_Hamamatsu:   nel = NBIN ; break ; 
      case kPMT_HZC:         nel = NBIN ; break ; 
      case kPMT_NNVT_HighQE: nel = NBIN ; break ; 
    }
    return nel  ; 
}
double* PMTEfficiency::ThetaArray(int pmtcat)  
{
    double* theta = NULL ; 
    switch(pmtcat)
    {
       case kPMT_Unknown      : theta = s_theta_Unknown     ; break ; 
       case kPMT_NNVT         : theta = s_theta_NNVT        ; break ; 
       case kPMT_Hamamatsu    : theta = s_theta_Hamamatsu   ; break ; 
       case kPMT_HZC          : theta = s_theta_HZC         ; break ; 
       case kPMT_NNVT_HighQE  : theta = s_theta_NNVT_HighQE ; break ; 
    }
    return theta ; 
}
double* PMTEfficiency::EfficiencyArray(int pmtcat) 
{
    double* ce = NULL ; 
    switch(pmtcat)
    {
       case kPMT_Unknown      : ce = s_ce_Unknown     ; break ; 
       case kPMT_NNVT         : ce = s_ce_NNVT        ; break ; 
       case kPMT_Hamamatsu    : ce = s_ce_Hamamatsu   ; break ; 
       case kPMT_HZC          : ce = s_ce_HZC         ; break ; 
       case kPMT_NNVT_HighQE  : ce = s_ce_NNVT_HighQE ; break ; 
    }
    return ce ; 
}

int PMTEfficiency::FindIndex(double theta, int pmtcat, double epsilon) 
{
    int num = NumElements(pmtcat);  
    double* th = ThetaArray(pmtcat); 
    if(!th) return -2 ; 
    for(int i=0 ; i < num ; i++)
    {
        if(std::abs(th[i]-theta) < epsilon ) return i  ; 
    }
    return -1 ; 
}

void PMTEfficiency::Dump(const char* msg, int pmtcat) 
{
    int num = NumElements(pmtcat);  
    std::cout 
        << msg 
        << " pmtcat " << pmtcat 
        << " num " << num
        << std::endl
        ; 

    double* th = ThetaArray(pmtcat); 
    double* ce = EfficiencyArray(pmtcat); 
    double epsilon = 1e-6 ; 
 
    for(int i=0 ; i < num ; i++)
    {
        int j = FindIndex(th[i], pmtcat, epsilon) ; 
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

std::string PMTEfficiency::PresentSample(int pmtcat, int idx)
{
    double* th = ThetaArray(pmtcat); 
    double* ce = EfficiencyArray(pmtcat); 
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
    double pFirstDerStart = 0. ; 
    double pFirstDerFinish = 0. ; 
    m_di[kPMT_Unknown]     = new G4DataInterpolation(s_theta_Unknown    , s_ce_Unknown    , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[kPMT_NNVT]        = new G4DataInterpolation(s_theta_NNVT       , s_ce_NNVT       , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[kPMT_Hamamatsu]   = new G4DataInterpolation(s_theta_Hamamatsu  , s_ce_Hamamatsu  , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[kPMT_HZC]         = new G4DataInterpolation(s_theta_HZC        , s_ce_HZC        , NBIN, pFirstDerStart,  pFirstDerFinish ); 
    m_di[kPMT_NNVT_HighQE] = new G4DataInterpolation(s_theta_NNVT_HighQE, s_ce_NNVT_HighQE, NBIN, pFirstDerStart,  pFirstDerFinish );
}

G4DataInterpolation* PMTEfficiency::getInterpolator(int pmtcat) const 
{
    G4DataInterpolation* di = m_di.count(pmtcat) == 1 ? m_di.at(pmtcat) : NULL ;  
    return di ; 
}
bool PMTEfficiency::hasInterpolator(int pmtcat) const 
{
    return getInterpolator(pmtcat) != NULL ; 
}

double PMTEfficiency::getEfficiency(double theta, int pmtcat) const 
{
    G4DataInterpolation* di = getInterpolator(pmtcat); 
    return di ? di->CubicSplineInterpolation(theta) : MISSING_EFFICIENCY ; 
}
int PMTEfficiency::getLocateArgument(double theta, int pmtcat) const
{
    G4DataInterpolation* di = getInterpolator(pmtcat); 
    return di ? di->LocateArgument(theta) : -999 ;  
}

void PMTEfficiency::scan(const char* msg, int pmtcat) const 
{
    bool interpol = hasInterpolator(pmtcat) ;  
    std::cout 
           << msg 
           << " pmtcat " << pmtcat 
           << ( interpol ? "" : " NO-INTERPOLATOR" )
           << std::endl
           ; 

    if(!interpol) return ; 

    for(int ith=0 ; ith < 91 ; ith++ )
    {
         double theta = double(ith)*deg ; 
         double epsilon = 1e-6 ; 
         int idx = FindIndex(theta, pmtcat, epsilon); 

         double efficiency = getEfficiency(theta, pmtcat ); 
         int arg = getLocateArgument(theta, pmtcat); 

         std::cout 
              << " ith " << std::setw(3) << ith
              << std::setw(10) << std::fixed << std::setprecision(6) << theta 
              << std::setw(10) << std::fixed << std::setprecision(6) << efficiency 
              << std::setw(3) << arg 
              << PresentSample(pmtcat, idx)
              << std::endl 
              ;
    }
}

void PMTEfficiency::Dump(const char* msg)
{
    std::cout << msg << std::endl ; 

    int* pmtcats = PMTEfficiency::PMTCategories(); 
    int ncat = PMTEfficiency::NumPMTCategories();
    for(int i=0 ; i < ncat ; i++) PMTEfficiency::Dump("Dump", pmtcats[i]); 
}

void PMTEfficiency::Scan(const char* msg)
{
    std::cout << msg << std::endl ; 

    int* pmtcats = PMTEfficiency::PMTCategories(); 
    int ncat = PMTEfficiency::NumPMTCategories();

    PMTEfficiency eff ;
    for(int i=0 ; i < ncat ; i++) eff.scan("scan", pmtcats[i]); 
}

