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
//    This class implements the master model run manager for TBB bases
//    application.
//    It is instantiated by user main (or equivalent function) instead
//    of G4[MT]RunManager. It controls the creation of tbb::tasks.
//    See G4MTRunManager for documentation of methods relative to base
//    class. Only class specific methods are documented here.
//
// Equivalent in traditional MT:
//    G4MTRunManager
//
// History:
//    Nov 4th, 2013 A. Dotti - First Implementation

#ifndef TBBMASTERRUNMANAGER_HH
#define TBBMASTERRUNMANAGER_HH

#include "G4MTRunManager.hh"
// #include <tbb/task.h>

class tbbMasterRunManager : public G4MTRunManager {
public:
    tbbMasterRunManager();
    //Default construct, tasklist is the tbb::task_list
    //where the created tasks will be added to
    //nEvents is the number of events each tbb::task is
    //responsible for
    virtual ~tbbMasterRunManager();
    virtual void RunTermination();
    // void SetTaskList( tbb::task_list* tl ) { theTasks = tl; }
    //Set a reference to the output task list where new tasks will
    //be added to
    void SetNumberEventsPerTask( G4int nt ) { nEvtsPerTask = nt; }
    //Specify number of events that each simulation task is responsible
    //for


    // Same as above method, but the seeds are set only once over "eventModulo" events.
    // The return value shows the number of events the caller Worker has to process
    // (between 1 and eventModulo depending on number of events yet to be processed).
    // G4Event object has the event ID of the first event of this bunch.
    // If zero is returned no more event needs to be processed, and worker thread 
    // must delete that G4Event.
    virtual G4int SetUpNEvents(G4Event*, G4SeedsQueue* seedsQueue, G4bool reseedRequired=true);

protected:
    virtual void CreateAndStartWorkers();
    virtual void TerminateWorkers();
    virtual void CreateTask(G4int id,G4int evts);
    //Creates a concrete tbb::task with index id
    //responsible for evts events
protected:
    //Barriers mechanism for TBB is non existing
    virtual void WaitForReadyWorkers() {}
    virtual void WaitForEndEventLoopWorkers() {}
    virtual void ThisWorkerReady() {}
    virtual void ThisWorkerEndEventLoop() {}
    virtual WorkerActionRequest ThisWorkerWaitForNextAction() { return WorkerActionRequest::UNDEFINED; }
    virtual void NewActionRequest( WorkerActionRequest /*newRequest*/ ) {}
private:
    // tbb::task_list* theTasks;
    G4int nEvtsPerTask;
};

#endif //TBBMASTERRUNMANAGER_HH
