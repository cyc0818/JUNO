#ifndef DetSim2Construction_hh
#define DetSim2Construction_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IDetElement.h"
#include "dyb2NylonFilmOpticalModel.hh"

#include "globals.hh"

class G4Box;
class G4Sphere;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class DetSim2Construction : public IDetElement,
                            public ToolBase{
public:
    G4LogicalVolume* getLV();
    bool inject(std::string motherName, IDetElement* other, IDetElementPos* pos);

    DetSim2Construction(const std::string& name);
    ~DetSim2Construction();

private:

    void initMaterials();
    void initVariables();

    void makeSteelLogical();
    
    void makeLABLogical();
    void makeLABPhysical();
    // ++ Acrylic Ball
    void makeAcrylicLogical();
    void makeAcrylicPhysical();

    void makeBalloonLogical();
    void makeBalloonPhysical();
    
    void makeNylonFastSimulation();
    // +++ LS 
    void makeLSLogical();
    void makeLSPhysical();

    // ++ PMT
    //    The PMTs are injected into WP.
    void makePMTLogical();
    void makePMTPhysical();

private:
    G4Sphere*           solidSteel;
    G4LogicalVolume*    logicSteel;
    
    G4Sphere*           solidLAB;
    G4LogicalVolume*    logicLAB;
    G4VPhysicalVolume*  physiLAB;
    
    G4Sphere*           solidAcrylic;
    G4LogicalVolume*    logicAcrylic;
    G4VPhysicalVolume*  physiAcrylic;

    G4Sphere*           solidBalloon;
    G4LogicalVolume*    logicBalloon;
    G4VPhysicalVolume*  physiBalloon;
  
    G4Sphere*           solidBalloonFSMR;
    G4LogicalVolume*    logicBalloonFSMR;
    G4VPhysicalVolume*  physiBalloonFSMR;
    
    G4Sphere*           solidTarget;   // pointer to the solid Target
    G4LogicalVolume*    logicTarget;   // pointer to the logical Target
    G4VPhysicalVolume*  physiTarget;   // pointer to the physical Target

    G4LogicalVolume*   pmttube_log;
    G4VPhysicalVolume* pmttube_phys;
private:

    G4Material* LS;           // Target material
    G4Material* LAB;           
    G4Material* PA;           
    G4Material* PE_PA;           
    G4Material* Steel;           
    G4Material* ETFE;           
    G4Material* FEP;           
    G4Material* Acrylic;         
    G4Material* Water;
private:
    double m_radLS;
    double m_thicknessArcylic;
    double m_balloonThickness;
    double m_radAcrylic;
    double m_radAcrylic_in;
    double m_balloonRad;
    double m_radWP;
    double m_heightWP;
    double m_transparency; 
    double m_balloonFSMRThickness;
    double m_radSteel;
    double m_radLAB;
    double m_thicknessSteel;
    std::string m_balloon_material;
    double m_balloonthickness;
    dyb2NylonFilmOpticalModel* balloonFSM;
};

#endif
