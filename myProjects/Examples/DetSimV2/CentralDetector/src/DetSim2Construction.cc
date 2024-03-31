
#include "DetSim2Construction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "dyb2NylonFilmOpticalModel.hh"
#include "G4RegionStore.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
using namespace CLHEP;
DECLARE_TOOL(DetSim2Construction);

DetSim2Construction::DetSim2Construction(const std::string& name)
    : ToolBase(name)
    , solidSteel(0), logicSteel(0)
    , solidLAB(0), logicLAB(0), physiLAB(0)
    , solidAcrylic(0), logicAcrylic(0), physiAcrylic(0)
    , solidBalloon(0), logicBalloon(0), physiBalloon(0)
    , solidBalloonFSMR(0), logicBalloonFSMR(0), physiBalloonFSMR(0)
    , solidTarget(0), logicTarget(0), physiTarget(0)
{
    initVariables();
    declProp("BalloonMaterial", m_balloon_material = "PA");
    declProp("BalloonThickness", m_balloonThickness = 0.2);

}

DetSim2Construction::~DetSim2Construction() {

}

G4LogicalVolume* 
DetSim2Construction::getLV() {
    if (logicSteel) {
        return logicSteel;
    }
    initMaterials();

    makeSteelLogical();
    
    makeLABLogical();
    makeLABPhysical();
    
    makeAcrylicLogical();
    makeAcrylicPhysical();

    makeBalloonLogical();
    makeBalloonPhysical();
    
    makeLSLogical();
    makeLSPhysical();

    return logicSteel;
}

bool
DetSim2Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lWaterPool" ) {
        mothervol = 0;
    }else if (motherName == "lLAB" or motherName == "3inchInnerWater" ){
        mothervol = logicLAB;
    }
    if (not mothervol) {
        // don't find the volume.
        LogError << "Can't find mother volume " << motherName << std::endl;
        return false;
    }

    // retrieve the daughter's LV
    G4LogicalVolume* daughtervol = other->getLV();

    if (not daughtervol) {
        LogError << "Can't find dauther volume in " << motherName << std::endl;
        return false;
    }

    int copyno = 0;
    if (motherName == "3inchInnerWater") {
         copyno = 300000;
    }
    while (pos->hasNext()) {
        new G4PVPlacement(
            pos->next(),
            daughtervol,
            daughtervol->GetName()+"_phys",
            mothervol,
            false,
            copyno++
                );
    }

    if (motherName == "lLAB") {
        G4cout<<"PMT_Balloon_Number = "<<copyno<<G4endl;
    } else if (motherName == "3inchInnerWater") {
        G4cout<<"3inch PMT Number: = "<<copyno-300000<<G4endl;
    } else {

    }
    
    return true;
}

void
DetSim2Construction::initVariables() {
    m_transparency =  0.95;
    m_radLS = 17.7*m;
    m_balloonThickness = 0.2 *mm;
    m_balloonFSMRThickness = 0.01*um;
    m_balloonRad = m_radLS + m_balloonThickness;
    m_radAcrylic_in = m_balloonRad  + 0.1*mm ;
    m_thicknessArcylic = 10 *mm;
    m_radAcrylic = m_radAcrylic_in + m_thicknessArcylic;
    m_radLAB = m_radAcrylic + 2.5 *m;
    m_thicknessSteel = 4 *cm;
    m_radSteel = m_radLAB + m_thicknessSteel;

    m_radWP = 25.*m; // This value will be changed in the future
    m_heightWP = 50 * m;
}

void 
DetSim2Construction::initMaterials() {
    LS = G4Material::GetMaterial("LS");
    LAB = G4Material::GetMaterial("LAB");
    ETFE = G4Material::GetMaterial("ETFE");
    PA = G4Material::GetMaterial("PA");
    PE_PA = G4Material::GetMaterial("PE_PA");
    FEP = G4Material::GetMaterial("FEP");
    Water = G4Material::GetMaterial("Water");
    Steel = G4Material::GetMaterial("Steel");
    Acrylic = G4Material::GetMaterial("Acrylic");
}


void
DetSim2Construction::makeSteelLogical() {

    solidSteel = new G4Sphere("sSteel", 
            0*m, 
            m_radSteel, 
            0.*deg, 
            360.*deg, 
            0.*deg, 
            180.*deg);
    logicSteel = new G4LogicalVolume(solidSteel, 
            Steel, 
            "lSteel",
            0,
            0,
            0);

}

void
DetSim2Construction::makeLABLogical() {

    solidLAB = new G4Sphere("sLAB", 
            0*m, 
            m_radLAB, 
            0.*deg, 
            360.*deg, 
            0.*deg, 
            180.*deg);
    logicLAB = new G4LogicalVolume(solidLAB, 
            LAB, 
            "lLAB",
            0,
            0,
            0);

}

void
DetSim2Construction::makeLABPhysical() {
    physiLAB = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicLAB,    // its logical volume 
            "pLAB",       // its name
            logicSteel,  // its mother  volume
            false,           // no boolean operations
            0);              // no particular field
}

void
DetSim2Construction::makeAcrylicLogical() {
    solidAcrylic = new G4Sphere("sAcrylic",
            m_radAcrylic_in,
            m_radAcrylic,
            0.*deg, 
            360.*deg, 
            0.*deg, 
            180.*deg);
    logicAcrylic = new G4LogicalVolume(solidAcrylic, 
            Acrylic, 
            "lAcrylic",
            0,
            0,
            0);

    G4VisAttributes* acrylic_visatt = new G4VisAttributes(G4Colour(0, 0, 1.0));
    acrylic_visatt -> SetForceWireframe(true);  
    acrylic_visatt -> SetForceAuxEdgeVisible(true);
    //acrylic_visatt -> SetForceSolid(true);
    //acrylic_visatt -> SetForceLineSegmentsPerCircle(4);
    logicAcrylic -> SetVisAttributes(acrylic_visatt);
}

void
DetSim2Construction::makeAcrylicPhysical() {
    physiAcrylic = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicAcrylic,    // its logical volume 
            "pAcrylic",      // its name
            logicLAB,  // its mother  volume
            false,           // no boolean operations
            0);              // no particular field
}

void
DetSim2Construction::makeBalloonLogical() {
    solidBalloon = new G4Sphere("sBalloon", 
            0*m, 
            m_balloonRad,
            0.*deg,
            360.*deg, 
            0.*deg,
            180.*deg);
    
    G4Material* balloonmat = ETFE;
    if (m_balloon_material == "FEP") {
        balloonmat = FEP;
    }else if (m_balloon_material == "PA") {
        balloonmat = PA;
    }else if (m_balloon_material == "PE_PA") {
        balloonmat = PE_PA;
    }

    logicBalloon = new G4LogicalVolume(solidBalloon, 
            balloonmat, // Material FEP or ETFE 
            "lBalloon",
            0,
            0,
            0);

    // FSMR volume:
    // Fast Simulation Model Region to trigger dyb2NylonFilmOpticalModel.
    solidBalloonFSMR = new G4Sphere("sBalloonFSMR",
            m_balloonRad - m_balloonThickness/2. - m_balloonFSMRThickness/2., // inner R
            m_balloonRad - m_balloonThickness/2. + m_balloonFSMRThickness/2., // outer R
            0.*deg, // starting phi
            360.*deg, // delta phi
            0.*deg, // starting theta
            180.*deg // delta theta. G4 documentation:Theta must lie between 0-pi (incl).
            );
    logicBalloonFSMR = new G4LogicalVolume(solidBalloonFSMR, 
            balloonmat, // Material
            "lBalloonFSMR",
            0,
            0,
            0);
    G4cout<< "BalloonMat = "<<balloonmat<<G4endl;
    G4cout<< "BalloonThickness = "<<m_balloonThickness<<G4endl;
}

void
DetSim2Construction::makeBalloonPhysical() {
    // Main nylon volume:
    physiBalloon = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicBalloon,    // its logical volume
            "pBalloon",       // its name
            logicLAB,  // its mother  volume
            false,           // no boolean operations
            0,              // no particular field
            true);          // overlap check

    // FSMR volume:
    physiBalloonFSMR = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0.), // at (x,y,z)
            logicBalloonFSMR,    // its logical volume
            "pBalloonFSMR",       // its name
            logicBalloon,      // its mother  volume
            false,           // no boolean operations
            0,              // no particular field
            true);          // overlap check

}

void DetSim2Construction::makeNylonFastSimulation()
{
    G4Region* FSM_region = G4RegionStore::GetInstance()->GetRegion(logicBalloonFSMR->GetName()+"_FSM_region", false);
    if(FSM_region) G4RegionStore::DeRegister(FSM_region);
    FSM_region = new G4Region(logicBalloonFSMR->GetName()+"_FSM_region");
    FSM_region->AddRootLogicalVolume(logicBalloonFSMR);
    balloonFSM = new dyb2NylonFilmOpticalModel( logicBalloonFSMR->GetName()+"_optical_model",  FSM_region);
    balloonFSM->SetTransparency(m_transparency);
}

void
DetSim2Construction::makeLSLogical() {

    solidTarget = new G4Sphere("sTarget", 
            0*m, 
            m_radLS, 
            0.*deg, 
            360.*deg, 
            0.*deg, 
            180.*deg);
    logicTarget = new G4LogicalVolume(solidTarget, 
            LS, 
            "lTarget",
            0,
            0,
            0);

}

void
DetSim2Construction::makeLSPhysical() {
    physiTarget = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicTarget,    // its logical volume 
            "pTarget",       // its name
            logicBalloon,  // its mother  volume
            false,           // no boolean operations
            0);              // no particular field
}
