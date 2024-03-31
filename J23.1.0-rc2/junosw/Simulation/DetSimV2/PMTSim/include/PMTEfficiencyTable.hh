#pragma once

#include <vector>
class PMTEfficiency ; 

class PMTEfficiencyTable
{
    public:
        PMTEfficiencyTable(const PMTEfficiency* eff ); 
        void dumpTheta(const char* msg="PMTEfficiencyTable::dumpTheta");

        const std::vector<float>& getData() const ; 
        const std::vector<int>&   getShape() const ; 
        int   getThetaSteps() const ;
        float getThetaMin() const ;
        float getThetaMax() const ;
        float getPhiMin() const ;
        float getPhiMax() const ;
    private:
        void init(); 
        void initThetaDomain(); 
        void initThetaValues(); 
    private:
        const PMTEfficiency* m_eff ;  
        int    m_theta_steps ; 
        int    m_phi_steps ; 
        double m_theta_min ; 
        double m_theta_max ; 
        double m_phi_min ; 
        double m_phi_max ; 

        std::vector<float>   m_theta_domain ; 
        std::vector<float>   m_values ; 
        std::vector<int>     m_shape ;    

};



