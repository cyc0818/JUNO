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
// CHANGE HISTORY
// --------------

#include "G4ios.hh"
#include "LSExpMTEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"

#include "G4ios.hh"

#include "JUNOEventUserInfo.hh"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/SimHeader.h"

#include "junoHit_PMT.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

#include "Geometry/PMT.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

LSExpMTEventAction::LSExpMTEventAction()
  : drawFlag("all")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

LSExpMTEventAction::~LSExpMTEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void LSExpMTEventAction::BeginOfEventAction(const G4Event* evt)
{
    int eid = evt->GetEventID();

    if (evt->GetUserInformation()) {
        JUNOEventUserInfo* eui = dynamic_cast<JUNOEventUserInfo*>(evt->GetUserInformation());
        if (eui) {
            JunoGlobalBuffer::Elem* elem = eui->elem;
            std::shared_ptr<JM::EvtNavigator> nav{nullptr};
            JM::SimHeader* simhdr = nullptr;
            JM::SimEvt* simevt = nullptr;
            
            if (elem) {
                nav = elem->dptr->event;
            }
            if (nav) {
                simhdr = JM::getHeaderObject<JM::SimHeader>(nav.get());
            }
            if (simhdr) {
                simevt = dynamic_cast<JM::SimEvt*>(simhdr->event());
            }
            if (simevt) {
                eid = simevt->getEventID();
            }
        }
    }

    //if(evt->GetEventID()%1000 == 0) G4cout << "Begin of Event --> " << evt->GetEventID() << G4endl;
    G4cout << "Begin of Event --> " << evt->GetEventID()
           << " (" << eid << ")" << G4endl;

    //G4UImanager* uiMgr = G4UImanager::GetUIpointer();
    //if(evt->GetEventID() == 1537)
    // uiMgr->ApplyCommand("/tracking/verbose 1");
    //else
    //  uiMgr->ApplyCommand("/tracking/verbose 0");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void LSExpMTEventAction::EndOfEventAction(const G4Event* evt)
{
    if (evt->GetUserInformation()) {
        JUNOEventUserInfo* eui = dynamic_cast<JUNOEventUserInfo*>(evt->GetUserInformation());
        if (eui) {
            JunoGlobalBuffer::Elem* elem = eui->elem;

            // store the info into data model
            std::shared_ptr<JM::EvtNavigator> nav{nullptr};
            JM::SimHeader* simhdr = nullptr;
            JM::SimEvt* simevt = nullptr;
            
            if (elem) {
                nav = elem->dptr->event;
            }
            if (nav) {
                simhdr = JM::getHeaderObject<JM::SimHeader>(nav.get());
            }
            if (simhdr) {
                simevt = dynamic_cast<JM::SimEvt*>(simhdr->event());
            }
            if (simevt) {
                G4SDManager * SDman = G4SDManager::GetSDMpointer();
                G4int CollID = SDman->GetCollectionID("hitCollection");
                junoHit_PMT_Collection* col = 0;
                G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
                if (HCE && CollID >= 0) {
                    col = (junoHit_PMT_Collection*)(HCE->GetHC(CollID));
                } else {
                    G4cerr << "No hits collection found." << std::endl;
                }
                if (col) {
                    int n_hit = col->entries();
                    int m_nPhotons = n_hit;
                    double m_timewindow = 0.0;

                    bool minmax_initialized = false;
                    double max_CDLPMT_hittime = 0;
                    double min_CDLPMT_hittime = 0;
                    for (int i = 0; i < n_hit; ++i) {
                        // create new hit
                        // The PMT Hit can be from WP (Water Pool) or CD (Central
                        // Detector). 
                        // Please use the copy no to distinguish the PMT.
                        int copyno = (*col)[i]->GetPMTID();
                        JM::SimPMTHit* jm_hit = 0;
                        // FIXME: hard code the copy no
                        if (PMT::IsCD(copyno)) {
                            // TODO because in current Data Model, the 3inch and the 20inch
                            // PMTs are in the same collection.
                            jm_hit = simevt->addCDHit();
                        } else if (PMT::IsWP(copyno)) {
                            jm_hit = simevt->addWPHit();
                        }
                        if(PMT::IsCD(copyno) and PMT::Is20inch(copyno)) {
                            if (!minmax_initialized) { // initialize for the first hit
                                max_CDLPMT_hittime = ((*col)[i]->GetTime());
                                min_CDLPMT_hittime = ((*col)[i]->GetTime());
                                minmax_initialized = true;
                            } else {
                                if ((*col)[i]->GetTime() < min_CDLPMT_hittime) {
                                    min_CDLPMT_hittime = (*col)[i]->GetTime();
                                }
                                if ((*col)[i]->GetTime() > max_CDLPMT_hittime) {
                                    max_CDLPMT_hittime = (*col)[i]->GetTime();
                                }
                            }
                        }
                        jm_hit->setPMTID( (*col)[i]->GetPMTID() );
                        jm_hit->setNPE( (*col)[i]->GetCount() );
                        jm_hit->setHitTime( (*col)[i]->GetTime() );
                        jm_hit->setTrackID( (*col)[i]->GetProducerID() );
                        jm_hit->setLocalTheta( (*col)[i]->GetTheta() );
                        jm_hit->setLocalPhi( (*col)[i]->GetPhi() );
                    }
                    m_timewindow = max_CDLPMT_hittime - min_CDLPMT_hittime;

                    if (m_nPhotons>0) {
                        simhdr->setCDLPMTtotalHits(m_nPhotons);
                    }
                    if (m_timewindow>0) {
                        simhdr->setCDLPMTtimeWindow(m_timewindow);
                    }
                }
            }

            // Put hit data into data model is DONE

            JunoGlobalBuffer* m_gbuf = JunoGlobalStream::GetBufferFrom("GEvtStream");
            m_gbuf->setDone(elem); 
            G4cout << "Mark event " << evt->GetEventID() << " done: "
                   << " elem: " << elem
                   << " elem->dptr: " << elem->dptr
                   << G4endl;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

