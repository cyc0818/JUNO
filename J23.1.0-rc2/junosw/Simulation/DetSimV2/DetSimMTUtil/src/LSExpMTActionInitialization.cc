#include "LSExpMTActionInitialization.hh"
#include "G4Event.hh"
#include "LSExpMTPrimaryGeneratorAction.hh"
#include "LSExpMTEventAction.hh"

LSExpMTActionInitialization::LSExpMTActionInitialization()
    : G4VUserActionInitialization()
{}

LSExpMTActionInitialization::~LSExpMTActionInitialization()
{}

void LSExpMTActionInitialization::BuildForMaster() const
{

}

void LSExpMTActionInitialization::Build() const
{
    // Thread Local
    // the registered actions are created during the initialization of slave run manager
    LSExpMTPrimaryGeneratorAction* pga = new LSExpMTPrimaryGeneratorAction();
    pga->setMTmode(true);
    SetUserAction(pga);

    LSExpMTEventAction* ea = new LSExpMTEventAction();
    SetUserAction(ea);
}

G4VSteppingVerbose* LSExpMTActionInitialization::InitializeSteppingVerbose() const
{
  return 0;
}
