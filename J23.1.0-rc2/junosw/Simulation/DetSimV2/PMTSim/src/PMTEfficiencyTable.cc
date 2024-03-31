#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>

#include "G4SystemOfUnits.hh"
using CLHEP::deg ; 

#include "PMTEfficiency.hh"
#include "PMTEfficiencyTable.hh"


PMTEfficiencyTable::PMTEfficiencyTable(const PMTEfficiency* eff )
    :
    m_eff(eff),
    m_theta_steps(90),
    m_phi_steps(1),
    m_theta_min(0.*deg),
    m_theta_max(90.*deg),
    m_phi_min(0.),
    m_phi_max(0.)
{
    init(); 
}

int PMTEfficiencyTable::getThetaSteps() const 
{
    return m_theta_steps ; 
}
float PMTEfficiencyTable::getThetaMin() const 
{
    return m_theta_min ; 
}
float PMTEfficiencyTable::getThetaMax() const 
{
    return m_theta_max ; 
}
float PMTEfficiencyTable::getPhiMin() const 
{
    return m_phi_min ; 
}
float PMTEfficiencyTable::getPhiMax() const 
{
    return m_phi_max ; 
}

const std::vector<float>& PMTEfficiencyTable::getData() const 
{
    return m_values ; 
}
const std::vector<int>&  PMTEfficiencyTable::getShape() const 
{
    return m_shape ; 
} 

void PMTEfficiencyTable::init()
{
    initThetaDomain(); 
    initThetaValues(); 
}

void PMTEfficiencyTable::initThetaDomain()
{
    m_theta_domain.clear(); 
    double theta_range = m_theta_max - m_theta_min ;       // 90. - 0. 
    double theta_step = theta_range/double(m_theta_steps) ; // 90./90 = 1.

    for(int itheta=0 ; itheta < m_theta_steps+1 ; itheta++ ) // 0..90  (91 points : including the ends)
    {
        double theta = m_theta_min + theta_step*itheta ; 
        m_theta_domain.push_back(theta) ; 
    }
}

void PMTEfficiencyTable::initThetaValues()
{
    m_values.clear(); 

    int* cats = m_eff->PMTCategories() ; // could be simcat or parcat enum values

    unsigned ni = m_eff->NumPMTCategories() ; 
    unsigned nj = m_theta_domain.size() ; 
    unsigned nk = 1 ; // no phi dependency  
    unsigned nl = 1 ; // multiplicity 

    unsigned size = ni*nj*nk*nl ; 
    float fill = 0.f ; 
    m_values.resize(size, fill ); 
  
    m_shape.push_back(ni); 
    m_shape.push_back(nj); 
    m_shape.push_back(nk); 
    m_shape.push_back(nl); 

    for(unsigned i=0 ; i < ni ; i++)
    {
        int cat = cats[i] ;   

        // cats array defines the order of the items in the table 
        // hmm the cat used when querying the texture must be in same 
        // enum type as the one used here 

        for(unsigned j=0 ; j < nj ; j++)
        {
            double theta = m_theta_domain[j] ; 
            for(unsigned k=0 ; k < nk ; k++)
            {
                unsigned offset = i*nj*nk + j*nk + k  ; 

                double value = m_eff->getEfficiency(theta, cat) ; 
/*
                std::cout 
                   << " i " << i
                   << " j " << j
                   << " k " << k
                   << " offset " << offset  
                   << " theta " << theta 
                   << " value " << value
                   << std::endl 
                   ; 
*/
                m_values[offset] = float(value); 
            }
        }
    }
} 

void PMTEfficiencyTable::dumpTheta(const char* msg)
{
    unsigned ndom = m_theta_domain.size(); 
    unsigned nval = m_values.size(); 
    assert( nval % ndom == 0 ); 
    unsigned ncat = nval/ndom ; 

    std::cout 
        << msg 
        << " ndom " << ndom
        << " nval " << nval
        << " ncat " << ncat
        << std::endl 
        ;

    for(unsigned d=0 ; d < ndom ; d++)
    {
        std::cout
            << std::setw(3) << d 
            << " "
            << std::setw(10) << std::fixed << std::setprecision(6) << m_theta_domain[d]
            << " : "  
            ;

        for(unsigned c=0 ; c < ncat ; c++)
        {
             unsigned offset = c*ndom + d ; 
             float value = m_values[offset] ; 
             std::cout 
                 << " "
                 << std::setw(10) << std::fixed << std::setprecision(6) << value
                 << " "
                 ;
        }
        std::cout 
            << std::endl 
            ;
    }
}

