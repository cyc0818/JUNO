#include "RockConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"

DECLARE_TOOL(TopRockConstruction);
DECLARE_TOOL(BottomRockConstruction);


// = Top Rock =
TopRockConstruction::TopRockConstruction(const std::string& name)
    : ToolBase(name)
{
    logicTopRock = 0;
}

TopRockConstruction::~TopRockConstruction() {

}

G4LogicalVolume*
TopRockConstruction::getLV() {
    if (logicTopRock) {
        return logicTopRock;
    }
    initMaterials();
    initVariables();
    makeTopRockLogical();
    makeTopRockLogical();
    makeTopRockUpperLogical();//upper box
    makeTopRockUpperPhysical();
    makeTopRockDomeLogical(); //dome rock
    makeTopRockDomePhysical();
    makeTopRockDomeAirLogical();//semi-cylindircal air
    makeTopRockDomeAirPhysical();
    makeTopRockUnderLogical();//under box for ExpHall
    makeTopRockUnderPhysical();
    return logicTopRock;
}

bool
TopRockConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    G4LogicalVolume* mothervol = 0;
    G4LogicalVolume* daughtervol = 0;

    if (motherName == "lTopRock") {
        //mothervol = logicTopRock;
        mothervol = logicExpRockBox;
    } else {
        LogWarn << "Can't find < " 
                << motherName
                << "> in TopRock."
                << std::endl;
        return false;
    }

    daughtervol = other->getLV();
    LogDebug << "Exp Hall. LV: " << daughtervol << std::endl;
    if (not daughtervol) {
        LogWarn << "Can't find any injected LV in TopRock" << std::endl;
        return false;
    }

    // Place the Exp.Hall into the top rock
    if (pos == 0 and daughtervol->GetName() == "lExpHall") {
        LogDebug << "Create pExpHall Begin:" << std::endl;
        LogDebug << "lExpHall: " << daughtervol << std::endl;
        LogDebug << "lTopRock: " << mothervol << std::endl;
        new G4PVPlacement(0, // no rotation
                G4ThreeVector(m_expHall_offsetX, 0, 0), // offset
                daughtervol,
                "pExpHall",
                mothervol,
                false,
                0
                );
        LogDebug << "Create pExpHall End." << std::endl;
    }

    return true;
}

double
TopRockConstruction::geom_info(const std::string& param) {
    // make sure the geom is setup
    if (getLV()) {

    } else {
        throw SniperException(std::string("Failed to initialize TopRockConstruction "));
    }
    if (param == "TopRockOffset.X") {
        LogInfo << "Access geom info of TopRockConstruction: "
                << " TopRockOffset.X: "
                << m_topRock_offsetX
                << std::endl;
        return m_topRock_offsetX;
    }
    else if (param == "TopRockOffset.Z") {
        LogInfo << "Access geom info of TopRockConstruction: "
                << " TopRockOffset.Z: "
                << m_topRock_offsetZ 
                << std::endl;
        return m_topRock_offsetZ;
    }
    throw SniperException(std::string("Can't find param: ") + param);
    return 0;

}

void
TopRockConstruction::initMaterials() {
    Rock = G4Material::GetMaterial("Rock");
    Air = G4Material::GetMaterial("Air");
    Galactic=G4Material::GetMaterial("Galactic");
}

void
TopRockConstruction::initVariables() {
    SniperPtr<DetSimAlg> detsimalg(*getParent(), "DetSimAlg");
    if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
    }
    ToolBase* t = detsimalg->findTool("GlobalGeomInfo");
    assert(t);
    IDetElement* globalinfo = dynamic_cast<IDetElement*>(t);

    m_expHallXLen_tot = globalinfo->geom_info("ExpHall.XLenTot");//56.25*m
    m_expHall_domeHeight = globalinfo->geom_info("ExpHall.domeH"); //16*m
    m_expHall_domeRadius = globalinfo->geom_info("ExpHall.domeR");//26.76*m
    
    m_topRockX = globalinfo->geom_info("TopRock.XLen")/2;//Half length, (56.25+6)/2*m
    m_topRockY = globalinfo->geom_info("TopRock.YLen")/2;//Half length, (49+6)/2*m
    m_topRockZ = globalinfo->geom_info("TopRock.ZLen")/2;//Bottom rectangular rock, 11/2*m 
    m_topRockZ_tot = globalinfo->geom_info("TopRock.ZLenTot")/2;// (11+19)/2*m 
    LogInfo << " Top Rock, bottom rectangular rock size: ("
            << m_topRockX * 2./1000<<", "
            << m_topRockY * 2./1000<<", "
            << m_topRockZ * 2./1000<<") m"<<std::endl;

    double m_waterPoolRadius = globalinfo->geom_info("WaterPool.R");
    //double m_poolCH = globalinfo->geom_info("WaterPool.ConcreteH");//TODO in next version
    double m_poolCH = globalinfo->geom_info("WaterPool.H");//43.5m
    double expHallGroundZ = m_poolCH - m_waterPoolRadius; 
     LogInfo << " Exp. Hall ground positionZ: "<<expHallGroundZ/1000.<<" m"<<std::endl;

    m_topRock_offsetX = 3.125*m; //JUNOEng-doc-5-v3  
    m_topRock_offsetY = 0;
    m_topRock_offsetZ = m_topRockZ_tot + expHallGroundZ; //relative to world 
    LogInfo << " Top Rock, bottom rectangular rock offsetXYZ in world: ("
            << m_topRock_offsetX / 1000<<", "
            << m_topRock_offsetY / 1000<<", "
            << m_topRock_offsetZ / 1000<<") m"<<std::endl;

    m_topRock_domeHeight = globalinfo->geom_info("TopRock.domeH");//(16+3)*m
    m_topRock_domeRadius = globalinfo->geom_info("TopRock.domeR");//(26.76+3)*m,
    m_topRock_domeOffsetZ = - m_topRockZ * 2 - m_topRock_domeHeight / 2  + 0.24*m; //dome center is ~0.24m above ground  
    LogInfo << " Top Rock, dome Height and Radius: "<<m_topRock_domeHeight / 1000<<", "<<m_topRock_domeRadius / 1000<<" m"<<std::endl;
    
    m_expHall_offsetX = -m_topRock_offsetX;//keep expHall center above CD center 
    //m_expHall_offsetZ = 0;//Exp hall in top Rock, rock centerZ is the same with expHallZ
    LogInfo << " m_topRockZ: " << m_topRockZ
            << " WaterPool.R: " << globalinfo->geom_info("WaterPool.R")
            << std::endl;
}

void
TopRockConstruction::makeTopRockLogical() {
    //solidTopRock = new G4Box("sTopRock", m_topRockX, m_topRockY, m_topRockZ);
    //logicTopRock = new G4LogicalVolume(solidTopRock,
    //    Rock,
    //    "lTopRock",
    //    0,0,0);
    
    //Top Rock Box = ExpHall Rock Box + Dome Box
    solidTopRock = new G4Box("sTopRock", m_topRockX, m_topRockY, m_topRockZ_tot);
    logicTopRock = new G4LogicalVolume(solidTopRock, Rock, "lTopRock", 0, 0, 0);
}

void
TopRockConstruction::makeTopRockUpperLogical() {

    //Dome Box
    solidDomeRockBox = new G4Box("sDomeRockBox", m_topRockX, m_topRockY, m_topRock_domeHeight/2.);
    logicDomeRockBox = new G4LogicalVolume(solidDomeRockBox, Galactic, "lDomeRockBox", 0, 0, 0);
}

void
TopRockConstruction::makeTopRockUpperPhysical() {

    new G4PVPlacement(0, 
        G4ThreeVector(0,0,m_topRockZ_tot - m_topRock_domeHeight/2.), 
        logicDomeRockBox, "pDomeRockBox", logicTopRock, false, 0);               
}

void
TopRockConstruction::makeTopRockDomeLogical() {

    //Dome Rock in Dome Box
    G4VSolid* solidTopRock_tub = new G4Tubs("sTopRock_tub", 0, m_topRock_domeRadius, m_topRockX, 0, 360*deg);
    G4VSolid* solidTopRock_tub_sub = new G4Box("sTopRock_tub_sub", m_topRock_domeRadius*2 - m_topRock_domeHeight, m_topRock_domeRadius*2, m_topRockX + 1*m);
    G4ThreeVector trans_domesub(m_topRock_domeRadius, 0, 0);
    solidTopRock_dome = new G4SubtractionSolid("sTopRock_dome", solidTopRock_tub, solidTopRock_tub_sub, 0, trans_domesub);
    logicTopRock_dome = new G4LogicalVolume(solidTopRock_dome, Rock, "lDomeRock", 0, 0, 0);
}


void
TopRockConstruction::makeTopRockDomePhysical() {

    G4RotationMatrix* yRot_dome = new G4RotationMatrix; // Rotates X and Z axes only
    yRot_dome->rotateY(-90*deg); // Rotates 90 degrees
    new G4PVPlacement(yRot_dome, 
        G4ThreeVector(0, 0, m_topRock_domeOffsetZ), 
        logicTopRock_dome, "pDomeRock", logicDomeRockBox, false, 0);               
}

void
TopRockConstruction::makeTopRockDomeAirLogical() {
    //Dome Air 
    G4VSolid* solidTopRock_tubAir = new G4Tubs("sTopRock_tubAir", 0, m_expHall_domeRadius, m_expHallXLen_tot/2., 0, 360*deg);
    G4VSolid* solidTopRock_tubAir_sub = new G4Box("sTopRock_tubAir_sub", 2 * m_expHall_domeRadius - m_expHall_domeHeight, 2 * m_expHall_domeRadius, m_topRockX);
    G4ThreeVector trans_tubAir_sub(m_expHall_domeRadius, 0, 0);
    solidTopRock_domeAir = new G4SubtractionSolid("sTopRock_domeAir", solidTopRock_tubAir, solidTopRock_tubAir_sub, 0, trans_tubAir_sub);
    logicTopRock_domeAir = new G4LogicalVolume(solidTopRock_domeAir, Air, "lDomeAir", 0, 0, 0);
}

void
TopRockConstruction::makeTopRockDomeAirPhysical() {
    new G4PVPlacement(0, //mother is rock dome, so no rotation 
        G4ThreeVector(0,0,0), 
        logicTopRock_domeAir, "pDomeAir", logicTopRock_dome, false, 0);               
}

void
TopRockConstruction::makeTopRockUnderLogical() {
    //Bottom box in TopRock to fill ExpHall
    solidExpRockBox = new G4Box("sExpRockBox", m_topRockX, m_topRockY, m_topRockZ);
    logicExpRockBox = new G4LogicalVolume(solidExpRockBox, Rock, "lExpRockBox", 0, 0, 0);

}

void
TopRockConstruction::makeTopRockUnderPhysical() {

    new G4PVPlacement(0, 
        G4ThreeVector(0,0,- m_topRockZ_tot + m_topRockZ), 
        logicExpRockBox, "pExpRockBox", logicTopRock, false, 0);             

}

// = Bottom Rock =
BottomRockConstruction::BottomRockConstruction(const std::string& name)
    : ToolBase(name)
{
    logicBottomRock = 0;
}

BottomRockConstruction::~BottomRockConstruction() {

}

G4LogicalVolume*
BottomRockConstruction::getLV() {
    if (logicBottomRock) {
        return logicBottomRock;
    }
    initMaterials();
    initVariables();
    makeBottomRockLogical();

    makePoolLiningLogical();
    makePoolLiningPhysical();
    return logicBottomRock;
}

bool
BottomRockConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    G4LogicalVolume* mothervol = 0;
    G4LogicalVolume* daughtervol = 0;

    if (motherName == "lPoolLining") {
        mothervol = logicPoolLining;
    } else {
        return false;
    }

    if (not other) {
        return false;
    }
    daughtervol = other->getLV();
    if (not daughtervol) {
        return false;
    }

    // Place the Water Pool into the bottom rock
    if (pos == 0) {
        new G4PVPlacement(0, // no rotation
                G4ThreeVector(0, 0, 0), // offset
                daughtervol,
                "pOuterWaterPool",
                mothervol,
                false,
                0
                );
    }

    return true;
}

void
BottomRockConstruction::initMaterials() {
    Rock = G4Material::GetMaterial("Rock");
    Tyvek = G4Material::GetMaterial("Tyvek");
}

void
BottomRockConstruction::initVariables() {
    SniperPtr<DetSimAlg> detsimalg(*getParent(), "DetSimAlg");
    if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
    }
    ToolBase* t = detsimalg->findTool("GlobalGeomInfo");
    assert(t);
    IDetElement* globalinfo = dynamic_cast<IDetElement*>(t);

    // TODO: load these data from geom svc
    double m_waterPoolRadius = globalinfo->geom_info("WaterPool.R");//43.5/2*m
    double m_rockThickness =  globalinfo->geom_info("Rock.Thickness");//3*m
    //double m_poolCH = globalinfo->geom_info("WaterPool.ConcreteH");//44m of pool depth, water depth 43.5m
    double m_poolCH = globalinfo->geom_info("WaterPool.H");//43.5*m TODO 44*m
    m_btmRockH = globalinfo->geom_info("BtmRock.H");//(44+3)m 
    m_btmRockR = globalinfo->geom_info("BtmRock.R");//(43.5/2+3)m

    m_btmRockZtop = m_poolCH - m_waterPoolRadius; //21.75*m, still keep CD center as the world center
    m_btmRockZbottom = -m_waterPoolRadius - m_rockThickness;//(-21.75+3)*m
    
    double liningThickness = globalinfo->geom_info("WaterPool.LiningThick");//previous 3mm, now 5mm 
    m_poolLiningH = m_poolCH + liningThickness;

    m_poolLiningZtop = m_btmRockZtop;
    m_poolLiningZbottom = -m_waterPoolRadius - liningThickness;
    m_poolLiningR = m_waterPoolRadius + liningThickness;
}

void
BottomRockConstruction::makeBottomRockLogical() {
    // solidBottomRock = new G4Tubs("sBottomRock", 0, m_btmRockR, m_btmRockZ, 0, 360*deg);
    // please note, increase order
    static G4double  zPlane[] = {m_btmRockZbottom, m_btmRockZtop};
    static G4double  rInner[] = {0., 0.};
    static G4double  rOuter[] = {m_btmRockR, m_btmRockR};
    solidBottomRock = new G4Polycone("sBottomRock", //const G4String& pName,
                                     0, //G4double  phiStart,
                                     360*deg, //G4double  phiTotal,
                                     2, //G4int         numZPlanes,
                                     zPlane, //const G4double  zPlane[],
                                     rInner, //const G4double  rInner[],
                                     rOuter //const G4double  rOuter[])
                                    );
    logicBottomRock = new G4LogicalVolume(solidBottomRock,
        Rock,
        "lBtmRock",
        0, 0, 0);

}

void
BottomRockConstruction::makePoolLiningLogical()
{
    // solidPoolLining = new G4Tubs("sPoolLining", 0, m_poolLiningR, m_poolLiningZ, 0, 360*deg);
    static G4double  zPlane[] = {m_poolLiningZbottom, m_poolLiningZtop};
    static G4double  rInner[] = {0., 0.};
    static G4double  rOuter[] = {m_poolLiningR, m_poolLiningR};
    solidPoolLining = new G4Polycone("sPoolLining", //const G4String& pName,
                                     0, //G4double  phiStart,
                                     360*deg, //G4double  phiTotal,
                                     2, //G4int         numZPlanes,
                                     zPlane, //const G4double  zPlane[],
                                     rInner, //const G4double  rInner[],
                                     rOuter //const G4double  rOuter[])
                                    );
    logicPoolLining = new G4LogicalVolume(solidPoolLining,
        Tyvek,
        "lPoolLining",
        0, 0, 0);
}

void
BottomRockConstruction::makePoolLiningPhysical()
{
    physiPoolLining = new G4PVPlacement(0,              // no rotation
        G4ThreeVector(0,0,0), // at (x,y,z)
        logicPoolLining,    // its logical volume
        "pPoolLining",       // its name
        logicBottomRock,      // its mother  volume
        false,           // no boolean operations
        0);              // no particular field
}
