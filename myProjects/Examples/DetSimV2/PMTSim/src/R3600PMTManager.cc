#include "G4SystemOfUnits.hh"
#include "R3600PMTManager.hh"
#include "dywTorusStack.hh"
#include "dywPMTOpticalModel.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh" // for boolean solids
#include "G4SubtractionSolid.hh" // for boolean solids
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh" // for G4VisAttributes::Invisible
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;
// Interface
G4LogicalVolume* 
R3600PMTManager::getLV() {
    return m_logical_pmt;
}

G4double
R3600PMTManager::GetPMTRadius() {
    return m_pmt_r;
}

G4double
R3600PMTManager::GetPMTHeight() {
    return m_pmt_h;
}

G4double
R3600PMTManager::GetZEquator() {
    return m_z_equator;
}

G4ThreeVector
R3600PMTManager::GetPosInPMT() {
    G4ThreeVector rndm_pos;
    return rndm_pos;
}

// Constructor
R3600PMTManager::R3600PMTManager
    (const G4String& plabel, // label -- subvolume names are derived from this
     G4Material* inExteriorMat,// material which fills the bounding cylinder
     G4Material* inGlassMat,   // glass material
     G4OpticalSurface* inPhotocathode_opsurf, // photocathode 
     G4Material* inPMT_Vacuum, // vacuum inside tube
     G4Material* inDynodeMat,  // dynode material
     G4Material* inMaskMat,    // material for photocathode mask (e.g, blk acryl)
                             // OK to set MaskMat == NULL for no mask
     G4VSensitiveDetector *indetector // sensitive detector hook
    )
    : m_logical_pmt(NULL),
      m_mirror_opsurf(NULL),
      ExteriorMat(inExteriorMat),
      GlassMat(inGlassMat),
      Photocathode_opsurf(inPhotocathode_opsurf),
      PMT_Vacuum(inPMT_Vacuum),
      DynodeMat(inDynodeMat),
      MaskMat(inMaskMat),
      m_detector(indetector),
      m_label(plabel)
{

    initialize();

}
    

// Helper Methods
void
R3600PMTManager::initialize() {
    // construct
    // * construct a mirror surface
    init_variables();
    init_mirror_surface();
    init_pmt();
}

void
R3600PMTManager::init_variables() {
    m_pmt_r = 255.*mm;
    m_pmt_h = 680.*mm;
    m_z_equator = m_pmt_r;
}

void
R3600PMTManager::init_mirror_surface() {
    if ( m_mirror_opsurf == NULL ) {
        // construct a static mirror surface with idealized properties
        m_mirror_opsurf =  new G4OpticalSurface("Mirror_opsurf");
        m_mirror_opsurf->SetFinish(polishedfrontpainted); // needed for mirror
        m_mirror_opsurf->SetModel(glisur); 
        m_mirror_opsurf->SetType(dielectric_metal); 
        m_mirror_opsurf->SetPolish(0.999);              // a guess -- FIXME
        G4MaterialPropertiesTable* propMirror= NULL;
        G4Material *matMirror = G4Material::GetMaterial("PMT_Mirror");
        if (matMirror) {
            propMirror= matMirror->GetMaterialPropertiesTable();
        }
        if ( propMirror == NULL ) {
            G4cout << "Warning: setting PMT mirror reflectivity to 0.9999 "
                   << "because no PMT_Mirror material properties defined" << G4endl;
            propMirror= new G4MaterialPropertiesTable();
            propMirror->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(800.0e-9*m), 0.9999);
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(200.0e-9*m), 0.9999);
            propMirror->AddEntry("REFLECTIVITY", 1.55*eV, 0.9999);
            propMirror->AddEntry("REFLECTIVITY", 15.5*eV, 0.9999);
        }
        m_mirror_opsurf->SetMaterialPropertiesTable( propMirror );

    }

}

static const int R3600_n_edge= 9;
static const G4double R3600_z_edge[R3600_n_edge+1]= { 188.00,  116.71,    0.00, -116.71, -136.05, -195.00, -282.00, -355.00, -370.00, -422.00};
static const G4double R3600_rho_edge[R3600_n_edge+1]= { 0.00,  198.55,  254.00,  198.55,  165.40,  127.00,  127.00,   53.00,   41.35,   41.35};
static const G4double R3600_z_o[R3600_n_edge]= {     -127.00,    0.00,    0.00,  127.00, -195.00, -195.00, -280.00, -370.00, -370.00};

void
R3600PMTManager::init_pmt() {
    // Refer to dyw_PMT_LogicalVolume.cc
    //

    ConstructPMT_UsingTorusStack
     ( R3600_n_edge,
       R3600_z_edge,
       R3600_rho_edge,
       R3600_z_o,
        94.*millimeter,       // radius of dynode stack
      -117.*millimeter,       // z coordinate of top of dynode stack, equator=0
       4.  *millimeter,       // thickness of the walls
       ExteriorMat,                   // material outside tube
       GlassMat,                      // glass material
       Photocathode_opsurf,               // photocathode surface 
       PMT_Vacuum,                    // tube interior
       DynodeMat,                     // dynode stack metal
       m_detector                // detector hook
     );

}

void
R3600PMTManager::
ConstructPMT_UsingTorusStack
   (const G4int n_edge,
    const G4double outer_z_edge[],
    const G4double outer_rho_edge[],
    const G4double outer_z_o[],
    G4double r_dynode,     // radius of dynode stack
    G4double z_dynode,     // z coordinate of top of dynode stack, equator=0.
    G4double d_wall,       // thickness of the walls
    G4Material*  /* Exterior */  ,          // material outside tube
    G4Material* Glass,             // glass material
    G4OpticalSurface* OpPCSurface,  // photocathode surface
    G4Material* PMT_Vac,           // tube interior
    G4Material* Dynode_mat,        // dynode stack metal
    G4VSensitiveDetector* /*detector*/ // detector hook
   )
{
  ////////////////////////////////////////////////////////////////
  // MAKE SOLIDS
  ////
  helper_make_solid(n_edge,
                    outer_z_edge,
                    outer_rho_edge,
                    outer_z_o,
                    r_dynode,
                    z_dynode,
                    d_wall);  

  ////////////////////////////////////////////////////////////////
  // MAKE LOGICAL VOLUMES (add materials)
  ////
  helper_make_logical_volume(
                    Glass,
                    PMT_Vac,
                    Dynode_mat);
  
  
  ////////////////////////////////////////////////////////////////
  // MAKE PHYSICAL VOLUMES (place logical volumes)
  ////
  // TODO: face of tube 100 um from front of cylinder
  //m_z_equator= GetPMTHeight() / 2 - outer_z_edge[0]; 

  helper_make_physical_volume();

  ////////////////////////////////////////////////////////////////
  // Attach optical surfaces to borders
  ////
  helper_make_optical_surface(OpPCSurface);
  ////////////////////////////////////////////////////////////////
  // FastSimulationModel
  ////
  helper_fast_sim();
  
  ////////////////////////////////////////////////////////////////
  // Set colors and visibility
  ////
  helper_vis_attr();
}

void 
R3600PMTManager::helper_make_solid(const G4int n_edge,
      const G4double outer_z_edge[],
      const G4double outer_rho_edge[],
      const G4double outer_z_o[],
      G4double r_dynode,     // radius of dynode stack
      G4double z_dynode,     // z coordinate of top of dynode stack, equator=0.
      G4double d_wall       // thickness of the walls
            ) 
{
    this->z_dynode = z_dynode;
    // pmt-shaped volume (to be filled with glass)
    pmt_solid = new dywTorusStack
        ( GetName() + "_pmt_solid");
    G4double pmt_body_thickness = 1e-3*mm;
    body_solid= new dywTorusStack
        ( GetName() + "_body_solid" );
    body_solid->SetAllParameters (n_edge,
            outer_z_edge,
            outer_rho_edge,
            outer_z_o );

    // inner volume (to be filled with vacuum)
    inner_solid= new dywTorusStack(GetName()+"_inner_solid");

    G4int helper_equator_index = -1;

    // set shapes of inner volumes
    // also scan for lowest allowed point of dynode
    G4double z_lowest_dynode= this->z_dynode;
    {
        // We will have to calculate the inner dimensions of the PMT.
        // To allow this, we allocate some workspace.
        // Once upon a time, a very long time ago, g++ was inefficient
        // in allocating small arrays and would fail if the array size was < 4,
        // so here we make sure we allocate one array of sufficient size.
        G4double * dscratch= new G4double[4*(n_edge+1)];
        G4double * inner_z_edge= dscratch;
        G4double * inner_rho_edge= dscratch + n_edge+1;
        G4double * pmt_z_edge = dscratch + 2*(n_edge+1);
        G4double * pmt_rho_edge = dscratch + 3*(n_edge+1);
        G4ThreeVector norm;
        G4int iedge_equator= -1;
        // calculate inner surface edges, check dynode position, and find equator
        inner_z_edge[0]  = outer_z_edge[0]   - d_wall;
        inner_rho_edge[0]= 0.0;
        pmt_z_edge[0]  = outer_z_edge[0] + pmt_body_thickness;
        pmt_rho_edge[0] = 0.0;
        for (int i=1; i<n_edge; i++) {
            norm= body_solid->SurfaceNormal
                ( G4ThreeVector(0.0,
                                outer_rho_edge[i],
                                outer_z_edge[i]) );
            inner_z_edge[i]  = outer_z_edge[i]   - d_wall * norm.z();
            inner_rho_edge[i]= outer_rho_edge[i] - d_wall * norm.y();
            pmt_z_edge[i]  = outer_z_edge[i]  + pmt_body_thickness * norm.z();
            pmt_rho_edge[i]= outer_rho_edge[i] + pmt_body_thickness * norm.y();
            if (inner_rho_edge[i] > r_dynode && inner_z_edge[i] < z_lowest_dynode)
                z_lowest_dynode= inner_z_edge[i];
            if (outer_z_edge[i] == 0.0 || inner_z_edge[i] == 0.0)
                iedge_equator= i;
        }
        inner_z_edge[n_edge]  = outer_z_edge[n_edge]   + d_wall;
        inner_rho_edge[n_edge]= outer_rho_edge[n_edge] - d_wall;
        pmt_z_edge[n_edge]  = outer_z_edge[n_edge]   - pmt_body_thickness;
        pmt_rho_edge[n_edge]= outer_rho_edge[n_edge] + pmt_body_thickness;
        // one final check on dynode allowed position
        if (inner_rho_edge[n_edge] > r_dynode
                && inner_z_edge[n_edge] < z_lowest_dynode)
            z_lowest_dynode= inner_z_edge[n_edge];
        // sanity check equator index
        if (iedge_equator < 0) {
            iedge_equator= (1+n_edge)/2;
            G4cout << "dyw_PMT_LogicalVolume::ConstructPMT_UsingTorusStack: "
                "Warning, pathological PMT shape, equator edge not found!" << G4endl;
        }

        helper_equator_index = iedge_equator;

        // sanity check on dynode height
        if (this->z_dynode > inner_z_edge[iedge_equator]) {
            this->z_dynode= inner_z_edge[iedge_equator];
            G4cout << "dyw_PMT_LogicalVolume::ConstructPMT_UsingTorusStack: "
                "Warning, dynode higher than equator, dynode truncated!" << G4endl;
        }
        // set inner surfaces
        inner_solid->SetAllParameters(n_edge,inner_z_edge,inner_rho_edge,outer_z_o);
        pmt_solid->SetAllParameters(n_edge,
                                    pmt_z_edge,
                                    pmt_rho_edge,
                                    outer_z_o);
        // dywTorusStack keeps its own copy of edges, so we can delete our workspace
        delete [] dscratch;
    }

    G4double helper_sep_tube_r = outer_rho_edge[helper_equator_index];
    G4double helper_sep_tube_h = outer_z_edge[0] - outer_z_edge[helper_equator_index];
    G4double helper_sep_tube_hh = helper_sep_tube_h/2;

    // TODO: check the UNIT?

    G4VSolid * pInnerSep = new G4Tubs("Inner_Separator",
            0.,
            helper_sep_tube_r+1E-9*mm,
            helper_sep_tube_hh,
            0.,360.*degree);
    G4ThreeVector innerSepDispl(0.,0.,helper_sep_tube_hh-1E-9*mm);
    inner1_solid = new G4IntersectionSolid( GetName()
            + "_inner1_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
    inner2_solid = new G4SubtractionSolid( GetName()
            + "_inner2_solid", inner_solid, pInnerSep, NULL, innerSepDispl);

    // dynode volume
    hh_dynode= (this->z_dynode - z_lowest_dynode)/2.0;
    dynode_solid= new G4Tubs
        ( GetName()+"_dynode_solid",
          0.0, r_dynode,          // solid cylinder (fixme?)
          hh_dynode,              // half height of cylinder
          0., 2.*M_PI );          // cylinder complete in phi

}

void
R3600PMTManager::helper_make_logical_volume(
            G4Material* Glass,
            G4Material* PMT_Vac,
            G4Material* Dynode_mat
        )
{
    body_log= new G4LogicalVolume
        ( body_solid,
          Glass,
          GetName()+"_body_log" );

    m_logical_pmt = new G4LogicalVolume
        ( pmt_solid,
          Glass,
          GetName()+"_body_log" );

    body_log->SetSensitiveDetector(m_detector);

    inner1_log= new G4LogicalVolume
        ( inner1_solid,
          PMT_Vac,
          GetName()+"_inner1_log" );
    inner1_log->SetSensitiveDetector(m_detector);

    inner2_log= new G4LogicalVolume
        ( inner2_solid,
          PMT_Vac,
          GetName()+"_inner2_log" );

    dynode_log= new G4LogicalVolume
        ( dynode_solid,
          Dynode_mat,
          GetName()+"_dynode_log" );

}

void
R3600PMTManager::helper_make_physical_volume()
{
    G4ThreeVector noTranslation(0.,0.,0.);

    // place outer solids in envelope
    body_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,  // puts body equator in right place
          body_log,            // the logical volume
          GetName()+"_body_phys", // a name for this physical volume
          m_logical_pmt,                // the mother volume
          false,               // no boolean ops
          0 );                 // copy number

    // place inner solids in outer solid (vacuum)
    inner1_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_inner1_phys",         // a name for this physical volume
          inner1_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    inner2_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_inner2_phys",         // a name for this physical volume
          inner2_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number

    // place dynode in stem/back
    dynode_phys = new G4PVPlacement
        ( 0,
          G4ThreeVector(0.0, 0.0, this->z_dynode - hh_dynode),
          GetName()+"_dynode_phys",
          dynode_log,
          inner2_phys,
          false,
          0 );

}

void
R3600PMTManager::helper_make_optical_surface(
        G4OpticalSurface* OpPCSurface  // photocathode surface
        )
{
    new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf1",
            inner1_phys, body_phys,
            OpPCSurface);
    new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf2",
            body_phys, inner1_phys,
            OpPCSurface);
    new G4LogicalBorderSurface(GetName()+"_mirror_logsurf1",
            inner2_phys, body_phys,
            m_mirror_opsurf);
    new G4LogicalBorderSurface(GetName()+"_mirror_logsurf2",
            body_phys, inner2_phys,
            m_mirror_opsurf);
}

void
R3600PMTManager::helper_fast_sim()
{
    /**  dywPMTOpticalModel * pmtOpticalModel =  **/
    G4Region* body_region = new G4Region(this->GetName()+"_body_region");
    body_region->AddRootLogicalVolume(body_log);
    new dywPMTOpticalModel( GetName()+"_optical_model", 
            body_phys, body_region);

}
void
R3600PMTManager::helper_vis_attr()
{
    G4VisAttributes * visAtt;
    m_logical_pmt -> SetVisAttributes (G4VisAttributes::Invisible);
    // PMT glass
    // visAtt= new G4VisAttributes(G4Color(0.0,1.0,1.0,0.05));
    // body_log->SetVisAttributes( visAtt );
    body_log->SetVisAttributes( G4VisAttributes::Invisible );
    // dynode is medium gray
    visAtt= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.0));
    dynode_log->SetVisAttributes( visAtt );
    // (surface of) interior vacuum is clear orangish gray on top (PC),
    // silvery blue on bottom (mirror)
    //visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,0.27));
    visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,1.0));
    visAtt -> SetForceSolid(true);
    inner1_log->SetVisAttributes (visAtt);
    //visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.67));
    visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,1.0));
    visAtt -> SetForceSolid(true);
    inner2_log->SetVisAttributes (visAtt);

}
