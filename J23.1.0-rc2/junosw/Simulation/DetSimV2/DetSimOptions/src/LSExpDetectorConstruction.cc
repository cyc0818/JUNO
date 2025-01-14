//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include <boost/python.hpp>
#include "G4SystemOfUnits.hh"
#include "LSExpDetectorConstruction.hh"
#include "G4PhysicalConstants.hh"
#include "G4UImanager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

#include <sstream>
#include "Randomize.hh"

#include <cmath>
#include <cassert>

#include "OpticalProperty.icc"
#include "DetSimAlg/IDetElement.h"

// For CD
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Task.h"
#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"
// For PMT
#include "DetSimAlg/ISensitiveDetectorElement.h"
#include "R3600PMTManager.hh"
#include "CalPositionBall.hh"
#include "HexagonPosBall.hh"
#include "CalPositionCylinder.hh"
#include "VetoPmtPosBall.hh"
#include "PMTinPrototypePos.hh"
#include "IPMTManager.hh"
#include "ExplosionProofManager.hh"

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <MCParamsSvc/IMCParamsSvc.hh>
#include "Geometry/IPMTParamSvc.h"
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"

#include "LSExpDetectorConstruction_Opticks.hh"

#ifdef WITH_G4CXOPTICKS
#include "NPFold.h"
#include "NPX.h"

#include "PMTSimParamSvc/PMTSimParamData.h"
#include "PMTSimParamSvc/_PMTSimParamData.h"


#endif




using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
LSExpDetectorConstruction::LSExpDetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0),
 Galactic(0),
 Air(0), GdLS(0), LS(0), ESR(0), Tyvek(0), Acrylic(0),DummyAcrylic(0),Teflon(0),
 Photocathode_mat(0),Steel(0), Vacuum(0), VacuumT(0),
 Pyrex(0), Oil(0), Water(0), vetoWater(0), Rock(0), Copper(0),
 Photocathode_opsurf(0),Photocathode_opsurf_3inch(0),
 de_top_rock(0), de_exp_hall(0), de_bottom_rock(0), de_cd_det(0), de_wp_det(0),
 Aluminium(0), TiO2(0), Scintillator(0), Adhesive(0), TiO2Coating(0)
 , hits_merge_flag(false)
 , hits_merge_window(1000*ns)
 , m_sd_type("original_SD")
 , m_sd(0)
 , pmt_det(0)
 , pmt_det_3inch(0)
 , pmt_det_veto(0)
 , m_scope(0)
 , m_opticksMode(0)
// , m_flatQE(false)
 , m_isMTmode(false)
 , m_isCDInnerReflectorEnabled(false)
{
  Photocathode_opsurf = 0;
  Photocathode_opsurf_3inch = 0;
  m_pmt_qe_scale_for_elec = 1.0;

  m_rock_and_exphall = "RockAndExpHall";

  ball_r = 19.434*m; //19.5*m;
  ball_vetoPmt_r=20.55*m;
  m_cd_name = "DetSim1";
  m_cd_enabled = true;
  m_wp_enabled = true;
  m_tt_enabled = true;
  m_tt_name = "TT_OPERA";
  // m_pmt_name = "OnlyPMT";
  m_pmt_name = "PMTMask";
  m_extra_lpmt = "ONE";
  m_3inchpmt_name = "Tub3inchV3";
  m_3inchpmt_pos_offset = -50.0*mm;
  m_veto_pmt_name = "R12860OnlyFrontMask";
  m_strut_name = "StrutAcrylic";
  m_strut2_name="StrutBar2Acrylic";
  m_strutballhead_name="StrutBallheadAcrylic";
  m_fastener_name = "FastenerAcrylic";
  m_upper_name = "UpperAcrylic";
  m_addition_name = "AdditionAcrylic";
  m_xjanchor_name = "XJanchor";
  m_xjfixture_name = "XJfixture";
  m_sjclsanchor_name = "SJCLSanchor";
  m_sjfixture_name = "SJFixture";
  m_sjreceiver_name = "SJReceiver";
  m_sjreceiver_fastener_name = "SJReceiverFastern";
  m_pmt_mother = "lWaterPool";
  m_mask_mother = "lMaskVirtual";
  m_strut_mother = "lWaterPool";
  m_fastener_mother = "lWaterPool";
  m_upper_mother = "lWaterPool";
  m_addition_mother = "lWaterPool";
  m_xjanchor_mother = "lWaterPool";
  m_xjfixture_mother = "lWaterPool";
  m_sjclsanchor_mother = "lTarget";
  m_sjfixture_mother = "lTarget";
  m_sjreceiver_mother = "lTarget";
  m_sjreceiver_fastener_mother = "lTarget";


//  m_GdLSAbsLengthMode = "old";
  m_pmt_optical_model = "old";
  m_LS_optical_model = "old";
  m_use_pmtsimsvc = true;

//  m_cd_name = "Prototype";
//  m_pmt_mother = "lBuffer";
//  m_pmt_pos_mode = "FileMode";

  m_pmt_pos_dir = "./";
  m_strut_pos_dir = "./";
  m_fastener_pos_dir = "./";
  m_xjanchor_pos_dir = "./";
  m_sjclsanchor_pos_dir = "./";
  m_sjfixture_pos_dir = "./";
  m_sjreceiver_pos_dir = "./";
  m_sjreceiver_fastener_pos_dir = "./";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
LSExpDetectorConstruction::~LSExpDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void LSExpDetectorConstruction::DefineMaterials()
{

  IDetElement* material_builder =  det_elem("GDMLMaterialBuilder");
  if (material_builder) {
      material_builder->getLV();
  }
#include "LSExpDetectorConstructionMaterial.icc"
#include "OpticalSurfaceProperty.icc"

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* LSExpDetectorConstruction::Construct()
{
  G4cout << "Begin the Detector Construction" << G4endl;
  DefineMaterials();
  DefineVariables();

  //------------------------------ 
  // World
  //------------------------------ 

  solidWorld= new G4Box("sWorld", 60*m, 60*m, 60*m);
  logicWorld= new G4LogicalVolume( solidWorld, Galactic, "lWorld", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // = World =
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
                                 "pWorld",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // no field specific to volume
  // == Rock And Exp. Hall ==
  // For the prototype detector, we don't use this Rock and Exp. Hall
  if (m_cd_name != "Prototype" and m_cd_name != "PrototypeOnePMT"
        and  m_rock_and_exphall == "RockAndExpHall") {
      G4cout<<"Begin Rock and Exp Hall Construction. " << G4endl;
      // === build the geom info ===
      IDetElement* glob_info = det_elem("GlobalGeomInfo");
      assert(glob_info);
      setupRockAndExpHall();
      G4cout<<"End Rock and Exp Hall Construction. " << G4endl;
  }
 
  // == Veto (Outer) Water Pool ==
  if (m_cd_name == "DetSim0" 
   or m_cd_name == "DetSim1"
   or m_cd_name == "DetSim2") {
      if (m_wp_enabled) {
          setupOuterWaterPool();
      }
  }

  // == Central Detector ==
  if (m_cd_name == "Prototype") {
      setupPrototypeDetector();
  } else if (m_cd_name == "PrototypeOnePMT") {
      setupPrototypeDetectorOnePMT();
  } else if (m_cd_name == "DetSim0" 
          or m_cd_name == "DetSim1"
          or m_cd_name == "DetSim2") {
      if (m_cd_enabled) {
          setupCentralDetector();
      }
      if (m_cd_enabled and m_wp_enabled) {
          setupReflectorInCD();
      }
  }

  // == PMTs in CD and WP ==
  // === SD manager ===
  if (m_cd_name == "DetSim0" 
   or m_cd_name == "DetSim1"
   or m_cd_name == "DetSim2") {
      // Prepare the SD
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      G4VSensitiveDetector* not_a_leak = getPMTSD();
      m_sd = not_a_leak ; 

      SDman->AddNewDetector(not_a_leak);
      // Prepare the PMT
      setupPMT(not_a_leak); 
      if (m_cd_enabled) {
          setupCD_PMTs();
      }
      if (m_wp_enabled) {
          setupWP_PMTs();
      }
  }

  // construct Top Chimney 
  // inject the Top Chimney into CD
  
  if (m_cd_name != "Prototype" and m_cd_name != "PrototypeOnePMT"
      and m_chimney_top_name.size()) {
      if (m_tt_enabled) {
          setupTopChimney();
          // setupTopChimneyReflectorInCD();
      }
  }
   
  // construct Lower Chimney 
  // inject the Lower Chimney into CD
  if (m_cd_name != "Prototype" and m_cd_name != "PrototypeOnePMT"
      and m_chimney_lower_name.size()) {
      if (m_cd_enabled) {
          setupLowerChimney();
          // setupLowerChimneyReflectorInCD();
      }
  }
  // == Top Tracker ==
  
  G4cout << "---- TT Name: " << m_tt_name << G4endl;
  if (m_tt_name == "TT_OPERA") {
      if (m_tt_enabled) {
          setupTopTracker();
      }
  }

    if (m_cd_name != "Prototype" and m_cd_name != "PrototypeOnePMT") {
        //
        // construct Calibraition Box
        // inject the Logical Volume into CD
        G4cout << "START TO construct Calibration Units. " << G4endl;
        if (m_calibunit_name.size()) {
        G4cout << " * " << m_calibunit_name << G4endl;
        if (m_calibunit_name=="Calib_GuideTube_V1") {
            // shortcut, setup two tori at the same time
            m_calibunit_name = "Calib_GuideTube_V1_0";
            G4cout << " * " << m_calibunit_name << G4endl;
            setupCalibUnit();
            m_calibunit_name = "Calib_GuideTube_V1_1";
            G4cout << " * " << m_calibunit_name << G4endl;
            setupCalibUnit();
            
        } else {
            setupCalibUnit();
            }
        }

        // Place extra calibunit
      std::string old_calibunit = m_calibunit_name;
      for (std::vector<std::string>::iterator it = m_calibunit_extras.begin();
              it != m_calibunit_extras.end(); ++it) {
          // FIXME: We just set m_calibunit_name to (*it), then invoke setupCalibUnit
          // But the problem here is, de_calibunit is changed.
          m_calibunit_name = (*it);
          G4cout << " * " << m_calibunit_name << G4endl;
          setupCalibUnit();
      }
    
      m_calibunit_name = old_calibunit;
  }

  
  //--------- Visualization attributes -------------------------------
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  //logicOD->SetVisAttributes(G4VisAttributes::Invisible);
  //G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  //logicTarget ->SetVisAttributes(TargetVisAtt);
  //G4VisAttributes* DetectorVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  //logicAcrylic->SetVisAttributes(DetectorVisAtt);
  //G4VisAttributes* PoolVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //_logiTestPMT->SetVisAttributes(PoolVisAtt);


  //------------ set the incident position ------

 // get the pointer to the User Interface manager 


  setupOpticks(physiWorld);  

  return physiWorld;
}


void LSExpDetectorConstruction::setupOpticks( G4VPhysicalVolume* world )
{
#ifdef WITH_G4CXOPTICKS

    std::cout 
        << "LSExpDetectorConstruction::setupOpticks"
        << " completed construction of physiWorld " 
        << " m_opticksMode " << m_opticksMode 
        << " WITH_G4CXOPTICKS "
        << " proceeding to setup Opticks "
        << G4endl
        ; 

    SniperPtr<IPMTParamSvc> _pps(*m_scope, "PMTParamSvc");
    if(_pps.invalid()) exit(EXIT_FAILURE);
    IPMTParamSvc* pps = _pps.data();
    assert(pps); 
    PMTParamData* pps_d = pps->getPMTParamData() ; 
    assert(pps_d); 

    SniperPtr<IPMTSimParamSvc> _psps(*m_scope, "PMTSimParamSvc");
    if(_psps.invalid()) exit(EXIT_FAILURE);
    IPMTSimParamSvc* psps = _psps.data(); 
    assert(psps);
    PMTSimParamData* psps_d = psps->getPMTSimParamData() ; 
    assert(psps_d); 

    const char* ekey = "LSExpDetectorConstruction__setupOpticks_pmtscan" ; 
    bool do_pmtscan = getenv(ekey) != nullptr ; 

    NPFold* pmtscan = nullptr ; 
    
    if(do_pmtscan)
    {
        bool dump = false ; 
        unsigned num_energy = 100 ; 
        std::cout 
            << "LSExpDetectorConstruction::setupOpticks"
            << " ekey " << ekey 
            << " num_energy " << num_energy
            << " [pmtscan " 
            << std::endl 
            ;
 
        NP* scan_IPMTSimParamSvc = _PMTSimParamData::Scan_pmtid_qe<IPMTSimParamSvc>( psps, num_energy, dump ) ;  
        NP* scan_PMTSimParamData = _PMTSimParamData::Scan_pmtid_qe<PMTSimParamData>( psps_d, num_energy, dump ) ; 

        double dt_IPMTSimParamSvc = scan_IPMTSimParamSvc->get_meta<double>("scantime") ;
        double dt_PMTSimParamData = scan_PMTSimParamData->get_meta<double>("scantime") ;

        NPX::KV<double> kv ;
        kv.add("IPMTSimParamSvc", dt_IPMTSimParamSvc ); 
        kv.add("PMTSimParamData", dt_PMTSimParamData ); 
        kv.add("IPMTSimParamSvc/PMTSimParamData", dt_IPMTSimParamSvc/dt_PMTSimParamData ); 

        pmtscan = new NPFold ; 
        pmtscan->add("IPMTSimParamSvc", scan_IPMTSimParamSvc ); 
        pmtscan->add("PMTSimParamData", scan_PMTSimParamData ); 
        pmtscan->add("scantime", kv.values() ); 

        std::cout << "LSExpDetectorConstruction::setupOpticks ]pmtscan " << std::endl ; 
    }
    else
    {
        std::cout 
            << "LSExpDetectorConstruction::setupOpticks"
            << " ekey " << ekey 
            << " no pmtscan " 
            << std::endl 
            ;
    }


    LSExpDetectorConstruction_Opticks::Setup( m_opticksMode, world, m_sd, pps_d, psps_d, pmtscan );



#else
    G4cout 
      << __FILE__ << ":" << __LINE__ << " completed construction of physiWorld " 
      << " m_opticksMode " << m_opticksMode 
      << " NOT WITH_G4CXOPTICKS "
      << " skip setup Opticks "
      << G4endl
      ; 
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void
LSExpDetectorConstruction::ConstructSDandField() {
    // for compatibility between single thread and multi threading, only invoke following code in MT mode.
    if (m_isMTmode) {
        // Note: getPMTSD will create a new instance for each time.
        // To avoid the crash, the tool PMTSDMgr should be created at the master thread.
        // G4SDManager is a thread local object.
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4VSensitiveDetector* not_a_leak = getPMTSD();
        SDman->AddNewDetector(not_a_leak);

        // now, loop all the logical volumes with SD and register the new one.
        // In this code, we assume there is only one SD 

        // See: void G4VUserDetectorConstruction::CloneSD()
        //Loop on ALL logial volumes to search for attached SD
        G4LogicalVolumeStore* const logVolStore = G4LogicalVolumeStore::GetInstance();
        assert( logVolStore != NULL );

        for ( G4LogicalVolumeStore::const_iterator it = logVolStore->begin() ; it != logVolStore->end() ; ++it ) {
            G4LogicalVolume *g4LogicalVolume = *it;
            //Use shadow of master to get the instance of SD
            G4VSensitiveDetector* masterSD = g4LogicalVolume->GetMasterSensitiveDetector();
            G4VSensitiveDetector* clonedSD = 0;
            if ( masterSD ) {
                clonedSD = not_a_leak;
                g4LogicalVolume->SetSensitiveDetector(clonedSD);
                G4cout << "Register SD to volume: " << g4LogicalVolume->GetName() << G4endl;
            }

        }

    }
} 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 


void
LSExpDetectorConstruction::DefineOpticalPropertyCoefficient()
{
} 

void
LSExpDetectorConstruction::ModifyOpticalProperty()
{
}


void
LSExpDetectorConstruction::DefineVariables()
{
}

void
LSExpDetectorConstruction::setupCentralDetector()
{
  // construct the Central Detector
  IDetElement* cd_det = 0;
  if (m_cd_name == "DetSim0" or m_cd_name == "DetSim1" or m_cd_name == "DetSim2") {
      std::string new_name = m_cd_name + "Construction";
      cd_det = det_elem(new_name);
  } 

  assert(cd_det);

  de_cd_det = cd_det;

  if (m_cd_name == "DetSim1"){
    m_strut_name = "StrutAcrylic";
    m_strut2_name="StrutBar2Acrylic";
    m_strutballhead_name="StrutBallheadAcrylic";
    m_fastener_name = "FastenerAcrylic";
    m_pmt_mother = "20inchInnerWater";
    m_strut_mother = "lWaterPool";
    m_fastener_mother = "lWaterPool";
  }else if (m_cd_name == "DetSim2"){
    m_strut_name = "StrutBalloon";
    // FIXME no fastener in Balloon Option
    m_fastener_name = "FastenerBalloon";
    m_pmt_mother = "lLAB";
    m_strut_mother = "lLAB";
    m_fastener_mother = "lLAB";
  }
  G4cout<< "&&&& Strut = "<<m_strut_name
      <<"  PMT_Mother = "<<m_pmt_mother
      <<"  Strut_Mother = "<<m_strut_mother<<G4endl;
  
  // place the CD into bottom rock or into the world
  if (de_wp_det == 0) {
      assert(de_wp_det);
  } else {
      de_wp_det->inject("lWaterPool", de_cd_det, 0);
  }


  // === construct Sticks ===
  bool debug_disable_sticks = getenv("JUNO_DEBUG_DISABLE_STICKS") == nullptr ? false : true ; 
  if( debug_disable_sticks )
  {
      G4cout << "LSExpDetectorConstruction::setupCentralDetector setupCD_Sticks DISABLED BY OPTION --debug-disable-sticks " << G4endl ; 
  }
  else
  {
      G4cout << "[ LSExpDetectorConstruction::setupCentralDetector proceed with setupCD_Sticks " << G4endl ; 
      setupCD_Sticks(cd_det);
      G4cout << "] LSExpDetectorConstruction::setupCentralDetector completed setupCD_Sticks " << G4endl ; 
  }

}

void
LSExpDetectorConstruction::setupReflectorInCD()
{
    if (m_cd_name == "DetSim1") {
        G4PhysicalVolumeStore* store = G4PhysicalVolumeStore::GetInstance();
        G4VPhysicalVolume* outer_water = store->GetVolume("pOuterWaterPool");
        G4VPhysicalVolume* tyvek = store->GetVolume("pCentralDetector");
        G4VPhysicalVolume* inner_water = store->GetVolume("pInnerWater");
    
        assert(outer_water and tyvek);
    
        G4OpticalSurface* tyvek_surface = new G4OpticalSurface("CDTyvekOpticalSurface");
        G4LogicalBorderSurface* TyvekSurface = new G4LogicalBorderSurface(
                        "CDTyvekSurface", outer_water, tyvek, tyvek_surface);
        assert(TyvekSurface);

        G4MaterialPropertiesTable* tyvek_mt = new G4MaterialPropertiesTable();
        tyvek_surface->SetModel(unified);
        tyvek_surface->SetType(dielectric_metal);
        tyvek_surface->SetFinish(ground);
        tyvek_surface->SetSigmaAlpha(0.2);
      
      
        SniperPtr<IMCParamsSvc> mcgt(*m_scope, "MCParamsSvc");
        if (mcgt.invalid()) {
          G4cout << "Can't find MCParamsSvc." << G4endl;
          assert(0);
        }
        
       helper_mpt(tyvek_mt, "REFLECTIVITY",mcgt.data(), "Material.Tyvek.REFLECTIVITY");
       tyvek_surface->SetMaterialPropertiesTable(tyvek_mt);

       // The optical surface from inner water to tyvek.
       if (m_isCDInnerReflectorEnabled) {
           LogInfo << "m_isCDInnerReflectorEnabled is " << m_isCDInnerReflectorEnabled
                   << ", so the reflector from inner water to tyvek will be setup. "
                   << std::endl;
           G4OpticalSurface* inner_tyvek_surface = new G4OpticalSurface("CDInnerTyvekOpticalSurface");
           inner_tyvek_surface->SetMaterialPropertiesTable(G4Material::GetMaterial("CDReflectorSteel")->GetMaterialPropertiesTable());
           inner_tyvek_surface->SetModel(unified);
           inner_tyvek_surface->SetType(dielectric_metal);
           inner_tyvek_surface->SetFinish(ground);
           inner_tyvek_surface->SetSigmaAlpha(0.2);
           
           G4LogicalBorderSurface* innerTyvekSurface = new G4LogicalBorderSurface(
                                                                                  "CDInnerTyvekSurface",
                                                                                  inner_water,
                                                                                  tyvek,
                                                                                  inner_tyvek_surface);
                                                                             
       }


      /////////////////////surface of outer water veto and tyvek around water pool wall (current tyvek was put at lining place)////////////////
        G4VPhysicalVolume* tyvekOuter = store->GetVolume("pPoolLining");
        assert(tyvekOuter and outer_water);
        G4OpticalSurface* tyvekouter_surface = new G4OpticalSurface("VETOTyvekOpticalSurface");
        G4LogicalBorderSurface* TyvekouterSurface = new G4LogicalBorderSurface(
                        "VETOTyvekSurface",outer_water, tyvekOuter,tyvekouter_surface);
        assert(TyvekouterSurface);

        G4MaterialPropertiesTable* tyvekouter_mt = new G4MaterialPropertiesTable();
        tyvekouter_surface->SetModel(unified);
        tyvekouter_surface->SetType(dielectric_metal);
        tyvekouter_surface->SetFinish(ground);
        tyvekouter_surface->SetSigmaAlpha(0.2);
        helper_mpt(tyvekouter_mt, "REFLECTIVITY",mcgt.data(), "Material.Tyvek.REFLECTIVITY");
        tyvekouter_surface->SetMaterialPropertiesTable(tyvekouter_mt);

    }
}

void
LSExpDetectorConstruction::setupPrototypeDetector()
{
  SniperPtr<DetSimAlg> detsimalg(*m_scope, "DetSimAlg");
  if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
  }
  IDetElement* cd_det = 0;
  if (m_cd_name == "Prototype") {
      std::string new_name = m_cd_name + "Construction";
      ToolBase* t = 0;
      // find the tool first
      // create the tool if not exist
      t = detsimalg->findTool(new_name);
      if (not t) {
          t = detsimalg->createTool(new_name);
      }
      if (t) {
          cd_det = dynamic_cast<IDetElement*>(t);
      }
  }

  assert (cd_det);

  new G4PVPlacement(0, // no rotation
                    G4ThreeVector(0,0,0), // at (x,y,z)
                    cd_det -> getLV(),    // its logical volume
                    "pPrototypeDetector",   // its name
                    logicWorld,      // its mother  volume
                    false,           // no boolean operation|     s
                    0);              // no particular field

  // construct PMT
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4VSensitiveDetector* not_a_leak = getPMTSD();
  SDman->AddNewDetector(not_a_leak);

  // inject the Logical Volume into CD
  // 20 inch PMT
  IPMTElement* pmt_det_MCP20inch = 0;
  //inject MCP20inch
  IDetElement* t_MCP20inch = det_elem("MCP20inchPMTManager");
  if (t_MCP20inch) {
     pmt_det_MCP20inch = dynamic_cast<IPMTElement*>(t_MCP20inch);
  }
  assert(pmt_det_MCP20inch);

  IDetElementPos* pmt_detelem_pos_MCP20inch = 0;
  double ball_r_MCP20inch = 576.11*mm + 184*mm;
  pmt_detelem_pos_MCP20inch = new JUNO::Prototype::MCPPMT20inchPos(ball_r_MCP20inch);
  assert (pmt_detelem_pos_MCP20inch);
  bool status = cd_det->inject(m_pmt_mother, pmt_det_MCP20inch, pmt_detelem_pos_MCP20inch);
  assert (status);

  IPMTElement* pmt_det_Ham20inch = 0;
  //inject Ham20inch
  IDetElement* t_Ham20inch = det_elem("R12860TorusPMTManager");
  if (t_Ham20inch) {
     pmt_det_Ham20inch = dynamic_cast<IPMTElement*>(t_Ham20inch);
  }
  assert(pmt_det_Ham20inch);

  IDetElementPos* pmt_detelem_pos_Ham20inch = 0;
  double ball_r_Ham20inch = 576.11*mm + 187.5*mm;
  pmt_detelem_pos_Ham20inch = new JUNO::Prototype::HamPMT20inchPos(ball_r_Ham20inch);
  assert (pmt_detelem_pos_Ham20inch);
  status = cd_det->inject(m_pmt_mother, pmt_det_Ham20inch, pmt_detelem_pos_Ham20inch);
  assert (status);

  //inject MCP8inch middle
  IPMTElement* pmt_det_MCP8inch = 0;
  IDetElement* t_MCP8inch = det_elem("MCP8inchPMTManager");
  if (t_MCP8inch) {
     pmt_det_MCP8inch = dynamic_cast<IPMTElement*>(t_MCP8inch);
  }
  assert(pmt_det_MCP8inch);

  IDetElementPos* pmt_MCP8inch_detelem_pos = 0;
  double ball_r_MCP8inch = 559*mm;
  //G4cout<<" radius of MCP8inch = "<<pmt_det_MCP8inch->GetPMTRadius()<<G4endl;
  pmt_MCP8inch_detelem_pos = new JUNO::Prototype::MCPPMT8inchPos(ball_r_MCP8inch + pmt_det_MCP8inch->GetZEquator(),
                                    pmt_det_MCP8inch->GetPMTRadius(), 
                                    pmt_det_MCP8inch->GetPMTRadius() + 32 * mm);
  assert (pmt_MCP8inch_detelem_pos);
  status = cd_det->inject(m_pmt_mother, pmt_det_MCP8inch, pmt_MCP8inch_detelem_pos);
  assert (status);

  //inject Ham8inch middle
  IPMTElement* pmt_det_Ham8inch = 0;
  IDetElement* t_Ham8inch = det_elem("Ham8inchPMTManager");
  if (t_Ham8inch) {
     pmt_det_Ham8inch = dynamic_cast<IPMTElement*>(t_Ham8inch);
  }
  assert(pmt_det_Ham8inch);

  IDetElementPos* pmt_Ham8inch_detelem_pos = 0;
  double ball_r_Ham8inch = 559*mm;
  //G4cout<<" radius of Ham8inch = "<<pmt_det_Ham8inch->GetPMTRadius()<<G4endl;
  pmt_Ham8inch_detelem_pos = new JUNO::Prototype::HamPMT8inchPos(ball_r_Ham8inch + pmt_det_Ham8inch->GetZEquator(),
                                    pmt_det_Ham8inch->GetPMTRadius(),
                                    pmt_det_Ham8inch->GetPMTRadius() + 32 * mm);
  assert (pmt_Ham8inch_detelem_pos);
  status = cd_det->inject(m_pmt_mother, pmt_det_Ham8inch, pmt_Ham8inch_detelem_pos);
  assert (status);

  //inject HZC9inch middle
  IPMTElement* pmt_det_HZC9inch = 0;
  IDetElement* t_HZC9inch = det_elem("HZC9inchPMTManager");
  if (t_HZC9inch) {
     pmt_det_HZC9inch = dynamic_cast<IPMTElement*>(t_HZC9inch);
  }
  assert(pmt_det_HZC9inch);

  IDetElementPos* pmt_HZC9inch_detelem_pos = 0;
  double ball_r_HZC9inch = 559*mm;
  //G4cout<<" radius of HZC9inch = "<<pmt_det_HZC9inch->GetPMTRadius()<<G4endl;
  pmt_HZC9inch_detelem_pos = new JUNO::Prototype::HZCPMT9inchPos(ball_r_HZC9inch + pmt_det_HZC9inch->GetZEquator(),
                                    pmt_det_HZC9inch->GetPMTRadius(),
                                    pmt_det_HZC9inch->GetPMTRadius() + 19 * mm);
  assert (pmt_HZC9inch_detelem_pos);
  status = cd_det->inject(m_pmt_mother, pmt_det_HZC9inch, pmt_HZC9inch_detelem_pos);
  assert (status);

  //inject MCP8inch bottom
  IPMTElement* pmt_det_MCP8inch_BTM = 0;
  IDetElement* t_MCP8inch_BTM = det_elem("MCP8inchPMTManager");
  if (t_MCP8inch_BTM) {
     pmt_det_MCP8inch_BTM = dynamic_cast<IPMTElement*>(t_MCP8inch_BTM);
  }
  assert(pmt_det_MCP8inch_BTM);
  
  IDetElementPos* pmt_MCP8inch_detelem_pos_BTM = 0;
  double ball_r_MCP8inch_BTM = 576.5*mm+75.5*mm;
  pmt_MCP8inch_detelem_pos_BTM = new JUNO::Prototype::MCPPMT8inchPos_BTM(ball_r_MCP8inch_BTM,
                                    pmt_det_MCP8inch_BTM->GetPMTRadius());
  assert (pmt_MCP8inch_detelem_pos_BTM);
  status = cd_det->inject(m_pmt_mother, pmt_det_MCP8inch_BTM, pmt_MCP8inch_detelem_pos_BTM);
  assert (status); 

  //inject Ham8inch bottom
  IPMTElement* pmt_det_Ham8inch_BTM = 0;
  IDetElement* t_Ham8inch_BTM = det_elem("Ham8inchPMTManager");
  if (t_Ham8inch_BTM) {
     pmt_det_Ham8inch_BTM = dynamic_cast<IPMTElement*>(t_Ham8inch_BTM);
  }
  assert(pmt_det_Ham8inch_BTM);

  IDetElementPos* pmt_Ham8inch_detelem_pos_BTM = 0;
  double ball_r_Ham8inch_BTM = 576.5*mm+75.5*mm;
  pmt_Ham8inch_detelem_pos_BTM = new JUNO::Prototype::HamPMT8inchPos_BTM(ball_r_Ham8inch_BTM,
                                    pmt_det_Ham8inch_BTM->GetPMTRadius());
  assert (pmt_Ham8inch_detelem_pos_BTM);
  status = cd_det->inject(m_pmt_mother, pmt_det_Ham8inch_BTM, pmt_Ham8inch_detelem_pos_BTM);
  assert (status);

  //inject HZC9inch bottom
  IPMTElement* pmt_det_HZC9inch_BTM = 0;
  IDetElement* t_HZC9inch_BTM = det_elem("HZC9inchPMTManager");
  if (t_HZC9inch_BTM) {
     pmt_det_HZC9inch_BTM = dynamic_cast<IPMTElement*>(t_HZC9inch_BTM);
  }
  assert(pmt_det_HZC9inch_BTM);

  IDetElementPos* pmt_HZC9inch_detelem_pos_BTM = 0;
  double ball_r_HZC9inch_BTM = 576.5*mm+89.36*mm;
  pmt_HZC9inch_detelem_pos_BTM = new JUNO::Prototype::HZCPMT9inchPos_BTM(ball_r_HZC9inch_BTM,
                                    pmt_det_HZC9inch_BTM->GetPMTRadius());
  assert (pmt_HZC9inch_detelem_pos_BTM);
  status = cd_det->inject(m_pmt_mother, pmt_det_HZC9inch_BTM, pmt_HZC9inch_detelem_pos_BTM);
  assert (status);

/*
  IPMTElement* pmt_det = dynamic_cast<IPMTElement*>(det_elem("HelloPMTManager/PMT_20inch"));
  IDetElementPos* pmt_detelem_pos = 0;

  ball_r = 1056*mm / 2 + 254*mm;
  pmt_detelem_pos = new JUNO::Prototype::PMT20inchPos(ball_r);
  assert (pmt_detelem_pos);
  bool status = cd_det->inject(m_pmt_mother, pmt_det, pmt_detelem_pos);
  assert (status);

  // 8inch
  IPMTElement* pmt_det_8inch = dynamic_cast<IPMTElement*>(det_elem("Hello8inchPMTManager/PMT_8inch"));
  IDetElementPos* pmt_8inch_detelem_pos = 0;

  ball_r = 1056*mm / 2;
  pmt_8inch_detelem_pos = new JUNO::Prototype::PMT8inchPos(ball_r,
                                    pmt_det_8inch->GetPMTRadius(), 
                                    pmt_det_8inch->GetPMTRadius() + 10 * mm);
  // pmt_8inch_detelem_pos = new JUNO::Ball::HexagonPosBall("prototype_8inch_pmts.txt", ball_r + pmt_det_8inch->GetPMTRadius());
  assert (pmt_8inch_detelem_pos);
  status = cd_det->inject(m_pmt_mother, pmt_det_8inch, pmt_8inch_detelem_pos);
  assert (status);

  //bottom 8inch pmt
  IPMTElement* pmt_det_8inch_BTM = dynamic_cast<IPMTElement*>(det_elem("Hello8inchPMTManager/PMT_8inch_BTM"));
  IDetElementPos* pmt_8inch_detelem_pos_BTM = 0;

  ball_r = 1056*mm / 2+254*mm/2.5;
  pmt_8inch_detelem_pos_BTM = new JUNO::Prototype::PMT8inchPos_BTM(ball_r,
                                    pmt_det_8inch_BTM->GetPMTRadius());
  assert (pmt_8inch_detelem_pos_BTM);
  status = cd_det->inject(m_pmt_mother, pmt_det_8inch_BTM, pmt_8inch_detelem_pos_BTM);
  assert (status);
*/


}

void
LSExpDetectorConstruction::setupPrototypeDetectorOnePMT()
{
  SniperPtr<DetSimAlg> detsimalg(*m_scope, "DetSimAlg");
  if (detsimalg.invalid()) {
      std::cout << "Can't Load DetSimAlg" << std::endl;
      assert(0);
  }
  IDetElement* cd_det = 0;
  if (m_cd_name == "PrototypeOnePMT") {
      std::string new_name = m_cd_name + "Construction";
      ToolBase* t = 0;
      // find the tool first
      // create the tool if not exist
      t = detsimalg->findTool(new_name);
      if (not t) {
          t = detsimalg->createTool(new_name);
      }
      if (t) {
          cd_det = dynamic_cast<IDetElement*>(t);
      }
  }

  assert (cd_det);
  de_cd_det = cd_det;

  new G4PVPlacement(0, // no rotation
                    G4ThreeVector(0,0,0), // at (x,y,z)
                    cd_det -> getLV(),    // its logical volume
                    "pPrototypeDetector",   // its name
                    logicWorld,      // its mother  volume
                    false,           // no boolean operation|     s
                    0);              // no particular field
  // construct the PMT
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4VSensitiveDetector* not_a_leak = getPMTSD();

  if (!not_a_leak) {
      G4cerr << "Failed to load Sensitive Detector." << G4endl;
      G4cerr << "Skip to setup the SD. " << G4endl;
  } else {
      SDman->AddNewDetector(not_a_leak);  
  }

  IPMTElement* pmt_det = 0;
  std::string current_test_pmt = "20inchPMT";
  // XXX the m_pmt_name is used to switch different PMT types including 20inch 
  // and 3inch PMTs.
  current_test_pmt = m_pmt_name;
  // == 20inch PMT ==
  if (current_test_pmt == "20inchPMT" or current_test_pmt == "OnlyPMT") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("HelloPMTManager/PMT_20inch"));
  } else if (current_test_pmt == "R3600") {
      pmt_det = new R3600PMTManager(
              "PMT_20inch",
              LS,
              Pyrex,
              Photocathode_opsurf,
              Vacuum,
              Steel,
              NULL,
              not_a_leak);
  } else if (current_test_pmt == "R12860") {
      IDetElement* t = det_elem("R12860PMTManager/PMT_20inch");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  } else if (current_test_pmt == "R12860Torus") {
      IDetElement* t = det_elem("R12860TorusPMTManager");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  } else if (current_test_pmt == "R12860OnlyFront") {
      IDetElement* t = det_elem("R12860OnlyFrontPMTManager/PMT_20inch");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  } else if (current_test_pmt == "R12860OnlyFrontMask") {
      IDetElement* t = det_elem("R12860OnlyFrontPMTManager/PMT_20inch");
      IDetElement* mask = det_elem("R12860OnlyFrontMaskManager/mask_PMT_20inch");
      if (t && mask) {
          // build
          mask->getLV();
          t->getLV();
          pmt_det = dynamic_cast<IPMTElement*>(mask);
          pmt_det->inject("lMaskVirtual", t, 0);
      }

      assert(pmt_det);
  }else if (current_test_pmt == "MCP8inch") {
      IDetElement* t = det_elem("MCP8inchPMTManager");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  }else if (current_test_pmt == "MCP20inch") {
      IDetElement* t = det_elem("MCP20inchPMTManager");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  }else if (current_test_pmt == "Ham8inch") {
      IDetElement* t = det_elem("Ham8inchPMTManager");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  }else if (current_test_pmt == "HZC9inch") {
      IDetElement* t = det_elem("HZC9inchPMTManager");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  }else if (current_test_pmt == "HamamatsuR12860") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("HamamatsuR12860PMTManager/PMT_20inch"));
  } else if (current_test_pmt == "Tub3inch") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTManager/PMT_3inch"));
  } else if (current_test_pmt == "Tub3inchV2") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTV2Manager/PMT_3inch"));
  } else if (current_test_pmt == "Tub3inchV3") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTV3Manager/PMT_3inch"));
      // pmt_det = new Tub3inchPMTV3Manager(
      //         "PMT_3inch",
      //         LS,
      //         Pyrex,
      //         Photocathode_opsurf_3inch,
      //         Vacuum,
      //         Steel,
      //         Steel,
      //         not_a_leak);
  } else if (current_test_pmt == "Hello3inch") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("Hello3inchPMTManager/PMT_3inch"));
  } else if (current_test_pmt == "Hello8inch") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("Hello8inchPMTManager/PMT_8inch"));
  } else if (current_test_pmt == "PMTMask"){

      IPMTElement* pmt_inner = 0;
      IDetElement* t_inner = det_elem("R12860PMTManager/PMT_20inch");
      if (t_inner) {
          pmt_inner = dynamic_cast<IPMTElement*>(t_inner);
      }

      std::string new_name = "R12860MaskManager";
      IDetElement* t = det_elem(new_name);
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }


      assert(pmt_det);
      IDetElementPos* mask_detelem_pos = 0;
      auto lv = pmt_det -> getLV();
      assert ( lv );
      bool status = pmt_det->inject(m_mask_mother, pmt_inner, mask_detelem_pos);
      assert (status);
  } else if (current_test_pmt == "NNVTMCPPMT") {
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("NNVTMCPPMTManager/PMT_20inch"));
  } else if (current_test_pmt == "NNVTMask") {
      IPMTElement* pmt_inner = dynamic_cast<IPMTElement*>(det_elem("NNVTMCPPMTManager/PMT_20inch"));
      pmt_inner->getLV();
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("NNVTMaskManager/NNVTMask"));
      IDetElementPos* mask_detelem_pos = 0;
      pmt_det->inject(m_mask_mother, pmt_inner, mask_detelem_pos);
  } else if (current_test_pmt == "HamamatsuMask") {
      IPMTElement* pmt_inner = dynamic_cast<IPMTElement*>(det_elem("HamamatsuR12860PMTManager/PMT_20inch"));
      pmt_inner->getLV();
      pmt_det = dynamic_cast<IPMTElement*>(det_elem("HamamatsuMaskManager/HamamatsuMask"));
      IDetElementPos* mask_detelem_pos = 0;
      pmt_det->inject(m_mask_mother, pmt_inner, mask_detelem_pos);
  } else {
      G4cout << "Unknown PMT name: " << current_test_pmt << G4endl;
      // Maybe it is a detector element.
      IDetElement* t = det_elem(current_test_pmt);
      if (!t) {
          assert(pmt_det);
      } else {
          G4cout << current_test_pmt << " may be a detector element. " << G4endl;
      }
  }

  // inject the PMT into CD
  std::string pos_tool_name = "OnePMTPlacement";
  IDetElementPos* pos_pmt = det_elem_pos(pos_tool_name);
  assert(pos_pmt);
  bool status = false;
  if (pmt_det) {
      status = de_cd_det -> inject( "lBuffer", 
                                     pmt_det, 
                                     pos_pmt);
  } else {
      IDetElement* t = det_elem(current_test_pmt);
      auto lv = t->getLV();
      assert(lv);
      status = de_cd_det -> inject( "lBuffer", 
                                     t, 
                                     pos_pmt);
  }
  assert(status);
}

G4VSensitiveDetector*
LSExpDetectorConstruction::getPMTSD() {
    // TODO: Create an interface for PMT
    G4VSensitiveDetector* sd = 0;
    std::string sd_name = "PMTSDMgr";
    SniperPtr<DetSimAlg> detsimalg(*m_scope, "DetSimAlg");
    if (detsimalg.invalid()) {
        std::cout << "Can't Load DetSimAlg" << std::endl;
        assert(0);
    }
    ToolBase* t = 0;
    // find the tool first
    // create the tool if not exist
    t = detsimalg->findTool(sd_name);
    if (not t) {
        t = detsimalg->createTool(sd_name);
    }
    if (t) {
        G4cout << "FOUND PMTSDMgr. Now get the Sensitive Detector... " << G4endl;
        ISensitiveDetectorElement* sde = dynamic_cast<ISensitiveDetectorElement*>(t);
        if (sde) {
            sd = sde->getSD();
        }
    }

    return sd;
}

IDetElement* 
LSExpDetectorConstruction::det_elem(const std::string& name) {
    SniperPtr<DetSimAlg> detsimalg(*m_scope, "DetSimAlg");
    if (detsimalg.invalid()) {
        std::cout << "Can't Load DetSimAlg" << std::endl;
        assert(0);
    }

    // get type/nobj
    std::string type = name;
    std::string nobj = name;
    std::string::size_type pseg = name.find('/');
    if ( pseg != std::string::npos ) {
        type = type.substr(0, pseg);
        nobj = nobj.substr(pseg+1, std::string::npos);
    }

    ToolBase* t = 0;
    // find the tool first
    // create the tool if not exist
    t = detsimalg->findTool(nobj);
    if (not t) {
        t = detsimalg->createTool(name);
    }

    return dynamic_cast<IDetElement*>(t);
}

IDetElementPos* 
LSExpDetectorConstruction::det_elem_pos(const std::string& name) {
    SniperPtr<DetSimAlg> detsimalg(*m_scope, "DetSimAlg");
    if (detsimalg.invalid()) {
        std::cout << "Can't Load DetSimAlg" << std::endl;
        assert(0);
    }

    ToolBase* t = 0;
    // find the tool first
    // create the tool if not exist
    t = detsimalg->findTool(name);
    if (not t) {
        t = detsimalg->createTool(name);
    }

    return dynamic_cast<IDetElementPos*>(t);
}

void LSExpDetectorConstruction::setupPMT(G4VSensitiveDetector* not_a_leak) {

  // 20 inch PMT
  if (m_pmt_name == "OnlyPMT"){
      IDetElement* t = det_elem("HelloPMTManager/PMT_20inch");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }
  
  } else if (m_pmt_name == "R12860") {
      IDetElement* t = det_elem("R12860PMTManager/PMT_20inch");
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }

      assert(pmt_det);
  }else if (m_pmt_name == "PMTMask"){

      IPMTElement* pmt_inner = 0;
      IDetElement* t_inner = det_elem("R12860PMTManager/PMT_20inch");
      if (t_inner) {
          pmt_inner = dynamic_cast<IPMTElement*>(t_inner);
      }

      std::string new_name = "R12860MaskManager";
      IDetElement* t = det_elem(new_name);
      if (t) {
          pmt_det = dynamic_cast<IPMTElement*>(t);
      }


      if (pmt_det and pmt_det->getLV()) {
          IDetElementPos* mask_detelem_pos = 0;
          //if( m_pmt_name == "PMTMask" && m_qe_mode)
          bool status = pmt_det->inject(m_mask_mother, pmt_inner, mask_detelem_pos);
          if (not status) {
  	      G4cout << "Error: place PMT into Mask failed." << G4endl;
	  }
      } else {
	  G4cout << "Error: Can't find PMT Mask" << G4endl;
      }
  } else if (m_pmt_name == "HamamatsuMask") {
    IDetElementPos* mask_detelem_pos = 0;
    // Hamamatsu R12860 
    IPMTElement* r12860_pmt = dynamic_cast<IPMTElement*>(det_elem("HamamatsuR12860PMTManager/PMT_20inch"));
    r12860_pmt->getLV();
    IPMTElement* r12860_mask = dynamic_cast<IPMTElement*>(det_elem("HamamatsuMaskManager"));
    r12860_mask->inject(m_mask_mother, r12860_pmt, mask_detelem_pos);
    pmt_det = r12860_mask;
  } else if (m_pmt_name == "NNVTMask") {  
    IDetElementPos* mask_detelem_pos = 0;
    // NNVT MCP-PMT: objName: NNVTMCPPMT
    IPMTElement* mcppmt_pmt = dynamic_cast<IPMTElement*>(det_elem("NNVTMCPPMTManager/PMT_20inch"));
    mcppmt_pmt->getLV();
    IPMTElement* mcppmt_mask = dynamic_cast<IPMTElement*>(det_elem("NNVTMaskManager"));
    mcppmt_mask->inject(m_mask_mother, mcppmt_pmt, mask_detelem_pos);
    pmt_det = mcppmt_mask;
  }

  // Two categories LPMT mode
  if (m_extra_lpmt=="TWO") {
    // Hamamatsu R12860: objName: HamamatsuR12860
    det_elem("HamamatsuR12860PMTManager/HamamatsuR12860");
    // NNVT MCP-PMT: objName: NNVTMCPPMT
    det_elem("NNVTMCPPMTManager/NNVTMCPPMT");
  } else if (m_extra_lpmt=="TWO-mask") {
    // NOTE: make sure the name HamamatsuR12860 and NNVTMCPPMT are unchanged, 
    //       even mask is added. 
    IDetElementPos* mask_detelem_pos = 0;
    // Hamamatsu R12860 
    IPMTElement* r12860_pmt = dynamic_cast<IPMTElement*>(det_elem("HamamatsuR12860PMTManager/HamamatsuR12860_PMT_20inch"));
    r12860_pmt->getLV();
    IPMTElement* r12860_mask = dynamic_cast<IPMTElement*>(det_elem("HamamatsuMaskManager/HamamatsuR12860"));
    r12860_mask->inject(m_mask_mother, r12860_pmt, mask_detelem_pos);
    
    // NNVT MCP-PMT: objName: NNVTMCPPMT
    IPMTElement* mcppmt_pmt = dynamic_cast<IPMTElement*>(det_elem("NNVTMCPPMTManager/NNVTMCPPMT_PMT_20inch"));
    mcppmt_pmt->getLV();
    IPMTElement* mcppmt_mask = dynamic_cast<IPMTElement*>(det_elem("NNVTMaskManager/NNVTMCPPMT"));
    mcppmt_mask->inject(m_mask_mother, mcppmt_pmt, mask_detelem_pos);

  }

  // Veto PMT
  if (m_veto_pmt_name == "R12860OnlyFront") {
      pmt_det_veto = dynamic_cast<IPMTElement*>(det_elem("R12860OnlyFrontPMTManager/PMT_20inch_veto"));
      assert (pmt_det_veto);
      auto lv = pmt_det_veto->getLV();
      assert (lv);
  } else if (m_veto_pmt_name == "R12860OnlyFrontMask") {
      IDetElement* t = det_elem("R12860OnlyFrontPMTManager/PMT_20inch_veto");
      IDetElement* mask = det_elem("R12860OnlyFrontMaskManager/mask_PMT_20inch_veto");
      if (t && mask) {
          // build
          mask->getLV();
          t->getLV();
          pmt_det_veto = dynamic_cast<IPMTElement*>(mask);
          pmt_det_veto->inject("lMaskVirtual", t, 0);
      }

      assert(pmt_det_veto);
  } else {
      G4cerr << "Failed to find the VetoPMT named " << m_veto_pmt_name << G4endl;
      assert (pmt_det_veto);
  }

  // 3inch
  if (m_3inchpmt_name == "Tub3inch") {
      pmt_det_3inch = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTManager/PMT_3inch"));
      m_3inchpmt_pos_offset += 10*cm;
  } else if (m_3inchpmt_name == "Tub3inchV2") {
      pmt_det_3inch = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTV2Manager/PMT_3inch"));
  } else if (m_3inchpmt_name == "Tub3inchV3") {
      pmt_det_3inch = dynamic_cast<IPMTElement*>(det_elem("Tub3inchPMTV3Manager/PMT_3inch"));
      // pmt_det_3inch = new Tub3inchPMTV3Manager(
      //         "PMT_3inch",
      //         Water,
      //         Pyrex,
      //         Photocathode_opsurf_3inch,
      //         Vacuum,
      //         Steel,
      //         Steel,
      //         not_a_leak);
  } else if (m_3inchpmt_name == "Hello3inch") {
      pmt_det_3inch = dynamic_cast<IPMTElement*>(det_elem("Hello3inchPMTManager/PMT_3inch"));
  } else {
      G4cout << "Unknown 3inch PMT name: " << m_3inchpmt_name << G4endl;
      assert(pmt_det_3inch);
  }

}

void
LSExpDetectorConstruction::setupCD_PMTs() {
  // G4SDManager* SDman = G4SDManager::GetSDMpointer();
  // G4VSensitiveDetector* not_a_leak = getPMTSD();
  // SDman->AddNewDetector(not_a_leak);
  // IPMTElement* pmt_det = 0;
  // inject the Logical Volume into CD
  IDetElement* cd_det = de_cd_det;
  IDetElementPos* pmt_detelem_pos = 0;
  if (m_pmt_pos_mode == "CalMode") {
      pmt_detelem_pos = new JUNO::Ball::CalPositionBall(ball_r, 
              pmt_det->GetPMTRadius(), 
              pmt_det->GetZEquator(),
              5*mm);
  } else if (m_pmt_pos_mode == "FileMode") {
      pmt_detelem_pos = new JUNO::Ball::HexagonPosBall(m_scope, true);
  }
  std::cout << "20inch PMT Position Mode: " << m_pmt_pos_mode << std::endl;
  assert (pmt_detelem_pos);
  bool status = false;
  if (m_extra_lpmt=="TWO" or m_extra_lpmt=="TWO-mask") { // support two categories of LPMT.
    status = cd_det->inject(m_pmt_mother, 0, pmt_detelem_pos);
    assert (status);
  } else if (m_extra_lpmt == "ONE") {
    status = cd_det->inject(m_pmt_mother, pmt_det, pmt_detelem_pos);
    assert (status);
  } else {
      LogError << "Unknown value of option extra lpmt mode: " << m_extra_lpmt << std::endl;
      assert (status);
  }

  
  
  // update the ball_r according to the offset
  double ball_r_spmt = ball_r; // equator of LPMT
  ball_r_spmt += m_3inchpmt_pos_offset;
  G4cout << "Current 3inch PMT Pos: " << ball_r_spmt << G4endl;
  IDetElementPos* pmt_3inch_detelem_pos = 0;


  pmt_3inch_detelem_pos = new JUNO::Ball::HexagonPosBall(m_scope, false);

  assert (pmt_3inch_detelem_pos);
  status = cd_det->inject("3inchInnerWater", pmt_det_3inch, pmt_3inch_detelem_pos);
  assert (status);
}

void
LSExpDetectorConstruction::setupWP_PMTs() {

  //put veto pmts into water pool
  IDetElement* glob_info = det_elem("GlobalGeomInfo");
  G4double waterpool_r = glob_info->geom_info("WaterPool.R");
  G4double waterpool_h = glob_info->geom_info("WaterPool.H");
  G4double vetopmt_interval = 2*m;
  IDetElementPos* veto_pmt_detelem_pos = 0;
  if (m_veto_pmt_pos_mode == "CalMode") {
      veto_pmt_detelem_pos = new JUNO::Cylinder::CalPositionCylinder(
              waterpool_r, 
              waterpool_h,
              pmt_det_veto->GetPMTRadius(), 
              pmt_det_veto->GetPMTHeight(),
              vetopmt_interval);
  } else if (m_veto_pmt_pos_mode == "FileMode") {
   veto_pmt_detelem_pos = new JUNO::Ball::VetoPmtPosBall(m_scope);
  }
  assert (veto_pmt_detelem_pos);
 std::cout << "WP PMT Position Mode: " << m_veto_pmt_pos_mode << std::endl;
  // If the string m_veto_pmt_pos_mode is empty, don't place any PMTs.
  if (m_veto_pmt_pos_mode.size()!=0) {
      assert (veto_pmt_detelem_pos);
      m_pmt_mother = "OuterWaterVeto";
      bool veto_status = de_wp_det->inject(m_pmt_mother, pmt_det_veto, veto_pmt_detelem_pos); 
      assert (veto_status);
  }
}

void
LSExpDetectorConstruction::setupCD_Sticks(IDetElement* cd_det) {
//----------------------------ADD------------------------------------//
 
//--------------------------------------------------------------------//
//strut of type 1 
  setupAnchor(m_strut_name  , m_strut_mother , m_strut_pos_file, cd_det); 

//--------------------------------------------------------------------//
//strut of type 2 and strut ball head

 if (m_strut_name == "StrutAcrylic"){
     if(m_strut2_name == "StrutBar2Acrylic"){
       setupAnchor(m_strut2_name  , m_strut_mother , m_strut2_pos_file, cd_det);     
     } 
     
     if(m_strutballhead_name == "StrutBallheadAcrylic"){
       setupAnchor(m_strutballhead_name , m_strut_mother , m_fastener_pos_file , cd_det);
     }
}
//---------------------------------------------------------------------//
// strut anchor consist of FastenerAcrylic, UpperAcrylic and AdditionAcrylic.

  if (m_fastener_name == "FastenerAcrylic") {
     setupAnchor(m_fastener_name , m_fastener_mother , m_fastener_pos_file , cd_det);    
  } else if (m_fastener_name == "FastenerBalloon") {
      // no fastener in Balloon Option
  }

  if (m_upper_name == "UpperAcrylic") {
     setupAnchor(m_upper_name , m_upper_mother , m_fastener_pos_file , cd_det);    
  } else if (m_upper_name == "FastenerBalloon") {
      // no upper in Balloon Option
  }

  if (m_addition_name == "AdditionAcrylic") {
      setupAnchor(m_addition_name , m_addition_mother , m_fastener_pos_file , cd_det);  

  } else if (m_addition_name == "FastenerBalloon") {
      // no addition in Balloon Option
  }

//----------------------------------------------------------------------//


  bool DEBUG_DISABLE_XJ = getenv("JUNO_DEBUG_DISABLE_XJ") == nullptr ? false : true ; 
  if(DEBUG_DISABLE_XJ)
  {
      G4cout << "LSExpDetectorConstruction::setupCD_Sticks --debug-disable-xj " << G4endl ; 
  }
  else
  {
      // XJ
      setupAnchor(m_xjanchor_name  , m_xjanchor_mother , m_xjanchor_pos_file,cd_det);
      setupAnchor(m_xjfixture_name , m_xjfixture_mother , m_xjanchor_pos_file,cd_det);
  }


  bool DEBUG_DISABLE_SJ = getenv("JUNO_DEBUG_DISABLE_SJ") == nullptr ? false : true ; 
  if(DEBUG_DISABLE_SJ)
  {
      G4cout << "LSExpDetectorConstruction::setupCD_Sticks --debug-disable-sj " << G4endl ; 
  }
  else
  {
      // SJ
      setupAnchor(m_sjclsanchor_name , m_sjclsanchor_mother,  m_sjclsanchor_pos_file, cd_det);  
      setupAnchor(m_sjfixture_name , m_sjfixture_mother, m_sjfixture_pos_file, cd_det);
      setupAnchor(m_sjreceiver_name , m_sjreceiver_mother, m_sjreceiver_pos_file, cd_det);
      setupAnchor(m_sjreceiver_fastener_name , m_sjreceiver_fastener_mother, m_sjreceiver_pos_file , cd_det);
  }

//-----------------------END--------------------------------------------//

}

void LSExpDetectorConstruction::setupAnchor(std::string anchor_name , std::string anchor_mother_name,  std::string anchor_pos_file , IDetElement * cd_det){

    IDetElement* anchor_det = nullptr;
    std::string new_name = anchor_name + "Construction";
    anchor_det = det_elem(new_name);
    assert(anchor_det);
    IDetElementPos* anchor_detelem_pos = nullptr;
    G4double anchor_r = anchor_det->geom_info("PosR") ;  
    anchor_detelem_pos = new JUNO::Ball::HexagonPosBall(anchor_pos_file,anchor_r,false);
    assert (anchor_detelem_pos);
    bool anchor_det_status = cd_det->inject(anchor_mother_name, anchor_det, anchor_detelem_pos );
    assert(anchor_det_status);
     
    LogInfo<<"anchor_name = " << new_name <<"   "
           <<"anchor_r =  "   << anchor_r <<" mm  "
           <<"anchor_pos_file = "<<anchor_pos_file << std::endl;

}


void
LSExpDetectorConstruction::setupRockAndExpHall() {
    // == Top Rock ==
    de_top_rock = det_elem("TopRockConstruction");
    m_topRockOffsetX = de_top_rock->geom_info("TopRockOffset.X");
    m_topRockOffsetZ = de_top_rock->geom_info("TopRockOffset.Z");
    // --- inject the top and bottom into world ---
    new G4PVPlacement(0,              // no rotation
      G4ThreeVector(m_topRockOffsetX,0,m_topRockOffsetZ), // at (x,y,z)
      de_top_rock->getLV(),    // its logical volume
      "pTopRock",       // its name
      logicWorld,      // its mother  volume
      false,           // no boolean operations
      0);              // no particular field
    // === Exp. Hall ===
    de_exp_hall = det_elem("ExpHallConstruction");
    assert(de_exp_hall);
    de_top_rock->inject("lTopRock", de_exp_hall, 0);
    // == Bottom Rock ==
    // Bottom Rock contains the Steel Tube.
    de_bottom_rock = det_elem("BottomRockConstruction");
    // --- inject the top and bottom into world ---
    new G4PVPlacement(0,              // no rotation
      G4ThreeVector(0,0,0), // at (x,y,z)
      de_bottom_rock->getLV(),    // its logical volume
      "pBtmRock",       // its name
      logicWorld,      // its mother  volume
      false,           // no boolean operations
      0);              // no particular field
}

void 
LSExpDetectorConstruction::setupTopTracker()
{
  std::string tt_tool_name;
  if (m_tt_name == "TT_OPERA") {
      tt_tool_name = "TopTrackerConstruction";
  }
  IDetElement* tt_det = det_elem(tt_tool_name);
  assert (tt_det);

  if (not tt_det) {
      G4cout << "Can't find the TopTracker (" << tt_tool_name << ")" << std::endl;
      return; 
  }

  // inject the top tracker into Exp. Hall
  de_exp_hall -> inject("lExpHall", tt_det, 0);
}

void
LSExpDetectorConstruction::setupCalibUnit()
{
    // check WP and CD
    if (!de_cd_det) {
        G4cout << "WARNING: Central Detector is not enabled, so Calib Unit will be disabled." << G4endl;
        return;
    }

  std::string cu_tool_name;
  std::string pos_tool_name;
  if (m_calibunit_name == "CalibSource") {
      cu_tool_name = "CalibSourceConstruction";
      pos_tool_name = "CalibSourcePlacement";
  }
  else if(m_calibunit_name == "lSourceWorld"){
      cu_tool_name = m_calibunit_name;
      pos_tool_name = "CalibSourcePlacement";
  } else {
      if (m_calibunit_name == "CalibTube") {
          cu_tool_name = "CalibTubeConstruction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin: CalibTubeConstruction " << G4endl; 
      }else if (m_calibunit_name == "CalibTube_flatwindow"){
          cu_tool_name = "CalibTube_flatwindow_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin: CalibTube_flatwindow_Construction " << G4endl; 
      }else if (m_calibunit_name == "CalibTube_convexwindow"){
          cu_tool_name = "CalibTube_convexwindow_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin:  CalibTube_convexwindow_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_reflectwindow"){
          cu_tool_name = "CalibTube_reflectwindow_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin:  CalibTube_reflectwindow_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_offcenter"){
          cu_tool_name = "CalibTube_offcenter_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin:  CalibTube_offcenter_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_offcenter_reflectwindow"){
          cu_tool_name = "CalibTube_offcenter_reflectwindow_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin:  CalibTube_offcenter_reflectwindow_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_offcenter_twocone"){
          cu_tool_name = "CalibTube_offcenter_twocone_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin:  CalibTube_offcenter_twocone_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_onecone"){
          cu_tool_name = "CalibTube_onecone_Construction";
          pos_tool_name = "CalibTubePlacement";
          G4cout << "Xin: CalibTube_onecone_Construction" << G4endl; 
      }else if (m_calibunit_name == "CalibTube_twocone"){
          cu_tool_name = "CalibTube_twocone_Construction";
          pos_tool_name = "CalibTubePlacement";   
          G4cout << "Xin:  CalibTube_twocone_Construction" << G4endl; 
      } else if(m_calibunit_name == "Calib_GuideTube"){
          cu_tool_name = "Calib_GuideTube_Construction"; 
          pos_tool_name = "Calib_GuideTube_Placement";
          G4cout << "Calib/GuideTube is constructed. " << G4endl;
      } else if(m_calibunit_name == "Calib_GuideTube_V1_0"){ 
          cu_tool_name = "Calib_GuideTube_Construction_V1_0";  
          pos_tool_name = "Calib_GuideTube_Placement_V1_0"; 
          G4cout << "Calib/GuideTube V1_0 is constructed. " << G4endl; 
      } else if(m_calibunit_name == "Calib_GuideTube_V1_1"){ 
          cu_tool_name = "Calib_GuideTube_Construction_V1_1";  
           pos_tool_name = "Calib_GuideTube_Placement_V1_1"; 
           G4cout << "Calib/GuideTube V1_1 is constructed. " << G4endl; 
      } else {
          cu_tool_name = m_calibunit_name;
          pos_tool_name = "CalibTubePlacement";
      }
  }
  de_calibunit = det_elem(cu_tool_name);
  assert (de_calibunit);

  if (not de_calibunit) {
      G4cout << "Can't find the CalibUnit (" << cu_tool_name << ")" << std::endl;
      return; 
  }
  IDetElementPos* pos_calibunit = det_elem_pos(pos_tool_name);
  assert(pos_calibunit);
  // inject the calib unit into the LS 
  bool status; 
  if(m_calibunit_name != "Calib_GuideTube" 
     && m_calibunit_name != "Calib_GuideTube_V1_0" 
     && m_calibunit_name != "Calib_GuideTube_V1_1") {
    status = de_cd_det -> inject( "lTarget", 
                                     de_calibunit, 
                                     pos_calibunit);
  } else {
    status = de_cd_det -> inject( "lWaterPool",
                                     de_calibunit,
                                     pos_calibunit);
  }
  assert(status);

      return; 

}
 
 void
LSExpDetectorConstruction::setupTopChimney(){
    G4cout<<"Injecting Top Chimney..."<<G4endl;
  std::string cm_tool_name;
  std::string pos_tool_name;
  if (m_chimney_top_name== "TopChimney") {
      cm_tool_name = "UpperChimney";
      pos_tool_name = "UpperChimneyPlacement";
  }
 de_topchimney= det_elem(cm_tool_name);
  assert (de_topchimney);

  if (not de_topchimney) {
      G4cout << "Can't find the TopChimney(" << cm_tool_name << ")" << std::endl;
      return;
  }
  IDetElementPos* pos_topchimney= det_elem_pos(pos_tool_name);
  assert(pos_topchimney);
  // inject the calib unit into the LS 
  bool status = de_exp_hall-> inject( "lExpHall",
                                     de_topchimney,
                                     pos_topchimney);
  assert(status);

      return;
}

void
LSExpDetectorConstruction::setupLowerChimney(){
    G4cout<<"Injecting Lower Chimney..."<<G4endl;
  std::string cm_tool_name;
  std::string pos_tool_name;
  if (m_chimney_lower_name== "LowerChimney") {
      cm_tool_name = "LowerChimney";
      pos_tool_name = "LowerChimneyPlacement";
  }
 de_lowerchimney= det_elem(cm_tool_name);
  assert (de_lowerchimney);

  if (not de_lowerchimney) {
      G4cout << "Can't find the TopChimney(" << cm_tool_name << ")" << std::endl;
      return;
  }
  IDetElementPos* pos_lowerchimney= det_elem_pos(pos_tool_name);
  assert(pos_lowerchimney);
  // inject the lower chimney into the inner Water pool 
  bool status = de_cd_det->inject("lWaterPool", 
                                     de_lowerchimney,
                                     pos_lowerchimney);
  assert(status);


}

void
LSExpDetectorConstruction::setupOuterWaterPool()
{
    de_wp_det = det_elem("WaterPoolConstruction");
    assert(de_wp_det);
    // place the water pool into bottom rock or the world.
    if (de_bottom_rock == 0) {
        new G4PVPlacement(0, // no rotation
                          G4ThreeVector(0,0,0), // at (x,y,z)
                          de_wp_det -> getLV(),    // its logical volume
                          "pOuterWaterPool",   // its name
                          logicWorld,      // its mother  volume
                          false,           // no boolean operation|     s
                          0);              // no particular field
    } else {
        de_bottom_rock->inject("lPoolLining", de_wp_det, 0);
    }
}

bool
LSExpDetectorConstruction::helper_mpt(G4MaterialPropertiesTable* MPT, const std::string& mname, IMCParamsSvc* params, const std::string& name, double scale) {
    IMCParamsSvc::vec_d2d props;
    bool st = params->Get(name, props);
    if (!st) {
        LogError << "can't find material property: " << name << std::endl;
        return false;
    }
    G4MaterialPropertyVector* vec = new G4MaterialPropertyVector(0,0,0);

    int N = props.size();
    if (!N) {
        LogError << "empty material property: " << name << std::endl;
        return false;
    }
    for (int i = 0; i < N; ++i) {
        vec->InsertValues(props[i].get<0>(), props[i].get<1>()*scale);
    }
    MPT->AddProperty(mname.c_str(), vec);
    return true;
}

void
LSExpDetectorConstruction::setMTmode(bool flag) {
    m_isMTmode = flag;
}
