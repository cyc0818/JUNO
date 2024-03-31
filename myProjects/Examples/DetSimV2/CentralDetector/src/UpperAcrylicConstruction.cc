
#include "UpperAcrylicConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Polycone.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;

DECLARE_TOOL(UpperAcrylicConstruction);

UpperAcrylicConstruction::UpperAcrylicConstruction(const std::string& name)
    : ToolBase(name)
{
    logicUpper = 0;
    initVariables();

    declProp("option", option="simple");
}

UpperAcrylicConstruction::~UpperAcrylicConstruction() {

}

G4LogicalVolume* 
UpperAcrylicConstruction::getLV() {
    if (logicUpper) {
        return logicUpper;
    }
    initMaterials();

    makeUpperLogical();

    return logicUpper;
}

bool 
UpperAcrylicConstruction::inject(std::string /* motherName */, IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
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
    G4cout<< " Upper_Acrylic_Number = "<< copyno <<G4endl;

    return true;
}

void
UpperAcrylicConstruction::initVariables() {
}

void 
UpperAcrylicConstruction::initMaterials() {
    Steel   = G4Material::GetMaterial("Steel",false);
}

void
UpperAcrylicConstruction::makeUpperLogical(){


    if (option=="yumiao") {
        ZUpper[0] = 0*mm; RminUpper[0] = 43.*mm; RmaxUpper[0] = 195.*mm;
        ZUpper[1] = -15*mm; RminUpper[1] = 43.*mm; RmaxUpper[1] = 195.*mm;
        for(int i=2;i<503;i++)
            {
                ZUpper[i] = (-15. - 40./500.*(i-2))*mm;
                RminUpper[i] = 55.5*mm;
                RmaxUpper[i] = (110.-sqrt(40.*40.-(ZUpper[i]+55.)*(ZUpper[i]+55.)))*mm;
            }
        ZUpper[503] = -101.*mm; RminUpper[503] = 55.5*mm; RmaxUpper[503] = 70*mm;
    
        base_steel = new G4Polycone("base_steel",0.0*deg,360.0*deg,504,ZUpper,RminUpper,RmaxUpper);
    } else if (option=="simple") {

        static double ZUpper4[4];
        static double RminUpper4[4];
        static double RmaxUpper4[4];

        ZUpper4[0] = 0*mm;   RminUpper4[0] = 43.*mm; RmaxUpper4[0] = 195.*mm;
        ZUpper4[1] = -15*mm; RminUpper4[1] = 43.*mm; RmaxUpper4[1] = 195.*mm;
        ZUpper4[2] = -15*mm; RminUpper4[2] = 55.5*mm; RmaxUpper4[2] = 70.*mm;
        ZUpper4[3] = -101*mm; RminUpper4[3] = 55.5*mm; RmaxUpper4[3] = 70.*mm;

        base_steel = new G4Polycone("base_steel",0.0*deg,360.0*deg,4,ZUpper4,RminUpper4,RmaxUpper4);
    }
    logicUpper = new G4LogicalVolume(
            base_steel,
            Steel,
            "lUpper",
            0,
            0,
            0);
  G4VisAttributes* addition_visatt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
  addition_visatt -> SetForceWireframe(true);  
  addition_visatt -> SetForceAuxEdgeVisible(true);
  //fasteners_visatt -> SetForceSolid(true);
  //fasteners_visatt -> SetForceLineSegmentsPerCircle(4);
  logicUpper -> SetVisAttributes(addition_visatt);


}
