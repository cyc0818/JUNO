#ifndef DetSim0Construction_hh
#define DetSim0Construction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "globals.hh"
class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class LSExpDetectorMessenger;
class IPMTManager;


class DetSim0Construction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    DetSim0Construction(const std::string& name);
    ~DetSim0Construction();

private:
    void initMaterials();
    void initVariables();

    void makeSteelBallLogical();

    void makeLSLogical();
    void makeLSPhysical();

    void makePMTLogical();
    void makePMTPhysical();

private:

    G4Sphere*           solidSteelBall;
    G4LogicalVolume*    logicSteelBall;

    G4Sphere*           solidTarget;   // pointer to the solid Target
    G4LogicalVolume*    logicTarget;   // pointer to the logical Target
    G4VPhysicalVolume*  physiTarget;   // pointer to the physical Target

    G4Sphere*           solidAcrylic;
    G4LogicalVolume*    logicAcrylic;
    G4VPhysicalVolume*  physiAcrylic;

    G4Box*              solidOD;
    G4LogicalVolume*    logicOD;
    G4VPhysicalVolume*  physiOD;   
 
    G4Tubs*            solidCoverMom;
    G4LogicalVolume*   logicCoverMom;
    G4VPhysicalVolume* physiCoverMom;
     
    G4Tubs*            solidCover;
    G4LogicalVolume*   logicCover;
    G4VPhysicalVolume* physiCover;

    G4Tubs*            solidTopCoverMom;
    G4LogicalVolume*   logicTopCoverMom;
    G4VPhysicalVolume* physiTopCoverMom;

    G4Tubs*            solidTopCover;
    G4LogicalVolume*   logicTopCover;
    G4VPhysicalVolume* physiTopCover;

    G4Tubs*            solidPMT;
    G4LogicalVolume*   logicPMT;
    G4VPhysicalVolume* physiPMT;

    G4LogicalVolume*   pmttube_log;
    G4VPhysicalVolume* pmttube_phys;

private:
    // parameters of Detector
    // Variables
    G4double m_radLS;
    G4double m_steelBallThickness;
    G4double m_steelBallRad;
    G4double m_pmtPlaceRad;
    G4double m_pmtHeight;

private:
    // materials 
    G4Material* Galactic;          // Default material
    G4Material* Air;          // Default material
    G4Material* GdLS;           // Target material
    G4Material* LS;           // Target material
    G4Material* ESR;         // Detector material
    G4Material* Tyvek;         // Detector material
    G4Material* Acrylic;         // Detector material
    G4Material* DummyAcrylic;         // Detector material
    G4Material* Teflon;         // Detector material
    G4Material* Photocathode_mat;
    G4Material* Steel;
    G4Material* Vacuum;
    G4Material* Pyrex;
    G4Material* Oil;
    G4Material* Water;
};

#endif
