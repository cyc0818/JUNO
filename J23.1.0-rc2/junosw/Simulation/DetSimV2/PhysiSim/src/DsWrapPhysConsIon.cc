#include <boost/python.hpp>
#include "G4IonPhysicsPHP.hh"
#include "DsWrapPhysConsIon.h"
#include "G4IonPhysics.hh"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(DsWrapPhysConsIon);

DsWrapPhysConsIon::DsWrapPhysConsIon(const G4String& name): G4VPhysicsConstructor(name),ToolBase(name)
                                                          
{
   declProp("EnableIonPHP", m_enable_ionPHP=false);
  
}

DsWrapPhysConsIon::~DsWrapPhysConsIon()
{
}

void DsWrapPhysConsIon::ConstructParticle()
{
   if(m_enable_ionPHP){
       m_ion_process = new G4IonPhysicsPHP();
 //      std::cout<<"new G4IonPhysicsPHP();!!!!!!!!"<<std::endl;
    }
   else{
       m_ion_process = new G4IonPhysics();
 //      std::cout<<"new G4IonPhysics();!!!!!!!!"<<std::endl;
   } 
   assert(m_ion_process);
    m_ion_process->ConstructParticle();
}



void DsWrapPhysConsIon::ConstructProcess()
{
    m_ion_process -> ConstructProcess();
}
