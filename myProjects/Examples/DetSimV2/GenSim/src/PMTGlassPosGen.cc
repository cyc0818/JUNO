
#include "PMTGlassPosGen.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"

#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4AffineTransform.hh"
#include "RandomPositionTubeGen.hh"

#include "IPMTManager.hh"

#include <cassert>

namespace DYB2 {

G4ThreeVector
PMTGlassPosGen::getRndmPosSurface(G4VPhysicalVolume* target, 
                                  G4String material_name)
{
    // Get the transform
    G4AffineTransform pmt_local_to_global = getAffineTransform(target);
    // Create the position generator
    G4ThreeVector rndm_pos;
    while( true ) {
        assert(pmt_manager);
        rndm_pos=pmt_manager->GetPosInPMT();
        // convert local to global
        pmt_local_to_global.ApplyPointTransform(rndm_pos);
        // now, rndm_pos is global
        if (isPositionInMaterial(rndm_pos, material_name)) {
            break;
        }
    }

    // DEBUG
    if (false) {
        G4Navigator* gNavigator =
            G4TransportationManager::GetTransportationManager()
            ->GetNavigatorForTracking();
        G4VPhysicalVolume* pv;
        pv = gNavigator ->
                LocateGlobalPointAndSetup(rndm_pos, 0, true);
        G4LogicalVolume* pv_log = pv->GetLogicalVolume();
        G4Material* pv_mat = pv_log->GetMaterial();
        G4cout << "### TEST Material:";
        G4cout << pv_mat->GetName() << G4endl;
    }

    return rndm_pos;
}

G4AffineTransform 
PMTGlassPosGen::getAffineTransform(G4VPhysicalVolume* target) 
{
    // frm_pos is the global position
    G4ThreeVector frm_pos = target->GetTranslation();
    // use frm_pos to find the target.
    G4LogicalVolume* target_log = target->GetLogicalVolume();
    G4VSolid* target_solid = target_log->GetSolid();

    G4Navigator* gNavigator =
        G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking();
    G4VPhysicalVolume* pv;
    pv = gNavigator ->
            LocateGlobalPointAndSetup(frm_pos, 0, true);
    // from physical volume in target to world.
    G4AffineTransform local_to_global
        = gNavigator->GetLocalToGlobalTransform();

    // local_pos is a tmp local position, 
    // Navigator will find the target by this position
    G4ThreeVector local_pos;

    while ( pv != target ) {
        local_pos = target_solid -> GetPointOnSurface();    
        // Try to convert local_pos to the Global Position.
        local_to_global.ApplyPointTransform(local_pos);
        // Again, set the new Physical Volume
        pv = gNavigator -> 
            LocateGlobalPointAndSetup(local_pos, 0, true);
    }
    // from target to world.
    G4AffineTransform local_to_global_new
        = gNavigator->GetLocalToGlobalTransform();

    return local_to_global_new;

}

G4bool
PMTGlassPosGen::isPositionInMaterial(G4ThreeVector pos, 
                                     G4String material_name)
{
    // Make sure the material exists.
    // If not exists, Give a warning, 
    // and return true to terminate the loop.
    if ( ! G4Material::GetMaterial(material_name) ) {
        G4cout << "WARNNING: "
               << "In PMTGlassPosGen: "
               << "Can't find ["
               << material_name
               << "]"
               << G4endl;
        return true;
    }
    G4Navigator* gNavigator =
        G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking();
    G4VPhysicalVolume* pv;
    pv = gNavigator ->
            LocateGlobalPointAndSetup(pos, 0, true);
    G4LogicalVolume* pv_log = pv->GetLogicalVolume();
    G4Material* pv_mat = pv_log->GetMaterial();
    return pv_mat->GetName() == material_name;
}

}
