#include <boost/python.hpp>
#include "Dimensions.hh"
#include "UpperChimneyMaker.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"

DECLARE_TOOL(UpperChimney);

UpperChimney::UpperChimney(const std::string& name)
    : ToolBase(name),
solidUpperChimney(0),logicUpperChimney(0),
solidUpperChimneyLS(0),  logicUpperChimneyLS(0),  physiUpperChimneyLS(0),
solidUpperChimneySteel(0),  logicUpperChimneySteel(0),  physiUpperChimneySteel(0),
solidUpperChimneyTyvek(0),  logicUpperChimneyTyvek(0),  physiUpperChimneyTyvek(0)
{
//    declProp("UseUpperChimney", m_use_UpperChimney=false );
//    declProp("UpperChimneyTop", m_TopToWater=0 );
//    declProp("Reflectivity", m_reflectivity=0 );
    declProp("UseUpperChimney", m_use_UpperChimney=true);//Default: upper chimney is enabled
    declProp("UpperChimneyTop", m_TopToWater=3.5 );//Default: chimney top to the water surface is 3.5m
    declProp("Reflectivity", m_reflectivity=0.1 );// Default: reflectivity of the inner chimney is 0.1
}

UpperChimney::~UpperChimney() {

}

G4LogicalVolume* 
UpperChimney::getLV() {
    // if already initialized
    if (logicUpperChimney) {
        return logicUpperChimney;
    }
    initVariables();
    initMaterials();
   // G4cout<<"UpperChimney Option: "<<m_use_UpperChimney<<"top to water: "<<TopToWater<<"box center: "<<GateCenter<<"!!!!!!!!!!!!!"<<G4endl;
//    makeUpperChimneyLogicalandPhysical();
    if (m_use_UpperChimney and m_TopToWater!=0 ) {
      G4cout<<"Begin Upper UpperChimney Construction"<<G4endl;
    makeUpperChimneyLogicalandPhysical();
    setupTopChimneyReflectorInCD();
    }
    G4VisAttributes* ct_visatt = new G4VisAttributes(G4Colour(0, 0.5, 1.0));
    G4VisAttributes* acrylic_visatt = new G4VisAttributes(G4Colour(0, 0.5, 1.0));
    ct_visatt -> SetForceWireframe(true);
    ct_visatt -> SetForceAuxEdgeVisible(true);
    acrylic_visatt -> SetForceSolid(true);
    acrylic_visatt -> SetForceLineSegmentsPerCircle(4);
    logicUpperChimney -> SetVisAttributes(ct_visatt);
    return logicUpperChimney;
}

bool
UpperChimney::inject(std::string motherName, IDetElement* other, IDetElementPos* /*pos*/) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lUpperChimney" ) {
        mothervol = logicUpperChimney;
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
    
    
    return true;
}

void
UpperChimney::initVariables() {
    SniperPtr<DetSimAlg> detsimalg(*getParent(), "DetSimAlg");
    if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
    }
    ToolBase* t = detsimalg->findTool("GlobalGeomInfo");
    assert(t);
    //IDetElement* globalinfo = dynamic_cast<IDetElement*>(t);

    m_heightWP = WPHeight;
    //TopToWater=m_UpperChimneyTopToWater*m;
}

void 
UpperChimney::initMaterials() {
    LS = G4Material::GetMaterial("LS");
    Water = G4Material::GetMaterial("Water");
    Acrylic = G4Material::GetMaterial("Acrylic");
    Tyvek = G4Material::GetMaterial("Tyvek");
    Steel= G4Material::GetMaterial("Steel");
    Air= G4Material::GetMaterial("Air");
}

void UpperChimney::makeUpperChimneyLogicalandPhysical(){
    UpperChimneyMaker UpperChimney(m_TopToWater);
    
    solidUpperChimney=UpperChimney.getSolidUpperChimney();
    solidUpperChimneyLS=UpperChimney.getSolidALS();
    solidUpperChimneySteel=UpperChimney.getSolidBSteel();
    solidUpperChimneyTyvek=UpperChimney.getSolidBTyvek();
    
    logicUpperChimney=new G4LogicalVolume(solidUpperChimney,
                                      Air,
                                      "lUpperChimney",
                                      0,
                                      0,
                                      0);

 
    logicUpperChimneyLS = new G4LogicalVolume(solidUpperChimneyLS,
                                      LS,
                                      "lUpperChimneyLS",
                                      0,
                                      0,
                                      0);
    

    logicUpperChimneySteel= new G4LogicalVolume(solidUpperChimneySteel,
                                                  Steel,
                                                  "lUpperChimneySteel",
                                                  0,
                                                  0,
                                                  0);
    
    
    logicUpperChimneyTyvek = new G4LogicalVolume(solidUpperChimneyTyvek,
                                                 Tyvek,
                                                 "lUpperChimneyTyvek",
                                                 0,
                                                 0,
                                                 0);
    

    physiUpperChimneyLS = new G4PVPlacement(0,              // no rotation
                                      //G4ThreeVector(0,0,GateCenter+m_heightWP/2), // at (x,y,z)
                                      G4ThreeVector(0,0,0), // at (x,y,z)
                                      logicUpperChimneyLS,    // its logical volume
                                      "pUpperChimneyLS",       // its name
                                      logicUpperChimney,  // its mother  volume
                                      false,           // no boolean operations
                                      0);              // no particular field
    
    
    physiUpperChimneySteel = new G4PVPlacement(0,              // no rotation
                                               G4ThreeVector(0,0,0 ), // at (x,y,z)
                                               logicUpperChimneySteel,    // its logical volume
                                               "pUpperChimneySteel",       // its name
                                               logicUpperChimney,  // its mother  volume
                                               false,           // no boolean operations
                                               0);              // no particular field
    
    physiUpperChimneyTyvek = new G4PVPlacement(0,              // no rotation
                                                G4ThreeVector(0,0,0 ), // at (x,y,z)
                                                 logicUpperChimneyTyvek,    // its logical volume
                                                 "pUpperChimneyTyvek",       // its name
                                                 logicUpperChimney,  // its mother  volume
                                                 false,           // no boolean operations
                                                 0);              // no particular field
   
}
void
UpperChimney::setupTopChimneyReflectorInCD()
{
//    if (m_chimney_top_name== "TopChimney"){
//        G4PhysicalVolumeStore* store = G4PhysicalVolumeStore::GetInstance();

//        G4VPhysicalVolume* chimney_acrylic = store->GetVolume("pUpperChimneySteel");
//        G4VPhysicalVolume* chimney_tyvek = store->GetVolume("pUpperChimneyTyvek");

        assert(physiUpperChimneySteel and physiUpperChimneyTyvek);

        G4OpticalSurface* tyvek_surface = new G4OpticalSurface("UpperChimneyTyvekOpticalSurface");
        G4LogicalBorderSurface* TyvekSurface = new G4LogicalBorderSurface(
                        "UpperChimneyTyvekSurface", physiUpperChimneyLS, physiUpperChimneyTyvek, tyvek_surface);
    assert(TyvekSurface);

        G4MaterialPropertiesTable* tyvek_mt = new G4MaterialPropertiesTable();
        tyvek_surface->SetModel(unified);
        tyvek_surface->SetType(dielectric_metal);
        tyvek_surface->SetFinish(ground);
        tyvek_surface->SetSigmaAlpha(0.2);
        double TyvekEnergy[4] = {1.55*eV, 6.20*eV, 10.33*eV, 15.5*eV};
       // double TyvekReflectivity[4] = {0.98, 0.98, 0.98, 0.98};
       // double TyvekReflectivity[4] = {0.10, 0.10, 0.10, 0.10};
       // double TyvekReflectivity[4] = {0.010, 0.010, 0.010, 0.010};
        double TyvekReflectivity[4] = {m_reflectivity, m_reflectivity,m_reflectivity, m_reflectivity};
        tyvek_mt->AddProperty("REFLECTIVITY", TyvekEnergy, TyvekReflectivity, 4);
        tyvek_surface->SetMaterialPropertiesTable(tyvek_mt);
//    }
}
