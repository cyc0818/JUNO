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
//
// Class description:
//
//    This class implements the worker model run manager for TBB based
//    application.
//    It is instantiated by tbbUserWorkerInitialization and used by
//    tbbMasterRunManager.
//    See G4WorkerRunManager for documentation of methods relative to
//    base class. Only class specific methods are documented here.
//
// Equivalent in traditional MT:
//    G4WorkerRunManager
//
// History:
//    Oct 31st, 2013 A. Dotti - First Implementation

#include "DetSimMT/tbbMasterRunManager.hh"
#include "G4AutoLock.hh"
#include <iostream>
tbbMasterRunManager::tbbMasterRunManager() :
    G4MTRunManager(),
    // theTasks(static_cast<tbb::task_list*>(0)),
    nEvtsPerTask(1)
{
    std::cout << "invoke tbbMasterRunManager::ctor" << std::endl;
}

tbbMasterRunManager::~tbbMasterRunManager()
{
}

void tbbMasterRunManager::TerminateWorkers()
{
    //For TBB based example this should be empty
}

void tbbMasterRunManager::CreateAndStartWorkers()
{
    //Instead of pthread based workers, create tbbTask
    G4int ntasks = numberOfEventToBeProcessed/nEvtsPerTask;
    G4int remn = numberOfEventToBeProcessed % nEvtsPerTask;
    for ( G4int nt = 0 ; nt < ntasks ; ++nt )
    {
        G4int evts= nEvtsPerTask;
        if ( nt == ntasks - 1 ) evts+=remn;
        CreateTask(nt,evts);
    }
}

void tbbMasterRunManager::CreateTask(G4int id,G4int evts)
{
}

void tbbMasterRunManager::RunTermination()
{
    // Reduce results ....
    G4MTRunManager::RunTermination();
}
namespace {
 G4Mutex setUpEventMutex = G4MUTEX_INITIALIZER;
}
G4int tbbMasterRunManager::SetUpNEvents(G4Event* evt, G4SeedsQueue* seedsQueue,G4bool reseedRequired)
{
  G4AutoLock l(&setUpEventMutex);
  if( numberOfEventProcessed < numberOfEventToBeProcessed && !runAborted )
  {
    G4int nev = eventModulo;
    if(numberOfEventProcessed + nev > numberOfEventToBeProcessed)
    { nev = numberOfEventToBeProcessed - numberOfEventProcessed; }
    evt->SetEventID(numberOfEventProcessed);
    if(reseedRequired)
    {
      G4RNGHelper* helper = G4RNGHelper::GetInstance();
      G4int nevRnd = nev;
      if(seedOncePerCommunication>0) nevRnd = 1;
      for(int i=0;i<nevRnd;i++)
      {
        seedsQueue->push(helper->GetSeed(nSeedsPerEvent*nSeedsUsed));
        seedsQueue->push(helper->GetSeed(nSeedsPerEvent*nSeedsUsed+1));
        if(nSeedsPerEvent==3)
          seedsQueue->push(helper->GetSeed(nSeedsPerEvent*nSeedsUsed+2));
        nSeedsUsed++;
        if(nSeedsUsed==nSeedsFilled) RefillSeeds();
      }
    }
    numberOfEventProcessed += nev;
    return nev;
  }
  G4cout << "tbbMasterRunManager::SetUpNEvents "
         << " return 0 "
         << G4endl;
  return 0;
}
