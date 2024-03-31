#ifndef BirksLawCalculator_hh
#define BirksLawCalculator_hh

#include <map>
#include <string>
#include "MCParamsSvc/IMCParamsSvc.hh"

class G4Step;

class BirksLawCalculator{
    public:
        ~BirksLawCalculator();
        
        void setMCParamsSvc(IMCParamsSvc* svc);
        IMCParamsSvc* getMCParamsSvc() { return m_MCParamsSvc; }
        
        static BirksLawCalculator* Instance()
        {
            if(not s_instance){
                s_instance = new BirksLawCalculator();
            }

            return s_instance;
        }

        double calQuenchedEnergy(const G4Step* step);

    private:
        BirksLawCalculator();

        void initBirksConstants();
        std::map<int, double> m_BirksConstant1;
        std::map<int, double> m_BirksConstant2;

    private:
        IMCParamsSvc* m_MCParamsSvc;
        static BirksLawCalculator* s_instance;
};

#endif
