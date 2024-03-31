#include "BirksLawCalculator.hh"

#include "SniperKernel/SniperLog.h"
#include <MCParamsSvc/IMCParamsSvc.hh>

#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4LossTableManager.hh"

BirksLawCalculator* 
BirksLawCalculator::s_instance = 0;

BirksLawCalculator::BirksLawCalculator()
    : m_MCParamsSvc(nullptr)
{
}

BirksLawCalculator::~BirksLawCalculator()
{
}

void
BirksLawCalculator::initBirksConstants()
{
    if(!m_MCParamsSvc){
        G4cout<<"MCParamsSvc not found!"<<G4endl;
        assert(0);
    }

    IMCParamsSvc::map_s2d LS_constant_map;
    bool st = m_MCParamsSvc->Get("Material.LS.ConstantProperty", LS_constant_map);

    if(st && LS_constant_map.count("eBirksConstant1") && LS_constant_map.count("eBirksConstant2")
          && LS_constant_map.count("pBirksConstant1") && LS_constant_map.count("pBirksConstant2")
          && LS_constant_map.count("aBirksConstant1") && LS_constant_map.count("aBirksConstant2")) {

        m_BirksConstant1.insert(std::pair<int, double> (11, LS_constant_map["eBirksConstant1"]));
        m_BirksConstant2.insert(std::pair<int, double> (11, LS_constant_map["eBirksConstant2"]));
        m_BirksConstant1.insert(std::pair<int, double> (2212, LS_constant_map["pBirksConstant1"]));
        m_BirksConstant2.insert(std::pair<int, double> (2212, LS_constant_map["pBirksConstant2"]));
        m_BirksConstant1.insert(std::pair<int, double> (1000020040, LS_constant_map["aBirksConstant1"]));
        m_BirksConstant2.insert(std::pair<int, double> (1000020040, LS_constant_map["aBirksConstant2"]));
    }else{
        LogError<<"BirksLawCalculator:: BirksConstant doesn't exist.Please check."<<std::endl;
    }
}

double
BirksLawCalculator::calQuenchedEnergy(const G4Step* aStep)
{
    double quenchedEnergy = 0.;
    double dE = aStep->GetTotalEnergyDeposit();
    double dx = aStep->GetStepLength();

    G4Track* aTrack = aStep->GetTrack();
    G4ParticleDefinition* aParticle = aTrack->GetDefinition();

    if(dE>0){
        if(aParticle == G4Gamma::Gamma()){
            G4LossTableManager* manager = G4LossTableManager::Instance();
            dx = manager->GetRange(G4Electron::Electron(), dE, aTrack->GetMaterialCutsCouple());
        }

        G4Material* aMaterial = aStep->GetPreStepPoint()->GetMaterial();
        G4MaterialPropertiesTable* aMaterialPropertiesTable = aMaterial->GetMaterialPropertiesTable();
        if(aMaterialPropertiesTable){
            const G4MaterialPropertyVector* Fast_Intensity = aMaterialPropertiesTable->GetProperty("FASTCOMPONENT");
            const G4MaterialPropertyVector* Slow_Intensity = aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");

            if(Fast_Intensity || Slow_Intensity){
                double delta = dE/dx/aMaterial->GetDensity();
                double birk1 = 0.;
                double birk2 = 0.;
                if(aParticle == G4Gamma::Gamma() || aParticle == G4Positron::Positron() || aParticle == G4Electron::Electron()){
                    birk1 = m_BirksConstant1[11];
                    birk2 = m_BirksConstant2[11];
                }else if(abs(aParticle->GetPDGCharge())<1.5){
                    birk1 = m_BirksConstant1[2212];
                    birk2 = m_BirksConstant2[2212];
                }else{
                    birk1 = m_BirksConstant1[1000020040];
                    birk2 = m_BirksConstant2[1000020040];
                }

                quenchedEnergy = dE/(1+birk1*delta+birk2*delta*delta);
            }
        }
    }

    return quenchedEnergy;
}

void 
BirksLawCalculator::setMCParamsSvc(IMCParamsSvc* svc)
{
    m_MCParamsSvc = svc;
    initBirksConstants();
}
