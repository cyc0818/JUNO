#ifndef IDetSimFactory_h
#define IDetSimFactory_h

class G4VUserDetectorConstruction;
class G4VUserPhysicsList;
class G4VUserPrimaryGeneratorAction;
class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;
class G4UserSteppingAction;
class G4VUserActionInitialization;

class IDetSimFactory {
public:
    virtual ~IDetSimFactory() {};
    virtual G4VUserDetectorConstruction* createDetectorConstruction() = 0;
    virtual G4VUserPhysicsList* createPhysicsList() = 0;
    virtual G4VUserPrimaryGeneratorAction* createPrimaryGenerator() = 0;

    virtual G4UserRunAction* createRunAction() {return NULL;}
    virtual G4UserEventAction* createEventAction() {return NULL;}
    virtual G4UserStackingAction* createStackingAction() {return NULL;} 
    virtual G4UserTrackingAction* createTrackingAction() {return NULL;} 
    virtual G4UserSteppingAction* createSteppingAction() {return NULL;}

    // a new interface which will be used in the Geant4 MT mode.
    virtual G4VUserActionInitialization* createUserActionInitialization() {return NULL;}
};

#endif
