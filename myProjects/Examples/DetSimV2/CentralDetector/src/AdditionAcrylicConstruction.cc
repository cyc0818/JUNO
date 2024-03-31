
#include "AdditionAcrylicConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Polycone.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;

DECLARE_TOOL(AdditionAcrylicConstruction);

AdditionAcrylicConstruction::AdditionAcrylicConstruction(const std::string& name)
    : ToolBase(name)
{
    logicAddition = 0;
    initVariables();

    declProp("option", option="simple");
}

AdditionAcrylicConstruction::~AdditionAcrylicConstruction() {

}

G4LogicalVolume* 
AdditionAcrylicConstruction::getLV() {
    if (logicAddition) {
        return logicAddition;
    }
    initMaterials();

    makeAdditionLogical();

    return logicAddition;
}

bool 
AdditionAcrylicConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lAddition")
    {
        mothervol = logicAddition;
    }
    if (not mothervol) {
        // don't find the volume.
        return false;
    }

    // retrieve the daughter's LV
    G4LogicalVolume* daughtervol = other->getLV();

    if (not daughtervol) {
        return false;
    }

    int copyno = 0;
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
    G4cout<< " Addition_Acrylic_Number = "<< copyno <<G4endl;

    return true;
}

void
AdditionAcrylicConstruction::initVariables() {
}

void 
AdditionAcrylicConstruction::initMaterials() {
    Acrylic = G4Material::GetMaterial("Acrylic",false);
    Water   = G4Material::GetMaterial("Water",false);
}

void
AdditionAcrylicConstruction::makeAdditionLogical(){

    if (option=="yumiao") {

        ZNodes[0] = 5.7*mm; RminNodes[0] = 0*mm; RmaxNodes[0] = 450.*mm;
        for(int i=1;i<502;i++)
            {
                ZNodes[i] = 0 - 140./500.*(i-1)*mm;
                RminNodes[i] = 0*mm;
                RmaxNodes[i] = (697.64-sqrt(776*776-(ZNodes[i]+735.43)*(ZNodes[i]+735.43)))*mm;
            }

        solidAddition_down = new G4Polycone("solidAddition_down",0.0*deg,360.0*deg,502,ZNodes,RminNodes,RmaxNodes);

    } else if (option=="simple") {

        double ZNodes3[3];
        double RminNodes3[3];
        double RmaxNodes3[3];
        ZNodes3[0] = 5.7*mm; RminNodes3[0] = 0*mm; RmaxNodes3[0] = 450.*mm;
        ZNodes3[1] = 0.0*mm; RminNodes3[1] = 0*mm; RmaxNodes3[1] = 450.*mm;
        ZNodes3[2] = -140.0*mm; RminNodes3[2] = 0*mm; RmaxNodes3[2] = 200.*mm;

        solidAddition_down = new G4Polycone("solidAddition_down",0.0*deg,360.0*deg,3,ZNodes3,RminNodes3,RmaxNodes3);

    }


//    solidAddition_down = new G4Tubs("solidAddition_down",0,199.67*mm,140*mm,0.0*deg,360.0*deg);
//    solidAddition_down = new G4Cons("solidAddition_down",0.*mm,450.*mm,0.*mm,200*mm,70.*mm,0.*deg,360.*deg);
    solidAddition_up = new G4Sphere("solidAddition_up",0*mm,17820*mm,0.0*deg,360.0*deg,0.0*deg,180.*deg);
    
    uni_acrylic1 = new G4SubtractionSolid("uni_acrylic1",solidAddition_down,solidAddition_up,0,G4ThreeVector(0.01*mm,0*mm,+17820.0*mm));

    solidAddition_up1 = new G4Tubs("solidAddition_up1",120*mm,208*mm,15.2*mm,0.0*deg,360.0*deg);
    uni_acrylic2 = new G4SubtractionSolid("uni_acrylic2",uni_acrylic1,solidAddition_up1,0,G4ThreeVector(0.*mm,0.*mm,-20*mm));
    solidAddition_up2 = new G4Tubs("solidAddition_up2",0,14*mm,52.5*mm,0.0*deg,360.0*deg);

    for(int i=0;i<8;i++)
    {
    uni_acrylic3 = new G4SubtractionSolid("uni_acrylic3",uni_acrylic2,solidAddition_up2,0,G4ThreeVector(164.*cos(i*pi/4)*mm,164.*sin(i*pi/4)*mm,-87.5));
    uni_acrylic2 = uni_acrylic3;

    }

 
    logicAddition = new G4LogicalVolume(
            uni_acrylic2,// solidAddition_down,
            Acrylic,
            "lAddition",
            0,
            0,
            0);
  G4VisAttributes* addition_visatt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
  addition_visatt -> SetForceWireframe(true);  
  addition_visatt -> SetForceAuxEdgeVisible(true);
  //fasteners_visatt -> SetForceSolid(true);
  //fasteners_visatt -> SetForceLineSegmentsPerCircle(4);
  logicAddition -> SetVisAttributes(addition_visatt);


}
