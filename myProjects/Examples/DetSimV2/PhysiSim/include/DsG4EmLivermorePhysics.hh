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
// $Id: DsG4EmLivermorePhysics.hh 105735 2017-08-16 12:59:43Z gcosmo $

#ifndef DsG4EmLivermorePhysics_h
#define DsG4EmLivermorePhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "G4EmParticleList.hh"
#include "globals.hh"

#include "SniperKernel/ToolBase.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DsG4EmLivermorePhysics : public G4VPhysicsConstructor
                              , public ToolBase
{
public:

  DsG4EmLivermorePhysics( const G4String& name="EM" );

  virtual ~DsG4EmLivermorePhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
  G4EmParticleList partList;

  bool m_positronium_flag;   //true: on; false: off
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






