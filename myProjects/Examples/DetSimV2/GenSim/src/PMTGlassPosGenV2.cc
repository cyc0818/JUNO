
#include "PMTGlassPosGenV2.hh"
#include "IPMTManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "LocalPVTransform.hh"

#include <cassert>

namespace DYB2 {

PMTGlassPosGenV2::~PMTGlassPosGenV2() {
    CACHE::iterator it;
    for (it=m_cache.begin(); it!=m_cache.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = NULL;
        }
    }
    m_cache.clear();
}

G4ThreeVector
PMTGlassPosGenV2::getRndmPosSurface(G4VPhysicalVolume* target, 
                                  G4String material_name)
{
    LocalPVTransform* lpvt = NULL;
    if (m_cache.count(target)) {
        lpvt = m_cache[target];
    } else {
        lpvt = new LocalPVTransform(m_parentpath, target);
        m_cache[target] = lpvt;
    }

    // Create the position generator
    G4ThreeVector rndm_pos;
    while( true ) {
        assert(pmt_manager);
        rndm_pos=pmt_manager->GetPosInPMT();
        // convert local to global
        lpvt->PointLTG(rndm_pos);
        // now, rndm_pos is global
        if (isPositionInMaterial(rndm_pos, material_name)) {
            break;
        }
    }

    return rndm_pos;
}

G4bool
PMTGlassPosGenV2::isPositionInMaterial(G4ThreeVector pos, 
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
