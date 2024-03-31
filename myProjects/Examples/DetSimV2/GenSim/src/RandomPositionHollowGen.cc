
#include "DYB2PositionGenInterface.hh"
#include "RandomPositionGen.hh"
#include "RandomPositionHollowGen.hh"
#include "G4Navigator.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TransportationManager.hh"
namespace DYB2 {
namespace Ball {

RandomPositionHollowGen::RandomPositionHollowGen(double ball_r,double thick,G4String material)
    :m_ball_r(ball_r),m_thick(thick),m_material(material){
    m_gen = new RandomPositionGen(
                                    - m_ball_r,
                                    m_ball_r,
                                    -m_ball_r,
                                    m_ball_r,
                                    -m_ball_r,
                                    m_ball_r
                                    );

}
void
RandomPositionHollowGen::setSeed(long seed) 
{
    m_gen -> setSeed(seed);
}

G4ThreeVector
RandomPositionHollowGen::next()
{
    G4ThreeVector tmpvec;

    tmpvec = m_gen -> next();

    while( ! isOk(tmpvec) ) {
        tmpvec = m_gen -> next();
    }

    return tmpvec;
}
bool
RandomPositionHollowGen::isOk(G4ThreeVector pos)
{
    double x = pos.x();
    double y = pos.y();
    double z = pos.z();

    G4Navigator* gNavigator =
      G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking();
    G4VPhysicalVolume* pv;
    pv = gNavigator ->LocateGlobalPointAndSetup(pos, 0, true);
    G4LogicalVolume* pv_log = pv->GetLogicalVolume();
    G4Material* pv_mat = pv_log->GetMaterial();
    G4String material_name =pv_mat ->GetName();

    if (pv_mat ->GetName() == m_material )
    {
      return sqrt(x*x+y*y+z*z) <= m_ball_r && m_ball_r<=(sqrt(x*x+y*y+z*z)+m_thick);
    }else{return 0;}
}

}
   
}
