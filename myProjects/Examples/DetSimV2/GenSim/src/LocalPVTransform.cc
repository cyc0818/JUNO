
#include "LocalPVTransform.hh"

#include "PVPathTransformV2.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VPhysicalVolume.hh"

LocalPVTransform::LocalPVTransform(const G4String& parent_path,
                                   const G4VPhysicalVolume* pv)
    : m_parent_path(parent_path)
{
    m_local_to_global = getAffineTransform(pv);
    m_global_to_local = m_local_to_global.Inverse();

}

LocalPVTransform::LocalPVTransform(const G4String& path)
{
    // Using PVPathTransformV2 to setup the value
    PVPathTransformV2& pvpt = PVPathTransformV2::getInstance();
    m_local_to_global = pvpt.LocalToGlobal(path);
    m_global_to_local = m_local_to_global.Inverse();
}

bool
LocalPVTransform::PointGTL(G4ThreeVector& global_point) {
    m_global_to_local.ApplyPointTransform(global_point);
    return true;
}
bool
LocalPVTransform::PointLTG(G4ThreeVector& local_point) {
    m_local_to_global.ApplyPointTransform(local_point);
    return true;
}
bool
LocalPVTransform::AxisGTL(G4ThreeVector& global_axis) {
    m_global_to_local.ApplyAxisTransform(global_axis);
    return true;
}
bool
LocalPVTransform::AxisLTG(G4ThreeVector& local_axis) {
    m_local_to_global.ApplyAxisTransform(local_axis);
    return true;
}


// private helper methods
G4AffineTransform
LocalPVTransform::getAffineTransform(const G4VPhysicalVolume* target)
{
    // Using PVPathTransformV2 to get the translation
    PVPathTransformV2& pvpt = PVPathTransformV2::getInstance();

    // TODO
    //G4AffineTransform ltg = pvpt.LocalToGlobal(m_parent_path);
    //G4ThreeVector tv = target->GetTranslation();
    //ltg.ApplyPointTransform(tv);
    // now the tv became global
    //
    // use gNavigator to setup again?
    // Is it necessary

    G4AffineTransform parent_gtl = pvpt.GlobalToLocal(m_parent_path);
    G4AffineTransform global_to_local;
    global_to_local.InverseProduct(parent_gtl, 
                                     G4AffineTransform(
                                        target->GetRotation(),
                                        target->GetTranslation()
                                     ));
    return global_to_local.Inverse();
}
