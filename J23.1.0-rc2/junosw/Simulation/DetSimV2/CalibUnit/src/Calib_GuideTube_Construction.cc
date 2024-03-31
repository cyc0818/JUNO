#include "Calib_GuideTube_Construction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <string>

using namespace CLHEP;
DECLARE_TOOL(Calib_GuideTube_Construction);

Calib_GuideTube_Construction::Calib_GuideTube_Construction(const std::string& name)
    : ToolBase(name) ,counter(-1)
	,solidSourceEnclosure_shell(0),logicSourceEnclosure_shell(0),physiSourceEnclosure_shell(0)
	,solidSourceEnclosure_innershell(0),solidSourceEnclosure_innershell_a(0),solidSourceEnclosure_innershell_b(0)
	,solidSourceEnclosure_screw(0),solidSourceEnclosure_inner_uni1(0),solidSourceEnclosure_inner_uni2(0)
	,solidSourceEnclosure_inner_uni3(0),solidSourceEnclosure_inner(0),logicSourceEnclosure_inner(0),physiSourceEnclosure_inner(0)
        ,solidSourceEnclosure_capsule(0),logicSourceEnclosure_capsule(0),physiSourceEnclosure_capsule(0)
	,solidSourceEnclosure_cavity(0),logicSourceEnclosure_cavity(0),physiSourceEnclosure_cavity(0)
	,Teflon(0),VacuumT(0),Water(0),PE_PA(0),StainlessSteel(0)
{
	declProp("UseSource",m_use_source=false);
	declProp("Theta",m_theta=90.);

        declProp("Option", m_guidetube_option="V0");
}

Calib_GuideTube_Construction::~Calib_GuideTube_Construction() {

}

G4LogicalVolume*
Calib_GuideTube_Construction::getLV() {

    if (counter == -1)
    {
	GT_Parameters();
        initMaterials();
        initVariables();
	makeSurftubeLogical();
	SetGuideTubeReflectivity();
        makevacSurftubeLogical();
        makevacSurftubePhysical();
        makeSourceEnclosureLogical();
        makeSourceEnclosurePhysical();
    }
    ++counter;

    LogInfo << " getLV at counter " << counter << std::endl;
    
    // if requested more than 39 logical volumes, return nullptr.
    // Tao Lin <lintao AT ihep.ac.cn>, 27 March 2022
    if (counter >= GT_number) {
        return nullptr;
    }

    G4VisAttributes* ct_visatt = new G4VisAttributes(G4Colour(0, 0.5, 1.0));
    ct_visatt -> SetForceWireframe(true);  
    ct_visatt -> SetForceAuxEdgeVisible(true);
    //acrylic_visatt -> SetForceSolid(true);
    //acrylic_visatt -> SetForceLineSegmentsPerCircle(4);
    logicSurftube[counter] -> SetVisAttributes(ct_visatt);
    return logicSurftube[counter];
}

bool
Calib_GuideTube_Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    return true;
}

void
Calib_GuideTube_Construction::GT_Parameters() {

//get parameter path
    std::string project_name = "junosw";
    if (getenv("JUNOSW_NAME")) {
        project_name = getenv("JUNOSW_NAME");
    }
    std::string f_path;
    std::string file_path = "/";
    file_path += project_name;
    file_path += "/Simulation/DetSimV2/DetSimOptions/data/";
    if (getenv("JUNOTOP"))
    {
        std::string base = getenv("JUNOTOP");
        base += file_path;
        if (boost::filesystem::exists(base))
        {
            f_path = base + "Guide_Tube.csv";
        }
    }
    if (getenv("WORKTOP"))
    {
        std::string base = getenv("WORKTOP");
        base += file_path;
        if (boost::filesystem::exists(base))
        {
            f_path = base + "Guide_Tube.csv";
        }
    }

//input parameters
    std::ifstream fin(f_path.c_str());
    std::string tmp_line;
    int GT_ID;       // col 1
    double Phi_Start;     // col 2
    double Phi_Delta;     // col 3
    double Radius_Plus;     // col 4

    while (fin.good())
    {
        std::getline(fin, tmp_line);
        if (fin.fail())
        {
            break;
        }

        std::stringstream ss;
        ss << tmp_line;

        ss >> GT_ID;
        ss >> Phi_Start;
        m_torus_swipe_phi_Start[GT_ID] = Phi_Start;
        ss >> Phi_Delta;
        m_torus_swipe_phi_Delta[GT_ID] = Phi_Delta;
        ss >> Radius_Plus;
        m_rad_guidetube_Plus[GT_ID] = Radius_Plus;
    }

}

void
Calib_GuideTube_Construction::initVariables() {
    if (m_guidetube_option=="V0") {
        m_torus_swipe_phi_start = 92.0*deg;
        m_torus_swipe_phi_delta = 356.0*deg;
    } else if (m_guidetube_option=="V1_0"
               or m_guidetube_option=="V1_1") {
        //torus_swipe_phi_start = 92.0*deg;
        //torus_swipe_phi_delta = 176.0*deg;  -> [92., 178.] old version
    } else { 
        G4cerr << "Unknown Guide Tube option: " << m_guidetube_option << G4endl;
    }
    G4double  radLS = 17.7*m;
    G4double radAcrylic = radLS + 124*mm;
    m_rad_guidetube = radAcrylic + 19.05/2.0*mm;  //when the guide tube is on the surface of CD
    //m_rad_guidetube_b = radAcrylic + 23*mm + 19.05/2.0*mm + 0.15*mm;  //when the guide tube is on the XJanchor
    //m_rad_guidetube_c = radAcrylic + 74.55*mm + 19.05/2.0*mm;  //when the guide tube is raised by the anchor (4 locations: theta = 17.8 or 162.2)
    //m_rad_guidetube_d = radAcrylic + 65.95*mm + 19.05/2.0*mm;  //when the guide tube is raised by the anchor (4 locations: theta = 24.8 or 155.2)
    //m_rad_guidetube_e = radAcrylic + 15.75*mm + 19.05/2.0*mm;  //when the guide tube is raised by the anchor (4 locations: theta = 32.4 or 147.6)
    //Different angles correspond to different radiuses
}

void 
Calib_GuideTube_Construction::initMaterials() {
    Teflon = G4Material::GetMaterial("Teflon");
    VacuumT = G4Material::GetMaterial("VacuumT");
    Air = G4Material::GetMaterial("Air");
    Steel = G4Material::GetMaterial("Steel");
    StainlessSteel = G4Material::GetMaterial("StainlessSteel");
    Water = G4Material::GetMaterial("Water");
    PE_PA = G4Material::GetMaterial("PE_PA");
}

void
Calib_GuideTube_Construction::makeSurftubeLogical() {
    for (int i = 0; i < GT_number; i++)
    {
        solidSurftube[i] = new G4Torus("sSurftube_"+std::to_string(i)+m_guidetube_option,
                                    0*mm,
                                    19.05/2.0*mm,
                                    m_rad_guidetube + m_rad_guidetube_Plus[i]*mm,
                                    m_torus_swipe_phi_Start[i]*deg,
                                    m_torus_swipe_phi_Delta[i]*deg
                                    );
    }

    for (int i = 0; i < GT_number; i++)
    {
	logicSurftube[i] = new G4LogicalVolume(solidSurftube[i],
                                        Teflon,   //   teflon
                                        "lSurftube_"+std::to_string(i)+m_guidetube_option,
                                        0,
                                        0,
                                        0);
    }
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
	for (int i = 0; i < GT_number; i++)
	{
	    new G4LogicalSkinSurface("Tube_surf_"+std::to_string(i)+m_guidetube_option,logicSurftube[i],TubeSurface);
	}
}


void
Calib_GuideTube_Construction::makevacSurftubeLogical() {
    for (int i = 0; i < GT_number; i++)
    {
        solidvacSurftube[i] = new G4Torus("svacSurftube_"+std::to_string(i)+m_guidetube_option,
                                    0*mm,
                                    15.88/2.0*mm,
                                    m_rad_guidetube + m_rad_guidetube_Plus[i]*mm,
                                    m_torus_swipe_phi_Start[i]*deg,
                                    m_torus_swipe_phi_Delta[i]*deg
                                    );
    }

    for (int i = 0; i < GT_number; i++)
    {
	logicvacSurftube[i] = new G4LogicalVolume(solidvacSurftube[i],
                                        Water,   //   Vacuum in old version
                                        "lvacSurftube_"+std::to_string(i)+m_guidetube_option,
                                        0,
                                        0,
                                        0);
    }
}
void
Calib_GuideTube_Construction::makevacSurftubePhysical() {
    for (int i = 0; i < GT_number; i++)
    {
	physivacSurftube[i] = new G4PVPlacement(0,
                                      G4ThreeVector(0,0,0),
                                      logicvacSurftube[i],    // its logical volume 
                                      "pvacSurftube_"+std::to_string(i)+m_guidetube_option,       // its name
                                      logicSurftube[i],  // its mother  volume
                                      false,           // no boolean operations
                                      0);              // no particular field
    }
}


// ----------This is the Guide Tube source geometry defination---------- //
void
Calib_GuideTube_Construction::makeSourceEnclosureLogical() {
            //outer shell
            double ZNodes4[4];
            double RminNodes4[4];
            double RmaxNodes4[4];
            ZNodes4[0] = 13.5*mm; RminNodes4[0] = 0*mm; RmaxNodes4[0] = 3.*mm;
            ZNodes4[1] = 10.2*mm; RminNodes4[1] = 0*mm; RmaxNodes4[1] = 6.5*mm;
            ZNodes4[2] = -10.2*mm; RminNodes4[2] = 0*mm; RmaxNodes4[2] = 6.5*mm;
            ZNodes4[3] = -13.5*mm; RminNodes4[3] = 0*mm; RmaxNodes4[3] = 3.*mm;
            solidSourceEnclosure_shell = new G4Polycone("sSourceEnclosure_shell",0.0*deg,360.0*deg,4,ZNodes4,RminNodes4,RmaxNodes4);

            logicSourceEnclosure_shell = new G4LogicalVolume(solidSourceEnclosure_shell,PE_PA,"lSourceEnclosure_shell",0,0,0);

            //inner shell
            solidSourceEnclosure_innershell_a = new G4Tubs("sSourceEnclosure_innershell_a",0*mm,5.6*mm,8.8*mm,0.*deg,360*deg);
            solidSourceEnclosure_innershell_b = new G4Tubs("sSourceEnclosure_innershell_b",0*mm,4.1*mm,5*mm,0.*deg,360*deg);
            
            solidSourceEnclosure_innershell = new G4SubtractionSolid("sSourceEnclosure_innershell",solidSourceEnclosure_innershell_a,solidSourceEnclosure_innershell_b,0,G4ThreeVector(0,0,-0.4));

            //screw
            solidSourceEnclosure_screw = new G4Tubs("sSourceEnclosure_screw",0*mm,1*mm,3.6/2.0*mm,0.*deg,360*deg);  
            
            //inner union
            solidSourceEnclosure_inner_uni1 = new G4UnionSolid("sSourceEnclosure_inner_uni1", solidSourceEnclosure_innershell, solidSourceEnclosure_screw, 0, G4ThreeVector(-3.*mm, 0.*mm, 10.6*mm));
            solidSourceEnclosure_inner_uni2 = new G4UnionSolid("sSourceEnclosure_inner_uni2", solidSourceEnclosure_inner_uni1, solidSourceEnclosure_screw, 0, G4ThreeVector(3.*mm, 0.*mm, 10.6*mm));
            solidSourceEnclosure_inner_uni3 = new G4UnionSolid("sSourceEnclosure_inner_uni3", solidSourceEnclosure_inner_uni2, solidSourceEnclosure_screw, 0, G4ThreeVector(-3.*mm, 0.*mm, -10.6*mm));
            solidSourceEnclosure_inner = new G4UnionSolid("sSourceEnclosure_inner", solidSourceEnclosure_inner_uni3, solidSourceEnclosure_screw, 0, G4ThreeVector(3.*mm, 0.*mm, -10.6*mm));

            logicSourceEnclosure_inner = new G4LogicalVolume(solidSourceEnclosure_inner,StainlessSteel,"lSourceEnclosure",0,0,0);

            //source capsule
            solidSourceEnclosure_capsule = new G4Tubs("sSourceEnclosure_capsule",0*mm,8.2/2.0*mm,8.5/2.0*mm,0.*deg,360*deg);
            logicSourceEnclosure_capsule = new G4LogicalVolume(solidSourceEnclosure_capsule,StainlessSteel,"lSourceEnclosure_capsule",0,0,0);

            //source cavity
            solidSourceEnclosure_cavity = new G4Tubs("sSourceEnclosure_cavity",0*mm,4.2/2.0*mm,4.5/2.0*mm,0.*deg,360*deg);
            logicSourceEnclosure_cavity = new G4LogicalVolume(solidSourceEnclosure_cavity,VacuumT,"lSourceEnclosure_cavity",0,0,0);
}

void
Calib_GuideTube_Construction::makeSourceEnclosurePhysical() {
        double abs_m_theta = abs(m_theta)*deg;
	int source_ID;

	for (int i = 0; i < GT_number; i++)
	{
		if (abs_m_theta > m_torus_swipe_phi_Start[i]*deg && abs_m_theta <= m_torus_swipe_phi_Start[i]*deg + m_torus_swipe_phi_Delta[i]*deg)
		{
			m_rad_source = m_rad_guidetube + m_rad_guidetube_Plus[i]*mm; //source enclosure could only be set from theta=2deg to 171.5deg
			source_ID = i;
		}
	}

	float m_x = m_rad_source * cos(abs_m_theta);
	float m_y = m_rad_source * sin(abs_m_theta);
	G4ThreeVector pos_enclosure(m_x, m_y, 0);
	G4RotationMatrix rot_enclosure;
	rot_enclosure.rotateX(90*deg);
	rot_enclosure.rotateZ(abs_m_theta);
	G4Transform3D trans_enclosure(rot_enclosure, pos_enclosure);
	if(m_use_source) {
		physiSourceEnclosure_inner = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicSourceEnclosure_inner, "pSourceEnclosure_inner", logicSourceEnclosure_shell, false,0);
		physiSourceEnclosure_capsule = new G4PVPlacement(0,G4ThreeVector(0,0,-1.15),logicSourceEnclosure_capsule, "pSourceEnclosure_capsule", logicSourceEnclosure_shell, false,0);
		physiSourceEnclosure_cavity = new G4PVPlacement(0,G4ThreeVector(0,0,-1.15),logicSourceEnclosure_cavity, "pSourceEnclosure_cavity", logicSourceEnclosure_capsule, false,0);
		physiSourceEnclosure_shell = new G4PVPlacement(trans_enclosure,logicSourceEnclosure_shell, "pSourceEnclosure_shell", logicvacSurftube[source_ID], false,0);
	}
}

