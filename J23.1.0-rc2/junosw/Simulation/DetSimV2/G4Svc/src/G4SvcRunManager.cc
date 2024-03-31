#include "G4Svc/G4SvcRunManager.h"
#include <iostream>

G4SvcRunManager::G4SvcRunManager() 
    : G4RunManager(){
    std::cout << "Create G4SvcRunManager" << std::endl;
}

G4SvcRunManager::~G4SvcRunManager() {
    std::cout << "Delete G4SvcRunManager" << std::endl;
}

// * initializeRM and finalizeRM is based G4RunManager::BeamOn
// * SimulateEvent is based on G4RunManager::DoEventLoop

bool G4SvcRunManager::initializeRM() {
  G4bool cond = ConfirmBeamOnCondition();
  if(cond)
  {
    ConstructScoringWorlds();
    RunInitialization();
    return true;
  }
  return false;
}

bool G4SvcRunManager::SimulateEvent(int i_event) {
    currentEvent = GenerateEvent(i_event);
    eventManager->ProcessOneEvent(currentEvent);
    AnalyzeEvent(currentEvent);
    UpdateScoring();
    StackPreviousEvent(currentEvent);
    currentEvent = 0;

    if(runAborted) return false;
    return true;

}

bool G4SvcRunManager::finalizeRM() {
    RunTermination();
    return true;
}
