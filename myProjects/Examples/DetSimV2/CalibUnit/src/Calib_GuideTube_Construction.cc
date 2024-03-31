#include "Calib_GuideTube_Construction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
using namespace CLHEP;
DECLARE_TOOL(Calib_GuideTube_Construction);

Calib_GuideTube_Construction::Calib_GuideTube_Construction(const std::string& name)
    : ToolBase(name)
	,solidSurftube(0),logicSurftube(0)
	,solidvacSurftube(0),logicvacSurftube(0),physivacSurftube(0)
	,Teflon(0),Vacuum(0)
{
	declProp("UseSource",m_use_source=false);
	declProp("Theta",m_theta=0.);

        declProp("Option", m_guidetube_option="V0");

}

Calib_GuideTube_Construction::~Calib_GuideTube_Construction() {

}

G4LogicalVolume*
Calib_GuideTube_Construction::getLV() {
    if (logicSurftube) {
        return logicSurftube;
    }

    initMaterials();
    initVariables();

	makeSurftubeLogical();
	SetGuideTubeReflectivity();
	makevacSurftubeLogical();
	makevacSurftubePhysical();
	makeSourceEnclosureLogical();
	makeSourceEnclosurePhysical();

    G4VisAttributes* ct_visatt = new G4VisAttributes(G4Colour(0, 0.5, 1.0));
    ct_visatt -> SetForceWireframe(true);  
    ct_visatt -> SetForceAuxEdgeVisible(true);
    //acrylic_visatt -> SetForceSolid(true);
    //acrylic_visatt -> SetForceLineSegmentsPerCircle(4);
    logicSurftube -> SetVisAttributes(ct_visatt);
    return logicSurftube;
}

bool
Calib_GuideTube_Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    return true;
}

void
Calib_GuideTube_Construction::initVariables() {
    if (m_guidetube_option=="V0") {
        torus_swipe_phi_start = 92.0*deg;
        torus_swipe_phi_delta = 356.0*deg;
    } else if (m_guidetube_option=="V1_0"
               or m_guidetube_option=="V1_1") {
        torus_swipe_phi_start = 92.0*deg;
        torus_swipe_phi_delta = 176.0*deg; // -> [92., 178.]
    } else { 
        G4cerr << "Unknown Guide Tube option: " << m_guidetube_option << G4endl;
    }
}

void 
Calib_GuideTube_Construction::initMaterials() {
    Teflon = G4Material::GetMaterial("Teflon");
    Vacuum = G4Material::GetMaterial("Vacuum");
    Steel = G4Material::GetMaterial("Steel");
}

void
Calib_GuideTube_Construction::makeSurftubeLogical() {
    solidSurftube = new G4Torus("sSurftube"+m_guidetube_option,
                                    0*mm,
                                    10*mm,
                                    17.836*m,       //when it is a inner_tube, the swept redius is 17.692*m. when it is a outer_tube, the swept redius is 17.836*m.  17.7303364 on 6N
                                    torus_swipe_phi_start,
                                    torus_swipe_phi_delta
                                    );
    logicSurftube = new G4LogicalVolume(solidSurftube,
                                        Teflon,   //   teflon
                                        "lSurftube"+m_guidetube_option,
                                        0,
                                        0,
                                        0);
}

void
Calib_GuideTube_Construction::SetGuideTubeReflectivity() {

	G4OpticalSurface* TubeSurface = new G4OpticalSurface("TubeSurface"+m_guidetube_option, unified, polished, dielectric_metal);
	G4MaterialPropertiesTable* Tube_mt = new G4MaterialPropertiesTable();
	double Tube_Energy[2] = {1.55*eV,15.5*eV};
	double m_reflect = 0.9;
	double Tube_Reflectivity[2] = {m_reflect,m_reflect};       
	Tube_mt->AddProperty("REFLECTIVITY",Tube_Energy,Tube_Reflectivity,2);
	TubeSurface->SetMaterialPropertiesTable(Tube_mt);
	new G4LogicalSkinSurface("Tube_surf"+m_guidetube_option,logicSurftube,TubeSurface); 

}


void
Calib_GuideTube_Construction::makevacSurftubeLogical() {
    solidvacSurftube = new G4Torus("svacSurftube"+m_guidetube_option,
                                    0*mm,
                                    8*mm,
                                    17.836*m,    //different data in inner and outer tube.
                                    torus_swipe_phi_start,
                                    torus_swipe_phi_delta
                                    );
    logicvacSurftube = new G4LogicalVolume(solidvacSurftube,
                                        Vacuum,   //   vacuum
                                        "lvacSurftube"+m_guidetube_option,
                                        0,
                                        0,
                                        0);
}
void
Calib_GuideTube_Construction::makevacSurftubePhysical() {
    physivacSurftube = new G4PVPlacement(0,
									  G4ThreeVector(0,0,0),
                                      logicvacSurftube,    // its logical volume 
                                      "pvacSurftube"+m_guidetube_option,       // its name
                                      logicSurftube,  // its mother  volume
                                      false,           // no boolean operations
                                      0);              // no particular field
}


// -------This is the Guide Tube source geometry defination-----------------
void
Calib_GuideTube_Construction::makeSourceEnclosureLogical() {

	    solidSourceEnclosurea = new G4Tubs("sSourceEnclosurea"+m_guidetube_option,0*mm,5*mm,5*mm,0.*deg,360*deg);
	    solidSourceEnclosureb = new G4Tubs("sSourceEnclosureb"+m_guidetube_option,0*mm,3*mm,3*mm,0.*deg,360*deg);
	    solidSourceEnclosure = new G4SubtractionSolid("solidSourceEnclosure"+m_guidetube_option,solidSourceEnclosurea,solidSourceEnclosureb,0,G4ThreeVector(0,0,0));
	    logicSourceEnclosure = new G4LogicalVolume(solidSourceEnclosure,Steel,"lSourceEnclosure"+m_guidetube_option,0,0,0);
}

void
Calib_GuideTube_Construction::makeSourceEnclosurePhysical() {

		float m_x = 17.836*sin(m_theta/180.0*3.141592654)*m;
		float m_y = 17.836*cos(m_theta/180.0*3.141592654)*m;
	    G4ThreeVector pos_enclosure(m_x, m_y, 0);
	    G4RotationMatrix rot_enclosure;
        rot_enclosure.rotateY(90*deg);
        rot_enclosure.rotateZ(-m_theta*deg);
	    G4Transform3D trans_enclosure(rot_enclosure, pos_enclosure);
		if(m_use_source)
	    physiSourceEnclosure = new G4PVPlacement(trans_enclosure,logicSourceEnclosure, "pSourceEnclosure"+m_guidetube_option, logicvacSurftube, false,0);

}

