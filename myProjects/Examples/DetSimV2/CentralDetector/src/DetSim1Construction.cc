#include "DetSim1Construction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4IntersectionSolid.hh"
#include "G4VisAttributes.hh"
#include "RoundBottomFlaskSolidMaker.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"
#include "DetSimAlg/IPMTElement.h"
#include "Geometry/PMTParamSvc.h"
using namespace CLHEP;
DECLARE_TOOL(DetSim1Construction);

DetSim1Construction::DetSim1Construction(const std::string& name)
    : ToolBase(name),
    solidReflector(0), logicReflector(0),
    solidWaterPool(0), logicWaterPool(0), physiWaterPool(0),
    solidAcrylic(0), logicAcrylic(0), physiAcrylic(0)
{

    declProp("UseChimney", m_use_chimney = true);
    declProp("CheckOverlap", m_check_overlap = false);

    declProp("IsCommissioning", m_is_commissioning = false);
    declProp("ZbelowWater", m_z_below_water = 0.0); // at equator.
}

DetSim1Construction::~DetSim1Construction() {

}

G4LogicalVolume* 
DetSim1Construction::getLV() {
    if (logicReflector) {
        return logicReflector;
    }
    initVariables();
    initMaterials();

    if (m_use_chimney) {
        makeReflectorChimneyLogical();
        makeWPWithChimneyLogical();
        makeAcrylicWithChimneyLogical();
        makeLSWithChimneyLogical();
    } else {
        makeReflectorLogical();
        makeWPLogical();
        makeAcrylicLogical();
        makeLSLogical();
    }
    makeWPPhysical();
    makeAcrylicPhysical();
    makeLSPhysical();

    if (m_is_commissioning) {
        if (m_use_chimney) {
            makeCommissioningWaterWithChimneyLogical();
        } else {
            makeCommissioningWaterLogical();
        }
        makeCommissioningWaterPhysical();
    }

    return logicReflector;
}

bool
DetSim1Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lWaterPool" or motherName =="OuterWaterVeto" or motherName == "3inchInnerWater" or motherName == "20inchInnerWater") {
        mothervol = logicWaterPool;
    } else if ( motherName == "lTarget" ) {
        mothervol = logicTarget;
    }
    if (not mothervol) {
        // don't find the volume.
        return false;
    }
    LogDebug << "The mother volume is " << mothervol->GetName() << std::endl;

    // retrieve the daughter's LV
    G4LogicalVolume* daughtervol = 0;

    // if other is NULL, assume it's 20inch PMT mixing mode.
    IPMTElement* pmt_elem_r12860 = 0; // R12860 w/ or w/o mask
    IPMTElement* pmt_elem_nnvt = 0; // NNVT w/ or w/o mask
    if (!other) {
        // retrieve
        // * get detsimalg 
        // * get pmt elem
        SniperPtr<DetSimAlg> detsimalg(*getParent(), "DetSimAlg");
        if (detsimalg.invalid()) {
            LogError << "Can't Load DetSimAlg" << std::endl;
            return false;
        }
        pmt_elem_r12860 = dynamic_cast<IPMTElement*>(detsimalg->findTool("HamamatsuR12860")); // need to define the official name
        pmt_elem_nnvt = dynamic_cast<IPMTElement*>(detsimalg->findTool("NNVTMCPPMT"));

        LogInfo << "Mixing PMT mode: " << std::endl;
        LogInfo << "-> Get Hamamatsu R12860: " << pmt_elem_r12860 << std::endl;
        LogInfo << "-> Get NNVT MCPPMT: " << pmt_elem_nnvt << std::endl;
 
    } else {

        daughtervol = other->getLV();

        if (not daughtervol) {
            return false;
        }

        LogDebug << "The daughter volume ptr is " 
                 << daughtervol << std::endl;
        LogDebug << "The daughter volume name is " 
                 << daughtervol->GetName() << std::endl;
    }


    int copyno = 0;
    if (motherName == "OuterWaterVeto") {
         copyno = 30000;
    }else if (motherName == "3inchInnerWater") {
         copyno = 300000;
    }
    bool is_20inch_pmt = false;
    if (motherName == "20inchInnerWater") {
        is_20inch_pmt = true;
    }
    // FIXME: Skip the top PMTs
    // skip 7 PMTs when chimney is enabled
    //
    // * 2016.07.21
    //   We modify the input data directly, so could keep consistency.
    //
    // if (m_use_chimney and is_20inch_pmt and copyno<7) {
    //     while (pos->hasNext()) {
    //         if (m_use_chimney and is_20inch_pmt and copyno<7) {
    //             pos->next(),
    //             copyno++;
    //             continue;
    //         } else {
    //             break;
    //         }
    //     }
    //     // reset the copyno to 0
    //     copyno = 0;
    // }

    // * Supporting different PMTs in simulation.
    //   Instead of passing another PMT's detelem pointer, we just retrieve 
    //   it from DetSimAlg directly. The default LPMT is NNVT MCP-PMT. 
    //   So here, pointer of NNVT MCP-PMT is passed with detelem, 
    //   while pointer of Hamamatsu PMT is got from DetSimAlg.
    //    
    //   -- Tao Lin, 2017.05.27
    PMTParamSvc* pmt_param_svc = 0;
    if (is_20inch_pmt) {
      SniperPtr<PMTParamSvc> svc(*getParent(), "PMTParamSvc");
      if (svc.invalid()) {
        LogError << "Can't get PMTParamSvc. We can't initialize PMT." << std::endl;
        assert(0);
      } else {
        LogInfo << "Retrieve PMTParamSvc successfully." << std::endl;
        pmt_param_svc = svc.data();
      }
    }

    while (pos->hasNext()) {

        if (!daughtervol and is_20inch_pmt and pmt_param_svc) {
	  // For LPMTs, Hamamatsu and NNVT 
          G4LogicalVolume* daughter_pmt = 0;
          G4String daughter_pmt_name = "pLPMT"; // prefix
	  // check
	  if (pmt_param_svc->isHamamatsu(copyno)) {
            // select Hamamatsu
            LogTest << "PMTID " << copyno << " is Hamamatsu PMT. " << std::endl;  
            daughter_pmt = pmt_elem_r12860->getLV();
            daughter_pmt_name += "_Hamamatsu_R12860";
          } else if (pmt_param_svc->isNNVT(copyno)) {
            daughter_pmt = pmt_elem_nnvt->getLV();
            daughter_pmt_name += "_NNVT_MCPPMT";
          }
            new G4PVPlacement(
                pos->next(),
                daughter_pmt,
                daughter_pmt_name,
                mothervol,
                false,
                copyno++,
                m_check_overlap // check overlap
                );
        } else {
	    if (!daughtervol) {
	      LogError << "can't construct daughter volume." << std::endl;
            }
            new G4PVPlacement(
                pos->next(),
                daughtervol,
                daughtervol->GetName()+"_phys",
                mothervol,
                false,
                copyno++,
                m_check_overlap // check overlap
                );
        }
    }
    if(motherName == "lWaterPool" or motherName=="20inchInnerWater") {
        G4cout<<"PMT_Acrylic_Number = "<<copyno<<G4endl;
    } else if (motherName == "3inchInnerWater") {
        G4cout<<"3inch PMT Number: = "<<copyno-300000<<G4endl;
    } else {
        G4cout<<"Veto_PMT_Number = "<<copyno-30000<<G4endl;
    }


    return true;
}

void
DetSim1Construction::initVariables() {
    SniperPtr<DetSimAlg> detsimalg(*getParent(), "DetSimAlg");
    if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
    }
    ToolBase* t = detsimalg->findTool("GlobalGeomInfo");
    assert(t);
    IDetElement* globalinfo = dynamic_cast<IDetElement*>(t);

    m_radLS = 17.7*m;
    m_thicknessAcrylic = 12.*cm;
    m_radAcrylic = m_radLS + m_thicknessAcrylic;

    m_radWP = globalinfo->geom_info("WaterPool.R"); // This value will be changed in the future
    m_heightWP = globalinfo->geom_info("WaterPool.H");

//    m_ChimneyTopToCenter = m_heightWP/2;
//    m_radLSChimney = 0.25*m;
    m_radLSChimney = 0.4*m;
    m_radAcrylicChimney  = m_radLSChimney + m_thicknessAcrylic;
    m_ChimneyTopToCenter = m_radLS+m_thicknessAcrylic;
    //m_ChimneyBoxOuterHeight = 21*cm +5*mm*2 + 2*mm*2;
    // == inner water buffer ==
    // the water buffer should be different from the Outer water pool.
    // FIXME what's the radius of the inner water?
    //m_radInnerWater = 20.25*m;;
    m_radInnerWater = 20.05*m; // inner radius of latticed shell
    // BUG?: m_radInnerWaterChimney = m_radLSChimney + 1*cm;
    m_radInnerWaterChimney = m_radAcrylicChimney + 1*cm;

    // FIXME: wha't the thickness of reflector
    m_thicknessReflector = 2*mm;
    m_radReflector = m_radInnerWater + m_thicknessReflector;
    m_radReflectorChimney = m_radInnerWaterChimney + m_thicknessReflector;
}

void 
DetSim1Construction::initMaterials() {
    LS = G4Material::GetMaterial("LS");
    Water = G4Material::GetMaterial("Water");
    Acrylic = G4Material::GetMaterial("Acrylic");
    Tyvek = G4Material::GetMaterial("Tyvek");
}

void 
DetSim1Construction::makeReflectorLogical() {
    solidReflector = new G4Sphere("sReflectorInCD",
                                0*m,
                                m_radReflector,
                                0.*deg,
                                360*deg,
                                0.*deg,
                                180.*deg);

    logicReflector = new G4LogicalVolume(solidReflector,
                                         Tyvek,
                                         "lReflectorInCD",
                                         0,
                                         0,
                                         0);

}

void
DetSim1Construction::makeReflectorChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sReflectorInCD",
                                    m_radReflector,             // 
                                    m_radReflectorChimney,      // 
                                    m_heightWP/2,
                                    TOPTOEQUATORH);
    solidReflector = rbfs.getSolid();

    logicReflector = new G4LogicalVolume(solidReflector,
                                         Tyvek,
                                         "lReflectorInCD",
                                         0,
                                         0,
                                         0);
    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0.5, 0., 0.5));
    visatt -> SetForceWireframe(true);  
    visatt -> SetForceAuxEdgeVisible(true);
    //visatt -> SetForceSolid(true);
    //visatt -> SetForceLineSegmentsPerCircle(4);
    logicReflector -> SetVisAttributes(visatt);

}

void
DetSim1Construction::makeWPLogical() {
    solidWaterPool = new G4Sphere("sInnerWater",
                                0*m,
                                m_radInnerWater,
                                0.*deg,
                                360*deg,
                                0.*deg,
                                180.*deg);

    logicWaterPool = new G4LogicalVolume(solidWaterPool,
                                         Water,
                                         "lInnerWater",
                                         0,
                                         0,
                                         0);
}

void
DetSim1Construction::makeWPWithChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sInnerWater",
                                    m_radInnerWater,
                                    m_radInnerWaterChimney,
                                     m_heightWP/2,
                                    TOPTOEQUATORH);
    solidWaterPool = rbfs.getSolid();

    logicWaterPool = new G4LogicalVolume(solidWaterPool,
                                         Water,
                                         "lInnerWater",
                                         0,
                                         0,
                                         0);
    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0.5, 0.8, 0.5));
    visatt -> SetForceWireframe(true);  
    visatt -> SetForceAuxEdgeVisible(true);
    //visatt -> SetForceSolid(true);
    //visatt -> SetForceLineSegmentsPerCircle(4);
    logicWaterPool -> SetVisAttributes(visatt);
}

void
DetSim1Construction::makeWPPhysical() {
    physiWaterPool = new G4PVPlacement(0,
                                       G4ThreeVector(0,0,0),
                                       logicWaterPool,
                                       "pInnerWater",
                                       logicReflector,
                                       false,
                                       0);
}

void
DetSim1Construction::makeAcrylicLogical() {
    solidAcrylic = new G4Sphere("sAcrylic",
                                0*m,
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
DetSim1Construction::makeAcrylicPhysical() {
    physiAcrylic = new G4PVPlacement(0,              // no rotation
                                     G4ThreeVector(0,0,0), // at (x,y,z)
                                     logicAcrylic,    // its logical volume 
                                     "pAcrylic",       // its name
                                     logicWaterPool,  // its mother  volume
                                     false,           // no boolean operations
                                     0);              // no particular field
}

void
DetSim1Construction::makeLSLogical() {

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
DetSim1Construction::makeLSPhysical() {
    physiTarget = new G4PVPlacement(0,              // no rotation
                                    G4ThreeVector(0,0,0), // at (x,y,z)
                                    logicTarget,    // its logical volume 
                                    "pTarget",       // its name
                                    logicAcrylic,  // its mother  volume
                                    false,           // no boolean operations
                                    0);              // no particular field
}

void
DetSim1Construction::makeAcrylicWithChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sAcrylic",
                                    m_radAcrylic,
                                    m_radAcrylicChimney,
                                    m_ChimneyTopToCenter,
                                    TOPTOEQUATORH);
    solidAcrylic = rbfs.getSolid();
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
DetSim1Construction::makeLSWithChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sTarget",
                                    m_radLS,
                                    m_radLSChimney,
                                    m_ChimneyTopToCenter,
                                    TOPTOEQUATORH);
    solidTarget = rbfs.getSolid();
    logicTarget = new G4LogicalVolume(solidTarget, 
                                      LS, 
                                      "lTarget",
                                      0,
                                      0,
                                      0);

    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0, 1.0, 0));
    visatt -> SetForceWireframe(true);  
    visatt -> SetForceAuxEdgeVisible(true);
    //visatt -> SetForceSolid(true);
    //visatt -> SetForceLineSegmentsPerCircle(4);
    logicTarget -> SetVisAttributes(visatt);

}


void
DetSim1Construction::makeCommissioningWaterLogical() {

    G4VSolid* solid_full = new G4Sphere("sCommissioningWater_all", 
                               0*m, 
                               m_radLS, 
                               0.*deg, 
                               360.*deg, 
                               0.*deg, 
                               180.*deg);
    // intersection with a Polycon
    G4double zPlane[] = {
        -m_radLS,         // bottom
        m_z_below_water   // top
    };
    G4double rInner[] = {0.,
                         0.};
    G4double rOuter[] = {m_radLS,
                         m_radLS};


    G4VSolid* below_water_solid = new G4Polycone("sCommissioningWater_below",
                                                 0,
                                                 360*deg,
                                                 2,
                                                 zPlane,
                                                 rInner,
                                                 rOuter);
                                                 
    // so we get the below part
    solidCommissioningWater = new G4IntersectionSolid("sCommissioningWater",
                                                      solid_full,
                                                      below_water_solid,
                                                      NULL,
                                                      G4ThreeVector());

    logicCommissioningWater = new G4LogicalVolume(solidCommissioningWater, 
                                      Water, 
                                      "lCommissioningWater",
                                      0,
                                      0,
                                      0);
    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0.5, 0.8, 0.5));
    // visatt -> SetForceWireframe(true);  
    // visatt -> SetForceAuxEdgeVisible(true);
    visatt -> SetForceSolid(true);
    // visatt -> SetForceLineSegmentsPerCircle(4);
    logicCommissioningWater -> SetVisAttributes(visatt);

}

void
DetSim1Construction::makeCommissioningWaterWithChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sCommissioningWater_all",
                                    m_radLS,
                                    m_radLSChimney,
                                    m_ChimneyTopToCenter,
                                    TOPTOEQUATORH);
    G4VSolid* solid_full = rbfs.getSolid();

    // intersection with a Polycon
    G4double zPlane[] = {
        -m_radLS,         // bottom
        m_z_below_water   // top
    };
    G4double rInner[] = {0.,
                         0.};
    G4double rOuter[] = {m_radLS,
                         m_radLS};
    G4VSolid* below_water_solid = new G4Polycone("sCommissioningWater_below",
                                                 0,
                                                 360*deg,
                                                 2,
                                                 zPlane,
                                                 rInner,
                                                 rOuter);
                                                 
    // so we get the below part
    solidCommissioningWater = new G4IntersectionSolid("sCommissioningWater",
                                                      solid_full,
                                                      below_water_solid,
                                                      NULL,
                                                      G4ThreeVector());

    
    logicCommissioningWater = new G4LogicalVolume(solidCommissioningWater, 
                                      Water, 
                                      "lCommissioningWater",
                                      0,
                                      0,
                                      0);

    G4VisAttributes* visatt = new G4VisAttributes(G4Colour(0.5, 0.8, 0.5));
    // visatt -> SetForceWireframe(true);  
    // visatt -> SetForceAuxEdgeVisible(true);
    visatt -> SetForceSolid(true);
    // visatt -> SetForceLineSegmentsPerCircle(4);
    logicCommissioningWater -> SetVisAttributes(visatt);

}


void
DetSim1Construction::makeCommissioningWaterPhysical() {
    physiCommissioningWater = new G4PVPlacement(0,              // no rotation
                                    G4ThreeVector(0,0,0), // at (x,y,z)
                                    logicCommissioningWater,    // its logical volume 
                                    "pCommissioningWater",       // its name
                                    logicTarget,  // its mother  volume
                                    false,           // no boolean operations
                                    0);              // no particular field
}


