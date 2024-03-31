//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "DetSimMT/tbbWorkerRunManager.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"
#include "G4UserWorkerInitialization.hh"
#include "G4UserRunAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SDManager.hh"

tbbWorkerRunManager::tbbWorkerRunManager() :
    G4WorkerRunManager()
{}

tbbWorkerRunManager::~tbbWorkerRunManager()
{}

void tbbWorkerRunManager::MergePartialResults()
{
    //TBB ??????
}

void tbbWorkerRunManager::ConstructScoringWorlds()
{
    //Forward call to protected method, that's all
    G4WorkerRunManager::ConstructScoringWorlds();
}

bool tbbWorkerRunManager::SimulateEventInit()
{
    // Reset random number seeds queue
    while(seedsQueue.size()>0)
    { seedsQueue.pop(); }
    // for each run, worker should receive at least one set of random number seeds.
    runIsSeeded = false; 

    eventLoopOnGoing = true;

    nevModulo = -1;
    currEvID = -1;
    return true;
}
bool tbbWorkerRunManager::SimulateEvent(int i_event)
{

    /// debug only ///
    // DoEventLoop(1);
    // return true;
    /// 

    // force i_event = -1
    // using currEvID instead
    i_event = -1;
    ProcessOneEvent(i_event);
    if (eventLoopOnGoing) {
        TerminateOneEvent();
        if(runAborted) {
            eventLoopOnGoing = false;
            // G4cout << "tbbWorkerRunManager::SimulateEvent "
            //        << " runAborted: " << runAborted
            //        << " which causes eventLoopOnGoing: " << eventLoopOnGoing
            //        << G4endl;
        }
    }
    // G4cout << "tbbWorkerRunManager::SimulateEvent "
    //        << "eventLoopOnGoing: " << eventLoopOnGoing
    //        << G4endl;
    return eventLoopOnGoing;
}

bool tbbWorkerRunManager::SimulateEventFinale()
{
    return true;
}

G4Event* tbbWorkerRunManager::GenerateEvent(G4int i_event)
{
  G4Event* anEvent = new G4Event(i_event);
  long s1 = 0;
  long s2 = 0;
  long s3 = 0;
  G4bool eventHasToBeSeeded = true;
  if(G4MTRunManager::SeedOncePerCommunication()==1 && runIsSeeded)
  { eventHasToBeSeeded = false; }

  //  For debug only
  // G4cout << "=== "
  //        << " runIsSeeded: " << runIsSeeded
  //        << " eventHasToBeSeeded: " << eventHasToBeSeeded << G4endl;

  if(i_event<0)
  {
    G4int nevM = G4MTRunManager::GetMasterRunManager()->GetEventModulo();
    // G4cout << "tbbWorkerRunManager::GenerateEvent: "
    //        << " nevM " << nevM << G4endl;
    if(nevM==1)
    {
      eventLoopOnGoing = G4MTRunManager::GetMasterRunManager()
                       ->SetUpAnEvent(anEvent,s1,s2,s3,eventHasToBeSeeded);
      runIsSeeded = true;
    }
    else
    {
        // G4cout << "tbbWorkerRunManager::GenerateEvent: "
        //        << " nevM: " << nevM
        //        << " nevModulo: " << nevModulo
        //        << G4endl;
      if(nevModulo<=0)
      {
        G4int nevToDo = G4MTRunManager::GetMasterRunManager()
                         ->SetUpNEvents(anEvent,&seedsQueue,eventHasToBeSeeded);
        // G4cout << "tbbWorkerRunManager::GenerateEvent: "
        //        << " nevToDo: " << nevToDo
        //        << G4endl;
        
        if(nevToDo==0)
        { eventLoopOnGoing = false; }
        else
        {
          currEvID = anEvent->GetEventID();
          nevModulo = nevToDo - 1;
        }
      }
      else
      {
        if(G4MTRunManager::SeedOncePerCommunication()>0) eventHasToBeSeeded = false;
        anEvent->SetEventID(++currEvID);
        nevModulo--;
      }
      if(eventLoopOnGoing && eventHasToBeSeeded)
      {
        s1 = seedsQueue.front(); seedsQueue.pop();
        s2 = seedsQueue.front(); seedsQueue.pop();
      }
    }

    if(!eventLoopOnGoing)
    {
      delete anEvent;
      return 0;
    }
  }
  else if(eventHasToBeSeeded)
  {
    //Need to reseed random number generator
    G4RNGHelper* helper = G4RNGHelper::GetInstance();
    s1 = helper->GetSeed(i_event*2);
    s2 = helper->GetSeed(i_event*2+1);
  }

  if(eventHasToBeSeeded) 
  {
    long seeds[3] = { s1, s2, 0 };
    G4Random::setTheSeeds(seeds,-1);
    runIsSeeded = true;
////G4cout<<"Event "<<currEvID<<" is seeded with { "<<s1<<", "<<s2<<" }"<<G4endl;
  }


  if(printModulo > 0 && anEvent->GetEventID()%printModulo == 0 )
  {
    G4cout << "--> Event " << anEvent->GetEventID() << " starts";
    if(eventHasToBeSeeded)
    { G4cout << " with initial seeds (" << s1 << "," << s2 << ")"; }
    G4cout << "." << G4endl;
  }
  userPrimaryGeneratorAction->GeneratePrimaries(anEvent);
  return anEvent;
}
