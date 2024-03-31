#ifndef LocalPVTransform_hh
#define LocalPVTransform_hh

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"

class G4VPhysicalVolume;

class LocalPVTransform {
public:
    // TODO:
    // currently, the PV can't locate in a replicate Volume.
    // This ctor is for compatible.
    LocalPVTransform(const G4String& parent_path, const G4VPhysicalVolume* pv);
    LocalPVTransform(const G4String& whole_path);

    // G: Global 
    // L: Local

    bool PointGTL(G4ThreeVector& global_point);
    bool PointLTG(G4ThreeVector& local_point);

    bool AxisGTL(G4ThreeVector& global_axis);
    bool AxisLTG(G4ThreeVector& local_axis);

private:
    G4AffineTransform getAffineTransform(const G4VPhysicalVolume* target);

private:
    G4String m_parent_path;
    G4AffineTransform m_local_to_global;
    G4AffineTransform m_global_to_local;

};

#endif
